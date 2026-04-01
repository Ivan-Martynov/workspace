import { z } from 'zod'
import { emailSchema, passwordSchema } from './sharedSchemas.js'

export const loginSchema = z.object({
  identifier: z.string().min(1, 'Identifier is required').trim().toLowerCase(),
  password: z.string().min(1, 'Password is required'),
})

export const forgotPasswordSchema = z.object({
  email: emailSchema,
  redirectUrl: z.string().optional(),
})

export const resetPasswordSchema = z.object({
  password: passwordSchema,
})

export const resendVerificationSchema = z.object({
  identifier: z.string().min(1, 'Identifier is required').trim().toLowerCase(),
  redirectUrl: z.string().optional(),
})
