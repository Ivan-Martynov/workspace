import { useState } from 'react'
import { render, screen, fireEvent, waitFor } from '@testing-library/react'
import { vi, describe, expect, test, beforeEach } from 'vitest'

import { AuthProvider, useAuth } from '../context/AuthContext'
import { AUTH_STORAGE_KEY } from '../services/constants'

const TestComponent = () => {
  const { user, login, logout, register, deleteAccount } = useAuth()
  const [error, setError] = useState(null)

  const handleLogin = async () => {
    try {
      await login('alice', 'secret')
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
      <button onClick={() => register('alice', 'alice@example.com', 'secret')}>
        register
      </button>
      <button onClick={deleteAccount}>delete</button>
    </div>
  )
}

const renderWithAuth = () =>
  render(
    <AuthProvider>
      <TestComponent />
    </AuthProvider>,
  )

vi.mock('../services/auth', () => ({
  loginRequest: vi.fn(),
  registerRequest: vi.fn(),
  deleteAccountRequest: vi.fn(),
}))

import {
  loginRequest,
  registerRequest,
  deleteAccountRequest,
} from '../services/auth'

const mockUser = { id: '1', username: 'alice', email: 'alice@example.com' }
const mockLoginResponse = { token: 'abc', user: mockUser }

describe.skip('AuthContext', () => {
  beforeEach(() => {
    localStorage.clear()
    vi.clearAllMocks()
  })

  describe('initial state', () => {
    test('user is null when no stored session', () => {
      renderWithAuth()
      expect(screen.getByTestId('user').textContent).toBe('no user')
    })

    test('restores user from localStorage on mount', () => {
      localStorage.setItem(
        AUTH_STORAGE_KEY,
        JSON.stringify({ token: 'abc', user: mockUser }),
      )

      renderWithAuth()
      expect(screen.getByTestId('user').textContent).toBe('alice')
    })

    test('handles corrupted localStorage gracefully', () => {
      localStorage.setItem(AUTH_STORAGE_KEY, 'invalid json{{{')

      renderWithAuth()
      expect(screen.getByTestId('user').textContent).toBe('no user')
    })
  })

  describe('login', () => {
    test('sets user on successful login', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() => {
        expect(screen.getByTestId('user').textContent).toBe('alice')
      })
    })

    test('stores response in localStorage on login', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() => {
        const stored = JSON.parse(localStorage.getItem(AUTH_STORAGE_KEY))
        expect(stored.token).toBe('abc')
        expect(stored.user.username).toBe('alice')
      })
    })

    test('throws if loginRequest fails', async () => {
      loginRequest.mockRejectedValue(new Error('Invalid credentials'))
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /login/i }))

      await waitFor(() => {
        expect(screen.getByTestId('user').textContent).toBe('no user')
        expect(screen.getByTestId('error').textContent).toBe(
          'Invalid credentials',
        )
      })
    })
  })

  describe('logout', () => {
    test('clears user on logout', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user').textContent).toBe('alice')
      })

      fireEvent.click(screen.getByRole('button', { name: /logout/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user').textContent).toBe('no user')
      })
    })

    test('removes localStorage on logout', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(localStorage.getItem(AUTH_STORAGE_KEY)).not.toBeNull()
      })

      fireEvent.click(screen.getByRole('button', { name: /logout/i }))
      await waitFor(() => {
        expect(localStorage.getItem(AUTH_STORAGE_KEY)).toBeNull()
      })
    })
  })

  describe('register', () => {
    test('logs in automatically after registration', async () => {
      registerRequest.mockResolvedValue({
        message: 'User created',
        userId: '1',
      })
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(screen.getByTestId('user').textContent).toBe('alice')
      })
    })

    test('calls loginRequest after registerRequest', async () => {
      registerRequest.mockResolvedValue({
        message: 'User created',
        userId: '1',
      })
      loginRequest.mockResolvedValue(mockLoginResponse)
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(registerRequest).toHaveBeenCalledOnce()
        expect(loginRequest).toHaveBeenCalledOnce()
      })
    })
  })

  describe('deleteAccount', () => {
    test('logs out after deleting account', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      deleteAccountRequest.mockResolvedValue(null)
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user').textContent).toBe('alice')
      })

      fireEvent.click(screen.getByRole('button', { name: /delete/i }))
      await waitFor(() => {
        expect(screen.getByTestId('user').textContent).toBe('no user')
      })
    })

    test('removes localStorage after deleting account', async () => {
      loginRequest.mockResolvedValue(mockLoginResponse)
      deleteAccountRequest.mockResolvedValue(null)
      renderWithAuth()

      fireEvent.click(screen.getByRole('button', { name: /login/i }))
      await waitFor(() => {
        expect(localStorage.getItem(AUTH_STORAGE_KEY)).not.toBeNull()
      })

      fireEvent.click(screen.getByRole('button', { name: /delete/i }))
      await waitFor(() => {
        expect(localStorage.getItem(AUTH_STORAGE_KEY)).toBeNull()
      })
    })
  })
})
