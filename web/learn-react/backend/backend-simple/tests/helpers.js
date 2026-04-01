import mongoose from 'mongoose'
import User from '../src/models/User.js'
import config from '../src/utils/config.js'

export const TEST_PASSWORD = 'secret123'

/**
 * Connect to MongoDB using Mongoose.
 * @async
 * @returns {Promise<mongoose.Connection>} The Mongoose connection.
 */
export const connectDb = async () => await mongoose.connect(config.MONGODB_URI)

/**
 * Connect to MongoDB using Mongoose.
 * @async
 * @returns {Promise<mongoose.Connection>} The Mongoose connection.
 */
export const disconnectDb = async () => await mongoose.connection.close()

/**
 * Remove all users from the database.
 * @async
 * @returns {Promise<void>}
 */
export const clearDb = async () => await User.deleteMany({})

/**
 * Create a new user via API.
 * @param {import('supertest').SuperTest<import('supertest').Test>} api -
    SuperTest instance
 * @param {object} userData - Object with username, email, password
 * @returns {import('supertest').Test} SuperTest chain (can chain `.expect()`)
 */
export const createUser = (api, userData) => {
  return api
    .post('/api/users')
    .send(userData)
    .expect('Content-Type', /application\/json/)
}

/**
 * Creates a test user and returns the response body.
 * @async
 * @param {import('supertest').SuperTest<import('supertest').Test>} api -
    SuperTest instance
 * @param {string} name - Username for the test user
 * @returns {Promise<{userId: string, message: string}>} The API response body
 */
export const makeTestUser = async (api, name = 'testuser') => {
  const response = await createUser(api, {
    username: name,
    email: `${name}@example.com`,
    password: TEST_PASSWORD,
  })
  return response.body
}

/**
 * Marks an existing user as verified by setting the `verified` flag to `true`
 * and clearing the `verificationToken` directly in the database.
 * @async
 * @param {string} userId - The MongoDB ObjectId string of the user to verify.
 * @returns {Promise<void>}
 */
export const verifyTestUser = async (userId) => {
  await User.findByIdAndUpdate(userId, {
    verified: true,
    verificationToken: null,
  })
}

/**
 * Creates a test user and immediately marks them as verified.
 * Combines {@link makeTestUser} and {@link verifyTestUser} for convenience.
 * @async
 * @param {import('supertest').SuperTest<import('supertest').Test>} api -
    SuperTest instance
 * @param {string} [name='testuser'] - Username for the test user
 * @returns {Promise<{userId: string, message: string}>} The API response body
    from user creation
 */
export const makeVerifiedUser = async (api, name = 'testuser') => {
  const body = await makeTestUser(api, name)
  await verifyTestUser(body.userId)
  return body
}

/**
 * Creates an admin user, verifies them, and returns a valid JWT token.
 * The created user has the username `admin` and email `admin@example.com`.
 * @async
 * @param {import('supertest').SuperTest<import('supertest').Test>} api -
    SuperTest instance
 * @returns {Promise<string>} A JWT token for the authenticated admin user
 */
export const makeAdminUser = async (api) => {
  const { body } = await api.post('/api/users').send({
    username: 'admin',
    email: 'admin@example.com',
    password: TEST_PASSWORD,
  })
  await User.findByIdAndUpdate(body.userId, {
    role: 'admin',
    verified: true,
    verificationToken: null,
  })
  return (
    await api
      .post('/api/auth/login')
      .send({ identifier: 'admin', password: TEST_PASSWORD })
  ).body.token
}

/**
 * Fetches a single user by ID via the API.
 * @param {import('supertest').SuperTest<import('supertest').Test>} api -
    SuperTest instance
 * @param {string} id - The MongoDB ObjectId string of the user to retrieve
 * @returns {import('supertest').Test} SuperTest chain (can chain `.expect()`)
 */
export const getUser = (api, id) => api.get(`/api/users/${id}`)

/**
 * Logs in a user via API.
 * @async
 * @param {import('supertest').SuperTest<import('supertest').Test>} api -
    SuperTest instance
 * @param {string} identifier - Username or email
 * @param {string} [password=TEST_PASSWORD] - Password
 * @returns {Promise<{token: string, user: object}>} JWT token and user object
 */
export const loginUser = async (api, identifier, password = TEST_PASSWORD) => {
  const response = await api
    .post('/api/auth/login')
    .send({ identifier, password })
    .expect(200)
  return {
    token: response.body.token,
    user: response.body.user,
  }
}

/**
 * Generates the Authorization header for a user.
 * @async
 * @param {import('supertest').SuperTest<import('supertest').Test>} api -
    SuperTest instance
 * @param {string} identifier - Username or email
 * @param {string} [password=TEST_PASSWORD] - Password
 * @returns {Promise<{Authorization: string}>} Header object
 */
export const getAuthHeader = async (
  api,
  identifier,
  password = TEST_PASSWORD,
) => {
  const { token } = await loginUser(api, identifier, password)
  return { Authorization: `Bearer ${token}` }
}
