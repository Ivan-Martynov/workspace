import { render, screen, waitFor } from '@testing-library/react'
import { vi, beforeEach, describe, test, expect } from 'vitest'
import userEvent from '@testing-library/user-event'

import { AuthContext } from '../context/AuthContext'
import RegisterForm from './RegisterForm'

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key) => key,
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
}))

const mockData = {
  username: 'alice',
  email: 'alice@example.com',
  password: 'strongsecretpassword',
  confirmPassword: 'strongsecretpassword',
}

const onSuccess = vi.fn()

const renderForm = (authValue = {}) => {
  const defaultAuth = { register: vi.fn(), ...authValue }
  return render(
    <AuthContext.Provider value={defaultAuth}>
      <RegisterForm onSuccess={onSuccess} />
    </AuthContext.Provider>,
  )
}

const usernamePlaceholder = /fields\.username/i
const emailPlaceholder = /fields\.email/i
const passwordPlaceholder = /^fields\.password$/i
const confirmPasswordPlaceholder = /^fields\.confirmPassword$/i

const fillForm = async (user, data = mockData) => {
  await user.type(
    screen.getByPlaceholderText(usernamePlaceholder),
    data.username ?? '',
  )
  await user.type(
    screen.getByPlaceholderText(emailPlaceholder),
    data.email ?? '',
  )
  await user.type(
    screen.getByPlaceholderText(passwordPlaceholder),
    data.password ?? '',
  )
  await user.type(
    screen.getByPlaceholderText(confirmPasswordPlaceholder),
    data.confirmPassword ?? '',
  )
}

describe('RegisterForm', () => {
  const submitCode = /register\.submit/i
  const submittingCode = /register\.submitting/i

  let user
  beforeEach(() => {
    vi.clearAllMocks()
    user = userEvent.setup()
  })

  describe('render', () => {
    test.each([
      usernamePlaceholder,
      emailPlaceholder,
      passwordPlaceholder,
      confirmPasswordPlaceholder,
    ])('renders %s input', (placeholder) => {
      renderForm()
      expect(screen.getByPlaceholderText(placeholder)).toBeInTheDocument()
    })

    test('renders register button', () => {
      renderForm()
      expect(
        screen.getByRole('button', { name: submitCode }),
      ).toBeInTheDocument()
    })

    test('does not show error initially', () => {
      renderForm()
      expect(screen.queryByRole('alert')).not.toBeInTheDocument()
    })

    test('submit button is disabled when password fields are empty', () => {
      renderForm()
      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
    })
  })

  describe('validation', () => {
    test('shows error when backend rejects short password', async () => {
      const register = vi.fn().mockRejectedValue({
        code: 'passwordTooShort',
        params: { min: 8 },
      })
      renderForm({ register })

      await fillForm(user, {
        ...mockData,
        password: 'short',
        confirmPassword: 'short',
      })
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(
          'errors.passwordTooShort',
        )
      })
    })

    test('shows error when passwords do not match', async () => {
      renderForm()
      await fillForm(user, {
        ...mockData,
        confirmPassword: 'differentPassword',
      })
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(
          'errors.passwordMismatch',
        )
      })
    })

    test('does not call register when passwords do not match', async () => {
      const register = vi.fn()
      renderForm({ register })
      await fillForm(user, {
        ...mockData,
        confirmPassword: 'differentPassword',
      })
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => expect(register).not.toHaveBeenCalled())
    })
  })

  describe('submission', () => {
    test('calls register with username, email and password', async () => {
      const register = vi.fn().mockResolvedValue(undefined)
      renderForm({ register })

      await fillForm(user)
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(register).toHaveBeenCalledWith(
          mockData.username,
          mockData.email,
          mockData.password,
        )
      })
    })

    test('trims username and email before calling register', async () => {
      const register = vi.fn().mockResolvedValue(undefined)
      renderForm({ register })

      await fillForm(user, {
        ...mockData,
        username: `   ${mockData.username} `,
        email: ` ${mockData.email}    `,
      })
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(register).toHaveBeenCalledWith(
          mockData.username,
          mockData.email,
          mockData.password,
        )
      })
    })

    test('calls onSuccess with email after registration', async () => {
      const register = vi.fn().mockResolvedValue(undefined)
      renderForm({ register })

      await fillForm(user)
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(onSuccess).toHaveBeenCalledWith(mockData.email)
      })
    })

    test('shows error on failed registration', async () => {
      const register = vi.fn().mockRejectedValue({
        code: 'duplicateKey',
        message: 'Request could not be completed, please check your input',
      })
      renderForm({ register })

      await fillForm(user)
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(
          'errors.duplicateKey',
        )
      })
    })

    test('clears error on new submission', async () => {
      const register = vi
        .fn()
        .mockRejectedValueOnce({
          code: 'duplicateKey',
          message: 'Request could not be completed, please check your input',
        })
        .mockResolvedValueOnce(undefined)
      renderForm({ register })

      await fillForm(user)
      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(screen.getByRole('alert')).toBeInTheDocument()
      })

      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(screen.queryByRole('alert')).not.toBeInTheDocument()
      })
    })

    test('shows loading state during submission', async () => {
      let resolve
      const register = vi.fn().mockReturnValueOnce(
        new Promise((res) => {
          resolve = res
        }),
      )
      renderForm({ register })

      await fillForm(user)
      await user.click(screen.getByRole('button', { name: submitCode }))

      expect(
        screen.getByRole('button', { name: submittingCode }),
      ).toBeDisabled()
      expect(screen.getByPlaceholderText(usernamePlaceholder)).toBeDisabled()
      expect(screen.getByPlaceholderText(emailPlaceholder)).toBeDisabled()
      expect(screen.getByPlaceholderText(passwordPlaceholder)).toBeDisabled()
      expect(
        screen.getByPlaceholderText(confirmPasswordPlaceholder),
      ).toBeDisabled()

      resolve()

      await waitFor(() => {
        expect(
          screen.getByRole('button', { name: submitCode }),
        ).toBeInTheDocument()
      })
    })
  })
})
