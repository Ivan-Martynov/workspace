import { useState, useEffect } from 'react'
import { useNavigate } from 'react-router-dom'
import { ROUTES } from '../routes'

const RedirectConfirmation = ({
  message,
  targetName = 'main',
  target = ROUTES.HOME,
  countdownStart = 5,
}) => {
  const navigate = useNavigate()
  const [countdown, setCountdown] = useState(countdownStart)

  useEffect(() => {
    if (countdown > 0) {
      const timer = setTimeout(() => setCountdown((prev) => prev - 1), 1000)
      return () => clearTimeout(timer)
    }
    navigate(target)
  }, [navigate, countdown])

  const textSeconds = countdown === 1 ? 'second' : 'seconds'

  return (
    <div>
      <p>{message}</p>
      <p>
        You'll be redirected to {targetName} in {countdown} {textSeconds}
      </p>
      <button type='button' onClick={() => navigate(target)}>
        Go now
      </button>
    </div>
  )
}

export default RedirectConfirmation
