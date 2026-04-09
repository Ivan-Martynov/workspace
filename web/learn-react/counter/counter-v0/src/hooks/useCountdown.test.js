import { renderHook, act } from '@testing-library/react'
import { vi, beforeEach, afterEach, describe, test, expect } from 'vitest'

import { useCountdown } from './useCountdown'

describe('useCountdown', () => {
  beforeEach(() => vi.useFakeTimers())
  afterEach(() => vi.useRealTimers())

  describe('initial state', () => {
    test('starts at 0 by default', () => {
      const { result } = renderHook(() => useCountdown())
      expect(result.current.countdown).toBe(0)
    })

    test('starts at provided initial value', () => {
      const value = 5
      const { result } = renderHook(() => useCountdown(value))
      expect(result.current.countdown).toBe(value)
    })
  })

  describe('countdown behaviour', () => {
    test('decrements by 1 each second', () => {
      const { result } = renderHook(() => useCountdown(3))

      act(() => vi.advanceTimersByTime(1000))
      expect(result.current.countdown).toBe(2)
      act(() => vi.advanceTimersByTime(1000))
      expect(result.current.countdown).toBe(1)
      act(() => vi.advanceTimersByTime(1000))
      expect(result.current.countdown).toBe(0)
    })

    test('stops at 0', () => {
      const { result } = renderHook(() => useCountdown(1))

      act(() => vi.advanceTimersByTime(1000))
      expect(result.current.countdown).toBe(0)

      act(() => vi.advanceTimersByTime(5000))
      expect(result.current.countdown).toBe(0)
    })

    test('does not decrement when starting at 0', () => {
      const { result } = renderHook(() => useCountdown(0))

      act(() => vi.advanceTimersByTime(5000))
      expect(result.current.countdown).toBe(0)
    })
  })

  describe('reset', () => {
    test('resets countdown to given value', () => {
      const { result } = renderHook(() => useCountdown())

      act(() => result.current.reset(3))
      expect(result.current.countdown).toBe(3)
    })

    test('restarts countdown after reset', () => {
      const { result } = renderHook(() => useCountdown())

      act(() => result.current.reset(3))
      act(() => vi.advanceTimersByTime(1000))
      expect(result.current.countdown).toBe(2)
    })

    test('resets mid-countdown', () => {
      const { result } = renderHook(() => useCountdown(15))

      act(() => vi.advanceTimersByTime(1000))
      expect(result.current.countdown).toBe(14)

      act(() => result.current.reset(3))
      expect(result.current.countdown).toBe(3)

      act(() => vi.advanceTimersByTime(1000))
      expect(result.current.countdown).toBe(2)
    })
  })
})
