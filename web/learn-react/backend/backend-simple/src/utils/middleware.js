import morgan from 'morgan'
import { ZodError } from 'zod'

import config, { MODES } from './config.js'
import logger from './logger.js'
import { SYSTEM_ERRORS, VALIDATION_ERRORS } from './messages.js'

const ERROR_PARAMS = {
  [VALIDATION_ERRORS.PASSWORD_TOO_SHORT]: { min: config.PASSWORD_MIN_LENGTH },
  [VALIDATION_ERRORS.PASSWORD_TOO_LONG]: { max: config.PASSWORD_MAX_LENGTH },
  [VALIDATION_ERRORS.USERNAME_TOO_SHORT]: { min: config.USERNAME_MIN_LENGTH },
  [VALIDATION_ERRORS.USERNAME_TOO_LONG]: { max: config.USERNAME_MAX_LENGTH },
}

/**
 * Express middleware factory that validates `request.body` against a Zod
 * schema. Replaces the body with parsed data on success, or responds 400
 * with an array of error messages on failure.
 * @param {import('zod').ZodSchema} schema
 * @returns {import('express').RequestHandler}
 */
export const validate = (schema) => (request, response, next) => {
  const result = schema.safeParse(request.body)
  if (!result.success) {
    const errors = result.error.issues.map((err) => ({
      code: err.message,
      params: ERROR_PARAMS[err.message] ?? {},
    }))
    return response.status(400).json({ errors })
  }
  request.body = result.data
  next()
}

// Redacts sensitive fields before logging the request body.
const sanitizeBody = (body) => {
  const sanitized = { ...body }
  const sensitiveFields = [
    'password',
    'passwordHash',
    'hashedPassword',
    'token',
  ]
  sensitiveFields.forEach((field) => {
    if (field in sanitized) {
      sanitized[field] = '***'
    }
  })
  return JSON.stringify(sanitized)
}

const colorCodes = {
  reset: '\x1b[0m',
  bold: '\x1b[1m',
  green: '\x1b[32m',
  yellow: '\x1b[33m',
  red: '\x1b[31m',
  cyan: '\x1b[36m',
  dim: '\x1b[2m',
}

const statusColor = (status) =>
  status >= 500
    ? colorCodes.red
    : status >= 400
      ? colorCodes.yellow
      : status >= 200
        ? colorCodes.green
        : colorCodes.reset

const methodColor = (method) => {
  switch (method) {
    case 'GET':
      return colorCodes.green
    case 'POST':
      return colorCodes.cyan
    case 'PATCH':
    case 'PUT':
      return colorCodes.yellow
    case 'DELETE':
      return colorCodes.red
    default:
      return colorCodes.reset
  }
}

// Disable colors in production since logs are typically piped to files.
const colorize = config.MODE !== MODES.PRODUCTION

const formatMethod = (method) =>
  colorize
    ? `${colorCodes.bold}${methodColor(method)}${method}${colorCodes.reset}`
    : method

const formatStatus = (status, color) =>
  colorize ? `${color}${status}${colorCodes.reset}` : `${status}`

const dim = (str) =>
  colorize ? `${colorCodes.dim}${str}${colorCodes.reset}` : str

const IGNORED_PATHS = ['.well-known', 'favicon.ico']

// Morgan's token to receive content in a JSON format.
morgan.token('body-json', (request) => sanitizeBody(request.body))
morgan.token('local-date', () =>
  new Date().toLocaleString('en-GB', {
    timeZone: Intl.DateTimeFormat().resolvedOptions().timeZone,
    weekday: 'short',
    year: 'numeric',
    month: 'short',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
  }),
)

// Custom morgan configuration. Basically, copying the default output with a
// small change for the POST method.
export const morganLogger = morgan(function (tokens, request, response) {
  if (IGNORED_PATHS.some((path) => request.url.includes(path))) {
    return null
  }

  const method = tokens.method(request, response)
  const status = parseInt(tokens.status(request, response))
  const color = statusColor(status)
  const parts = [
    formatMethod(method),
    tokens.url(request, response),
    formatStatus(status, color),
    dim(`${tokens.res(request, response, 'content-length')}bytes`),
    dim(`${tokens['response-time'](request, response)}ms`),
    dim(tokens['local-date'](request, response)),
  ]

  return request.method === 'POST'
    ? parts.concat(tokens['body-json'](request, response)).join(' ¦ ')
    : parts.join(' ¦ ')
})

export const unknownEndpoint = (_request, response) => {
  response.status(404).send({ error: 'Unknown endpoint' })
}

// The error-handling middleware has to be the last loaded middleware, also all
// the routes should be registered before the error-handler.
export const errorHandler = (error, _request, response, next) => {
  logger.error(error.message)

  if (error.name === 'CastError') {
    return response.status(400).json({ error: SYSTEM_ERRORS.MALFORMATTED_ID })
  }

  if (error.name === 'ValidationError') {
    return response.status(400).json({ error: error.message })
  }

  if (
    error.name === 'MongoServerError' &&
    error.message.includes('E11000 duplicate key error')
  ) {
    return response.status(409).json({ error: SYSTEM_ERRORS.DUPLICATE_KEY })
  }

  if (error.name === 'JsonWebTokenError') {
    return response.status(401).json({ error: SYSTEM_ERRORS.INVALID_TOKEN })
  }

  if (error.name === 'TokenExpiredError') {
    return response.status(401).json({ error: SYSTEM_ERRORS.TOKEN_EXPIRED })
  }

  if (error instanceof ZodError) {
    // Fallback: catches ZodErrors thrown directly rather than via validate()
    return response
      .status(400)
      .json({ errors: error.issues.map((e) => e.message) })
  }

  if (error.statusCode) {
    return response.status(error.statusCode).json({
      error: error.message,
    })
  }

  next(error)
}
