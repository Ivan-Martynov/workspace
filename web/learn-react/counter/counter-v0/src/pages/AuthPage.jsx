import { useState } from 'react'
import { useTranslation } from 'react-i18next'

import LoginForm from '../components/LoginForm'
import RegisterForm from '../components/RegisterForm'
import ForgotPasswordForm from '../components/ForgotPasswordForm'
import VerifyEmailNotice from '../components/VerifyEmailNotice'
import Message from '../components/Message'

const getModes = (t) => ({
  login: t('login.submit'),
  register: t('register.submit'),
  forgot: t('auth.forgottenPassword'),
  verify: t('verifyEmail.notice.title'),
})

const AuthPage = () => {
  const { t } = useTranslation()
  // 'login' | 'register' | 'forgot' | 'verify'
  const [mode, setMode] = useState('login')
  const [successKey, setSuccessKey] = useState('')
  const [verificationEmail, setVerificationEmail] = useState('')

  const switchMode = (newMode, key = '') => {
    setSuccessKey(key)
    setMode(newMode)
  }

  return (
    <main>
      <h1>{getModes(t)[mode]}</h1>
      <Message
        id={successKey}
        text={successKey ? t(`success.${successKey}`) : ''}
        type='success'
        duration={5000}
      />
      <div>
        {mode === 'login' && <LoginForm />}
        {mode === 'register' && (
          <RegisterForm
            onSuccess={(email) => {
              setVerificationEmail(email)
              switchMode('verify')
            }}
          />
        )}
        {mode === 'forgot' && (
          <ForgotPasswordForm
            onSuccess={(msg) => {
              switchMode('login', msg)
            }}
          />
        )}
        {mode === 'verify' && <VerifyEmailNotice email={verificationEmail} />}

        {(mode === 'login' || mode === 'register') && (
          <button
            onClick={() => {
              switchMode(mode === 'login' ? 'register' : 'login')
            }}
          >
            {mode === 'login' ? t('auth.noAccount') : t('auth.hasAccount')}
          </button>
        )}

        {mode === 'login' && (
          <button
            onClick={() => {
              switchMode('forgot')
            }}
          >
            {t('auth.forgotPassword')}
          </button>
        )}

        {mode !== 'login' && mode !== 'register' && (
          <button
            onClick={() => {
              switchMode('login')
            }}
          >
            {t('auth.backToLogin')}
          </button>
        )}
      </div>
    </main>
  )
}

export default AuthPage
