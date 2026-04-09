import { useState } from 'react'
import { render, screen, waitFor } from '@testing-library/react'
import { vi, describe, test, beforeEach, expect } from 'vitest'
import userEvent from '@testing-library/user-event'

import { AuthProvider } from './AuthProvider'
import { useAuth } from './useAuth'
import { AUTH_STORAGE_KEY } from '../services/constants'
import {
  loginRequest,
  registerRequest,
  deleteAccountRequest,
  sendVerifySessionToken,
} from '../services/auth'

// Mock the auth service.
vi.mock('../services/auth', () => ({
  loginRequest: vi.fn(),
  registerRequest: vi.fn(),
  deleteAccountRequest: vi.fn(),
  sendVerifySessionToken: vi.fn(),
}))

const TestComponent = () => {
  const { user, loading, login, logout, register, deleteAccount } = useAuth()

  const [error, setError] = useState(null)

  const handleLogin = async () => {
    try {
      await login('alice', 'secret123')
    } catch (err) {
      setError(err.message)
    }
  }

  const handleDelete = async () => {
    try {
      await deleteAccount()
    } catch (err) {
      setError(err.message)
    }
  }

  return (
    <div>
      <span data-testid='user'>{user ? user.username : 'no user'}</span>
      {error && <span data-testid='error'>{error}</span>}
      <button onClick={handleLogin}>login</button>
      <button onClick={logout}>logout</button>
      <button
        onClick={() => register('alice', 'alice@example.com', 'secret123')}
      >
        register
      </button>
      <button onClick={handleDelete}>delete</button>
      <span data-testid='loading'>{loading ? 'loading' : 'ready'}</span>
    </div>
  )
}

const renderWithAuth = () =>
  render(
    <AuthProvider>
      <TestComponent />
    </AuthProvider>,
  )

const mockUser = { id: '1', username: 'alice', email: 'alice@example.com' }
const mockLoginResponse = { token: 'abc', user: mockUser }

describe('AuthProvider', () => {
  let user
  beforeEach(() => {
    localStorage.clear()
    vi.clearAllMocks()
    user = userEvent.setup()
  })

  describe('initial state', () => {
    test('user is null when no stored session', () => {
      renderWithAuth()
      expect(screen.getByTestId('user')).toHaveTextContent('no user')
    })

    test('restores user from localStorage on mount', () => {
      localStorage.setItem(
        AUTH_STORAGE_KEY,
        JSON.stringify({ token: 'abc', user: mockUser }),
      )

      renderWithAuth()
      expect(screen.getByTestId('user')).toHaveTextContent('alice')
    })

    test('handles corrupted localStorage gracefully', () => {
      localStorage.setItem(AUTH_STORAGE_KEY, 'invalid json{{{')

      renderWithAuth()
      expect(screen.getByTestId('user')).toHaveTextContent('no user')
    })

    test('loading is false after mount', async () => {
      renderWithAuth()
      await waitFor(() =>
        expect(screen.getByTestId('loading')).toHaveTextContent('ready'),
      )
    })
  })

  describe('login', () => {
    test('sets user on successful login', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('alice')
      })
    })

    test('stores response in localStorage on login', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        const stored = JSON.parse(localStorage.getItem(AUTH_STORAGE_KEY))
        expect(stored.token).toBe('abc')
        expect(stored.user.username).toBe('alice')
      })
    })

    test('throws if loginRequest fails', async () => {
      loginRequest.mockRejectedValue(new Error('Invalid credentials'))
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('no user')
        expect(screen.getByTestId('error')).toHaveTextContent(
          'Invalid credentials',
        )
      })
    })
  })

  describe('logout', () => {
    test('clears user on logout', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('alice')
      })

      await user.click(screen.getByRole('button', { name: /logout/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('no user')
      })
    })

    test('removes localStorage on logout', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(localStorage.getItem(AUTH_STORAGE_KEY)).not.toBeNull()
      })

      await user.click(screen.getByRole('button', { name: /logout/i }))
      await waitFor(() => {
        expect(localStorage.getItem(AUTH_STORAGE_KEY)).toBeNull()
      })
    })
  })

  describe('register', () => {
    test('calls registerRequest with correct arguments', async () => {
      registerRequest.mockResolvedValue({
        message: 'User created',
        userId: '1',
      })
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(registerRequest).toHaveBeenCalledWith(
          'alice',
          'alice@example.com',
          'secret123',
        )
      })
    })

    test('treats 409 conflict as success', async () => {
      const conflictError = new Error('Conflict')
      conflictError.status = 409
      registerRequest.mockRejectedValue(conflictError)

      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /register/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('no user')
        expect(screen.queryByTestId('error')).not.toBeInTheDocument()
      })
    })
  })

  describe('deleteAccount', () => {
    test('logs out after deleting account', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      deleteAccountRequest.mockResolvedValue(null)
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('alice')
      })

      await user.click(screen.getByRole('button', { name: /delete/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('no user')
      })
    })

    test('removes localStorage after deleting account', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      deleteAccountRequest.mockResolvedValue(null)
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(localStorage.getItem(AUTH_STORAGE_KEY)).not.toBeNull()
      })

      await user.click(screen.getByRole('button', { name: /delete/i }))
      await waitFor(() => {
        expect(localStorage.getItem(AUTH_STORAGE_KEY)).toBeNull()
      })
    })

    test('shows error if deleteAccountRequest fails', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      deleteAccountRequest.mockRejectedValue(new Error('Delete failed'))
      renderWithAuth()

      await user.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('alice')
      })

      await user.click(screen.getByRole('button', { name: /delete/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user')).toHaveTextContent('alice')
        expect(screen.getByTestId('error')).toHaveTextContent('Delete failed')
      })
    })

    test('does nothing when no user', async () => {
      renderWithAuth()
      await user.click(screen.getByRole('button', { name: /delete/i }))
      await waitFor(() => {
        expect(deleteAccountRequest).not.toHaveBeenCalled()
      })
    })
  })
})
