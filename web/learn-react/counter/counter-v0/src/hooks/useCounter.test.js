import { renderHook, act } from '@testing-library/react'
import { describe, test, expect } from 'vitest'

import { useCounter } from './useCounter'

describe('useCounter', () => {
  describe('initial state', () => {
    test('starts at 0 by default', () => {
      const { result } = renderHook(() => useCounter())
      expect(result.current.value).toBe(0)
    })

    test('starts at provided initial value', () => {
      const value = 4
      const { result } = renderHook(() => useCounter(value))
      expect(result.current.value).toBe(value)
    })
  })

  describe('increment', () => {
    test('increments a value by one', () => {
      const { result } = renderHook(() => useCounter(4))
      act(() => result.current.increment())
      expect(result.current.value).toBe(5)
    })

    test('increment beyond MAX_SAFE_INTEGER loses precision', () => {
      const { result } = renderHook(() => useCounter(Number.MAX_SAFE_INTEGER))
      act(() => result.current.increment())
      expect(result.current.value).toBe(Number.MAX_SAFE_INTEGER + 1)
    })
  })

  describe('decrement', () => {
    test('decrements a value by one', () => {
      const { result } = renderHook(() => useCounter(7))
      act(() => result.current.decrement())
      expect(result.current.value).toBe(6)
    })

    test('decrements below zero', () => {
      const { result } = renderHook(() => useCounter())
      act(() => result.current.decrement())
      expect(result.current.value).toBe(-1)
    })
  })

  describe('double', () => {
    test('doubles a value', () => {
      const { result } = renderHook(() => useCounter(5))
      act(() => result.current.double())
      expect(result.current.value).toBe(10)
    })

    test('double of zero stays zero', () => {
      const { result } = renderHook(() => useCounter())
      act(() => result.current.double())
      expect(result.current.value).toBe(0)
    })

    test('double of a large number', () => {
      const { result } = renderHook(() => useCounter(Number.MAX_SAFE_INTEGER))
      act(() => result.current.double())
      expect(result.current.value).toBe(Number.MAX_SAFE_INTEGER * 2)
    })

    test('double of number at 32-bit boundary', () => {
      const { result } = renderHook(() => useCounter(1_073_741_824))
      act(() => result.current.double())
      expect(result.current.value).toBe(2_147_483_648)
    })
  })

  describe('halve', () => {
    test('halve of an even value', () => {
      const { result } = renderHook(() => useCounter(8))
      act(() => result.current.halve())
      expect(result.current.value).toBe(4)
    })

    test('halve of a negative even value', () => {
      const { result } = renderHook(() => useCounter(-8))
      act(() => result.current.halve())
      expect(result.current.value).toBe(-4)
    })

    test('halves and truncates the result for an odd value', () => {
      const { result } = renderHook(() => useCounter(11))
      act(() => result.current.halve())
      expect(result.current.value).toBe(5)
    })

    test('halve truncates toward zero for a negative odd value', () => {
      const { result } = renderHook(() => useCounter(-11))
      act(() => result.current.halve())
      expect(result.current.value).toBe(-5)
    })

    test('halve of zero stays zero', () => {
      const { result } = renderHook(() => useCounter())
      act(() => result.current.halve())
      expect(result.current.value).toBe(0)
    })

    test('halve of number beyond 32-bit range', () => {
      const { result } = renderHook(() => useCounter(2_147_483_648))
      act(() => result.current.halve())
      expect(result.current.value).toBe(1_073_741_824)
    })

    test('halve of MAX_SAFE_INTEGER', () => {
      const { result } = renderHook(() => useCounter(Number.MAX_SAFE_INTEGER))
      act(() => result.current.halve())
      expect(result.current.value).toBe(4_503_599_627_370_495)
    })
  })

  describe('power2', () => {
    test('squares a value', () => {
      const { result } = renderHook(() => useCounter(7))
      act(() => result.current.power2())
      expect(result.current.value).toBe(49)
    })

    test('squares negative numbers to positive', () => {
      const { result } = renderHook(() => useCounter(-3))
      act(() => result.current.power2())
      expect(result.current.value).toBe(9)
    })

    test('handles zero initial value for power2', () => {
      const { result } = renderHook(() => useCounter())
      act(() => result.current.power2())
      expect(result.current.value).toBe(0)
    })

    test('squaring a number beyond safe integer range', () => {
      const { result } = renderHook(() => useCounter(100_000_000))
      act(() => result.current.power2())
      expect(result.current.value).toBe(10_000_000_000_000_000)
    })
  })

  describe('zero', () => {
    test('sets the value to zero', () => {
      const { result } = renderHook(() => useCounter(-5))
      act(() => result.current.zero())
      expect(result.current.value).toBe(0)
    })
  })

  describe('reset', () => {
    test('resets the value to the original value', () => {
      const value = 4
      const { result } = renderHook(() => useCounter(value))
      act(() => result.current.increment())

      act(() => result.current.reset())
      expect(result.current.value).toBe(value)
    })

    test('resetting differs from zero when initial value is not zero', () => {
      const value = 4
      const { result } = renderHook(() => useCounter(value))
      act(() => result.current.zero())
      expect(result.current.value).toBe(0)

      act(() => result.current.reset())
      expect(result.current.value).toBe(value)
    })
  })
})
