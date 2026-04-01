import app from '../src/app.js'
import config, { MODES } from '../src/utils/config.js'

import supertest from 'supertest'
import jwt from 'jsonwebtoken'
import { before, after, describe, test } from 'node:test'
import assert from 'node:assert'

import {
  clearDb,
  connectDb,
  makeAdminUser,
  makeTestUser,
  makeVerifiedUser,
  disconnectDb,
  TEST_PASSWORD,
} from './helpers.js'
import User from '../src/models/User.js'
import { hashToken } from '../src/utils/auth.js'

assert.strictEqual(config.MODE, MODES.TEST, 'Tests require MODE=TEST')

const api = supertest(app)

describe('Auth API', () => {
  before(async () => {
    await connectDb()
    await clearDb()

    await makeAdminUser(api)
  })

  after(async () => await disconnectDb())

  describe('POST /api/auth/login', () => {
    test('login with username', async () => {
      // Login as admin, using username with predefined password.
      // The status code 200 (OK) means successful HTTP request.
      const response = await api
        .post('/api/auth/login')
        .send({
          identifier: 'admin',
          password: TEST_PASSWORD,
        })
        .expect(200)

      // The response should contain the generated token and  the username
      // should match.
      assert.ok(response.body.token)
      assert.strictEqual(response.body.user.username, 'admin')
    })

    test('login with email', async () => {
      // Login as admin, using email with predefined password.
      const response = await api
        .post('/api/auth/login')
        .send({
          identifier: 'admin@example.com',
          password: TEST_PASSWORD,
        })
        .expect(200)

      assert.ok(response.body.token)
      assert.strictEqual(response.body.user.username, 'admin')
      assert.strictEqual(response.body.user.email, 'admin@example.com')
    })

    test('fails to login with wrong password', async () => {
      // Here status code 401 (Unauthorized) means failed authentication.
      const response = await api
        .post('/api/auth/login')
        .send({
          identifier: 'admin',
          password: 'wrongpassword',
        })
        .expect(401)

      assert.strictEqual(response.body.error, 'Invalid username or password')
    })

    test('login is case-insensitive', async () => {
      const response = await api
        .post('/api/auth/login')
        .send({
          identifier: 'ADMIN',
          password: TEST_PASSWORD,
        })
        .expect(200)

      assert.ok(response.body.token)
    })

    test('fails with non-existing user', async () => {
      const response = await api
        .post('/api/auth/login')
        .send({
          identifier: 'non-existing',
          password: TEST_PASSWORD,
        })
        .expect(401)

      assert.strictEqual(response.body.error, 'Invalid username or password')
    })

    test('returns a valid JWT token', async () => {
      const response = await api
        .post('/api/auth/login')
        .send({
          identifier: 'admin',
          password: TEST_PASSWORD,
        })
        .expect(200)

      // Decode the received token and compare to confirm that the encoded
      // data is correct.
      const decoded = jwt.verify(response.body.token, config.JWT_SECRET)
      assert.strictEqual(decoded.username, 'admin')
      assert.ok(decoded.id)
    })

    test('fails with missing password', async () => {
      // Now the status code 400 (Bad Request) means malformed data - in this
      // case a missing password.
      const response = await api
        .post('/api/auth/login')
        .send({ identifier: 'admin' })
        .expect(400)

      // The error should come from Zod validation check, which returns an
      // array of errors/issues.
      assert.ok(Array.isArray(response.body.errors))
      assert.ok(response.body.errors.length > 0)
    })

    test('fails with missing identifier', async () => {
      // Malformed data - missing identifier.
      const response = await api
        .post('/api/auth/login')
        .send({ password: TEST_PASSWORD })
        .expect(400)

      assert.ok(Array.isArray(response.body.errors))
      assert.ok(response.body.errors.length > 0)
    })

    test('fails with empty body', async () => {
      // Malformed data - no data at all.
      const response = await api.post('/api/auth/login').send({}).expect(400)

      assert.ok(Array.isArray(response.body.errors))

      // Now there are two malformed fields thus expecting more errors.
      assert.ok(response.body.errors.length > 1)
    })

    test('token is not included in response on failure', async () => {
      const response = await api
        .post('/api/auth/login')
        .send({
          identifier: 'admin',
          password: 'wrongpassword',
        })
        .expect(401)

      assert.strictEqual(response.body.token, undefined)
    })

    test('fails for unverified', async () => {
      // The user is to be created, but not verified.
      const name = 'unverified_login'
      await makeTestUser(api, name)

      // The status code 403 (Forbidden) signals successful a valid request,
      // however, the further action is forbidden. In this case, due to
      // incomplete verification step.
      const response = await api
        .post('/api/auth/login')
        .send({ identifier: name, password: TEST_PASSWORD })
        .expect(403)

      assert.strictEqual(
        response.body.error,
        'Please verify your email before logging in',
      )
    })
  })

  describe('POST /api/auth/verify/:token', () => {
    test('verifies a user with a valid token', async () => {
      // The raw verificationToken is only exposed in the response body in
      // TEST mode
      const { userId, verificationToken } = await makeTestUser(
        api,
        'user_to_verify',
      )

      // The token, generated specifically for verification, is used to
      // verify the user, and is later removed.
      await api.post(`/api/auth/verify/${verificationToken}`).expect(200)

      const verified = await User.findById(userId)
      assert.strictEqual(verified.verified, true)

      // The token should be set as null, because it is not needed anymore.
      assert.strictEqual(verified.verificationToken, null)
    })

    test('user can login after verification', async () => {
      const name = 'verify_then_login'
      // The raw verificationToken is only exposed in the response body in
      // TEST mode
      const { verificationToken } = await makeTestUser(api, name)

      await api.post(`/api/auth/verify/${verificationToken}`).expect(200)

      const response = await api
        .post('/api/auth/login')
        .send({ identifier: name, password: TEST_PASSWORD })
        .expect(200)

      // Now it is a different token - the one generated for login by jwt.
      assert.ok(response.body.token)
    })

    test('fails with invalid token', async () => {
      // Code 400 = Bad Request = malformed data.
      const response = await api
        .post('/api/auth/verify/invalidtoken')
        .expect(400)

      assert.strictEqual(
        response.body.error,
        'Invalid or expired verification token',
      )
    })

    test('fails with already used token', async () => {
      // The raw verificationToken is only exposed in the response body in
      // TEST mode
      const { verificationToken: token } = await makeTestUser(api, 'used_token')
      // const token = verificationToken

      await api.post(`/api/auth/verify/${token}`).expect(200)

      // The used token is not supposed to work twice.
      const response = await api.post(`/api/auth/verify/${token}`).expect(400)
      assert.strictEqual(
        response.body.error,
        'Invalid or expired verification token',
      )
    })

    test('fails with expired verification token', async () => {
      // The raw verificationToken is only exposed in the response body in
      // TEST mode
      const { verificationToken: token } = await makeTestUser(
        api,
        'expired_token',
      )

      // Manually expire the token by setting expiry in the past.
      await User.updateOne(
        { verificationToken: hashToken(token) },
        { verificationTokenExpiresAt: new Date(Date.now() - 1000) },
      )

      const response = await api.post(`/api/auth/verify/${token}`).expect(400)
      assert.strictEqual(
        response.body.error,
        'Invalid or expired verification token',
      )
    })
  })

  describe('POST /api/auth/resend-verification', () => {
    test('returns same message whether user exists or not', async () => {
      // Return the same message to avoid checking for existing accounts.
      const existing = await api
        .post('/api/auth/resend-verification')
        .send({ identifier: 'unverified_login' })
        .expect(200)
      const nonExisting = await api
        .post('/api/auth/resend-verification')
        .send({ identifier: 'non_existing_user' })
        .expect(200)

      assert.strictEqual(existing.body.message, nonExisting.body.message)
    })

    test('updates verification token on resend', async () => {
      const name = 'resend_user'
      // The raw verificationToken is only exposed in the response body in
      // TEST mode
      const { userId, verificationToken } = await makeTestUser(api, name)
      const originalToken = verificationToken

      // The reuquest should generate a new token on success.
      await api
        .post('/api/auth/resend-verification')
        .send({ identifier: name })
        .expect(200)

      const after = await User.findById(userId)
      assert.notStrictEqual(after.verificationToken, originalToken)
    })

    test('does nothing for already verified user', async () => {
      const name = 'already_verified'

      // Note the different function.
      const { userId } = await makeVerifiedUser(api, name)

      // The request should not generate a token.
      await api
        .post('/api/auth/resend-verification')
        .send({ identifier: name })
        .expect(200)

      const user = await User.findById(userId)
      assert.strictEqual(user.verificationToken, null)
    })

    test('fails with empty body', async () => {
      const response = await api
        .post('/api/auth/resend-verification')
        .send({})
        .expect(400)

      // Zod should throw errors for wrong input.
      assert.ok(Array.isArray(response.body.errors))
    })
  })

  describe('POST /api/auth/forgot-password', () => {
    test('returns same message whether user exists or not', async () => {
      // Should return the same messages to prevent from malicious tracking
      // of existing accounts.
      const existing = await api
        .post('/api/auth/forgot-password')
        .send({ identifier: 'admin' })
        .expect(200)
      const nonExisting = await api
        .post('/api/auth/forgot-password')
        .send({ identifier: 'non_existing_user' })
        .expect(200)

      assert.strictEqual(existing.body.message, nonExisting.body.message)
    })

    test('sets reset token and expiry for verified user', async () => {
      // The response should provide a valid token.
      await api
        .post('/api/auth/forgot-password')
        .send({ identifier: 'admin' })
        .expect(200)

      const user = await User.findOne({ username: 'admin' })
      assert.ok(user.resetToken)
      assert.ok(user.resetTokenExpiresAt)
      assert.ok(user.resetTokenExpiresAt > new Date())
    })

    test('does not reset token for unverified user', async () => {
      const name = 'unverified_forgot'
      await makeTestUser(api, name)

      await api
        .post('/api/auth/forgot-password')
        .send({ identifier: name })
        .expect(200)

      const user = await User.findOne({ username: name })
      assert.strictEqual(user.resetToken, null)
    })

    test('fails with empty body', async () => {
      const response = await api
        .post('/api/auth/forgot-password')
        .send({})
        .expect(400)

      assert.ok(Array.isArray(response.body.errors))
    })
  })

  describe('POST /api/auth/reset-password/:token', () => {
    test('resets password with valid token', async () => {
      const name = 'verified_reset_password_valid'
      await makeVerifiedUser(api, name)

      // First using the forgot-password endpoint to send a reset token.
      const res = await api
        .post('/api/auth/forgot-password')
        .send({ identifier: name })
      // The resetToken is only exposed in the response body in TEST mode.
      const resetToken = res.body.resetToken

      // Update password.
      const updatedPassword = 'updatedPassword'
      await api
        .post(`/api/auth/reset-password/${resetToken}`)
        .send({ password: updatedPassword })
        .expect(200)

      const response = await api
        .post('/api/auth/login')
        .send({ identifier: name, password: updatedPassword })
        .expect(200)

      assert.ok(response.body.token)
    })

    test('clears reset token after use', async () => {
      const name = 'verified_clear_password'
      await makeVerifiedUser(api, name)
      // The request will send a new reset token.
      const res = await api
        .post('/api/auth/forgot-password')
        .send({ identifier: name })
      // The resetToken is only exposed in the response body in TEST mode.
      const resetToken = res.body.resetToken

      const updatedPassword = 'updatedPassword'
      // Now using the provided reset token to reset the password.
      await api
        .post(`/api/auth/reset-password/${resetToken}`)
        .send({ password: updatedPassword })
        .expect(200)

      const updatedUser = await User.findOne({ username: name })

      // The token should be null as it was one time token.
      assert.strictEqual(updatedUser.resetToken, null)
      assert.strictEqual(updatedUser.resetTokenExpiresAt, null)
    })

    test('fails with invalid token', async () => {
      const response = await api
        .post('/api/auth/reset-password/invalidtoken')
        .send({ password: 'newAwesomePassword' })
        .expect(400)

      assert.strictEqual(response.body.error, 'Invalid or expired reset token')
    })

    test('fails with too short password', async () => {
      // Token doesn't matter here — Zod rejects the short password first
      const response = await api
        .post('/api/auth/reset-password/invalidtoken')
        .send({ password: 'short' })
        .expect(400)

      // Zod should have given errors.
      assert.ok(Array.isArray(response.body.errors))
    })

    test('token cannot be reused', async () => {
      const name = 'verified_token_cannot_reuse'
      await makeVerifiedUser(api, name)

      const res = await api
        .post('/api/auth/forgot-password')
        .send({ identifier: name })
      // The resetToken is only exposed in the response body in TEST mode.
      const token = res.body.resetToken

      await api
        .post(`/api/auth/reset-password/${token}`)
        .send({ password: 'newAwesomePassword' })
        .expect(200)

      // After the first response the reset token expiration should be null
      // and, therefore, the following request should not succeed.
      const response = await api
        .post(`/api/auth/reset-password/${token}`)
        .send({ password: 'notSoAwesomePassword' })
        .expect(400)

      assert.strictEqual(response.body.error, 'Invalid or expired reset token')
    })
  })
})
