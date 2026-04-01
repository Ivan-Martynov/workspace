import app from '../src/app.js'

import supertest from 'supertest'
import jwt from 'jsonwebtoken'
import { after, describe, test, before, beforeEach, afterEach } from 'node:test'
import assert from 'node:assert'

import {
  clearDb,
  connectDb,
  makeAdminUser,
  makeTestUser,
  createUser,
  makeVerifiedUser,
  disconnectDb,
  getUser,
  loginUser,
  TEST_PASSWORD,
} from './helpers.js'
import config from '../src/utils/config.js'
import User from '../src/models/User.js'

const api = supertest(app)

describe('Users API', () => {
  let adminToken

  before(async () => {
    await connectDb()
  })
  after(async () => await disconnectDb())

  beforeEach(async () => {
    await clearDb()
    adminToken = await makeAdminUser(api)
  })
  afterEach(async () => {
    await clearDb()
  })

  describe('POST /api/users', () => {
    test('creates a new user', async () => {
      // Response should succeed and have correct Content-Type. Here the code
      // 201 (Created) signals that a resouce (the user) has been created.
      const response = await createUser(api, {
        username: 'alice',
        email: 'alice@example.com',
        password: 'longenoughpassword',
      }).expect(201)

      assert.strictEqual(response.body.message, 'User created')
      assert.ok(response.body.userId)
    })

    test('stores username and email as lowercase', async () => {
      const { userId } = await makeTestUser(api, 'MixedCase')
      const user = await getUser(api, userId)
      assert.strictEqual(user.body.username, 'mixedcase')
      assert.strictEqual(user.body.email, 'mixedcase@example.com')
    })

    test('does not expose passwordHash in response', async () => {
      // The user creation should not forward the passwordHash.
      const { userId } = await makeTestUser(api, 'nohash')
      const user = await getUser(api, userId)
      assert.strictEqual(user.body.passwordHash, undefined)
    })

    test('fails with missing password', async () => {
      const response = await createUser(api, {
        username: 'name_no_pass',
        email: 'name_no_pass@example.com',
      }).expect(400)

      // Zod schemas throw errors on malformed input.
      assert.ok(Array.isArray(response.body.errors))
      assert.ok(response.body.errors.length > 0)
    })

    test('fails with missing email', async () => {
      const response = await createUser(api, {
        username: 'name_no_pass',
        password: TEST_PASSWORD,
      }).expect(400)

      assert.ok(Array.isArray(response.body.errors))
      assert.ok(response.body.errors.length > 0)
    })

    test('fails with missing username', async () => {
      const response = await createUser(api, {
        email: 'wrong@example.com',
        password: TEST_PASSWORD,
      }).expect(400)

      assert.ok(Array.isArray(response.body.errors))
      assert.ok(response.body.errors.length > 0)
    })

    test('fails with short password', async () => {
      const response = await createUser(api, {
        username: 'name_short_pass',
        email: 'name_short_pass@example.com',
        password: 'short',
      }).expect(400)

      assert.ok(Array.isArray(response.body.errors))
      // Check that Zod errors inform about password being malformed.
      assert.ok(
        response.body.errors.some((e) => e.toLowerCase().includes('password')),
      )
    })

    test('fails with invalid email format', async () => {
      const response = await createUser(api, {
        username: 'name_invalid_email',
        email: 'notanemail',
        password: TEST_PASSWORD,
      }).expect(400)

      assert.ok(Array.isArray(response.body.errors))
      // Check that Zod errors inform about malformed email.
      assert.ok(
        response.body.errors.some((e) => e.toLowerCase().includes('email')),
      )
    })

    test('fails with username too short', async () => {
      const response = await createUser(api, {
        username: '   ab  ',
        email: 'ab@example.com',
        password: TEST_PASSWORD,
      }).expect(400)

      assert.ok(Array.isArray(response.body.errors))
      // Check that Zod errors inform about malformed username.
      assert.ok(
        response.body.errors.some((e) => e.toLowerCase().includes('username')),
      )
    })

    test('fails with username containing invalid characters', async () => {
      const response = await createUser(api, {
        username: 'bad username',
        email: 'bad@example.com',
        password: TEST_PASSWORD,
      }).expect(400)

      assert.ok(Array.isArray(response.body.errors))
      assert.ok(
        response.body.errors.some((e) => e.toLowerCase().includes('username')),
      )
    })

    test('fails with duplicate username', async () => {
      await makeTestUser(api, 'bob')

      // Response status code 409 (Conflict) signals about a conflicting case.
      // In this case the database requires unique usernames and emails.
      const response = await createUser(api, {
        username: 'bob',
        email: 'bob.other@example.com',
        password: TEST_PASSWORD,
      }).expect(409)

      assert.strictEqual(
        response.body.error,
        'Username or email already exists',
      )
    })

    test('fails with duplicate email', async () => {
      await makeTestUser(api, 'bob2')
      const response = await createUser(api, {
        username: 'bobbie',
        email: 'bob2@example.com',
        password: TEST_PASSWORD,
      }).expect(409)

      assert.strictEqual(
        response.body.error,
        'Username or email already exists',
      )
    })

    test('user is unverified after creation', async () => {
      // The user is not verified by default, but contains verification token
      // to be used later for verification.
      const { userId } = await makeTestUser(api, 'check_unverified')
      const user = await User.findById(userId)
      assert.strictEqual(user.verified, false)
      assert.ok(user.verificationToken)
    })

    test('user can verify with token', async () => {
      const { userId, verificationToken } = await makeTestUser(
        api,
        'check_verify_flow',
      )

      // Using the verification token should succeed and nullify the token.
      await api.post(`/api/auth/verify/${verificationToken}`).expect(200)

      const verifiedUser = await User.findById(userId)
      assert.strictEqual(verifiedUser.verified, true)
      assert.strictEqual(verifiedUser.verificationToken, null)
    })
  })

  describe('PATCH /api/users/:id', () => {
    // Build a request, which will be run later.
    const patchUser = (id, token) => {
      const request = api.patch(`/api/users/${id}`)
      return token ? request.set('Authorization', `Bearer ${token}`) : request
    }

    test('admin can update an existing user', async () => {
      const { userId } = await makeTestUser(api, 'user_to_update')
      await patchUser(userId, adminToken)
        .send({ email: 'user_to_update.UPdATe@example.com' })
        .expect(200)

      const user = await getUser(api, userId)
      assert.strictEqual(user.body.email, 'user_to_update.update@example.com')
    })

    test('user can update own account', async () => {
      const { userId } = await makeVerifiedUser(api, 'create_and_update')
      const { token } = await loginUser(api, 'create_and_update')
      await patchUser(userId, token).send({ username: 'Dumbledor' }).expect(200)

      const user = await getUser(api, userId)
      assert.strictEqual(user.body.username, 'dumbledor')
    })

    test('fails with no valid fields', async () => {
      const { userId } = await makeTestUser(api, 'dummy_patch')
      const response = await patchUser(userId, adminToken)
        .send({ wrong: 'wrong' })
        .expect(400)

      // Zod informs about malformed input.
      assert.ok(Array.isArray(response.body.errors))
      assert.ok(response.body.errors.length > 0)
    })

    test('fails with invalid email format', async () => {
      const { userId } = await makeTestUser(api, 'invalid_email_patch')
      const response = await patchUser(userId, adminToken)
        .send({ email: 'notanemail' })
        .expect(400)

      assert.ok(Array.isArray(response.body.errors))
      assert.ok(
        response.body.errors.some((e) => e.toLowerCase().includes('email')),
      )
    })

    test('fails with short password', async () => {
      const { userId } = await makeTestUser(api, 'short_pw_patch')
      const response = await patchUser(userId, adminToken)
        .send({ password: 'short' })
        .expect(400)

      assert.ok(Array.isArray(response.body.errors))
      assert.ok(
        response.body.errors.some((e) => e.toLowerCase().includes('password')),
      )
    })

    test('fails with invalid username characters', async () => {
      const { userId } = await makeTestUser(api, 'bad_username_patch')
      const response = await patchUser(userId, adminToken)
        .send({ username: 'bad username' })
        .expect(400)

      assert.ok(Array.isArray(response.body.errors))
      assert.ok(
        response.body.errors.some((e) => e.toLowerCase().includes('username')),
      )
    })

    test('fails without token', async () => {
      const { userId } = await makeTestUser(api, 'no_token_patch')
      // The status code 401 (Unauthorized) signals about required
      // authentication/authorization.
      await patchUser(userId).send({ username: 'bad username' }).expect(401)
    })

    test('user cannot update duplicate username', async () => {
      await makeTestUser(api, 'already_there')
      // The user should be verified in order to run PATCH requests.
      const { userId } = await makeVerifiedUser(api, 'to_be_there')
      const { token } = await loginUser(api, 'to_be_there')

      // The conflict should come from the database.
      await patchUser(userId, token)
        .send({ username: 'already_there' })
        .expect(409)

      const user = await getUser(api, userId)
      assert.strictEqual(user.body.username, 'to_be_there')
    })

    test('user cannot update duplicate email', async () => {
      await makeTestUser(api, 'email_already_there')
      const { userId } = await makeVerifiedUser(api, 'email_to_be_there')
      const { token } = await loginUser(api, 'email_to_be_there')

      await patchUser(userId, token)
        .send({ email: 'email_already_there@example.com' })
        .expect(409)

      const user = await getUser(api, userId)
      assert.strictEqual(user.body.email, 'email_to_be_there@example.com')
    })

    test("user cannot update another user's account", async () => {
      await makeVerifiedUser(api, 'attacker')
      const { userId } = await makeTestUser(api, 'victim')
      const { token } = await loginUser(api, 'attacker')

      // The status code 403 (Forbidden) signals about missing permission. That
      // is, the user is valid and authenticated, but the Authorization token
      // does not provide permission.
      await patchUser(userId, token).send({ username: 'hacked' }).expect(403)
    })
  })

  describe('DELETE /api/users/:id', () => {
    // Build a request, which will be run later.
    const deleteUser = (id, token) => {
      const request = api.delete(`/api/users/${id}`)
      return token ? request.set('Authorization', `Bearer ${token}`) : request
    }

    test('admin can delete an existing user', async () => {
      const { userId } = await makeTestUser(api, 'to_be_deleted')
      // The status code 204 (No Content) means success and response returns
      // no data.
      await deleteUser(userId, adminToken).expect(204)
    })

    test('user can delete own account', async () => {
      const { userId } = await makeVerifiedUser(api, 'self_delete')
      const { token } = await loginUser(api, 'self_delete')
      await deleteUser(userId, token).expect(204)
    })

    test("user cannot delete another user's account", async () => {
      await makeVerifiedUser(api, 'delete_attacker')
      const { userId } = await makeVerifiedUser(api, 'delete_victim')
      const { token } = await loginUser(api, 'delete_attacker')
      await deleteUser(userId, token).expect(403)
    })

    test('fails without token', async () => {
      const { userId } = await makeTestUser(api, 'no_token_delete')
      await deleteUser(userId).expect(401)
    })

    test('fails with expired token', async () => {
      const { userId } = await makeTestUser(api, 'expired_token_delete')
      const expiredToken = jwt.sign({ id: userId }, config.JWT_SECRET, {
        expiresIn: '0s',
      })
      await deleteUser(userId, expiredToken).expect(401)
    })

    test('fails with malformed id', async () => {
      await deleteUser('0000000', adminToken).expect(400)
    })

    test('returns 404 for non-existing user', async () => {
      await deleteUser('000000000000000000000000', adminToken).expect(404)
    })
  })

  describe('PATCH /api/users/:id/role', () => {
    let userId

    // Build a request, which will be run later.
    const patchRole = (token) => {
      const request = api.patch(`/api/users/${userId}/role`)
      return token ? request.set('Authorization', `Bearer ${token}`) : request
    }

    beforeEach(async () => {
      userId = (await makeVerifiedUser(api, 'test_patch_role_user')).userId
    })

    test('admin can promote a user to admin', async () => {
      const response = await patchRole(adminToken)
        .send({ role: 'admin' })
        .expect(200)
      assert.strictEqual(response.body.role, 'admin')
    })

    test('admin can demote a user to user', async () => {
      const response = await patchRole(adminToken)
        .send({ role: 'user' })
        .expect(200)
      assert.strictEqual(response.body.role, 'user')
    })

    test('fails with invalid role', async () => {
      const response = await patchRole(adminToken)
        .send({ role: 'superuser' })
        .expect(400)

      assert.ok(Array.isArray(response.body.errors))
      assert.ok(response.body.errors.length > 0)
    })

    test('fails without token', async () => {
      await patchRole().send({ role: 'admin' }).expect(401)
    })

    test('fails for non-admin user', async () => {
      const { token } = await loginUser(api, 'test_patch_role_user')
      const response = await patchRole(token)
        .send({ role: 'admin' })
        .expect(403)

      assert.strictEqual(response.body.error, 'Admin access required')
    })
  })
})
