import { renderHook } from '@testing-library/react'
import { vi, describe, test, expect } from 'vitest'

import { useAuth } from './useAuth'
import { AuthContext } from './AuthContext'

describe('useAuth', () => {
  test('returns context value when used within AuthProvider', () => {
    const mockContext = {
      user: { id: '1', username: 'alice' },
      login: vi.fn(),
      logout: vi.fn(),
      register: vi.fn(),
      deleteAccount: vi.fn(),
    }
    const wrapper = ({ children }) => (
      <AuthContext.Provider value={mockContext}>
        {children}
      </AuthContext.Provider>
    )

    const { result } = renderHook(() => useAuth(), { wrapper })
    expect(result.current).toBe(mockContext)
  })

  test('throws when used outside AuthProvider', () => {
    expect(() => renderHook(() => useAuth())).toThrow(
      'useAuth must be used within AuthProvider',
    )
  })
})
