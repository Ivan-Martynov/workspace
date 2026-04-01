import { vi } from 'vitest'

const navigate = vi.fn()
vi.mock('react-router-dom', async () => {
  const actual = await vi.importActual('react-router-dom')
  return {
    ...actual,
    useNavigate: () => navigate,
  }
})

import { render, screen, fireEvent, waitFor } from '@testing-library/react'
import { MemoryRouter } from 'react-router-dom'
import { describe, expect, beforeEach } from 'vitest'

import { AuthContext } from '../context/AuthContext'
import DeleteAccountDialog from './DeleteAccountDialog'
import { ROUTES } from '../routes'

const mockUser = { id: '1', username: 'alice' }

const renderDialog = (authValue = {}, onCancel = vi.fn()) => {
  const defaultAuth = {
    user: mockUser,
    deleteAccount: vi.fn(),
    ...authValue,
  }
  return render(
    // Using MemoryRouter to simulate navigation
    <MemoryRouter>
      <AuthContext.Provider value={defaultAuth}>
        <DeleteAccountDialog onCancel={onCancel} />
      </AuthContext.Provider>
    </MemoryRouter>,
  )
}

describe('DeleteAccountDialog', () => {
  beforeEach(() => navigate.mockClear())

  describe('render', () => {
    test('renders confirmation input', () => {
      renderDialog()
      expect(screen.getByPlaceholderText('alice')).toBeInTheDocument()
    })

    test('renders delete button as disabled initially', () => {
      renderDialog()
      expect(
        screen.getByRole('button', { name: /delete account/i }),
      ).toBeDisabled()
    })

    test('renders cancel button', () => {
      renderDialog()
      expect(
        screen.getByRole('button', { name: /cancel/i }),
      ).toBeInTheDocument()
    })

    test('renders username in confirmation message', () => {
      renderDialog()
      expect(screen.getByText('alice')).toBeInTheDocument()
    })

    test('returns null when user is null', () => {
      renderDialog({ user: null })
      expect(screen.queryByText(/delete account/i)).not.toBeInTheDocument()
    })
  })

  describe('confirmation input', () => {
    test('delete button becomes enabled when username matches', () => {
      renderDialog()
      fireEvent.change(screen.getByPlaceholderText('alice'), {
        target: { value: 'alice' },
      })
      expect(
        screen.getByRole('button', { name: /delete account/i }),
      ).not.toBeDisabled()
    })

    test('delete button stays disabled when username mismatches', () => {
      renderDialog()
      fireEvent.change(screen.getByPlaceholderText('alice'), {
        target: { value: 'bob' },
      })
      expect(
        screen.getByRole('button', { name: /delete account/i }),
      ).toBeDisabled()
    })

    test('delete button stays disabled on partial match', () => {
      renderDialog()
      fireEvent.change(screen.getByPlaceholderText('alice'), {
        target: { value: 'alic' },
      })
      expect(
        screen.getByRole('button', { name: /delete account/i }),
      ).toBeDisabled()
    })

    test('input is not trimmed', () => {
      renderDialog()
      fireEvent.change(screen.getByPlaceholderText('alice'), {
        target: { value: ' alice ' },
      })
      expect(
        screen.getByRole('button', { name: /delete account/i }),
      ).toBeDisabled()
    })
  })

  describe('actions', () => {
    test('calls deleteAccount when confirmed and button clicked', async () => {
      const deleteAccount = vi.fn().mockResolvedValue(undefined)
      renderDialog({ deleteAccount })

      fireEvent.change(screen.getByPlaceholderText('alice'), {
        target: { value: 'alice' },
      })
      fireEvent.click(screen.getByRole('button', { name: /delete account/i }))

      await waitFor(() => {
        expect(deleteAccount).toHaveBeenCalledOnce()
      })
    })

    test('shows error if deleteAccount throws', async () => {
      const deleteAccount = vi
        .fn()
        .mockRejectedValue(new Error('Failed to delete account'))
      renderDialog({ deleteAccount })

      fireEvent.change(screen.getByPlaceholderText('alice'), {
        target: { value: 'alice' },
      })
      fireEvent.click(screen.getByRole('button', { name: /delete account/i }))

      await waitFor(() => {
        expect(screen.getByText('Failed to delete account')).toBeInTheDocument()
      })
    })

    test('calls onCancel when cancel button is clicked', () => {
      const onCancel = vi.fn()
      renderDialog({}, onCancel)

      fireEvent.click(screen.getByRole('button', { name: /cancel/i }))
      expect(onCancel).toHaveBeenCalledOnce()
    })

    test('navigates after successful deletion', async () => {
      const deleteAccount = vi.fn().mockResolvedValue(undefined)
      renderDialog({ deleteAccount })

      fireEvent.change(screen.getByPlaceholderText('alice'), {
        target: { value: 'alice' },
      })
      fireEvent.click(screen.getByRole('button', { name: /delete account/i }))

      await waitFor(() => {
        expect(navigate).toHaveBeenCalledOnce()
        expect(navigate).toHaveBeenCalledWith(ROUTES.CONFIRM_ACCOUNT_DELETION)
      })
    })
  })
})
