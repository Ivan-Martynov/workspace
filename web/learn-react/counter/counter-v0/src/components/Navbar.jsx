import { useState, useEffect } from 'react'
import { Link } from 'react-router-dom'
import { Trans, useTranslation } from 'react-i18next'

import { useAuth } from '../context/useAuth'
import { ROUTES } from '../routes'
import DeleteAccountDialog from './DeleteAccountDialog'
import ThemeToggle from './ThemeToggle'
import LanguageSelect from './LanguageToggle'

const Navbar = () => {
  const { t } = useTranslation()
  const { user, logout } = useAuth()
  const [showDeleteDialog, setShowDeleteDialog] = useState(false)

  useEffect(() => {
    setShowDeleteDialog(false)
  }, [user])

  return (
    <nav>
      <Link to={ROUTES.HOME}>{t('navbar.game')}</Link>
      <LanguageSelect />
      <ThemeToggle />
      {user ? (
        <>
          <Trans
            i18nKey='navbar.loggedInAs'
            values={{ username: user.username }}
            components={{ strong: <strong></strong> }}
          />
          <button onClick={logout}>{t('common.logout')}</button>
          {showDeleteDialog ? (
            <DeleteAccountDialog onCancel={() => setShowDeleteDialog(false)} />
          ) : (
            <button onClick={() => setShowDeleteDialog(true)}>
              {t('delete.submit')}
            </button>
          )}
        </>
      ) : (
        <Link to={ROUTES.AUTH}>{t('login.submit')}</Link>
      )}
    </nav>
  )
}

export default Navbar
