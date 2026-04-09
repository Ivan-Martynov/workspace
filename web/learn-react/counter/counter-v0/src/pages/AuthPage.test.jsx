import { render, screen } from '@testing-library/react'
import userEvent from '@testing-library/user-event'
import { vi, beforeEach, describe, expect, test } from 'vitest'

import { AuthContext } from '../context/AuthContext'
import AuthPage from './AuthPage'

vi.mock('react-i18next', () => ({
  useTranslation: () => ({ t: (key) => key }),
}))

const defaultAuth = {
  login: vi.fn(),
  register: vi.fn(),
}

const renderPage = () =>
  render(
    <AuthContext.Provider value={defaultAuth}>
      <AuthPage />
    </AuthContext.Provider>,
  )

describe('AuthPage', () => {
  const usernamePlaceholder = /^fields\.username$/i
  const identifierPlaceholder = /^fields\.usernameOrEmail$/i
  const noAccountPlaceholder = /^auth\.noAccount$/i
  const hasAccountPlaceholder = /^auth\.hasAccount$/i

  let user
  beforeEach(() => (user = userEvent.setup()))

  describe('render', () => {
    test('shows login form by default', () => {
      renderPage()
      expect(
        screen.getByPlaceholderText(identifierPlaceholder),
      ).toBeInTheDocument()
    })

    test('shows register button by default', () => {
      renderPage()
      expect(
        screen.getByRole('button', { name: noAccountPlaceholder }),
      ).toBeInTheDocument()
    })
  })

  describe('toggle', () => {
    test('switches to register form when toggle is clicked', async () => {
      renderPage()
      await user.click(
        screen.getByRole('button', { name: noAccountPlaceholder }),
      )
      expect(
        screen.queryByPlaceholderText(identifierPlaceholder),
      ).not.toBeInTheDocument()
      expect(
        screen.getByPlaceholderText(usernamePlaceholder),
      ).toBeInTheDocument()
    })

    test('shows login button when register form is visible', async () => {
      renderPage()
      await user.click(
        screen.getByRole('button', { name: noAccountPlaceholder }),
      )
      expect(
        screen.getByRole('button', { name: hasAccountPlaceholder }),
      ).toBeInTheDocument()
    })

    test('switches back to login form when toggle is clicked again', async () => {
      renderPage()
      await user.click(
        screen.getByRole('button', { name: noAccountPlaceholder }),
      )
      await user.click(
        screen.getByRole('button', { name: hasAccountPlaceholder }),
      )
      expect(
        screen.queryByPlaceholderText(usernamePlaceholder),
      ).not.toBeInTheDocument()
      expect(
        screen.getByPlaceholderText(identifierPlaceholder),
      ).toBeInTheDocument()
    })
  })
})
