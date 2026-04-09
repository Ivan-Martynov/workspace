import { useTranslation } from 'react-i18next'

import { resendVerificationRequest } from '../services/auth'
import { useCountdown, useFormSubmit } from '../hooks'
import Message from './Message'
import { useState } from 'react'

const VerifyEmailNotice = ({ email }) => {
  const { t } = useTranslation()
  const { countdown, reset } = useCountdown()
  const [messageId, setMessageId] = useState(0)

  const { errorMessage, loading, handleSubmit } = useFormSubmit(async () => {
    await resendVerificationRequest(email)
    reset(3)
    setMessageId((prev) => prev + 1)
  })

  return (
    <div>
      {errorMessage ? (
        <Message text={errorMessage} type='error' />
      ) : (
        <Message
          id={messageId}
          text={t('verifyEmail.notice.checkInbox')}
          type='success'
          duration={5000}
        />
      )}

      <form onSubmit={handleSubmit}>
        <button type='submit' disabled={loading || countdown > 0}>
          {countdown > 0
            ? t('verifyEmail.notice.resendIn', { count: countdown })
            : loading
              ? t('verifyEmail.notice.resending')
              : t('verifyEmail.notice.resend')}
        </button>
      </form>
    </div>
  )
}

export default VerifyEmailNotice
