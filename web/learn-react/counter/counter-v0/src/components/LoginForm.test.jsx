import { render, screen, fireEvent, waitFor } from '@testing-library/react'
import { AuthContext } from '../context/AuthContext'
import { vi, describe, expect, test } from 'vitest'

import LoginForm from './LoginForm'

const renderLoginForm = (authValue = {}) => {
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

describe('LoginForm', () => {
  describe('render', () => {
    test('renders identifier and password inputs', () => {
      renderLoginForm()
      expect(
        screen.getByPlaceholderText('username or email'),
      ).toBeInTheDocument()
      expect(screen.getByPlaceholderText('password')).toBeInTheDocument()
    })

    test('renders login button', () => {
      renderLoginForm()
      expect(screen.getByRole('button', { name: /login/i })).toBeInTheDocument()
    })

    test('does not show error initially', () => {
      renderLoginForm()
      expect(screen.queryByText('Invalid credentials')).not.toBeInTheDocument()
    })
  })

  describe('form interaction', () => {
    test('updates identifier input on change', () => {
      renderLoginForm()
      const input = screen.getByPlaceholderText('username or email')
      fireEvent.change(input, { target: { value: 'alice' } })
      expect(input.value).toBe('alice')
    })

    test('updates password input on change', () => {
      renderLoginForm()
      const input = screen.getByPlaceholderText('password')
      fireEvent.change(input, { target: { value: 'secret' } })
      expect(input.value).toBe('secret')
    })
  })

  describe('form submission', () => {
    test('calls login with identifier and password on submit', async () => {
      const login = vi.fn().mockResolvedValue(undefined)
      renderLoginForm({ login })

      fireEvent.change(screen.getByPlaceholderText('username or email'), {
        target: { value: 'alice' },
      })
      fireEvent.change(screen.getByPlaceholderText('password'), {
        target: { value: 'secret' },
      })
      fireEvent.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() => {
        expect(login).toHaveBeenCalledWith('alice', 'secret')
      })
    })

    test('trims identifier before calling login', async () => {
      const login = vi.fn().mockResolvedValue(undefined)
      renderLoginForm({ login })

      fireEvent.change(screen.getByPlaceholderText('username or email'), {
        target: { value: '  alice   ' },
      })
      fireEvent.change(screen.getByPlaceholderText('password'), {
        target: { value: 'secret' },
      })
      fireEvent.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() => {
        expect(login).toHaveBeenCalledWith('alice', 'secret')
      })
    })

    test('shows error message on failed login', async () => {
      const errorMessage = 'Invalid credentials'
      const login = vi.fn().mockRejectedValue(new Error(errorMessage))
      renderLoginForm({ login })

      fireEvent.change(screen.getByPlaceholderText('username or email'), {
        target: { value: 'alice' },
      })
      fireEvent.change(screen.getByPlaceholderText('password'), {
        target: { value: 'wrong' },
      })
      fireEvent.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() =>
        expect(screen.getByText(errorMessage)).toBeInTheDocument(),
      )
    })

    test('clears error on new submission attempt', async () => {
      const errorMessage = 'Invalid credentials'
      const login = vi
        .fn()
        .mockRejectedValueOnce(new Error(errorMessage))
        .mockResolvedValueOnce()
      renderLoginForm({ login })

      fireEvent.change(screen.getByPlaceholderText('username or email'), {
        target: { value: 'alice' },
      })
      fireEvent.change(screen.getByPlaceholderText('password'), {
        target: { value: 'wrong' },
      })
      fireEvent.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() =>
        expect(screen.getByText(errorMessage)).toBeInTheDocument(),
      )

      fireEvent.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() =>
        expect(screen.queryByText(errorMessage)).not.toBeInTheDocument(),
      )
    })
  })
})
