export class AppError extends Error {
  constructor(message, statusCode) {
    super(message || AppError.formatClassNameToMessage(new.target.name))
    this.statusCode = statusCode
    this.name = this.constructor.name
    Error.captureStackTrace(this, this.constructor)
  }

  static formatClassNameToMessage(className) {
    const nameWithoutError = className.replace(/Error$/, '')
    const words = nameWithoutError.replace(/([a-z0-9])[A-Z]/g, '$1 $2')
    return words.charAt(0).toUpperCase() + words.slice(1).toLowerCase()
  }

  toJSON() {
    return { error: this.message }
  }
}

export class BadRequestError extends AppError {
  constructor(message) {
    super(message, 400)
    this.name = 'BadRequest'
  }
}

export class UnauthorizedError extends AppError {
  constructor(message) {
    super(message, 401)
  }
}

export class ForbiddenError extends AppError {
  constructor(message) {
    super(message, 403)
  }
}

export class NotFoundError extends AppError {
  constructor(message) {
    super(message, 404)
  }
}

export class ConflictError extends AppError {
  constructor(message) {
    super(message, 409)
  }
}
