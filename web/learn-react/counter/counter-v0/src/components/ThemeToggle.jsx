import { useTranslation } from 'react-i18next'

import { useTheme } from '../hooks'

const ThemeToggle = () => {
  const { t } = useTranslation()
  const { theme, toggle } = useTheme()

  return (
    <button
      onClick={toggle}
      title={
        theme === 'light' ? t('theme.switchToDark') : t('theme.switchToLight')
      }
    >
      {theme === 'light' ? t('theme.dark') : t('theme.light')}
    </button>
  )
}

export default ThemeToggle
