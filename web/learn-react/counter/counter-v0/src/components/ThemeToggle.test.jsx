import { render, screen } from '@testing-library/react'
import { vi, beforeEach, describe, expect, test } from 'vitest'
import userEvent from '@testing-library/user-event'

import ThemeToggle from './ThemeToggle'

vi.mock('react-i18next', () => ({
  useTranslation: () => ({ t: (key) => key }),
}))

describe('ThemeToggle', () => {
  let user
  beforeEach(() => {
    user = userEvent.setup()
    localStorage.clear()
    document.documentElement.removeAttribute('data-theme')
  })

  test('renders toggle button', () => {
    render(<ThemeToggle />)
    expect(screen.getByRole('button')).toBeInTheDocument()
  })

  test('toggles theme on click', async () => {
    render(<ThemeToggle />)
    const button = screen.getByRole('button')
    expect(button).toHaveTextContent('theme.dark')
    await user.click(button)
    expect(button).toHaveTextContent('theme.light')
  })

  test('persists theme to localStorage', async () => {
    render(<ThemeToggle />)
    expect(localStorage.getItem('theme')).toBe('light')
    await user.click(screen.getByRole('button'))
    expect(localStorage.getItem('theme')).toBe('dark')
  })

  test('sets data-theme attribute on documentElement', async () => {
    render(<ThemeToggle />)
    expect(document.documentElement.getAttribute('data-theme')).toBe('light')
    await user.click(screen.getByRole('button'))
    expect(document.documentElement.getAttribute('data-theme')).toBe('dark')
  })
})
