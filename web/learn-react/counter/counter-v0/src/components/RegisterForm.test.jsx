import { render, screen, fireEvent, waitFor } from '@testing-library/react'
import { vi, describe, expect } from 'vitest'

import { AuthContext } from '../context/AuthContext'
import RegisterForm from './RegisterForm'

const renderRegisterForm = (authValue = {}) => {
  const defaultAuth = {
    register: vi.fn(),
    ...authValue,
  }
  return render(
    <AuthContext.Provider value={defaultAuth}>
      <RegisterForm />
    </AuthContext.Provider>,
  )
}
const mockData = {
  username: 'alice',
  email: 'alice@example.com',
  password: 'strongsecretpassword',
  ['confirm password']: 'strongsecretpassword',
}

const fillForm = (username, email, password, confirmPassword) => {
  fireEvent.change(screen.getByPlaceholderText('username'), {
    target: { value: username },
  })
  fireEvent.change(screen.getByPlaceholderText('email'), {
    target: { value: email },
  })
  fireEvent.change(screen.getByPlaceholderText('password'), {
    target: { value: password },
  })
  fireEvent.change(screen.getByPlaceholderText('confirm password'), {
    target: { value: confirmPassword },
  })
}

describe('RegisterForm', () => {
  describe('render', () => {
    test('renders username, email and password inputs', () => {
      renderRegisterForm()

      expect(screen.getByPlaceholderText('username')).toBeInTheDocument()
      expect(screen.getByPlaceholderText('email')).toBeInTheDocument()
      expect(screen.getByPlaceholderText('password')).toBeInTheDocument()
      expect(
        screen.getByPlaceholderText('confirm password'),
      ).toBeInTheDocument()
    })

    test('renders register button', () => {
      renderRegisterForm()

      expect(
        screen.getByRole('button', { name: /register/i }),
      ).toBeInTheDocument()
    })

    test('does not show error initially', () => {
      renderRegisterForm()

      expect(document.querySelector('p')).not.toBeInTheDocument()
    })
  })

  describe.only('interaction', () => {
    test('updates username, email and password input on change', () => {
      renderRegisterForm()

      for (const [key, value] of Object.entries(mockData)) {
        const input = screen.getByPlaceholderText(key)
        fireEvent.change(input, { target: { value: value } })
        expect(input.value).toBe(value)
      }
    })
  })

  describe('validation', () => {
    test('shows error when password is too short', async () => {
      renderRegisterForm()
      fillForm(mockData.username, mockData.email, 'short')

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(
          screen.getByText('Password must be at least 8 characters'),
        ).toBeInTheDocument()
      })
    })

    test('does not call register if password is too short', async () => {
      const register = vi.fn()
      renderRegisterForm({ register })
      fillForm(mockData.username, mockData.email, 'short')

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(register).not.toHaveBeenCalled()
      })
    })
  })

  describe('submission', () => {
    test('calls register with username, email and password', async () => {
      const register = vi.fn().mockResolvedValue(undefined)
      renderRegisterForm({ register })
      fillForm(mockData.username, mockData.email, mockData.password)

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

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
      renderRegisterForm({ register })
      fillForm(
        `   ${mockData.username} `,
        ` ${mockData.email}    `,
        mockData.password,
      )

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(register).toHaveBeenCalledWith(
          mockData.username,
          mockData.email,
          mockData.password,
        )
      })
    })

    test('shows error on failed registration', async () => {
      const register = vi
        .fn()
        .mockRejectedValue(new Error('Username or email already exists'))
      renderRegisterForm({ register })
      fillForm(mockData.username, mockData.email, mockData.password)

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(
          screen.getByText('Username or email already exists'),
        ).toBeInTheDocument()
      })
    })

    test('clears error on new submission attempt', async () => {
      const register = vi
        .fn()
        .mockRejectedValueOnce(new Error('Username or email already exists'))
        .mockResolvedValueOnce(undefined)
      renderRegisterForm({ register })
      fillForm(mockData.username, mockData.email, mockData.password)

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(
          screen.getByText('Username or email already exists'),
        ).toBeInTheDocument()
      })

      fireEvent.click(screen.getByRole('button', { name: /register/i }))

      await waitFor(() => {
        expect(
          screen.queryByText('Username or email already exists'),
        ).not.toBeInTheDocument()
      })
    })
  })
})
