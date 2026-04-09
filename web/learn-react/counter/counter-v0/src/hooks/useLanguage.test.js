import { renderHook, act } from '@testing-library/react'
import { vi, beforeEach, describe, test, expect } from 'vitest'

import { useLanguage } from './useLanguage'
import i18n from '../i18n'

describe('useLanguage', () => {
  beforeEach(() => {
    vi.clearAllMocks()
    localStorage.clear()
    i18n.changeLanguage('en')
  })

  describe('initial state', () => {
    test('language is i18n.language', () => {
      const { result } = renderHook(() => useLanguage())
      expect(result.current.language).toBe(i18n.language)
      expect(result.current.language).toBe('en')
    })
  })

  describe('setLanguage', () => {
    test('changes language', () => {
      const { result } = renderHook(() => useLanguage())
      act(() => result.current.setLanguage('fi'))
      expect(result.current.language).toBe('fi')
      expect(i18n.language).toBe('fi')
    })

    test('saves language to localStorage', () => {
      const { result } = renderHook(() => useLanguage())
      act(() => result.current.setLanguage('fi'))
      expect(localStorage.getItem('language')).toBe('fi')
    })
  })

  describe('toggle', () => {
    test('toggles from en to fi', () => {
      const { result } = renderHook(() => useLanguage())
      act(() => result.current.toggle())
      expect(result.current.language).toBe('fi')
      expect(localStorage.getItem('language')).toBe('fi')
    })

    test('toggles from fi to en', () => {
      i18n.changeLanguage('fi')
      const { result } = renderHook(() => useLanguage())
      act(() => result.current.toggle())
      expect(result.current.language).toBe('en')
      expect(localStorage.getItem('language')).toBe('en')
    })
  })
})
