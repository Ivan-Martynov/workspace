import userEvent from '@testing-library/user-event'
import { render, screen, waitFor } from '@testing-library/react'
import { vi, describe, test, expect, beforeEach } from 'vitest'

import { AuthContext } from '../context/AuthContext'
import DeleteAccountDialog from './DeleteAccountDialog'
import { ROUTES } from '../routes'

const navigate = vi.fn()
vi.mock('react-router-dom', async () => {
  const actual = await vi.importActual('react-router-dom')
  return { ...actual, useNavigate: () => navigate }
})

const mockUser = { id: '1', username: 'alice' }

const renderForm = (authValue = {}, onCancel = vi.fn()) => {
  const defaultAuth = {
    user: mockUser,
    deleteAccount: vi.fn(),
    ...authValue,
  }
  return render(
    <AuthContext.Provider value={defaultAuth}>
      <DeleteAccountDialog onCancel={onCancel} />
    </AuthContext.Provider>,
  )
}

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key) => key,
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
  Trans: ({ i18nKey, values }) =>
    values ? `${i18nKey} ${Object.values(values).join(' ')}` : i18nKey,
}))

describe('DeleteAccountDialog', () => {
  const submitCode = /delete.submit/i
  const cancelCode = /cancel/i
  let user

  beforeEach(() => {
    vi.clearAllMocks()
    user = userEvent.setup()
  })

  describe('render', () => {
    test('renders confirmation input', () => {
      renderForm()
      expect(screen.getByPlaceholderText(/alice/)).toBeInTheDocument()
    })

    test('renders delete button as disabled initially', () => {
      renderForm()
      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
    })

    test('renders cancel button', () => {
      renderForm()
      expect(
        screen.getByRole('button', { name: cancelCode }),
      ).toBeInTheDocument()
    })

    test('renders username in confirmation message', () => {
      renderForm()
      expect(screen.getByText(/alice/)).toBeInTheDocument()
    })
  })

  describe('confirmation input', () => {
    test('delete button becomes enabled when username matches', async () => {
      renderForm()

      await user.type(screen.getByPlaceholderText('alice'), 'alice')
      expect(
        screen.getByRole('button', { name: submitCode }),
      ).not.toBeDisabled()
    })

    test('delete button stays disabled when username mismatches', async () => {
      renderForm()
      await user.type(screen.getByPlaceholderText('alice'), 'bob')
      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
    })

    test('delete button stays disabled on partial match', async () => {
      renderForm()
      await user.type(screen.getByPlaceholderText('alice'), 'alic')
      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
    })

    test('input is not trimmed', async () => {
      renderForm()
      await user.type(screen.getByPlaceholderText('alice'), ' alice ')
      expect(screen.getByRole('button', { name: submitCode })).toBeDisabled()
    })
  })

  describe('actions', () => {
    test('calls deleteAccount when confirmed and button clicked', async () => {
      const deleteAccount = vi.fn().mockResolvedValue(undefined)
      renderForm({ deleteAccount })

      await user.type(screen.getByPlaceholderText('alice'), 'alice')
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(deleteAccount).toHaveBeenCalledOnce()
      })
    })

    test('shows error if deleteAccount throws', async () => {
      const deleteAccount = vi.fn().mockRejectedValue({
        code: 'deleteAccountFailed',
        message: 'Failed to delete account',
      })
      renderForm({ deleteAccount })

      await user.type(screen.getByPlaceholderText('alice'), 'alice')
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(screen.getByRole('alert')).toHaveTextContent(
          /errors.deleteAccountFailed/i,
        )
      })
    })

    test('calls onCancel when cancel button is clicked', async () => {
      const onCancel = vi.fn()
      renderForm({}, onCancel)

      await user.click(screen.getByRole('button', { name: cancelCode }))
      expect(onCancel).toHaveBeenCalledOnce()
    })

    test('navigates after successful deletion', async () => {
      const deleteAccount = vi.fn().mockResolvedValue(undefined)
      renderForm({ deleteAccount })

      await user.type(screen.getByPlaceholderText('alice'), 'alice')
      await user.click(screen.getByRole('button', { name: submitCode }))

      await waitFor(() => {
        expect(navigate).toHaveBeenCalledOnce()
        expect(navigate).toHaveBeenCalledWith(ROUTES.CONFIRM_ACCOUNT_DELETION)
      })
    })

    test('shows loading state during deletion', async () => {
      let resolve
      const deleteAccount = vi
        .fn()
        .mockReturnValueOnce(new Promise((res) => (resolve = res)))
      renderForm({ deleteAccount })

      await user.type(screen.getByPlaceholderText('alice'), 'alice')
      await user.click(screen.getByRole('button', { name: submitCode }))

      expect(
        screen.getByRole('button', { name: /delete.submitting/i }),
      ).toBeDisabled()
      expect(screen.getByRole('button', { name: cancelCode })).toBeDisabled()
      expect(screen.getByPlaceholderText('alice')).toBeDisabled()

      resolve()
      await waitFor(() => {
        expect(navigate).toHaveBeenCalledWith(ROUTES.CONFIRM_ACCOUNT_DELETION)
      })
    })
  })
})
