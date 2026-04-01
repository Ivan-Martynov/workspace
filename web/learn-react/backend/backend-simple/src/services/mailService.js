import { Resend } from 'resend'

import config, { MODES } from '../utils/config.js'
import logger from '../utils/logger.js'

let resend = null
const getResend = () => {
  if (config.MODE === MODES.TEST) {
    return null
  }
  if (!resend) {
    resend = new Resend(config.RESEND_API_KEY)
  }
  return resend
}

const sendEmail = async ({ to, subject, html }) => {
  try {
    const client = getResend()
    if (!client) {
      return
    }
    await client.emails.send({
      from: config.FROM_EMAIL,
      to,
      subject,
      html,
    })
  } catch (err) {
    logger.error({
      msg: 'Email send failed',
      to,
      subject,
      error: err.message,
    })
  }
}

/**
 * Sends a verification email with a link the user clicks to confirm
 * their email address.
 * @param {string} to - Recipient email address
 * @param {string} token - The raw verification token from the database
 * @param {string} baseUrl - Base URL of the frontend
    (e.g. 'http://localhost:3000')
 * @returns {Promise<void>}
 */
const sendVerificationEmail = (to, token, redirectUrl) => {
  // const url = `${baseUrl}/api/auth/verify/${token}`
  const url = redirectUrl
    ? `${redirectUrl}/${token}`
    : `${config.APP_URL}/api/auth/verify/${token}`

  return sendEmail({
    to,
    subject: 'Verify your email address',
    html: `
      <p>Thanks for signing up. Please verify your email address:</p>
      <a href="${url}">Click here</a>
      <p>This link expires in 24 hours.</p>
      <p>If you didn't request this, ignore this email.</p>
      `,
  })
}

/**
 * Sends a password reset email with a one-hour expiry link.
 * @param {string} to - Recipient email address
 * @param {string} token - The raw reset token from the database
 * @param {string} redirectUrl - Base URL of the frontend
    (e.g. 'http://localhost:3000')
 * @returns {Promise<void>}
 */
const sendPasswordResetEmail = (to, token, redirectUrl) => {
  const url = redirectUrl
    ? `${redirectUrl}/${token}`
    : `${config.APP_URL}/api/auth/reset-password/${token}`

  return sendEmail({
    to,
    subject: 'Reset your password',
    html: `
      <p>You requested a password reset. Use the link below:</p>
      <a href="${url}">Click here</a>
      <p>This link expires in 1 hour. If you didn't request this, ignore this email.</p>
      `,
  })
}

export default { sendVerificationEmail, sendPasswordResetEmail }
