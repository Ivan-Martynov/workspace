export const MODES = {
  DEVELOPMENT: 'development',
  PRODUCTION: 'production',
  TEST: 'test',
}
const PORT = process.env.PORT
const MODE = process.env.NODE_ENV

// Test environment uses separate DB and JWT secret to isolate test data.
const isTestEnv = MODE === MODES.TEST
const MONGODB_URI = isTestEnv
  ? process.env.TEST_MONGODB_URI
  : process.env.MONGODB_URI
const JWT_SECRET = isTestEnv
  ? process.env.TEST_JWT_SECRET
  : process.env.JWT_SECRET

// short expiry in test to catch token expiry bugs
const JWT_EXPIRES = isTestEnv ? '15m' : '1h'
const VERIFICATION_TOKEN_EXPIRES_MS = 24 * 60 * 60 * 1000
const RESET_TOKEN_EXPIRES_MS = 60 * 60 * 1000

// 1 in test mode for faster hashing
const SALT_ROUNDS = isTestEnv ? 1 : 10
const PASSWORD_MIN_LENGTH = 8
const PASSWORD_MAX_LENGTH = 128

const ALLOWED_ORIGINS = process.env.ALLOWED_ORIGINS
  ? process.env.ALLOWED_ORIGINS.split(',').map((origin) => origin.trim())
  : []

const RESEND_API_KEY = process.env.RESEND_API_KEY
const FROM_EMAIL = process.env.FROM_EMAIL
const APP_URL = process.env.APP_URL
const FRONTEND_URL = process.env.FRONTEND_URL

export default {
  PORT,
  MODE,
  MONGODB_URI,
  JWT_SECRET,
  SALT_ROUNDS,
  JWT_EXPIRES,
  VERIFICATION_TOKEN_EXPIRES_MS,
  RESET_TOKEN_EXPIRES_MS,
  PASSWORD_MIN_LENGTH,
  PASSWORD_MAX_LENGTH,
  ALLOWED_ORIGINS,
  RESEND_API_KEY,
  FROM_EMAIL,
  APP_URL,
  FRONTEND_URL,
}
