import config, { MODES } from './config.js'

import rateLimit from 'express-rate-limit'

// The 15-minute window for all limiters
const createLimiter = (limit) =>
  rateLimit({
    windowMs: 15 * 60 * 1000,
    limit: config.MODE === MODES.PRODUCTION ? limit : 1000,
    message: { error: 'Too many requests, please try again later' },
    standardHeaders: 'draft-8',
    legacyHeaders: false,
  })

const loginLimiterBase = createLimiter(100)
const passwordLimiterBase = createLimiter(50)

export const loginLimiter = (request, response, next) =>
  config.MODE === MODES.TEST
    ? next()
    : loginLimiterBase(request, response, next)

export const passwordLimiter = (request, response, next) =>
  config.MODE === MODES.TEST
    ? next()
    : passwordLimiterBase(request, response, next)
