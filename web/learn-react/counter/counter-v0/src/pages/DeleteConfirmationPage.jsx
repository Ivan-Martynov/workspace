import { useTranslation } from 'react-i18next'

import { useAuth } from '../context/useAuth'
import RedirectConfirmation from '../components/RedirectConfirmation'

const DeleteConfirmationPage = () => {
  const { t } = useTranslation()
  const { user } = useAuth()

  return (
    <main>
      <h1>{t('delete.confirmTitle')}</h1>
      {user ? (
        <p>{t('common.waitingUser')}</p>
      ) : (
        <RedirectConfirmation message={t('delete.successMessage')} />
      )}
    </main>
  )
}

export default DeleteConfirmationPage
