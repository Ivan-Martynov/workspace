import { vi, describe, expect, test, beforeEach, afterEach } from 'vitest'
import { api } from './api'
import { AUTH_STORAGE_KEY } from './constants'

const mockFetchResponse = (overrides = {}) => {
  vi.mocked(fetch).mockResolvedValue({
    ok: true,
    status: 200,
    headers: { get: () => null },
    json: async () => null,
    ...overrides,
  })
}

beforeEach(() => {
  vi.stubGlobal('fetch', vi.fn())
  localStorage.clear()
})

afterEach(() => {
  vi.unstubAllGlobals()
})

describe.skip('api', () => {
  describe('headers', () => {
    test('sends Content-Type application/json', async () => {
      mockFetchResponse()
      await api.get('/api/test')
      expect(fetch).toHaveBeenCalledWith(
        '/api/test',
        expect.objectContaining({
          headers: expect.objectContaining({
            'Content-Type': 'application/json',
          }),
        }),
      )
    })

    test('injects Authorization header when token exists', async () => {
      localStorage.setItem(
        AUTH_STORAGE_KEY,
        JSON.stringify({ token: 'abc123' }),
      )
      mockFetchResponse()
      await api.get('/api/test')
      expect(fetch).toHaveBeenCalledWith(
        '/api/test',
        expect.objectContaining({
          headers: expect.objectContaining({
            Authorization: 'Bearer abc123',
          }),
        }),
      )
    })

    test('does not inject Authorization header when no token', async () => {
      mockFetchResponse()
      await api.get('/api/test')
      const calledWith = vi.mocked(fetch).mock.calls[0][1]
      expect(calledWith.headers).not.toHaveProperty('Authorization')
    })
  })

  describe('error handling', () => {
    test('throws server error message on non-2xx response', async () => {
      mockFetchResponse({
        ok: false,
        status: 401,
        json: async () => ({ error: 'Unauthorized' }),
      })

      await expect(api.get('/api/test')).rejects.toThrow('Unauthorized')
    })

    test('throws server error message on 5xx response', async () => {
      mockFetchResponse({
        ok: false,
        status: 500,
        json: async () => ({}),
      })

      await expect(api.get('/api/test')).rejects.toThrow(
        'Server error, please try again later',
      )
    })

    test('throws with fallback message with no error in response', async () => {
      mockFetchResponse({
        ok: false,
        status: 400,
        json: async () => ({}),
      })

      await expect(api.get('/api/test')).rejects.toThrow('Request failed')
    })

    test('throws connection error when fetch fails', async () => {
      vi.mocked(fetch).mockRejectedValue(new TypeError('Failed to fetch'))
      await expect(api.get('/api/test')).rejects.toThrow(
        'Unable to connect to server, please try again later',
      )
    })
  })

  describe('empty responses', () => {
    test('returns null for 204 response', async () => {
      mockFetchResponse({ status: 204, headers: { get: () => '0' } })
      const result = await api.delete('/api/test')
      expect(result).toBeNull()
    })

    test('returns null when content-length is 0', async () => {
      mockFetchResponse({ status: 200, headers: { get: () => '0' } })
      const result = await api.delete('/api/test')
      expect(result).toBeNull()
    })
  })

  describe('api methods', () => {
    test('api.get calls fetch with the GET method', async () => {
      mockFetchResponse()
      await api.get('/api/test')
      expect(fetch).toHaveBeenCalledWith(
        '/api/test',
        expect.objectContaining({
          method: 'GET',
        }),
      )
    })

    test('api.post calls fetch with the POST method and serialized body', async () => {
      mockFetchResponse()
      await api.post('/api/test', { username: 'alice' })
      expect(fetch).toHaveBeenCalledWith(
        '/api/test',
        expect.objectContaining({
          method: 'POST',
          body: JSON.stringify({ username: 'alice' }),
        }),
      )
    })

    test('api.patch calls fetch with the PATCH method and serialized body', async () => {
      mockFetchResponse()
      await api.patch('/api/test', { username: 'alice' })
      expect(fetch).toHaveBeenCalledWith(
        '/api/test',
        expect.objectContaining({
          method: 'PATCH',
          body: JSON.stringify({ username: 'alice' }),
        }),
      )
    })

    test('api.put calls fetch with the PUT method and serialized body', async () => {
      mockFetchResponse()
      await api.put('/api/test', { username: 'alice' })
      expect(fetch).toHaveBeenCalledWith(
        '/api/test',
        expect.objectContaining({
          method: 'PUT',
          body: JSON.stringify({ username: 'alice' }),
        }),
      )
    })

    test('api.delete calls fetch with the DELETE method', async () => {
      mockFetchResponse()
      await api.delete('/api/test')
      expect(fetch).toHaveBeenCalledWith(
        '/api/test',
        expect.objectContaining({
          method: 'DELETE',
        }),
      )
    })

    test('body is not included when not provided', async () => {
      mockFetchResponse()
      await api.get('/api/test')
      const calledWith = vi.mocked(fetch).mock.calls[0][1]
      expect(calledWith).not.toHaveProperty('body')
    })
  })
})
