import { render, screen } from '@testing-library/react'
import { vi, beforeEach, describe, expect, test } from 'vitest'
import userEvent from '@testing-library/user-event'

import OperationButton from './OperationButton'

describe('OperationButton', () => {
  let user
  beforeEach(() => (user = userEvent.setup()))

  test('renders children', () => {
    render(<OperationButton onClick={vi.fn()}>+1</OperationButton>)
    expect(screen.getByRole('button', { name: '+1' })).toBeInTheDocument()
  })

  test('calls onClick when clicked', async () => {
    const onClick = vi.fn()
    render(<OperationButton onClick={onClick}>+1</OperationButton>)
    await user.click(screen.getByRole('button', { name: '+1' }))
    expect(onClick).toHaveBeenCalledOnce()
  })

  test('is disabled when disabled prop is true', () => {
    render(
      <OperationButton onClick={vi.fn()} disabled={true}>
        +1
      </OperationButton>,
    )
    expect(screen.getByRole('button', { name: '+1' })).toBeDisabled()
  })

  test('is not disabled by default', () => {
    render(<OperationButton onClick={vi.fn()}>+1</OperationButton>)
    expect(screen.getByRole('button', { name: '+1' })).not.toBeDisabled()
  })

  test('has type button to prevent form submission', () => {
    render(<OperationButton onClick={vi.fn()}>+1</OperationButton>)
    expect(screen.getByRole('button', { name: '+1' })).toHaveAttribute(
      'type',
      'button',
    )
  })
})
