import { useState } from 'react'
import { useParams } from 'react-router-dom'
import { useTranslation } from 'react-i18next'

import { resetPasswordReset } from '../services/auth'
import RedirectConfirmation from '../components/RedirectConfirmation'
import { ROUTES } from '../routes'
import { useFormSubmit, useInputField } from '../hooks'
import Message from '../components/Message'

const ResetPasswordPage = () => {
  const { t } = useTranslation()
  const { token } = useParams()

  const password = useInputField(t('fields.newPassword'), { type: 'password' })
  const confirmPassword = useInputField(t('fields.confirmPassword'), {
    type: 'password',
  })

  const [messageKey, setMessageKey] = useState('')

  const { errorMessage, loading, handleSubmit } = useFormSubmit(async () => {
    if (password.value !== confirmPassword.value) {
      throw { code: 'passwordMismatch', params: {} }
    }

    const response = await resetPasswordReset(token, password.value)
    setMessageKey(response.message)

    password.reset()
    confirmPassword.reset()
  })

  return (
    <main>
      <h1>{t('forgot.setNewTitle')}</h1>
      {messageKey ? (
        <RedirectConfirmation
          message={t(`success.${messageKey}`)}
          targetName='login'
          target={ROUTES.AUTH}
          countdownSeconds={5}
        />
      ) : (
        <form onSubmit={handleSubmit}>
          <Message text={errorMessage} type='error' />
          <input {...password.inputProps} />
          <input {...confirmPassword.inputProps} />

          <button
            type='submit'
            disabled={loading || !(password.value && confirmPassword.value)}
          >
            {loading ? t('forgot.resetting') : t('forgot.reset')}
          </button>
        </form>
      )}
    </main>
  )
}

export default ResetPasswordPage
