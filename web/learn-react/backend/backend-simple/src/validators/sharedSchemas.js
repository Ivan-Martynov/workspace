import { z } from 'zod'
import config from '../utils/config.js'
import { VALIDATION_ERRORS } from '../utils/messages.js'

export const emailSchema = z
  .email(VALIDATION_ERRORS.INVALID_EMAIL)
  .toLowerCase()

export const passwordSchema = z
  .string()
  .min(config.PASSWORD_MIN_LENGTH, VALIDATION_ERRORS.PASSWORD_TOO_SHORT)
  .max(config.PASSWORD_MAX_LENGTH, VALIDATION_ERRORS.PASSWORD_TOO_LONG)
