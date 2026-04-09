import { useState, useEffect } from 'react'

export const useCountdown = (initialValue = 0) => {
  const [countdown, setCountdown] = useState(initialValue)

  useEffect(() => {
    if (countdown > 0) {
      const timer = setTimeout(() => setCountdown((prev) => prev - 1), 1000)
      return () => clearTimeout(timer)
    }
  }, [countdown])

  const reset = (seconds) => setCountdown(seconds)

  return { countdown, reset }
}
