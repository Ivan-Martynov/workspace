import { useTranslation } from 'react-i18next'

export const useLanguage = () => {
  const { i18n } = useTranslation()

  const setLanguage = (lang) => {
    i18n.changeLanguage(lang)
    localStorage.setItem('language', lang)
  }

  const toggle = () => setLanguage(i18n.language === 'en' ? 'fi' : 'en')

  return { language: i18n.language, setLanguage, toggle }
}
