import { vi, test, describe, beforeEach, afterEach, expect } from 'vitest'

import { deleteAccountRequest, loginRequest, registerRequest } from './auth'

const mockFetchResponse = (overrides = {}) => {
  // mockResolvedValue defines what the fake fetch call will return.
  vi.mocked(fetch).mockResolvedValue({
    ok: true,
    status: 200,
    headers: { get: () => null },
    json: async () => null,
    ...overrides,
  })
}

// Set up before each test: replace the global fetch with a mock function
beforeEach(() => {
  vi.stubGlobal('fetch', vi.fn())
})

// Clean up after each test: restore all stubbed globals
afterEach(() => {
  vi.unstubAllGlobals()
})

describe.skip('auth service', () => {
  describe('loginRequest', () => {
    // Test case: verify that loginRequest returns a token and user object
    test('loginRequest returns token and user', async () => {
      // Mock the fetch function to return a successful response with token
      // and user data
      mockFetchResponse({
        json: async () => ({
          token: 'abc',
          user: { id: '1', username: 'alice' },
        }),
      })

      const result = await loginRequest('alice', 'secret')
      expect(result.user.username).toBe('alice')
      expect(result.token).toBe('abc')
    })

    test('throws on invalid credentials', async () => {
      mockFetchResponse({
        ok: false,
        status: 401,
        json: async () => ({ error: 'Invalid credentials' }),
      })

      await expect(loginRequest('alice', 'wrong')).rejects.toThrow(
        'Invalid credentials',
      )
    })

    test('throws with fallback message when no error in response', async () => {
      mockFetchResponse({ ok: false, status: 400, json: async () => ({}) })

      await expect(loginRequest('alice', 'secret')).rejects.toThrow(
        'Request failed',
      )
    })

    test('calls fetch with correct url and body', async () => {
      const dummy = 'dummy'

      mockFetchResponse({
        json: async () => ({
          token: 'abc',
          user: { id: '1', username: dummy },
        }),
      })

      await loginRequest(dummy, 'pass')
      expect(fetch).toHaveBeenCalledWith(
        '/api/auth/login',
        expect.objectContaining({
          method: 'POST',
          body: JSON.stringify({ identifier: dummy, password: 'pass' }),
        }),
      )
    })
  })

  describe('registerRequest', () => {
    test('returns userId on success', async () => {
      mockFetchResponse({
        status: 201,
        json: async () => ({
          message: 'User created',
          userId: '123',
        }),
      })

      const result = await registerRequest(
        'alice',
        'alice@example.com',
        'secret',
      )
      expect(result.userId).toBe('123')
    })

    test('throws on duplicate username or email', async () => {
      mockFetchResponse({
        ok: false,
        status: 409,
        json: async () => ({
          error: 'Username or email already exists',
        }),
      })

      await expect(
        registerRequest('alice', 'alice@example.com', 'secret'),
      ).rejects.toThrow('Username or email already exists')
    })

    test('calls fetch with correct url and body', async () => {
      mockFetchResponse({
        status: 201,
        json: async () => ({ message: 'User created', userId: '123' }),
      })

      await registerRequest('alice', 'alice@example.com', 'secret')
      expect(fetch).toHaveBeenCalledWith(
        '/api/users',
        expect.objectContaining({
          method: 'POST',
          body: JSON.stringify({
            username: 'alice',
            email: 'alice@example.com',
            password: 'secret',
          }),
        }),
      )
    })
  })

  describe('deleteAccountRequest', () => {
    test('returns null on success', async () => {
      mockFetchResponse({ status: 204, headers: { get: () => '0' } })

      const result = await deleteAccountRequest('123')
      expect(result).toBeNull()
    })

    test('throws on unauthorized', async () => {
      mockFetchResponse({
        ok: false,
        status: 401,
        json: async () => ({ error: 'Authentication required' }),
      })

      await expect(deleteAccountRequest('123')).rejects.toThrow(
        'Authentication required',
      )
    })

    test('calls fetch with correct url and method', async () => {
      mockFetchResponse({ status: 204, headers: { get: () => '0' } })

      await deleteAccountRequest('123')

      expect(fetch).toHaveBeenCalledWith(
        '/api/users/123',
        expect.objectContaining({ method: 'DELETE' }),
      )
    })

    test('calls fetch with correct user id in url', async () => {
      mockFetchResponse({ status: 204, headers: { get: () => '0' } })

      await deleteAccountRequest('abc123')
      expect(fetch).toHaveBeenCalledWith('/api/users/abc123', expect.anything())
    })
  })
})
