import mongoose from 'mongoose'
import cron from 'node-cron'

import userService from './services/userService.js'
import logger from './utils/logger.js'
import config from './utils/config.js'

const removeUnverifiedUsers = async () => {
  if (mongoose.connection.readyState !== 1) {
    logger.warn('MongoDB not connected, skipping unverified user deletion')
    return
  }

  try {
    const result = await userService.deleteUnverifiedUsers()
    if (result?.deletedCount > 0) {
      logger.info(`Deleted ${result.deletedCount} unverified users`)
    }
  } catch (err) {
    logger.error(`Error deleting unverified users: ${err.message}`)
  }
}
const task = cron.schedule('0 2 * * *', removeUnverifiedUsers)

const connectDB = async () => {
  mongoose.set('strictQuery', false)

  let retries = 5
  // Waiting time between connection attempts
  const delayMs = 1000
  while (retries) {
    try {
      await mongoose.connect(config.MONGODB_URI)
      logger.info('Connected to MongoDB')

      await removeUnverifiedUsers()
      break
    } catch (err) {
      logger.error(
        `Error connecting to MongoDB: ${err.message}. Retries left: ${--retries}`,
      )
      if (!retries) {
        process.exit(1)
      }
      await new Promise((resolve) => setTimeout(resolve, delayMs))
    }
  }
}

const gracefulShutdown = async () => {
  task.stop()
  await mongoose.connection.close()
  logger.info('MongoDB connection closed due to app termination')
  process.exit(0)
}

process.on('SIGINT', gracefulShutdown)
process.on('SIGTERM', gracefulShutdown)

export default connectDB
