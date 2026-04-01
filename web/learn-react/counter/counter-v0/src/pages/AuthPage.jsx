import { useState } from 'react'

import LoginForm from '../components/LoginForm'
import RegisterForm from '../components/RegisterForm'
import ForgotPasswordForm from '../components/ForgotPasswordForm'

const AuthPage = () => {
  const [mode, setMode] = useState('login')
  const [successMessage, setSuccessMessage] = useState('')

  return (
    <div>
      {mode === 'login' && <LoginForm />}
      {mode === 'register' && <RegisterForm />}
      {mode === 'forgot' && (
        <ForgotPasswordForm
          onSuccess={(msg) => {
            setSuccessMessage(msg)
            setMode('login')
          }}
        />
      )}

      {successMessage && <p>{successMessage}</p>}

      {mode !== 'forgot' && (
        <button
          onClick={() => {
            setSuccessMessage('')
            setMode(mode === 'login' ? 'register' : 'login')
          }}
        >
          {mode === 'login'
            ? "Don't have an account? Register"
            : 'Already have an account? Login'}
        </button>
      )}

      {mode === 'login' && (
        <button
          onClick={() => {
            setSuccessMessage('')
            setMode('forgot')
          }}
        >
          Forgot password?
        </button>
      )}

      {mode === 'forgot' && (
        <button
          onClick={() => {
            setSuccessMessage('')
            setMode('login')
          }}
        >
          Back to login
        </button>
      )}
    </div>
  )
}

export default AuthPage
