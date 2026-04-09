import { useTranslation } from 'react-i18next'

import { useAuth } from '../context/useAuth'
import { useFormSubmit, useInputField } from '../hooks'
import Message from './Message'

const RegisterForm = ({ onSuccess }) => {
  const { t } = useTranslation()
  const { register } = useAuth()

  const username = useInputField(t('fields.username'))
  const email = useInputField(t('fields.email'), { type: 'email' })
  const password = useInputField(t('fields.password'), { type: 'password' })
  const confirmPassword = useInputField(t('fields.confirmPassword'), {
    type: 'password',
  })

  const fields = [
    { field: username, key: 'username' },
    { field: email, key: 'email' },
    { field: password, key: 'password' },
    { field: confirmPassword, key: 'confirmPassword' },
  ]

  const { errorMessage, loading, handleSubmit } = useFormSubmit(async () => {
    if (password.value !== confirmPassword.value) {
      throw { code: 'passwordMismatch', params: {} }
    }
    const inputEmail = email.value.trim().toLowerCase()
    await register(username.value.trim(), inputEmail, password.value)
    onSuccess(inputEmail)
  })

  return (
    <form noValidate onSubmit={handleSubmit}>
      <Message text={errorMessage} type='error' />
      {fields.map(({ field, key }) => (
        <input key={key} {...field.inputProps} disabled={loading} />
      ))}
      <button
        type='submit'
        disabled={loading || !(password.value && confirmPassword.value)}
      >
        {loading ? t('register.submitting') : t('register.submit')}
      </button>
    </form>
  )
}

export default RegisterForm
