import { useEffect, useState } from 'react'
import { useParams } from 'react-router-dom'
import { sendVerifyToken } from '../services/auth'
import RedirectConfirmation from '../components/RedirectConfirmation'
import { ROUTES } from '../routes'

const VerifyConfirmationPage = () => {
  const { token } = useParams()

  const [message, setMessage] = useState('')
  const [error, setError] = useState('')
  const [loading, setLoading] = useState(false)

  useEffect(() => {
    const verify = async () => {
      setLoading(true)
      setError('')

      try {
        const response = await sendVerifyToken(token)
        setMessage(response.message)
      } catch (err) {
        setError(err.message)
      } finally {
        setLoading(false)
      }
    }

    verify()
  }, [token])

  if (loading) {
    return <p>Verifying...</p>
  }

  if (error) {
    return <p>{error}</p>
  }

  return (
    message && (
      <RedirectConfirmation
        message={message}
        targetName='login'
        target={ROUTES.AUTH}
      />
    )
  )
}

export default VerifyConfirmationPage
