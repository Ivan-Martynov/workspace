import { useNavigate } from 'react-router-dom'
import { Trans, useTranslation } from 'react-i18next'

import { useAuth } from '../context/useAuth'
import { ROUTES } from '../routes'
import { useFormSubmit, useInputField } from '../hooks'
import Message from './Message'

const DeleteAccountDialog = ({ onCancel }) => {
  const { t } = useTranslation()
  const { user, deleteAccount } = useAuth()

  const confirmText = useInputField(user.username)
  const navigate = useNavigate()

  const { errorMessage, loading, handleSubmit } = useFormSubmit(async () => {
    await deleteAccount()
    navigate(ROUTES.CONFIRM_ACCOUNT_DELETION)
  })

  return (
    <form onSubmit={handleSubmit}>
      <h3>{t('delete.title')}</h3>
      <Message text={errorMessage} type='error' />
      <p>
        <Trans
          i18nKey='delete.confirm'
          values={{ username: user.username }}
          components={{ strong: <strong></strong> }}
        />
      </p>
      <input {...confirmText.inputProps} disabled={loading} />
      <button
        type='submit'
        disabled={confirmText.value !== user.username || loading}
      >
        {loading ? t('delete.submitting') : t('delete.submit')}
      </button>
      <button type='button' onClick={onCancel} disabled={loading}>
        {t('common.cancel')}
      </button>
    </form>
  )
}

export default DeleteAccountDialog
