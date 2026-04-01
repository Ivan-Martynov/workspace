import { render, screen, fireEvent } from '@testing-library/react'
import { vi, describe, expect, test } from 'vitest'

import OperationButtonContainer from './OperationButtonContainer'

const createMockCounter = () => ({
  increment: vi.fn(),
  double: vi.fn(),
  decrement: vi.fn(),
  halve: vi.fn(),
  power2: vi.fn(),
  reset: vi.fn(),
})

const renderContainer = (props = {}) => {
  const defaults = {
    counter: createMockCounter(),
    disabled: false,
    ...props,
  }

  return {
    ...render(<OperationButtonContainer {...defaults} />),
    counter: defaults.counter,
  }
}

describe('OperationButtonContainer', () => {
  describe('render', () => {
    test('renders all operation buttons', () => {
      renderContainer()
      expect(screen.getByRole('button', { name: '+1' })).toBeInTheDocument()
      expect(screen.getByRole('button', { name: '×2' })).toBeInTheDocument()
      expect(screen.getByRole('button', { name: '-1' })).toBeInTheDocument()
      expect(screen.getByRole('button', { name: '÷2' })).toBeInTheDocument()
      expect(screen.getByRole('button', { name: '^2' })).toBeInTheDocument()
      expect(screen.getByRole('button', { name: 'reset' })).toBeInTheDocument()
    })
  })

  describe('operations', () => {
    test('+1 calls updateCount with count + 1', () => {
      const { counter } = renderContainer()
      fireEvent.click(screen.getByRole('button', { name: '+1' }))
      expect(counter.increment).toHaveBeenCalledOnce()
    })

    test('×2 calls updateCount with count * 2', () => {
      const { counter } = renderContainer()

      fireEvent.click(screen.getByRole('button', { name: '×2' }))
      expect(counter.double).toHaveBeenCalledOnce()
    })

    test('-1 calls updateCount with count - 1', () => {
      const { counter } = renderContainer()
      fireEvent.click(screen.getByRole('button', { name: '-1' }))
      expect(counter.decrement).toHaveBeenCalledOnce()
    })

    test('÷2 calls updateCount with Math.trunc(count / 2)', () => {
      const { counter } = renderContainer()
      fireEvent.click(screen.getByRole('button', { name: '÷2' }))
      expect(counter.halve).toHaveBeenCalledOnce()
    })

    test('^2 calls updateCount with count * count', () => {
      const { counter } = renderContainer()
      fireEvent.click(screen.getByRole('button', { name: '^2' }))
      expect(counter.power2).toHaveBeenCalledOnce()
    })

    test('reset calls updateCount with 0', () => {
      const { counter } = renderContainer()
      fireEvent.click(screen.getByRole('button', { name: 'reset' }))
      expect(counter.reset).toHaveBeenCalledOnce()
    })
  })

  describe('disabled', () => {
    test('disables all buttons except reset when disabled is true', () => {
      renderContainer({ disabled: true })
      expect(screen.getByRole('button', { name: '+1' })).toBeDisabled()
      expect(screen.getByRole('button', { name: '×2' })).toBeDisabled()
      expect(screen.getByRole('button', { name: '-1' })).toBeDisabled()
      expect(screen.getByRole('button', { name: '÷2' })).toBeDisabled()
      expect(screen.getByRole('button', { name: '^2' })).toBeDisabled()
      expect(screen.getByRole('button', { name: 'reset' })).not.toBeDisabled()
    })
  })
})
