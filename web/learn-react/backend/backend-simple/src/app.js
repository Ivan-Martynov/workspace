import 'dotenv/config'
import config from './utils/config.js'

import express from 'express'
import helmet from 'helmet'
import cors from 'cors'
import {
  errorHandler,
  morganLogger,
  unknownEndpoint,
} from './utils/middleware.js'

import usersRouter from './controllers/usersRouter.js'
import infoRouter from './controllers/infoRouter.js'
import authRouter from './controllers/authRouter.js'

const app = express()

app.use(helmet())

app.use(
  cors({
    origin: config.ALLOWED_ORIGINS,
    methods: ['GET', 'POST', 'PATCH', 'DELETE'],
    allowedHeaders: ['Content-Type', 'Authorization'],
  }),
)

app.use(express.static('dist'))
app.use(express.json())
app.use(morganLogger)

app.use('/api/users', usersRouter)
app.use('/api/info', infoRouter)
app.use('/api/auth', authRouter)

app.use(unknownEndpoint)
app.use(errorHandler)

export default app
