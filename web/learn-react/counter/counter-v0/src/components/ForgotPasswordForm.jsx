import { useState } from 'react'
import { requestPasswordReset } from '../services/auth'

const ForgotPasswordForm = ({ onSuccess }) => {
  const [email, setEmail] = useState('')
  const [error, setError] = useState('')
  const [loading, setLoading] = useState(false)

  const handleSubmit = async (ev) => {
    ev.preventDefault()
    setError('')
    setLoading(true)

    try {
      const response = await requestPasswordReset(email.trim())
      setEmail('')
      onSuccess(response.message)
    } catch (err) {
      setError(err.message)
    } finally {
      setLoading(false)
    }
  }

  return (
    <form onSubmit={handleSubmit}>
      {error && (
        <p className='error' role='alert'>
          {error}
        </p>
      )}
      <input
        type='email'
        value={email}
        onChange={(ev) => setEmail(ev.target.value)}
        placeholder='email'
      />
      <button type='submit' disabled={loading || !email.trim()}>
        {loading ? 'Sending...' : 'Send reset link'}
      </button>
    </form>
  )
}

export default ForgotPasswordForm
