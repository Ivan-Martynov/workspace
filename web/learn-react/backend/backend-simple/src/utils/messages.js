export const AUTH_ERRORS = {
  LOGIN_FAILED: 'invalidCredentials',
  REMIND_VERIFY: 'emailNotVerified',
  VERIFY_FAILED: 'invalidVerificationToken',
  PASSWORD_RESET_FAILED: 'invalidResetToken',
}

export const HTTP_ERRORS = {
  TOO_MANY_REQUESTS: 'tooManyRequests',
}

export const SYSTEM_ERRORS = {
  MALFORMATTED_ID: 'malformattedId',
  DUPLICATE_KEY: 'duplicateKey',
  INVALID_TOKEN: 'invalidToken',
  TOKEN_EXPIRED: 'tokenExpired',
}

export const VALIDATION_ERRORS = {
  INVALID_EMAIL: 'invalidEmail',
  PASSWORD_TOO_SHORT: 'passwordTooShort',
  PASSWORD_TOO_LONG: 'passwordTooLong',
  USERNAME_TOO_SHORT: 'usernameTooShort',
  USERNAME_TOO_LONG: 'usernameTooLong',
  USERNAME_INVALID_CHARS: 'usernameInvalidChars',
  IDENTIFIER_REQUIRED: 'identifierRequired',
  PASSWORD_REQUIRED: 'passwordRequired',
  AT_LEAST_ONE_FIELD: 'atLeastOneField',
  INVALID_ROLE: 'invalidRole',
}

export const AUTH_SUCCESS = {
  REGISTER: 'registrationSuccess',
  FORGOT_PASSWORD: 'forgotPasswordSuccess',
  RESEND_VERIFICATION: 'resendVerificationSuccess',
  EMAIL_VERIFIED: 'emailVerified',
  PASSWORD_RESET: 'passwordReset',
}
