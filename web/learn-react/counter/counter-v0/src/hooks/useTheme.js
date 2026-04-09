import { useState, useEffect } from 'react'

export const useTheme = () => {
  const [theme, setTheme] = useState(
    () => localStorage.getItem('theme') ?? 'light',
  )

  useEffect(() => {
    document.documentElement.setAttribute('data-theme', theme)
    document.documentElement.style.colorScheme = theme
    localStorage.setItem('theme', theme)
  }, [theme])

  const toggle = () => setTheme((t) => (t === 'light' ? 'dark' : 'light'))

  return { theme, toggle }
}
