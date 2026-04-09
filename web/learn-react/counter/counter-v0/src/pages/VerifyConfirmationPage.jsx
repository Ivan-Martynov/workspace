import { useEffect, useState } from 'react'
import { useParams } from 'react-router-dom'
import { useTranslation } from 'react-i18next'

import { sendVerifyEmailToken } from '../services/auth'
import RedirectConfirmation from '../components/RedirectConfirmation'
import { ROUTES } from '../routes'
import Message from '../components/Message'

const VerifyConfirmationPage = () => {
  const { t } = useTranslation()
  const { token } = useParams()

  const [messageKey, setMessageKey] = useState('')
  const [errorCode, setErrorCode] = useState('')
  const [loading, setLoading] = useState(false)

  useEffect(() => {
    const verify = async () => {
      setLoading(true)
      setErrorCode('')

      try {
        const response = await sendVerifyEmailToken(token)
        setMessageKey(response.message)
      } catch (err) {
        setErrorCode(err.code ?? err.message)
      } finally {
        setLoading(false)
      }
    }
    verify()
  }, [token])

  return (
    <main>
      <h1>{t('verifyEmail.confirmation.title')}</h1>
      {loading ? (
        <p>{t('verifyEmail.confirmation.verifying')}</p>
      ) : errorCode ? (
        <Message text={t(`errors.${errorCode}`)} type='error' />
      ) : (
        messageKey && (
          <RedirectConfirmation
            message={t(`success.${messageKey}`)}
            targetName='login'
            target={ROUTES.AUTH}
            countdownSeconds={5}
          />
        )
      )}
    </main>
  )
}

export default VerifyConfirmationPage
