import { useState } from 'react'
import { useAuth } from '../context/useAuth'
import { useInputField } from '../hooks'

const LoginForm = () => {
  const { login } = useAuth()

  const identifier = useInputField('username or email')
  const password = useInputField('password', { type: 'password' })
  const fields = [identifier, password]

  const [error, setError] = useState('')
  const [loading, setLoading] = useState(false)

  const handleSubmit = async (ev) => {
    ev.preventDefault()
    setError('')
    setLoading(true)
    try {
      await login(identifier.value.trim(), password.value)
      for (const f of fields) {
        f.reset()
      }
    } catch (err) {
      setError(err.message)
    } finally {
      setLoading(false)
    }
  }

  return (
    <form onSubmit={handleSubmit}>
      {error && <p>{error}</p>}
      {fields.map((f) => (
        <input key={f.inputProps.placeholder} {...f.inputProps} />
      ))}
      <button
        type='submit'
        disabled={loading || !(identifier.value && password.value)}
      >
        {loading ? 'Logging in...' : 'Login'}
      </button>
    </form>
  )
}

export default LoginForm
