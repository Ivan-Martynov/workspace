import bcrypt from 'bcrypt'
import crypto from 'crypto'
import jwt from 'jsonwebtoken'

import config from './config.js'
import User from '../models/User.js'

/**
 * Hashes a plain-text password using bcrypt.
 * @param {string} password - Plain-text password
 * @param {number} [saltRounds=config.SALT_ROUNDS] - bcrypt salt rounds
 * @returns {Promise<string>} The bcrypt hash
 */
export const getPasswordHash = (password, saltRounds = config.SALT_ROUNDS) =>
  bcrypt.hash(password, saltRounds)

/**
 * Generates a secure random token along with its hashed version and an expiry
 * date.
 *
 * This is useful for operations like email verification or password resets,
 * where you need to store a hashed token in the database but send the raw token
 * to the user (e.g., via email).
 *
 * @param {number} expiresMs - Token validity duration in milliseconds.
 * @returns {Object} An object containing:
 *   - token: the raw token to send to the user,
 *   - hashedToken: the hashed version to store securely in the database,
 *   - expiresAt: the Date when the token expires.
 */
export const generateTokenWithExpiry = (expiresMs) => {
  const token = crypto.randomBytes(32).toString('hex')
  return {
    token,
    hashedToken: hashToken(token),
    expiresAt: new Date(Date.now() + expiresMs),
  }
}

/**
 * Generates a signed JSON Web Token (JWT) for authentication.
 *
 * The token contains the user's id and username as payload and is signed
 * using the application's JWT secret. It can be used for authenticating
 * subsequent requests.
 *
 * @param {Object} user - The user for whom the token is generated.
 * @param {string} user.id - The unique identifier of the user.
 * @param {string} user.username - The username of the user.
 * @returns {string} A signed JWT token.
 */
export const generateJSONWebToken = (user) =>
  jwt.sign({ id: user.id, username: user.username }, config.JWT_SECRET, {
    expiresIn: config.JWT_EXPIRES,
  })

/**
 * Extracts the Bearer token from the Authorization header.
 * @param {import('express').Request} request
 * @returns {string|null} The token string, or null if not present
 */
export const getToken = (request) => {
  const authorization = request.get('authorization')
  if (authorization && authorization.startsWith('Bearer ')) {
    return authorization.replace('Bearer ', '')
  }
  return null
}

export const verifyJSONWebToken = (token) => {
  return jwt.verify(token, config.JWT_SECRET)
}

/**
 * Returns the SHA-256 hex digest of a token. Used to store reset/verification
 * tokens in the database without saving the raw value.
 * @param {string} token - Raw token string
 * @returns {string} Hex-encoded hash
 */
export const hashToken = (token) =>
  crypto.createHash('sha256').update(token).digest('hex')

/**
 * Express middleware that extracts the Bearer token from the request and
 * attaches it to `request.token` if present.
 * @type {import('express').RequestHandler}
 */
export const tokenExtractor = (request, _response, next) => {
  const token = getToken(request)
  if (token) {
    request.token = token
  }
  next()
}

/**
 * Express middleware that verifies the JWT and attaches the matching User
 * document to `request.user`. Calls next() without a user if no token is
 * present, and forwards JWT errors to the error handler.
 * @type {import('express').RequestHandler}
 */
export const userExtractor = async (request, _response, next) => {
  try {
    const token = getToken(request)
    if (!token) {
      return next()
    }
    const decodedToken = jwt.verify(token, config.JWT_SECRET)
    if (!decodedToken?.id) {
      return next()
    }
    const user = await User.findById(decodedToken.id)
    if (user) {
      request.user = user
    }
    next()
  } catch (error) {
    next(error)
  }
}

const isAdminRole = (request) => request.user?.role === 'admin'
const isOwnerUser = (request) => request.user?.id === request.params.id

/**
 * Express middleware that requires the requesting user to have the admin role.
 * Responds 403 if not.
 * @type {import('express').RequestHandler}
 */
export const isAdmin = (request, response, next) => {
  if (!isAdminRole(request)) {
    return response.status(403).json({ error: 'Admin access required' })
  }
  next()
}

/**
 * Express middleware that requires the requesting user to be either the
 * resource owner (matched by `request.params.id`) or an admin.
 * Responds 403 if not.
 * @type {import('express').RequestHandler}
 */
export const isOwnerOrAdmin = (request, response, next) => {
  if (!(isAdminRole(request) || isOwnerUser(request))) {
    return response.status(403).json({ error: 'Unauthorized' })
  }
  next()
}

/**
 * Express middleware that requires a user to be authenticated.
 * Responds 401 if `request.user` is not set.
 * @type {import('express').RequestHandler}
 */
export const requireAuth = (request, response, next) => {
  if (!request.user) {
    return response.status(401).json({ error: 'Authentication required' })
  }
  next()
}
