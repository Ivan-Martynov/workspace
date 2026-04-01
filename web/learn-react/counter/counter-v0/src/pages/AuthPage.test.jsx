import { render, screen, fireEvent } from '@testing-library/react'
import { vi, describe, expect, test } from 'vitest'

import { AuthContext } from '../context/AuthContext'
import AuthPage from './AuthPage'

const defaultAuth = {
  login: vi.fn(),
  register: vi.fn(),
}

const renderAuthPage = () =>
  render(
    <AuthContext.Provider value={defaultAuth}>
      <AuthPage />
    </AuthContext.Provider>,
  )

describe.skip('AuthPage', () => {
  describe('render', () => {
    test('shows login form by default', () => {
      renderAuthPage()
      expect(
        screen.getByPlaceholderText('username or email'),
      ).toBeInTheDocument()
    })

    test('shows register button by default', () => {
      renderAuthPage()
      expect(
        screen.getByRole('button', { name: /don't have an account/i }),
      ).toBeInTheDocument()
    })
  })

  describe('toggle', () => {
    test('switches to register form when toggle is clicked', () => {
      renderAuthPage()
      fireEvent.click(
        screen.getByRole('button', { name: /don't have an account/i }),
      )
      expect(
        screen.queryByPlaceholderText('username or email'),
      ).not.toBeInTheDocument()
      expect(screen.getByPlaceholderText('username')).toBeInTheDocument()
    })

    test('shows login button when register form is visible', () => {
      renderAuthPage()
      fireEvent.click(
        screen.getByRole('button', { name: /don't have an account/i }),
      )
      expect(
        screen.getByRole('button', { name: /already have an account/i }),
      ).toBeInTheDocument()
    })

    test('switches back to login form when toggle is clicked again', () => {
      renderAuthPage()
      fireEvent.click(
        screen.getByRole('button', { name: /don't have an account/i }),
      )
      fireEvent.click(
        screen.getByRole('button', { name: /already have an account/i }),
      )
      expect(screen.queryByPlaceholderText('username')).not.toBeInTheDocument()
      expect(
        screen.getByPlaceholderText('username or email'),
      ).toBeInTheDocument()
    })
  })
})
