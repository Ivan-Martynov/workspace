import User from '../src/models/User.js'
import { getPasswordHash } from '../src/utils/auth.js'
import logger from '../src/utils/logger.js'

const createAdmin = async () => {
  const existing = await User.findOne({ role: 'admin' })
  if (existing) {
    logger.warn('Admin already exists')
    process.exit(0)
  }

  const passwordHash = await getPasswordHash(process.env.ADMIN_PASSWORD)
  await User.create({
    username: process.env.ADMIN_USERNAME,
    email: process.env.ADMIN_EMAIL,
    passwordHash,
    role: 'admin',
    verified: true,
  })
  logger.info('Admin created')
  process.exit(0)
}

createAdmin()
