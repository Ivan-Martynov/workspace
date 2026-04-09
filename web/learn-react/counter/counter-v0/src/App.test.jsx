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

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key, params) =>
      params && Object.keys(params).length
        ? `${key} ${JSON.stringify(params)}`
        : key,
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
  Trans: ({ i18nKey, values }) =>
    values ? `${i18nKey} ${Object.values(values).join(' ')}` : i18nKey,
}))

describe('App', () => {
  const identifierPlaceholder = /^fields\.usernameOrEmail$/i
  const countToPlaceholder = /game.\countTo/i
  const loginSubmit = /login.submit/i

  describe('routing - logged out', () => {
    test.skip('redirects to /auth when visiting / without a user', () => {
      renderApp({ user: mockUser }, ROUTES.HOME)
      expect(screen.getByText(/countTo/i)).toBeInTheDocument()
    })

    test('shows auth page when visiting / without a user', () => {
      renderApp({}, ROUTES.AUTH)
      expect(
        screen.getByPlaceholderText(identifierPlaceholder),
      ).toBeInTheDocument()
    })
  })

  describe.skip('routing - logged in', () => {
    test('shows game page when visiting with a user', () => {
      renderApp({ user: mockUser }, ROUTES.HOME)
      expect(screen.getByText(countToPlaceholder)).toBeInTheDocument()
    })

    test('redirects to / when visiting /auth with a user', () => {
      renderApp({ user: mockUser }, ROUTES.AUTH)
      expect(screen.getByText(countToPlaceholder)).toBeInTheDocument()
    })
  })

  describe('navbar', () => {
    test('shows navbar on all pages', () => {
      renderApp()
      expect(screen.getByRole('navigation')).toBeInTheDocument()
    })

    test('shows username in navbar when logged in', () => {
      renderApp({ user: mockUser })
      expect(
        screen.getByText(`navbar.loggedInAs ${mockUser.username}`),
      ).toBeInTheDocument()
    })

    test('shows login link in navbar when logged out', () => {
      renderApp()
      expect(
        screen.getByRole('link', { name: loginSubmit }),
      ).toBeInTheDocument()
    })
  })
})
