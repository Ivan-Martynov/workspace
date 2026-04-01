import { Router } from 'express'

import { loginLimiter, passwordLimiter } from '../utils/rateLimiter.js'
import { validate } from '../utils/middleware.js'
import {
  forgotPasswordSchema,
  loginSchema,
  resendVerificationSchema,
  resetPasswordSchema,
} from '../validators/authValidator.js'
import { getFrontendUrl } from '../utils/url.js'
import authService from '../services/authService.js'

const authRouter = Router()

authRouter.post(
  '/login',
  loginLimiter,
  validate(loginSchema),
  async (request, response, next) => {
    try {
      const { identifier, password } = request.body
      const payload = await authService.loginUser({ identifier, password })
      response.json(payload)
    } catch (err) {
      next(err)
    }
  },
)

authRouter.post('/verify/:token', async (request, response, next) => {
  try {
    const payload = await authService.verifyEmail(request.params.token)
    response.json(payload)
  } catch (err) {
    next(err)
  }
})

authRouter.post(
  '/resend-verification',
  passwordLimiter,
  validate(resendVerificationSchema),
  async (request, response, next) => {
    try {
      const { identifier, redirectUrl } = request.body
      const payload = await authService.resendVerificationEmail(
        identifier,
        getFrontendUrl(redirectUrl),
      )
      response.json(payload)
    } catch (err) {
      next(err)
    }
  },
)

authRouter.post(
  '/forgot-password',
  passwordLimiter,
  validate(forgotPasswordSchema),
  async (request, response, next) => {
    try {
      const { email, redirectUrl } = request.body
      const payload = await authService.forgotPassword(
        email,
        getFrontendUrl(redirectUrl),
      )
      response.json(payload)
    } catch (err) {
      next(err)
    }
  },
)

authRouter.post(
  '/reset-password/:token',
  passwordLimiter,
  validate(resetPasswordSchema),
  async (request, response, next) => {
    try {
      const payload = await authService.resetPassword(
        request.params.token,
        request.body.password,
      )
      response.json(payload)
    } catch (err) {
      next(err)
    }
  },
)

export default authRouter
