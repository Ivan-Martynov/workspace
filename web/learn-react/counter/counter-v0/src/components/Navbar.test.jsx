import { render, screen, fireEvent } from '@testing-library/react'
import { vi, describe, expect, test } from 'vitest'
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

describe('Navbar', () => {
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
      expect(screen.getByText('alice')).toBeInTheDocument()
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

    test('calling logout when logout button is clicked', () => {
      const logout = vi.fn()
      renderNavbar({ user: mockUser, logout })
      fireEvent.click(screen.getByRole('button', { name: /logout/i }))
      expect(logout).toHaveBeenCalledOnce()
    })
  })

  describe('delete account dialog', () => {
    test('shows delete account button when logged in', () => {
      renderNavbar({ user: mockUser })
      expect(
        screen.getByRole('button', { name: /delete account/i }),
      ).toBeInTheDocument()
    })

    test('shows dialog button when delete account is clicked', () => {
      renderNavbar({ user: mockUser })
      fireEvent.click(screen.getByRole('button', { name: /delete account/i }))
      expect(screen.getByText(/confirm deletion/i)).toBeInTheDocument()
    })

    test('hides dialog when cancel is clicked', () => {
      renderNavbar({ user: mockUser })
      fireEvent.click(screen.getByRole('button', { name: /delete account/i }))
      fireEvent.click(screen.getByRole('button', { name: /cancel/i }))
      expect(screen.queryByText(/confirm deletion/i)).not.toBeInTheDocument()
    })
  })
})
