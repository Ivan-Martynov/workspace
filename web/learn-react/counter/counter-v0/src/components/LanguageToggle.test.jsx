import { render, screen } from '@testing-library/react'
import { vi, beforeEach, describe, expect, test } from 'vitest'
import userEvent from '@testing-library/user-event'

import LanguageToggle from './LanguageToggle'

const changeLanguage = vi.fn()
vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key) => key,
    i18n: { language: 'en', changeLanguage },
  }),
}))

describe('LanguageToggle', () => {
  let user
  beforeEach(() => {
    user = userEvent.setup()
    localStorage.clear()
    vi.clearAllMocks()
  })

  test('renders select with language options', () => {
    render(<LanguageToggle />)
    expect(screen.getByRole('combobox')).toBeInTheDocument()
    expect(screen.getByRole('option', { name: /english/i })).toBeInTheDocument()
    expect(screen.getByRole('option', { name: /finnish/i })).toBeInTheDocument()
  })

  test('shows current language as selected', () => {
    render(<LanguageToggle />)
    expect(screen.getByRole('combobox')).toHaveValue('en')
  })

  test('calls changeLanguage when selection changes', async () => {
    render(<LanguageToggle />)
    await user.selectOptions(screen.getByRole('combobox'), 'fi')
    expect(changeLanguage).toHaveBeenCalledWith('fi')
  })

  test('persists language to localStorage', async () => {
    render(<LanguageToggle />)
    await user.selectOptions(screen.getByRole('combobox'), 'fi')
    expect(localStorage.getItem('language')).toBe('fi')
  })
})
