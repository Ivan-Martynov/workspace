import { render, screen } from '@testing-library/react'
import { vi, describe, expect, test } from 'vitest'
import { MemoryRouter } from 'react-router-dom'

import { AuthContext } from '../context/AuthContext'
import DeleteConfirmationPage from './DeleteConfirmationPage'

vi.mock('react-i18next', () => ({
  useTranslation: () => ({ t: (key) => key }),
}))

const renderPage = (authValue = {}) => {
  const auth = { user: null, ...authValue }
  return render(
    <MemoryRouter>
      <AuthContext.Provider value={auth}>
        <DeleteConfirmationPage />
      </AuthContext.Provider>
    </MemoryRouter>,
  )
}

describe('AuthPage', () => {
  test('shows redirecting if no user', () => {
    renderPage()
    expect(screen.getByRole('status')).toHaveTextContent(
      'delete.successMessage',
    )
  })

  test('shows waiting test if user is logged in', () => {
    renderPage({ user: { id: '1', username: 'alice' } })
    expect(screen.getByText('common.waitingUser')).toBeInTheDocument()
  })
})
