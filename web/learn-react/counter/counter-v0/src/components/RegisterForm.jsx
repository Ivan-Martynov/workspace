import { useAuth } from '../context/useAuth'
import { useInputField } from '../hooks'
import VerifyEmailNotice from './VerifyEmailNotice'
import { PASSWORD_MIN_LENGTH } from '../services/constants'
import { useState } from 'react'

const RegisterForm = () => {
  const { register } = useAuth()

  const username = useInputField('username')
  const email = useInputField('email', { type: 'email' })
  const password = useInputField('password', { type: 'password' })
  const confirmPassword = useInputField('confirm password', {
    type: 'password',
  })

  const fields = [username, email, password, confirmPassword]

  const [message, setMessage] = useState('')
  const [error, setError] = useState('')
  const [verificationEmail, setVerificationEmail] = useState('')
  const [loading, setLoading] = useState(false)

  const handleSubmit = async (ev) => {
    ev.preventDefault()
    setError('')

    if (password.value.length < PASSWORD_MIN_LENGTH) {
      setError('Password must be at least 8 characters')
      return
    }

    if (password.value !== confirmPassword.value) {
      setError('Passwords do not match')
      return
    }

    setLoading(true)
    try {
      const inputEmail = email.value.trim().toLowerCase()
      const response = await register(
        username.value.trim(),
        inputEmail,
        password.value,
      )
      setVerificationEmail(inputEmail)
      setMessage(response.message)

      for (const f of fields) {
        f.reset()
      }
    } catch (err) {
      setError(err.message)
    } finally {
      setLoading(false)
    }
  }

  return verificationEmail ? (
    <VerifyEmailNotice email={verificationEmail} message={message} />
  ) : (
    <form onSubmit={handleSubmit}>
      {error && <p>{error}</p>}
      <div>
        {fields.map((f) => (
          <input key={f.inputProps.placeholder} {...f.inputProps} />
        ))}
      </div>
      <button
        type='submit'
        disabled={loading || !(password.value && confirmPassword.value)}
      >
        {loading ? 'Registering...' : 'Register'}
      </button>
    </form>
  )
}

export default RegisterForm
