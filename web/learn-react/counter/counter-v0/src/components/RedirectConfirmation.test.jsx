import { render, screen, act } from '@testing-library/react'
import { vi, beforeEach, afterEach, describe, expect, test } from 'vitest'
import userEvent from '@testing-library/user-event'

const navigate = vi.fn()
vi.mock('react-router-dom', async () => {
  const actual = await vi.importActual('react-router-dom')
  return { ...actual, useNavigate: () => navigate }
})

import { ROUTES } from '../routes'
import RedirectConfirmation from './RedirectConfirmation'

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key, params) => (params ? `${key} ${JSON.stringify(params)}` : key),
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
}))

describe('RedirectConfirmation', () => {
  let user
  beforeEach(() => {
    vi.clearAllMocks()
    user = userEvent.setup()
  })

  describe('render', () => {
    test('renders success message', () => {
      render(<RedirectConfirmation message='Account deleted' />)
      expect(screen.getByRole('status')).toHaveTextContent('Account deleted')
    })

    test('renders redirect countdown text', () => {
      render(<RedirectConfirmation message='Some message' />)
      expect(screen.getByText(/common.redirectIn/i)).toBeInTheDocument()
    })

    test('render goNow button', () => {
      render(<RedirectConfirmation message='Some message' />)
      expect(
        screen.getByRole('button', { name: /common.goNow/i }),
      ).toBeInTheDocument()
    })

    test('uses default target name when not provided', () => {
      render(<RedirectConfirmation message='Some message' />)
      expect(screen.getByText(/common.toMainPage/i)).toBeInTheDocument()
    })

    test('uses provided target name', () => {
      render(
        <RedirectConfirmation
          message='Some message'
          targetName='common.toLoginPage'
        />,
      )
      expect(screen.getByText(/common.toLoginPage/i)).toBeInTheDocument()
    })
  })

  describe('navigation', () => {
    test('navigates to default target when go now clicked', async () => {
      render(<RedirectConfirmation message='Some message' />)
      await user.click(screen.getByRole('button', { name: /common.goNow/i }))
      expect(navigate).toHaveBeenCalledWith(ROUTES.HOME)
    })

    test('navigates to custom target when go now clicked', async () => {
      render(
        <RedirectConfirmation message='Some message' target={ROUTES.AUTH} />,
      )
      await user.click(screen.getByRole('button', { name: /common.goNow/i }))
      expect(navigate).toHaveBeenCalledWith(ROUTES.AUTH)
    })
  })

  describe('auto-redirect', () => {
    beforeEach(() => vi.useFakeTimers())
    afterEach(() => vi.useRealTimers())

    test('redirects after default countdown', () => {
      render(<RedirectConfirmation message='Some message' />)
      expect(navigate).not.toHaveBeenCalled()
      for (let i = 0; i < 5; i++) {
        act(() => vi.advanceTimersByTime(1000))
      }
      expect(navigate).toHaveBeenCalledWith(ROUTES.HOME)
    })

    test('redirects after custom countdown', () => {
      render(
        <RedirectConfirmation
          message='Some message'
          target={ROUTES.AUTH}
          countdownSeconds={3}
        />,
      )
      act(() => vi.advanceTimersByTime(1000))
      act(() => vi.advanceTimersByTime(1000))
      expect(navigate).not.toHaveBeenCalled()
      act(() => vi.advanceTimersByTime(1000))
      expect(navigate).toHaveBeenCalledWith(ROUTES.AUTH)
    })

    test('does not redirect before countdown finished', () => {
      render(<RedirectConfirmation message='Some message' />)
      act(() => vi.advanceTimersByTime(4000))
      expect(navigate).not.toHaveBeenCalledWith(ROUTES.HOME)
    })
  })
})
