import bcrypt from 'bcrypt'

import config, { MODES } from '../utils/config.js'
import User from '../models/User.js'
import mailService from './mailService.js'
import userService from './userService.js'
import {
  BadRequestError,
  ForbiddenError,
  UnauthorizedError,
} from '../utils/error.js'
import {
  generateJSONWebToken,
  generateTokenWithExpiry,
  getPasswordHash,
  hashToken,
} from '../utils/auth.js'
import logger from '../utils/logger.js'
import { AUTH_MESSAGES } from '../utils/messages.js'

let dummyHash = null
const getDummyHash = async () => {
  if (!dummyHash) {
    dummyHash = await getPasswordHash('dummy')
  }
  return dummyHash
}

const registerUser = async ({ username, email, password, frontendUrl }) => {
  const { verificationToken, user } = await userService.createUser({
    username,
    email,
    password,
  })

  mailService
    .sendVerificationEmail(user.email, verificationToken, frontendUrl)
    .catch((err) => logger.error('Failed to send verification email: ', err))

  const payload = { message: AUTH_MESSAGES.REGISTER, userId: user.id }
  if (config.MODE === MODES.TEST) {
    payload.verificationToken = verificationToken
  }
  return payload
}

const loginUser = async ({ identifier, password }) => {
  // The database searches for a user with identifier matching either
  // their username or email.
  const user = await userService.findUserByIdentifier(identifier)

  // If user not found, then use a dummy hash to run the bcrypt algorithm
  // to prevent analysis for existing accounts using time of response.
  const hash = user?.passwordHash ?? (await getDummyHash())
  const passwordMatch = await bcrypt.compare(password, hash)

  if (!(user && passwordMatch)) {
    throw new UnauthorizedError(AUTH_MESSAGES.LOGIN_FAILED)
  }

  // If the user is registered, but not verified, remind about verification.
  if (!user.verified) {
    throw new ForbiddenError(AUTH_MESSAGES.REMIND_VERIFY)
  }

  return {
    token: generateJSONWebToken(user),
    user: {
      id: user.id,
      username: user.username,
      email: user.email,
    },
  }
}

const verifyEmail = async (token) => {
  const user = await User.findOne({
    verificationToken: hashToken(token),
    verificationTokenExpiresAt: { $gt: new Date() },
  })

  if (!user) {
    throw new BadRequestError(AUTH_MESSAGES.VERIFY_FAILED)
  }

  // Mark user as verified and then nullify the token (not needed anymore).
  user.verified = true
  user.verificationToken = null
  user.verificationTokenExpiresAt = null

  await user.save()

  return { message: AUTH_MESSAGES.EMAIL_VERIFIED }
}

const resendVerificationEmail = async (identifier, frontendUrl) => {
  const user = await userService.findUserByIdentifier(identifier)

  if (user && !user.verified) {
    const { token, hashedToken, expiresAt } = generateTokenWithExpiry(
      config.VERIFICATION_TOKEN_EXPIRES_MS,
    )

    user.verificationToken = hashedToken
    user.verificationTokenExpiresAt = expiresAt
    await user.save()

    mailService
      .sendVerificationEmail(user.email, token, frontendUrl)
      .catch((err) => logger.error('Failed to send verification email: ', err))
  }

  // Regardless whether a user exists or not, we send the same message to
  // avoid checking for existing users/accounts.
  return { message: AUTH_MESSAGES.RESEND_VERIFICATION }
}

const forgotPassword = async (email, frontendUrl) => {
  const user = await userService.findUserByIdentifier(email)

  // Generate a token to send to the reset-password endpoint.
  const { token, hashedToken, expiresAt } = generateTokenWithExpiry(
    config.RESET_TOKEN_EXPIRES_MS,
  )

  // Resetting password should naturally work only for verified users.
  if (user && user.verified) {
    user.resetToken = hashedToken
    user.resetTokenExpiresAt = expiresAt
    await user.save()
    await mailService.sendPasswordResetEmail(user.email, token, frontendUrl)
  }

  const payload = { message: AUTH_MESSAGES.FORGOT_PASSWORD }

  if (config.MODE === MODES.TEST) {
    payload.resetToken = token
  }

  return payload
}

const resetPassword = async (token, password) => {
  // The token comes from the forgot-password endpoint.
  const user = await User.findOne({
    resetToken: hashToken(token),
    resetTokenExpiresAt: { $gt: new Date() },
  })

  if (!user) {
    throw new BadRequestError(AUTH_MESSAGES.PASSWORD_RESET_FAILED)
  }

  // Generate new hash and nullify the reset token.
  user.passwordHash = await getPasswordHash(password)
  user.resetToken = null
  user.resetTokenExpiresAt = null

  await user.save()

  return { message: AUTH_MESSAGES.PASSWORD_RESET }
}

export default {
  registerUser,
  loginUser,
  verifyEmail,
  resendVerificationEmail,
  forgotPassword,
  resetPassword,
}
