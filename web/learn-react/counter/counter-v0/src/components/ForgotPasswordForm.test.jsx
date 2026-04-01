import { render, screen, waitFor } from '@testing-library/react'
import userEvent from '@testing-library/user-event'
import { vi, describe, expect, test } from 'vitest'

import ForgotPasswordForm from './ForgotPasswordForm'
import { requestPasswordReset } from '../services/auth'
import { beforeEach } from 'vitest'

vi.mock('../services/auth')

const onSuccess = vi.fn()
const renderForm = () => render(<ForgotPasswordForm onSuccess={onSuccess} />)

describe('ForgotPasswordForm', () => {
  beforeEach(() => vi.clearAllMocks())

  describe('render', () => {
    test('renders email input', () => {
      renderForm()
      expect(screen.getByPlaceholderText('email')).toBeInTheDocument()
    })

    test('submit button is disabled when email is empty', () => {
      renderForm()
      expect(
        screen.getByRole('button', { name: /send reset link/i }),
      ).toBeDisabled()
    })

    test('submit button is disabled when email is whitespace', async () => {
      renderForm()
      const user = userEvent.setup()
      await user.type(screen.getByPlaceholderText('email'), '     ')

      expect(
        screen.getByRole('button', { name: /send reset link/i }),
      ).toBeDisabled()
    })

    test('submit button is enabled when email is not empty', async () => {
      renderForm()
      const user = userEvent.setup()
      await user.type(screen.getByPlaceholderText('email'), 'alice@example.com')

      expect(
        screen.getByRole('button', { name: /send reset link/i }),
      ).toBeEnabled()
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

      const user = userEvent.setup()
      await user.type(screen.getByPlaceholderText('email'), inputEmail)
      await user.click(screen.getByRole('button', { name: /send reset link/i }))

      expect(screen.getByRole('button', { name: /sending/i })).toBeDisabled()
      const message = 'Reset link sent'
      resolve({ message })

      await waitFor(() => {
        expect(onSuccess).toHaveBeenCalledWith(message)
      })
      expect(requestPasswordReset).toHaveBeenCalledWith(inputEmail)
      expect(screen.getByPlaceholderText('email')).toHaveValue('')
    })

    test('shows error message on failure', async () => {
      const errorMessage = 'Network error'
      requestPasswordReset.mockRejectedValueOnce(new Error(errorMessage))

      renderForm()
      const user = userEvent.setup()
      await user.type(screen.getByPlaceholderText('email'), 'alice@example.com')
      await user.click(screen.getByRole('button', { name: /send reset link/i }))

      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(errorMessage)
        expect(onSuccess).not.toHaveBeenCalled()
      })
    })

    test('clear error on new submission', async () => {
      const errorMessage = 'Network error'
      requestPasswordReset
        .mockRejectedValueOnce(new Error(errorMessage))
        .mockResolvedValueOnce({ message: 'Reset link sent' })

      renderForm()
      const button = screen.getByRole('button', { name: /send reset link/i })

      const user = userEvent.setup()
      await user.type(screen.getByPlaceholderText('email'), 'alice@example.com')
      await user.click(button)

      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(errorMessage)
      })

      await user.click(button)
      await waitFor(() => {
        expect(screen.queryByRole('alert')).not.toBeInTheDocument()
      })
    })
  })
})
