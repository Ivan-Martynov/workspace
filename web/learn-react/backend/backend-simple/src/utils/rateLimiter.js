import rateLimit from 'express-rate-limit'

import config, { MODES } from './config.js'
import { HTTP_ERRORS } from './messages.js'

const loginLimits = {
  [MODES.PRODUCTION]: 5,
  [MODES.DEVELOPMENT]: 50,
  [MODES.TEST]: 200,
}

const passwordLimits = {
  [MODES.PRODUCTION]: 3,
  [MODES.DEVELOPMENT]: 30,
  [MODES.TEST]: 100,
}

// The 15-minute window for all limiters
const createLimiter = (limit) =>
  rateLimit({
    windowMs: 15 * 60 * 1000,
    limit,
    message: { error: HTTP_ERRORS.TOO_MANY_REQUESTS },
    standardHeaders: 'draft-8',
    legacyHeaders: false,
  })

const loginLimiterBase = createLimiter(loginLimits[config.MODE])
const passwordLimiterBase = createLimiter(passwordLimits[config.MODE])

export const loginLimiter = (request, response, next) =>
  config.MODE === MODES.TEST
    ? next()
    : loginLimiterBase(request, response, next)

export const passwordLimiter = (request, response, next) =>
  config.MODE === MODES.TEST
    ? next()
    : passwordLimiterBase(request, response, next)
