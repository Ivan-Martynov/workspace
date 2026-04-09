import { render, screen, waitFor } from '@testing-library/react'
import userEvent from '@testing-library/user-event'
import { vi, beforeEach, describe, expect, test } from 'vitest'

import ForgotPasswordForm from './ForgotPasswordForm'
import { requestPasswordReset } from '../services/auth'

vi.mock('../services/auth')

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key) => key,
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
}))

const onSuccess = vi.fn()
const renderForm = () => render(<ForgotPasswordForm onSuccess={onSuccess} />)

describe('ForgotPasswordForm', () => {
  const submitCode = /forgot\.submit/i
  const submittingCode = /forgot\.submitting/i
  const emailPlaceholder = /fields\.email/i

  const user = userEvent.setup()

  beforeEach(() => vi.clearAllMocks())

  describe('render', () => {
    test('renders email input', () => {
      renderForm()
      expect(screen.getByPlaceholderText(emailPlaceholder)).toBeInTheDocument()
    })

    test('submit button is disabled when email is empty', () => {
      renderForm()
      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
    })

    test('submit button is disabled when email is whitespace', async () => {
      renderForm()
      await user.type(screen.getByPlaceholderText(emailPlaceholder), '     ')

      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
    })

    test('submit button is enabled when email is not empty', async () => {
      renderForm()
      await user.type(
        screen.getByPlaceholderText(emailPlaceholder),
        'alice@example.com',
      )

      expect(screen.getByRole('button', { name: submitCode })).toBeEnabled()
    })

    test('does not show error initially', () => {
      renderForm()
      expect(screen.queryByRole('alert')).not.toBeInTheDocument()
    })
  })

  describe('submission', () => {
    test('calls requestPasswordReset and onSuccess on valid submit', async () => {
      let resolve
      requestPasswordReset.mockReturnValueOnce(
        new Promise((res) => {
          resolve = res
        }),
      )
      renderForm()

      const inputEmail = 'alice@example.com'

      await user.type(screen.getByPlaceholderText(emailPlaceholder), inputEmail)
      await user.click(screen.getByRole('button', { name: submitCode }))

      expect(
        screen.getByRole('button', { name: submittingCode }),
      ).toBeDisabled()
      const message = 'Reset link sent'
      resolve({ message })

      await waitFor(() => {
        expect(onSuccess).toHaveBeenCalledWith(message)
      })
      expect(requestPasswordReset).toHaveBeenCalledWith(inputEmail)
      expect(screen.getByPlaceholderText(emailPlaceholder)).toHaveValue('')
    })

    test('shows error message on failure', async () => {
      requestPasswordReset.mockRejectedValueOnce({
        code: 'networkError',
        message: 'Network error',
      })

      renderForm()
      await user.type(
        screen.getByPlaceholderText(emailPlaceholder),
        'alice@example.com',
      )
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(
          'errors.networkError',
        )
        expect(onSuccess).not.toHaveBeenCalled()
      })
    })

    test('clear error on new submission', async () => {
      requestPasswordReset
        .mockRejectedValueOnce({
          code: 'networkError',
          message: 'Network error',
        })
        .mockResolvedValueOnce({ message: 'ForgotPasswordSuccess' })

      renderForm()
      const button = screen.getByRole('button', { name: submitCode })

      await user.type(
        screen.getByPlaceholderText(emailPlaceholder),
        'alice@example.com',
      )
      await user.click(button)

      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(
          'errors.networkError',
        )
      })

      await user.click(button)
      await waitFor(() => {
        expect(screen.queryByRole('alert')).not.toBeInTheDocument()
      })
    })
  })
})
