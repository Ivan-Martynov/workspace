import { z } from 'zod'
import config from '../utils/config.js'

export const emailSchema = z
  .email('Please enter a valid email address')
  .toLowerCase()

export const passwordSchema = z
  .string()
  .min(
    config.PASSWORD_MIN_LENGTH,
    `Password must be at least ${config.PASSWORD_MIN_LENGTH} characters`,
  )
  .max(
    config.PASSWORD_MAX_LENGTH,
    `Password must be at most ${config.PASSWORD_MAX_LENGTH} characters`,
  )
