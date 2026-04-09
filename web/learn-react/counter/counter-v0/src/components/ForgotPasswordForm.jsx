import { requestPasswordReset } from '../services/auth'
import { useTranslation } from 'react-i18next'

import { useFormSubmit, useInputField } from '../hooks'
import Message from './Message'

const ForgotPasswordForm = ({ onSuccess }) => {
  const { t } = useTranslation()
  const email = useInputField(t('fields.email'), { type: 'email' })

  const { errorMessage, loading, handleSubmit } = useFormSubmit(async () => {
    const response = await requestPasswordReset(email.value.trim())
    onSuccess(response.message)
    email.reset()
  })

  return (
    <form noValidate onSubmit={handleSubmit}>
      <Message text={errorMessage} type='error' />
      <input {...email.inputProps} />
      <button type='submit' disabled={loading || !email.value.trim()}>
        {loading ? t('forgot.submitting') : t('forgot.submit')}
      </button>
    </form>
  )
}

export default ForgotPasswordForm
