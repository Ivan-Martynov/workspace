import { useEffect } from 'react'
import { useState } from 'react'
import { resendVerificationRequest } from '../services/auth'

const VerifyEmailNotice = ({ email, message }) => {
  const [error, setError] = useState('')
  const [countdown, setCountdown] = useState(0)
  const [loading, setLoading] = useState(false)

  const handleResend = async () => {
    setError('')

    if (loading || countdown > 0) {
      return
    }

    setLoading(true)
    try {
      await resendVerificationRequest(email)
      setCountdown(30)
    } catch (err) {
      setError(err.message)
    } finally {
      setLoading(false)
    }
  }

  useEffect(() => {
    if (countdown > 0) {
      const timer = setTimeout(() => setCountdown((prev) => prev - 1), 1000)
      return () => clearTimeout(timer)
    }
  }, [countdown])

  return (
    <div>
      <h2>Verify your email</h2>
      {error ? (
        <p>{error}</p>
      ) : (
        <p>If the address is available, a verification email has been sent.</p>
      )}

      <button onClick={handleResend} disabled={loading || countdown > 0}>
        {countdown > 0
          ? `Resend in ${countdown}s`
          : 'Resend verification email'}
      </button>
    </div>
  )
}

export default VerifyEmailNotice
