import { useState } from 'react'
import { useParams } from 'react-router-dom'
import { resetPasswordReset } from '../services/auth'
import RedirectConfirmation from '../components/RedirectConfirmation'
import { ROUTES } from '../routes'
import { useInputField } from '../hooks'

const ResetPasswordPage = () => {
  const { token } = useParams()

  const password = useInputField('new password', { type: 'password' })
  const confirmPassword = useInputField('confirm password', {
    type: 'password',
  })
  const fields = [password, confirmPassword]

  const [loading, setLoading] = useState(false)
  const [message, setMessage] = useState('')
  const [error, setError] = useState('')

  const handleSubmit = async (ev) => {
    ev.preventDefault()
    setError('')
    setMessage('')

    if (password.value !== confirmPassword.value) {
      setError('Passwords do not match')
      return
    }

    setLoading(true)
    try {
      const response = await resetPasswordReset(token, password.value)
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

  if (!token) {
    return <p>Invalid or missing token</p>
  }

  return message ? (
    <RedirectConfirmation
      message={message}
      targetName='login'
      target={ROUTES.AUTH}
    />
  ) : (
    <form onSubmit={handleSubmit}>
      <h2>Set new password</h2>

      {error && <p>{error}</p>}
      {fields.map((f) => (
        <input key={f.inputProps.placeholder} {...f.inputProps} />
      ))}

      <button
        type='submit'
        disabled={loading || !(password.value && confirmPassword.value)}
      >
        Reset password
      </button>
    </form>
  )
}

export default ResetPasswordPage
