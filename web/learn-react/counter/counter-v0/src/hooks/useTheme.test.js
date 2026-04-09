import { renderHook, act } from '@testing-library/react'
import { beforeEach, describe, test, expect } from 'vitest'

import { useTheme } from './useTheme'

describe('useTheme', () => {
  beforeEach(() => {
    localStorage.clear()
    document.documentElement.removeAttribute('data-theme')
    document.documentElement.style.colorScheme = ''
  })

  describe('initial state', () => {
    test('defaults to light theme when no stored theme', () => {
      const { result } = renderHook(() => useTheme())
      expect(result.current.theme).toBe('light')
    })

    test('restores theme from localStorage', () => {
      localStorage.setItem('theme', 'dark')
      const { result } = renderHook(() => useTheme())
      expect(result.current.theme).toBe('dark')
    })
  })

  describe('side effects on mount', () => {
    test('sets data-theme attribute on mount', () => {
      renderHook(() => useTheme())
      expect(document.documentElement.getAttribute('data-theme')).toBe('light')
    })

    test('sets colorScheme on mount', () => {
      renderHook(() => useTheme())
      expect(document.documentElement.style.colorScheme).toBe('light')
    })

    test('persists theme to localStorage on mount', () => {
      renderHook(() => useTheme())
      expect(localStorage.getItem('theme')).toBe('light')
    })
  })

  describe('toggle', () => {
    test('toggles from light to dark', () => {
      const { result } = renderHook(() => useTheme())
      act(() => result.current.toggle())
      expect(result.current.theme).toBe('dark')
    })

    test('toggles from dark to light', () => {
      localStorage.setItem('theme', 'dark')
      const { result } = renderHook(() => useTheme())
      act(() => result.current.toggle())
      expect(result.current.theme).toBe('light')
    })

    test('updates data-theme after toggle', () => {
      const { result } = renderHook(() => useTheme())
      act(() => result.current.toggle())
      expect(document.documentElement.getAttribute('data-theme')).toBe('dark')
    })

    test('updates colorScheme after toggle', () => {
      const { result } = renderHook(() => useTheme())
      act(() => result.current.toggle())
      expect(document.documentElement.style.colorScheme).toBe('dark')
    })

    test('persists theme to localStorage after toggle', () => {
      const { result } = renderHook(() => useTheme())
      act(() => result.current.toggle())
      expect(localStorage.getItem('theme')).toBe('dark')
    })

    test('toggles back to original two toggles', () => {
      const { result } = renderHook(() => useTheme())
      act(() => result.current.toggle())
      act(() => result.current.toggle())
      expect(result.current.theme).toBe('light')
    })
  })
})
