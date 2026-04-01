import { Router } from 'express'
import User from '../models/User.js'

const infoRouter = Router()

const aboutInfo = () => {
  return 'Simple app to manage user accounts.'
}

const timeDateInfo = () => {
  const date = new Date()
  return `${date.toDateString()}, ${date.toTimeString()}.`
}

const userInfo = async () => {
  const n = await User.countDocuments()
  return n < 1
    ? 'App has no users.'
    : `App has info for ${n} user${n > 1 ? 's' : ''}.`
}

infoRouter.get('/', async (_request, response, next) => {
  try {
    const about = aboutInfo()
    const time = timeDateInfo()
    const users = await userInfo()
    response.json({ about, time, users })
  } catch (error) {
    next(error)
  }
})

infoRouter.get('/about', (_request, response) => {
  response.json({ about: aboutInfo() })
})

infoRouter.get('/time', (_request, response) => {
  response.json({ time: timeDateInfo() })
})

infoRouter.get('/users', async (_request, response, next) => {
  try {
    response.json({ users: await userInfo() })
  } catch (error) {
    next(error)
  }
})

export default infoRouter
