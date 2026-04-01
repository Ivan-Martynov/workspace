import { render, screen, fireEvent } from '@testing-library/react'
import { vi, describe, expect, test } from 'vitest'

import OperationButton from './OperationButton'

describe('OperationButton', () => {
  test('renders children', () => {
    render(<OperationButton onClick={vi.fn()}>+1</OperationButton>)
    expect(screen.getByRole('button', { name: '+1' })).toBeInTheDocument()
  })

  test('calls onClick when clicked', () => {
    const onClick = vi.fn()
    render(<OperationButton onClick={onClick}>+1</OperationButton>)
    fireEvent.click(screen.getByRole('button', { name: '+1' }))
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

  test('has the button type', () => {
    render(<OperationButton onClick={vi.fn()}>+1</OperationButton>)
    expect(screen.getByRole('button', { name: '+1' })).toHaveAttribute(
      'type',
      'button',
    )
  })
})
