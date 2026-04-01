import { render, screen, fireEvent } from '@testing-library/react'
import { describe, expect, test } from 'vitest'
import GamePage from './GamePage'

const clickButton = (name, times = 1) => {
  for (let i = 0; i < times; ++i) {
    fireEvent.click(screen.getByRole('button', { name }))
  }
}

describe.skip('GamePage', () => {
  describe('render', () => {
    test('renders heading with target count', () => {
      render(<GamePage />)
      expect(screen.getByText('Count to 100')).toBeInTheDocument()
    })

    test('renders with custom count', () => {
      render(<GamePage targetCount={50} />)
      expect(screen.getByText('Count to 50')).toBeInTheDocument()
    })

    test('renders initial count as 0', () => {
      render(<GamePage />)
      expect(screen.getByText('0')).toBeInTheDocument()
    })
  })

  describe('counting', () => {
    test('increment count on +1 click', () => {
      render(<GamePage />)
      const n = 3
      clickButton('+1', n)
      expect(screen.getByText(`${n}`)).toBeInTheDocument()
    })

    test('decrements count on -1 click', () => {
      render(<GamePage />)

      const n = 2
      clickButton('-1', n)
      expect(screen.getByText(`-${n}`)).toBeInTheDocument()
    })

    test('doubles count on ×2 click', () => {
      render(<GamePage />)
      clickButton('+1', 3)
      clickButton('×2')
      expect(screen.getByText('6')).toBeInTheDocument()
    })

    test('halves count on ÷2 click', () => {
      render(<GamePage />)
      clickButton('+1', 2)
      clickButton('÷2')
      expect(screen.getByText('1')).toBeInTheDocument()
    })

    test('squares count on ^2 click', () => {
      render(<GamePage />)
      clickButton('+1', 2)
      clickButton('^2')
      expect(screen.getByText('4')).toBeInTheDocument()
    })

    test('resets count to 0 on reset click', () => {
      render(<GamePage />)
      clickButton('+1', 2)
      clickButton('reset')
      expect(screen.getByText('0')).toBeInTheDocument()
    })
  })

  describe('game over', () => {
    test('disables operation buttons when target is reached', () => {
      render(<GamePage targetCount={1} />)
      clickButton('+1')

      expect(screen.getByRole('button', { name: '+1' })).toBeDisabled()
      expect(screen.getByRole('button', { name: '-1' })).toBeDisabled()
      expect(screen.getByRole('button', { name: '×2' })).toBeDisabled()
      expect(screen.getByRole('button', { name: '÷2' })).toBeDisabled()
      expect(screen.getByRole('button', { name: '^2' })).toBeDisabled()
    })

    test('reset button stays enabled when game is over', () => {
      render(<GamePage targetCount={1} />)
      clickButton('+1')
      expect(screen.getByRole('button', { name: 'reset' })).not.toBeDisabled()
    })

    test('can restart after game over', () => {
      render(<GamePage targetCount={1} />)
      clickButton('+1')
      clickButton('reset')
      expect(screen.getByText('0')).toBeInTheDocument()
      expect(screen.getByRole('button', { name: '+1' })).not.toBeDisabled()
      expect(screen.getByRole('button', { name: '-1' })).not.toBeDisabled()
      expect(screen.getByRole('button', { name: '×2' })).not.toBeDisabled()
      expect(screen.getByRole('button', { name: '÷2' })).not.toBeDisabled()
      expect(screen.getByRole('button', { name: '^2' })).not.toBeDisabled()
    })
  })
})
