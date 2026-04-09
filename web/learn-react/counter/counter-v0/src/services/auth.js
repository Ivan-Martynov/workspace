import { api } from './api'

/**
 * Sends a login request to the API.
 * @param {string} identifier - Username or email.
 * @param {string} password
 * @returns {Promise<{token: string, user: {username: string, email: string}}>}
 */
export const loginRequest = async (identifier, password) =>
  api.post('/api/auth/login', {
    identifier,
    password,
  })

/**
 * Sends a registration request to the API.
 * @param {string} username
 * @param {string} email
 * @param {string} password
 * @returns {Promise<{message: string, userId: string}>}
 */
export const registerRequest = async (username, email, password) =>
  api.post('/api/users', {
    username,
    email,
    password,
    redirectUrl: `${window.location.origin}/verify-confirmation`,
  })

export const deleteAccountRequest = async (userId) =>
  api.delete(`/api/users/${userId}`)

export const requestPasswordReset = async (email) =>
  api.post('/api/auth/forgot-password', {
    email,
    redirectUrl: `${window.location.origin}/reset-password`,
  })

export const resetPasswordReset = async (token, password) =>
  api.post(`/api/auth/reset-password/${token}`, { password })

export const resendVerificationRequest = async (identifier) =>
  api.post('/api/auth/resend-verification', {
    identifier,
    redirectUrl: `${window.location.origin}/verify-confirmation`,
  })

export const sendVerifyEmailToken = async (token) =>
  api.post(`/api/auth/verify-email/${token}`)

export const sendVerifySessionToken = async () =>
  api.get(`/api/auth/verify-session/`)
