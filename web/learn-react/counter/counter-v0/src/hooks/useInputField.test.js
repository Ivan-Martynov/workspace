import { renderHook, act } from '@testing-library/react'
import { describe, test, expect } from 'vitest'

import { useInputField } from './useInputField'

describe('useInputField', () => {
  describe('initial state', () => {
    test('initial value is empty', () => {
      const { result } = renderHook(() => useInputField())
      expect(result.current.value).toBe('')
    })

    test('placeholder is undefined when not provided', () => {
      const { result } = renderHook(() => useInputField())
      expect(result.current.inputProps.placeholder).toBeUndefined()
    })

    test('empty placeholder is not undefined nor null', () => {
      const { result } = renderHook(() => useInputField(''))
      expect(result.current.inputProps.placeholder).toBeDefined()
      expect(result.current.inputProps.placeholder).not.toBeNull()
      expect(result.current.inputProps.placeholder).toBe('')
    })

    test('placeholder is set correctly when provided', () => {
      const { result } = renderHook(() => useInputField('identifier'))
      expect(result.current.inputProps.placeholder).toBe('identifier')
    })

    test('required is true by default', () => {
      const { result } = renderHook(() => useInputField())
      expect(result.current.inputProps.required).toBe(true)
    })

    test('required is set properly when provided', () => {
      const { result } = renderHook(() =>
        useInputField('', { required: false }),
      )
      expect(result.current.inputProps.required).toBe(false)
    })

    test('type is text by default', () => {
      const { result } = renderHook(() => useInputField())
      expect(result.current.inputProps.type).toBe('text')
    })

    test('type is set when provided', () => {
      const { result } = renderHook(() => useInputField('', { type: 'email' }))
      expect(result.current.inputProps.type).toBe('email')
    })

    test('additional options are spread to inputProps', () => {
      const { result } = renderHook(() =>
        useInputField('', { maxLength: 10, 'data-testid': 'my-input' }),
      )
      expect(result.current.inputProps.maxLength).toBe(10)
      expect(result.current.inputProps['data-testid']).toBe('my-input')
    })
  })

  describe('value updates', () => {
    test('onChange updates value', () => {
      const { result } = renderHook(() => useInputField())
      act(() =>
        result.current.inputProps.onChange({ target: { value: 'new value' } }),
      )
      expect(result.current.value).toBe('new value')
    })

    test('multiple onChange calls update value sequentially', () => {
      const { result } = renderHook(() => useInputField())

      act(() =>
        result.current.inputProps.onChange({ target: { value: 'first' } }),
      )
      expect(result.current.value).toBe('first')

      act(() =>
        result.current.inputProps.onChange({ target: { value: 'second' } }),
      )
      expect(result.current.value).toBe('second')
    })
  })

  describe('reset', () => {
    test('reset clears the value', () => {
      const { result } = renderHook(() => useInputField())
      act(() =>
        result.current.inputProps.onChange({ target: { value: 'test' } }),
      )
      act(() => result.current.reset())
      expect(result.current.value).toBe('')
    })

    test('reset after multiple changes clears value', () => {
      const { result } = renderHook(() => useInputField())

      act(() => {
        result.current.inputProps.onChange({ target: { value: 'first' } })
      })
      act(() => {
        result.current.inputProps.onChange({ target: { value: 'second' } })
      })
      act(() => {
        result.current.inputProps.onChange({ target: { value: 'third' } })
      })
      act(() => result.current.reset())
      expect(result.current.value).toBe('')
    })
  })
})
