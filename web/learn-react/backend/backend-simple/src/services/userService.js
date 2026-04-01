import config from '../utils/config.js'
import User from '../models/User.js'
import { generateTokenWithExpiry, getPasswordHash } from '../utils/auth.js'
import { NotFoundError } from '../utils/error.js'

const findUserByIdentifier = (identifier) =>
  User.findOne({ $or: [{ username: identifier }, { email: identifier }] })

const getUsers = async ({ page, limit }) => {
  const skip = (page - 1) * limit

  const [users, total] = await Promise.all([
    User.find({}).skip(skip).limit(limit),
    User.countDocuments(),
  ])
  return {
    data: users,
    pagination: {
      total,
      page,
      limit,
      pages: Math.ceil(total / limit),
    },
  }
}

const getUserById = async (id) => {
  const user = await User.findById(id)
  if (!user) {
    throw new NotFoundError('User not found')
  }
  return user
}

const createUser = async ({ username, email, password }) => {
  const passwordHash = await getPasswordHash(password)

  // The raw token is used for verification.
  const { token, hashedToken, expiresAt } = generateTokenWithExpiry(
    config.VERIFICATION_TOKEN_EXPIRES_MS,
  )

  const user = await User.create({
    username,
    email,
    passwordHash,
    verificationToken: hashedToken,
    verificationTokenExpiresAt: expiresAt,
  })

  return { user, verificationToken: token }
}

const updateUserRole = async (id, role) => {
  const user = await User.findByIdAndUpdate(
    id,
    { role },
    { returnDocument: 'after', runValidators: true },
  )

  if (!user) {
    throw new NotFoundError('User not found')
  }

  return user
}

const updateUserProfile = async (id, updates) => {
  const user = await User.findById(id)
  if (!user) {
    throw new NotFoundError('User not found')
  }
  const { username, email, password } = updates
  if (username) {
    user.username = username
  }
  if (email) {
    user.email = email
  }
  if (password) {
    user.passwordHash = await getPasswordHash(password)
  }
  return await user.save()
}

const deleteUser = async (id) => {
  const user = await User.findById(id)
  if (!user) {
    throw new NotFoundError('User not found')
  }
  await user.deleteOne()
  return user
}

const deleteUnverifiedUsers = async () => {
  const gracePeriodMs = 12 * 60 * 60 * 1000

  const now = Date.now()
  const expiryCutoff = new Date(now - gracePeriodMs)
  const createdCutoff = new Date(
    now - config.VERIFICATION_TOKEN_EXPIRES_MS - gracePeriodMs,
  )
  const result = await User.deleteMany({
    verified: false,
    verificationTokenExpiresAt: { $lt: expiryCutoff },
    createdAt: { $lt: createdCutoff },
  })

  return result
}

export default {
  findUserByIdentifier,
  getUsers,
  getUserById,
  createUser,
  updateUserRole,
  updateUserProfile,
  deleteUser,
  deleteUnverifiedUsers,
}
