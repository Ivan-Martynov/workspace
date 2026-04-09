import { useTranslation } from 'react-i18next'

import { useAuth } from '../context/useAuth'
import { useInputField, useFormSubmit } from '../hooks'
import Message from './Message'

const LoginForm = () => {
  const { t } = useTranslation()
  const { login } = useAuth()

  const identifier = useInputField(t('fields.usernameOrEmail'))
  const password = useInputField(t('fields.password'), { type: 'password' })

  const { errorMessage, loading, handleSubmit } = useFormSubmit(async () => {
    await login(identifier.value.trim(), password.value)
    identifier.reset()
    password.reset()
  })

  return (
    <form noValidate onSubmit={handleSubmit}>
      <Message text={errorMessage} type='error' />
      <input {...identifier.inputProps} disabled={loading} />
      <input {...password.inputProps} disabled={loading} />
      <button
        type='submit'
        disabled={loading || !(identifier.value && password.value)}
      >
        {loading ? t('login.submitting') : t('login.submit')}
      </button>
    </form>
  )
}

export default LoginForm
