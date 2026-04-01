import app from './app.js'
import connectDB from './db.js'
import logger from './utils/logger.js'
import config from './utils/config.js'

process.on('uncaughtException', (error) => {
  logger.error('Uncaught exception:', error)
  process.exit(1)
})

process.on('unhandledRejection', (error) => {
  logger.error('Unhandled rejection:', error)
  process.exit(1)
})

await connectDB()

const server = app.listen(config.PORT, () => {
  logger.info(`Server running on port ${config.PORT}`)
})

server.on('error', (err) => {
  if (err.code === 'EADDRINUSE') {
    logger.error(`Port ${config.PORT} is already in use.`)
  } else {
    logger.error(err)
  }
  process.exit(1)
})
