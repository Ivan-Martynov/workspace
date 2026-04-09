import { render, screen, waitFor } from '@testing-library/react'
import { vi, beforeEach, describe, test, expect } from 'vitest'
import userEvent from '@testing-library/user-event'

import * as hooks from '../hooks'

import VerifyEmailNotice from './VerifyEmailNotice'
import { resendVerificationRequest } from '../services/auth'

vi.mock('../services/auth')

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key, params) => (params ? `${key} ${JSON.stringify(params)}` : key),
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
}))

const renderForm = (email = 'alice@example.com') =>
  render(<VerifyEmailNotice email={email} />)

describe('VerifyEmailNotice', () => {
  const submitCode = /verifyEmail\.notice\.resend/i
  const submittingCode = /verifyEmail\.notice\.resendIn/i
  const checkInboxCode = /verifyEmail\.notice\.checkInbox/i

  let user
  beforeEach(() => {
    vi.clearAllMocks()
    user = userEvent.setup()
  })

  describe('render', () => {
    test('renders checkInbox initially', () => {
      renderForm()
      expect(screen.getByRole('status')).toHaveTextContent(checkInboxCode)
    })

    test('renders resend button', () => {
      renderForm()
      expect(
        screen.getByRole('button', { name: submitCode }),
      ).toBeInTheDocument()
    })

    test('resend button is enabled initially', () => {
      renderForm()
      expect(screen.getByRole('button', { name: submitCode })).toBeEnabled()
    })
  })

  describe('submission', () => {
    test('calls resendVerificationRequest on submit', async () => {
      resendVerificationRequest.mockResolvedValueOnce({})
      renderForm('bob@example.com')

      await user.click(screen.getByRole('button', { name: submitCode }))
      await waitFor(() => {
        expect(resendVerificationRequest).toHaveBeenCalledWith(
          'bob@example.com',
        )
      })
    })

    test('disables button during loading', async () => {
      let resolve
      resendVerificationRequest.mockReturnValueOnce(
        new Promise((res) => {
          resolve = res
        }),
      )
      renderForm()

      const button = screen.getByRole('button', { name: submitCode })
      user.click(button)
      await waitFor(() => expect(button).toBeDisabled())

      resolve()
    })

    test('shows countdown after successful resend', async () => {
      resendVerificationRequest.mockResolvedValueOnce({})
      renderForm()

      const button = screen.getByRole('button', { name: submitCode })
      await user.click(button)

      await waitFor(() => {
        expect(screen.getByRole('button')).toHaveTextContent(submittingCode)
        expect(screen.getByRole('button')).toBeDisabled()
      })
    })

    test('shows error message on failure', async () => {
      resendVerificationRequest.mockRejectedValueOnce({
        code: 'networkError',
        message: 'Network error',
      })
      renderForm()

      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(
          'errors.networkError',
        )
      })
    })
  })

  describe('countdown', () => {
    test('re-enables button after countdown', async () => {
      vi.spyOn(hooks, 'useCountdown').mockReturnValue({
        countdown: 0,
        reset: vi.fn(),
        isRunning: false,
      })

      renderForm()
      expect(screen.getByRole('button', { name: submitCode })).toBeEnabled()
    })

    test('disables button when countdown is running', () => {
      vi.spyOn(hooks, 'useCountdown').mockReturnValue({
        countdown: 2,
        reset: vi.fn(),
        isRunning: true,
      })

      renderForm()
      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
      expect(
        screen.getByRole('button', { name: submitCode }),
      ).toHaveTextContent(submittingCode)
    })
  })
})
