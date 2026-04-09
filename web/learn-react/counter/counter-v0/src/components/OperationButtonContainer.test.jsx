import { render, screen } from '@testing-library/react'
import { vi, beforeEach, describe, expect, test } from 'vitest'
import userEvent from '@testing-library/user-event'

import OperationButtonContainer from './OperationButtonContainer'

const createMockCounter = () => ({
  increment: vi.fn(),
  double: vi.fn(),
  decrement: vi.fn(),
  halve: vi.fn(),
  power2: vi.fn(),
  zero: vi.fn(),
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

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key) => key,
    i18n: { language: 'en', changeLanguage: vi.fn() },
  }),
}))

describe('OperationButtonContainer', () => {
  const zeroButtonName = /game\.zero/i
  const resetButtonName = /game\.reset/i

  let user
  beforeEach(() => (user = userEvent.setup()))

  describe('render', () => {
    test.each(['+1', '×2', '-1', '÷2', '^2', zeroButtonName, resetButtonName])(
      'renders %s button',
      (name) => {
        renderContainer()
        expect(screen.getByRole('button', { name })).toBeInTheDocument()
      },
    )
  })

  describe('operations', () => {
    test.each([
      ['+1', 'increment'],
      ['×2', 'double'],
      ['-1', 'decrement'],
      ['÷2', 'halve'],
      ['^2', 'power2'],
      [zeroButtonName, 'zero'],
      [resetButtonName, 'reset'],
    ])('%s calls %s', async (name, handler) => {
      const { counter } = renderContainer()
      await user.click(screen.getByRole('button', { name }))
      expect(counter[handler]).toHaveBeenCalledOnce()
    })
  })

  describe('disabled', () => {
    test.each(['+1', '×2', '-1', '÷2', '^2', zeroButtonName])(
      '%s button is disabled when disabled prop is true',
      (name) => {
        renderContainer({ disabled: true })
        expect(screen.getByRole('button', { name })).toBeDisabled()
      },
    )

    test('reset button is never disabled', () => {
      renderContainer({ disabled: true })
      expect(
        screen.getByRole('button', { name: resetButtonName }),
      ).not.toBeDisabled()
    })

    test.each([
      ['+1', 'increment'],
      ['×2', 'double'],
      ['-1', 'decrement'],
      ['÷2', 'halve'],
      ['^2', 'power2'],
      [zeroButtonName, 'zero'],
    ])(
      'disabled %s button does not call %s handler when clicked',
      async (name, handler) => {
        const { counter } = renderContainer({ disabled: true })
        await user.click(screen.getByRole('button', { name }))
        expect(counter[handler]).not.toHaveBeenCalled()
      },
    )
  })
})
