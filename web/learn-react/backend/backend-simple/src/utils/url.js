import config from './config.js'
import logger from './logger.js'

/**
 * Returns the frontend URL to use for email links. Uses
 * `request.body.redirectUrl` if it is provided and its origin is in the
 * ALLOWED_ORIGINS whitelist, otherwise falls back to config.FRONTEND_URL.
 * @param {import('express').Request} request
 * @returns {string} A validated frontend URL
 */
export const getFrontendUrl = (redirectUrl) => {
  logger.info(`[GETFRONTENDURL]: ${redirectUrl}`)
  if (redirectUrl) {
    try {
      const { origin } = new URL(redirectUrl)
      if (config.ALLOWED_ORIGINS.includes(origin)) {
        return redirectUrl
      }
    } catch {
      // Invalid URL — fall through to default
    }
  }
  return config.FRONTEND_URL
}
