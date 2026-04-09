import { useEffect } from 'react'
import { useNavigate } from 'react-router-dom'
import { useTranslation } from 'react-i18next'

import { ROUTES } from '../routes'
import { useCountdown } from '../hooks'
import Message from './Message'

const RedirectConfirmation = ({
  message,
  targetName,
  target = ROUTES.HOME,
  countdownSeconds = 5,
}) => {
  const { t } = useTranslation()
  const navigate = useNavigate()

  const { countdown } = useCountdown(countdownSeconds)

  useEffect(() => {
    if (countdown <= 0) {
      navigate(target)
    }
  }, [navigate, countdown, target])

  return (
    <div>
      <Message text={message} type='success' />
      <p>
        {t('common.redirectIn', {
          target: targetName ?? t('common.toMainPage'),
          count: countdown,
        })}
      </p>
      <button type='button' onClick={() => navigate(target)}>
        {t('common.goNow')}
      </button>
    </div>
  )
}

export default RedirectConfirmation
