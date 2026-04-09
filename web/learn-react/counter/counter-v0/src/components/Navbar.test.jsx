import { render, screen } from '@testing-library/react'
import { vi, describe, expect, test } from 'vitest'
import userEvent from '@testing-library/user-event'
import { MemoryRouter } from 'react-router-dom'

import { AuthContext } from '../context/AuthContext'
import Navbar from './Navbar'

const mockUser = { id: '1', username: 'alice' }

const renderNavbar = (authValue = {}) => {
  const defaultAuth = {
    user: null,
    logout: vi.fn(),
    deleteAccount: vi.fn(),
    ...authValue,
  }

  return render(
    <MemoryRouter>
      <AuthContext.Provider value={defaultAuth}>
        <Navbar />
      </AuthContext.Provider>
    </MemoryRouter>,
  )
}

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key) => key,
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
  Trans: ({ i18nKey, values }) =>
    values ? `${i18nKey} ${Object.values(values).join(' ')}` : i18nKey,
}))

describe('Navbar', () => {
  const user = userEvent.setup()

  describe('logged out', () => {
    test('shows login link when no user', () => {
      renderNavbar()
      expect(screen.getByRole('link', { name: /login/i })).toBeInTheDocument()
    })

    test('shows game link', () => {
      renderNavbar()
      expect(screen.getByRole('link', { name: /game/i })).toBeInTheDocument()
    })

    test('does not show logout button', () => {
      renderNavbar()
      expect(
        screen.queryByRole('button', { name: /logout/i }),
      ).not.toBeInTheDocument()
    })
  })

  describe('logged in', () => {
    test('shows username', () => {
      renderNavbar({ user: mockUser })
      expect(screen.getByText(/alice/)).toBeInTheDocument()
    })

    test('shows logout button', () => {
      renderNavbar({ user: mockUser })
      expect(
        screen.getByRole('button', { name: /logout/i }),
      ).toBeInTheDocument()
    })

    test('does not show login link', () => {
      renderNavbar({ user: mockUser })
      expect(
        screen.queryByRole('link', { name: /login/i }),
      ).not.toBeInTheDocument()
    })

    test('calling logout when logout button is clicked', async () => {
      const logout = vi.fn()
      renderNavbar({ user: mockUser, logout })
      await user.click(screen.getByRole('button', { name: /logout/i }))
      expect(logout).toHaveBeenCalledOnce()
    })
  })

  describe('delete account dialog', () => {
    test('shows delete account button when logged in', () => {
      renderNavbar({ user: mockUser })
      expect(
        screen.getByRole('button', { name: /delete.submit/i }),
      ).toBeInTheDocument()
    })

    test('shows dialog button when delete account is clicked', async () => {
      renderNavbar({ user: mockUser })
      await user.click(screen.getByRole('button', { name: /delete.submit/i }))
      expect(screen.getByText(/delete.confirm/i)).toBeInTheDocument()
    })

    test('hides dialog when cancel is clicked', async () => {
      renderNavbar({ user: mockUser })
      await user.click(screen.getByRole('button', { name: /delete.submit/i }))
      await user.click(screen.getByRole('button', { name: /cancel/i }))
      expect(screen.queryByText(/delete.confirm/i)).not.toBeInTheDocument()
    })
  })
})
