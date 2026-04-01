import { Router } from 'express'

import {
  isAdmin,
  isOwnerOrAdmin,
  requireAuth,
  userExtractor,
} from '../utils/auth.js'
import { validate } from '../utils/middleware.js'
import {
  createUserSchema,
  updateRoleSchema,
  updateUserSchema,
} from '../validators/userValidator.js'
import { getFrontendUrl } from '../utils/url.js'
import userService from '../services/userService.js'
import authService from '../services/authService.js'

const usersRouter = Router()

usersRouter.get('/', async (request, response, next) => {
  try {
    const page = Math.max(1, parseInt(request.query.page) || 1)
    const limit = Math.min(100, parseInt(request.query.limit) || 20)

    const serviceResponse = await userService.getUsers({ page, limit })
    response.json(serviceResponse)
  } catch (err) {
    next(err)
  }
})

usersRouter.get('/:id', async (request, response, next) => {
  try {
    const user = await userService.getUserById(request.params.id)
    response.json(user)
  } catch (err) {
    next(err)
  }
})

usersRouter.post(
  '/',
  validate(createUserSchema),
  async (request, response, next) => {
    try {
      const { username, email, password, redirectUrl } = request.body
      const frontendUrl = getFrontendUrl(redirectUrl)
      console.log(`[REGISTER]: ${frontendUrl}`)

      const payload = await authService.registerUser({
        username,
        email,
        password,
        frontendUrl,
      })

      response.status(201).json(payload)
    } catch (err) {
      next(err)
    }
  },
)

usersRouter.delete(
  '/:id',
  userExtractor,
  requireAuth,
  isOwnerOrAdmin,
  async (request, response, next) => {
    try {
      await userService.deleteUser(request.params.id)
      response.status(204).end()
    } catch (err) {
      next(err)
    }
  },
)

usersRouter.patch(
  '/:id/role',
  userExtractor,
  requireAuth,
  isAdmin,
  validate(updateRoleSchema),
  async (request, response, next) => {
    try {
      const user = await userService.updateUserRole(
        request.params.id,
        request.body.role,
      )
      response.json(user)
    } catch (err) {
      next(err)
    }
  },
)

usersRouter.patch(
  '/:id',
  userExtractor,
  requireAuth,
  isOwnerOrAdmin,
  validate(updateUserSchema),
  async (request, response, next) => {
    try {
      const { username, email, password } = request.body
      const updatedUser = await userService.updateUserProfile(
        request.params.id,
        {
          username,
          email,
          password,
        },
      )

      response.json(updatedUser)
    } catch (err) {
      next(err)
    }
  },
)

export default usersRouter
