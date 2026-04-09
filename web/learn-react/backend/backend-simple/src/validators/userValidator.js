import { z } from 'zod'
import { emailSchema, passwordSchema } from './sharedSchemas.js'
import { VALIDATION_ERRORS } from '../utils/messages.js'
import config from '../utils/config.js'

const usernameSchema = z
  .string()
  .trim()
  .toLowerCase()
  .min(config.USERNAME_MIN_LENGTH, VALIDATION_ERRORS.USERNAME_TOO_SHORT)
  .max(config.USERNAME_MAX_LENGTH, VALIDATION_ERRORS.USERNAME_TOO_LONG)
  .regex(/^[a-zA-Z0-9_]+$/, VALIDATION_ERRORS.USERNAME_INVALID_CHARS)

export const createUserSchema = z.object({
  username: usernameSchema,
  email: emailSchema,
  password: passwordSchema,
  redirectUrl: z.string().optional(),
})

export const updateUserSchema = z
  .object({
    username: usernameSchema.optional(),
    email: emailSchema.optional(),
    password: passwordSchema.optional(),
  })
  .refine((data) => Object.values(data).some((v) => v !== undefined), {
    message: VALIDATION_ERRORS.AT_LEAST_ONE_FIELD,
  })

export const updateRoleSchema = z.object({
  role: z.enum(['user', 'admin'], {
    errorMap: () => ({ message: VALIDATION_ERRORS.INVALID_ROLE }),
  }),
})
