import { render, screen } from '@testing-library/react'
import { vi, describe, expect, test } from 'vitest'
import { MemoryRouter } from 'react-router-dom'
import { AuthContext } from './context/AuthContext'
import { ROUTES } from './routes'
import App from './App'

const mockUser = { id: '1', username: 'alice' }

const renderApp = (authValue = {}, initialRoute = ROUTES.HOME) => {
  const defaultAuth = {
    user: null,
    login: vi.fn(),
    logout: vi.fn(),
    register: vi.fn(),
    deleteAccount: vi.fn(),
    ...authValue,
  }
  return render(
    <MemoryRouter initialEntries={[initialRoute]}>
      <AuthContext.Provider value={defaultAuth}>
        <App />
      </AuthContext.Provider>
    </MemoryRouter>,
  )
}

describe.skip('App', () => {
  describe('routing - logged out', () => {
    test('redirects to /auth when visiting / without a user', () => {
      renderApp({}, ROUTES.HOME)
      expect(
        screen.getByPlaceholderText('username or email'),
      ).toBeInTheDocument()
    })

    test('shows auth page when visiting / without a user', () => {
      renderApp({}, ROUTES.AUTH)
      expect(
        screen.getByPlaceholderText('username or email'),
      ).toBeInTheDocument()
    })
  })

  describe('routing - logged in', () => {
    test('shows game page when visiting with a user', () => {
      renderApp({ user: mockUser }, ROUTES.HOME)
      expect(screen.getByText(/count to/i)).toBeInTheDocument()
    })

    test('redirects to / when visiting /auth with a user', () => {
      renderApp({ user: mockUser }, ROUTES.AUTH)
      expect(screen.getByText(/count to/i)).toBeInTheDocument()
    })
  })

  describe('navbar', () => {
    test('shows navbar on all pages', () => {
      renderApp()
      expect(screen.getByRole('navigation')).toBeInTheDocument()
    })

    test('shows username in navbar when logged in', () => {
      renderApp({ user: mockUser })
      expect(screen.getByText('alice')).toBeInTheDocument()
    })

    test('shows login link in navbar when logged out', () => {
      renderApp()
      expect(screen.getByRole('link', { name: /login/i })).toBeInTheDocument()
    })
  })
})
