import { z } from 'zod'
import { emailSchema, passwordSchema } from './sharedSchemas.js'
import { VALIDATION_ERRORS } from '../utils/messages.js'

export const loginSchema = z.object({
  identifier: z
    .string()
    .min(1, VALIDATION_ERRORS.IDENTIFIER_REQUIRED)
    .trim()
    .toLowerCase(),
  password: z.string().min(1, VALIDATION_ERRORS.PASSWORD_REQUIRED),
})

export const forgotPasswordSchema = z.object({
  email: emailSchema,
  redirectUrl: z.string().optional(),
})

export const resetPasswordSchema = z.object({
  password: passwordSchema,
})

export const resendVerificationSchema = z.object({
  identifier: z
    .string()
    .min(1, VALIDATION_ERRORS.IDENTIFIER_REQUIRED)
    .trim()
    .toLowerCase(),
  redirectUrl: z.string().optional(),
})
