import { render, screen, act } from '@testing-library/react'
import { vi, beforeEach, afterEach, describe, expect, test } from 'vitest'

import Message from './Message'

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key) => key,
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
}))

describe('Message', () => {
  describe('render', () => {
    test('renders text content', () => {
      render(<Message text='Something went wrong' type='error' />)
      expect(screen.getByRole('alert')).toBeInTheDocument()
    })

    test('renders nothing when text is empty', () => {
      render(<Message text='' type='error' />)
      expect(screen.queryByRole('alert')).not.toBeInTheDocument()
    })

    test('renders nothing when text is null', () => {
      render(<Message text={null} type='error' />)
      expect(screen.queryByRole('alert')).not.toBeInTheDocument()
    })

    test('defaults to info type with status role', () => {
      render(<Message text='Some info' />)
      expect(screen.getByRole('status')).toBeInTheDocument()
    })
  })

  describe('types and roles', () => {
    test.each([
      ['error', 'alert'],
      ['warning', 'alert'],
      ['info', 'status'],
      ['success', 'status'],
    ])('%s type has %s role', (type, role) => {
      render(<Message text='Test' type={type} />)
      expect(screen.getByRole(role)).toBeInTheDocument()
    })

    test.each([
      ['error', 'alert'],
      ['warning', 'alert'],
      ['info', 'status'],
      ['success', 'status'],
    ])('%s type applies correct className', (type, role) => {
      render(<Message text='Test' type={type} />)
      expect(screen.getByRole(role)).toHaveClass(type)
    })
  })

  describe('as prop', () => {
    test('renders as p by default', () => {
      const { container } = render(<Message text='Test' type='info' />)
      expect(container.firstChild.tagName).toBe('P')
    })

    test('renders as div when specified', () => {
      const { container } = render(<Message text='Test' type='info' as='div' />)
      expect(container.firstChild.tagName).toBe('DIV')
    })
  })

  describe('visibility', () => {
    test('shows message when text is provided', () => {
      render(<Message text='Hello' type='info' />)
      expect(screen.getByRole('status')).toBeInTheDocument()
    })

    test('hides message when text changes to empty', () => {
      const { rerender } = render(<Message text='Hello' type='info' />)
      expect(screen.getByRole('status')).toBeInTheDocument()
      rerender(<Message text='' type='info' />)
      expect(screen.queryByRole('status')).not.toBeInTheDocument()
    })

    test('shows new message after being empty', () => {
      const { rerender } = render(<Message text='' type='info' />)
      expect(screen.queryByRole('status')).not.toBeInTheDocument()
      rerender(<Message text='Hello' type='info' />)
      expect(screen.getByRole('status')).toBeInTheDocument()
    })
  })

  describe('auto-dismiss', () => {
    beforeEach(() => vi.useFakeTimers())
    afterEach(() => vi.useRealTimers())

    test('hides message after duration', () => {
      render(<Message text='Hello' type='info' duration={3000} />)
      expect(screen.getByRole('status')).toBeInTheDocument()
      act(() => vi.advanceTimersByTime(3000))
      expect(screen.queryByRole('status')).not.toBeInTheDocument()
    })

    test('Doesn not hide message without duration', () => {
      render(<Message text='Hello' type='info' />)
      act(() => vi.advanceTimersByTime(10000))
      expect(screen.getByRole('status')).toBeInTheDocument()
    })

    test('resets timer when id changes', () => {
      const { rerender } = render(
        <Message id='msg1' text='Hello' type='info' duration={3000} />,
      )
      act(() => vi.advanceTimersByTime(2000))
      expect(screen.getByRole('status')).toBeInTheDocument()

      rerender(<Message id='msg2' text='Hello' type='info' duration={3000} />)
      act(() => vi.advanceTimersByTime(2000))
      expect(screen.getByRole('status')).toBeInTheDocument()

      act(() => vi.advanceTimersByTime(1000))
      expect(screen.queryByRole('status')).not.toBeInTheDocument()
    })

    test('does not reset timer on language change (text change with same id)', () => {
      const { rerender } = render(
        <Message id='msg1' text='Hello' type='info' duration={3000} />,
      )
      act(() => vi.advanceTimersByTime(2000))
      rerender(<Message id='msg1' text='Hei' type='info' duration={3000} />)
      act(() => vi.advanceTimersByTime(1000))
      expect(screen.queryByRole('status')).not.toBeInTheDocument()
    })
  })
})
