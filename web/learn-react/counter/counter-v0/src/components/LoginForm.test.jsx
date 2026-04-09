import { render, screen, waitFor } from '@testing-library/react'
import { vi, beforeEach, describe, expect, test } from 'vitest'
import userEvent from '@testing-library/user-event'

import { AuthContext } from '../context/AuthContext'
import LoginForm from './LoginForm'

const renderForm = (authValue = {}) => {
  const defaultAuth = {
    login: vi.fn(),
    ...authValue,
  }
  return render(
    <AuthContext.Provider value={defaultAuth}>
      <LoginForm />
    </AuthContext.Provider>,
  )
}

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key) => key,
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
}))

describe('LoginForm', () => {
  const submitCode = /login\.submit/i
  const submittingCode = /login\.submitting/i
  const identifierPlaceholder = /fields\.usernameOrEmail/i
  const passwordPlaceholder = /^fields\.password$/i

  let user
  beforeEach(() => {
    user = userEvent.setup()
    vi.clearAllMocks()
  })

  describe('render', () => {
    test('renders identifier and password inputs', () => {
      renderForm()
      expect(
        screen.getByPlaceholderText(identifierPlaceholder),
      ).toBeInTheDocument()
      expect(
        screen.getByPlaceholderText(passwordPlaceholder),
      ).toBeInTheDocument()
    })

    test('renders login button', () => {
      renderForm()
      expect(
        screen.getByRole('button', { name: submitCode }),
      ).toBeInTheDocument()
    })

    test('does not show error initially', () => {
      renderForm()
      expect(screen.queryByRole('alert')).not.toBeInTheDocument()
    })

    test('login button is disabled when fields are empty', () => {
      renderForm()
      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
    })

    test('login button is enabled when both fields are filled', async () => {
      renderForm()
      await user.type(
        screen.getByPlaceholderText(identifierPlaceholder),
        'alice',
      )
      await user.type(
        screen.getByPlaceholderText(passwordPlaceholder),
        'secret',
      )
      expect(screen.getByRole('button', { name: submitCode })).toBeEnabled()
    })
  })

  describe('form submission', () => {
    test('calls login with identifier and password on submit', async () => {
      const login = vi.fn().mockResolvedValue(undefined)
      renderForm({ login })

      await user.type(
        screen.getByPlaceholderText(identifierPlaceholder),
        'alice',
      )
      await user.type(
        screen.getByPlaceholderText(passwordPlaceholder),
        'secret',
      )
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(login).toHaveBeenCalledWith('alice', 'secret')
      })
    })

    test('trims identifier before calling login', async () => {
      const login = vi.fn().mockResolvedValue(undefined)
      renderForm({ login })

      await user.type(
        screen.getByPlaceholderText(identifierPlaceholder),
        '  alice   ',
      )
      await user.type(
        screen.getByPlaceholderText(passwordPlaceholder),
        'secret',
      )
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(login).toHaveBeenCalledWith('alice', 'secret')
      })
    })

    test('shows error message on failed login', async () => {
      const login = vi.fn().mockRejectedValue({
        code: 'invalidCredentials',
        message: 'Invalid credentials',
      })
      renderForm({ login })

      await user.type(
        screen.getByPlaceholderText(identifierPlaceholder),
        'alice',
      )
      await user.type(screen.getByPlaceholderText(passwordPlaceholder), 'wrong')
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() =>
        expect(screen.getByRole('alert')).toHaveTextContent(
          /invalidCredentials/i,
        ),
      )
    })

    test('shows loading state during submission', async () => {
      let resolve
      const login = vi.fn().mockReturnValueOnce(
        new Promise((res) => {
          resolve = res
        }),
      )
      renderForm({ login })

      await user.type(
        screen.getByPlaceholderText(identifierPlaceholder),
        'alice',
      )
      await user.type(
        screen.getByPlaceholderText(passwordPlaceholder),
        'secret',
      )
      await user.click(screen.getByRole('button', { name: submitCode }))

      expect(
        screen.getByRole('button', { name: submittingCode }),
      ).toBeDisabled()
      expect(screen.getByPlaceholderText(identifierPlaceholder)).toBeDisabled()
      expect(screen.getByPlaceholderText(passwordPlaceholder)).toBeDisabled()

      resolve()

      await waitFor(() => {
        expect(
          screen.getByRole('button', { name: submitCode }),
        ).toBeInTheDocument()
      })
    })

    test('clears error on new submission', async () => {
      const login = vi
        .fn()
        .mockRejectedValueOnce({
          code: 'invalidCredentials',
          message: 'Invalid Credentials',
        })
        .mockResolvedValueOnce(undefined)
      renderForm({ login })

      await user.type(
        screen.getByPlaceholderText(identifierPlaceholder),
        'alice',
      )
      await user.type(screen.getByPlaceholderText(passwordPlaceholder), 'wrong')
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(screen.getByRole('alert')).toBeInTheDocument()
      })

      await user.type(
        screen.getByPlaceholderText(passwordPlaceholder),
        'secret',
      )
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(screen.queryByRole('alert')).not.toBeInTheDocument()
      })
    })

    test('clears input after successful login', async () => {
      const login = vi.fn().mockResolvedValue(undefined)
      renderForm({ login })

      await user.type(
        screen.getByPlaceholderText(identifierPlaceholder),
        'alice',
      )
      await user.type(
        screen.getByPlaceholderText(passwordPlaceholder),
        'secret',
      )
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(screen.getByPlaceholderText(identifierPlaceholder)).toHaveValue(
          '',
        )
        expect(screen.getByPlaceholderText(passwordPlaceholder)).toHaveValue('')
      })
    })
  })
})
