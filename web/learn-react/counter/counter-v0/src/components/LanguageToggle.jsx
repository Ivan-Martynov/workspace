import { useLanguage } from '../hooks'

const LanguageToggle = () => {
  const { language, setLanguage } = useLanguage()
  return (
    <select value={language} onChange={(ev) => setLanguage(ev.target.value)}>
      <option value='en'>English</option>
      <option value='fi'>Finnish</option>
    </select>
  )
}

export default LanguageToggle
