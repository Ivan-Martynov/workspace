import { renderHook, act } from '@testing-library/react'
import { vi, beforeEach, describe, test, expect } from 'vitest'

import { useFormSubmit } from './useFormSubmit'

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key, params) =>
      params && Object.keys(params).length
        ? `${key} ${JSON.stringify(params)}`
        : key,
  }),
}))

const createMockEvent = () => ({ preventDefault: vi.fn() })

describe('useFormSubmit', () => {
  beforeEach(() => vi.clearAllMocks())

  describe('initial state', () => {
    test('initially error is null', () => {
      const { result } = renderHook(() => useFormSubmit())
      expect(result.current.error).toBeNull()
    })

    test('initially errorMessage is empty', () => {
      const { result } = renderHook(() => useFormSubmit())
      expect(result.current.errorMessage).toBe('')
    })

    test('initially loading is false', () => {
      const { result } = renderHook(() => useFormSubmit())
      expect(result.current.loading).toBe(false)
    })
  })

  describe('handleSubmit', () => {
    test('calls preventDefault', async () => {
      const onSubmit = vi.fn().mockResolvedValue(undefined)
      const { result } = renderHook(() => useFormSubmit(onSubmit))
      const mockEvent = createMockEvent()
      await act(async () => result.current.handleSubmit(mockEvent))
      expect(mockEvent.preventDefault).toHaveBeenCalledOnce()
    })

    test('calls onSubmit', async () => {
      const onSubmit = vi.fn().mockResolvedValue(undefined)
      const { result } = renderHook(() => useFormSubmit(onSubmit))
      await act(async () => result.current.handleSubmit(createMockEvent()))
      expect(onSubmit).toHaveBeenCalledOnce()
    })

    test('sets loading to true during submission', async () => {
      let resolve
      const onSubmit = vi.fn().mockReturnValue(
        new Promise((res) => {
          resolve = res
        }),
      )
      const { result } = renderHook(() => useFormSubmit(onSubmit))

      act(() => {
        result.current.handleSubmit(createMockEvent())
      })
      expect(result.current.loading).toBe(true)

      await act(async () => {
        resolve()
      })
      expect(result.current.loading).toBe(false)
    })

    test('clears error on new submission', async () => {
      const onSubmit = vi
        .fn()
        .mockRejectedValueOnce({ code: 'networkError' })
        .mockResolvedValueOnce(undefined)
      const { result } = renderHook(() => useFormSubmit(onSubmit))
      const mockEvent = createMockEvent()
      await act(async () => {
        result.current.handleSubmit(mockEvent)
      })
      expect(result.current.error).not.toBeNull()
      expect(result.current.errorMessage).toBe('errors.networkError')

      await act(async () => {
        result.current.handleSubmit(mockEvent)
      })
      expect(result.current.error).toBeNull()
    })
  })

  describe('error handling', () => {
    test('sets error with code from thrown error', async () => {
      const onSubmit = vi
        .fn()
        .mockRejectedValueOnce({ code: 'invalidCredentials' })
      const { result } = renderHook(() => useFormSubmit(onSubmit))

      await act(async () => {
        result.current.handleSubmit(createMockEvent())
      })
      expect(result.current.error).toEqual({
        code: 'invalidCredentials',
        params: {},
      })
    })

    test('falls back to message when code is missing', async () => {
      const onSubmit = vi
        .fn()
        .mockRejectedValueOnce(new Error('Something went wrong'))
      const { result } = renderHook(() => useFormSubmit(onSubmit))

      await act(async () => {
        result.current.handleSubmit(createMockEvent())
      })
      expect(result.current.error).toEqual({
        code: 'Something went wrong',
        params: {},
      })
    })

    test('sets error params when provided', async () => {
      const onSubmit = vi
        .fn()
        .mockRejectedValueOnce({ code: 'passwordTooShort', params: { min: 8 } })
      const { result } = renderHook(() => useFormSubmit(onSubmit))

      await act(async () => {
        result.current.handleSubmit(createMockEvent())
      })
      expect(result.current.error).toEqual({
        code: 'passwordTooShort',
        params: { min: 8 },
      })
    })

    test('errorMessage is translated using error code', async () => {
      const onSubmit = vi
        .fn()
        .mockRejectedValueOnce({ code: 'invalidCredentials' })
      const { result } = renderHook(() => useFormSubmit(onSubmit))

      await act(async () => {
        result.current.handleSubmit(createMockEvent())
      })
      expect(result.current.errorMessage).toBe('errors.invalidCredentials')
    })

    test('errorMessage includes params', async () => {
      const onSubmit = vi
        .fn()
        .mockRejectedValueOnce({ code: 'passwordTooShort', params: { min: 8 } })
      const { result } = renderHook(() => useFormSubmit(onSubmit))

      await act(async () => {
        result.current.handleSubmit(createMockEvent())
      })
      expect(result.current.errorMessage).toBe(
        'errors.passwordTooShort {"min":8}',
      )
    })

    test('loading is false after error', async () => {
      const onSubmit = vi.fn().mockRejectedValueOnce({ code: 'networkError' })
      const { result } = renderHook(() => useFormSubmit(onSubmit))

      await act(async () => {
        result.current.handleSubmit(createMockEvent())
      })
      expect(result.current.loading).toBe(false)
    })
  })
})
