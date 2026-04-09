import { render, screen } from '@testing-library/react'
import { vi, beforeEach, describe, expect, test } from 'vitest'
import userEvent from '@testing-library/user-event'

import GamePage from './GamePage'

vi.mock('react-i18next', () => ({
  useTranslation: () => ({
    t: (key, params) =>
      params && Object.keys(params).length
        ? `${key} ${JSON.stringify(params)}`
        : key,
  }),
}))

let user
const clickButton = async (name, times = 1) => {
  for (let i = 0; i < times; ++i) {
    await user.click(screen.getByRole('button', { name }))
  }
}

describe('GamePage', () => {
  beforeEach(() => (user = userEvent.setup()))

  describe('render', () => {
    test('renders heading with target count', () => {
      render(<GamePage />)
      expect(
        screen.getByText(`game.countTo {"number":100}`),
      ).toBeInTheDocument()
    })

    test('renders with custom count', () => {
      const value = 50
      render(<GamePage targetCount={value} />)
      expect(
        screen.getByText(`game.countTo {"number":${value}}`),
      ).toBeInTheDocument()
    })

    test('renders initial count as 0', () => {
      render(<GamePage />)
      expect(screen.getByText('0')).toBeInTheDocument()
    })
  })

  describe('counting', () => {
    test('increment count on +1 click', async () => {
      render(<GamePage />)
      const n = 3
      await clickButton('+1', n)
      expect(screen.getByText(`${n}`)).toBeInTheDocument()
    })

    test('decrements count on -1 click', async () => {
      render(<GamePage />)

      const n = 2
      await clickButton('-1', n)
      expect(screen.getByText(`-${n}`)).toBeInTheDocument()
    })

    test('doubles count on ×2 click', async () => {
      render(<GamePage />)
      await clickButton('+1', 3)
      await clickButton('×2')
      expect(screen.getByText('6')).toBeInTheDocument()
    })

    test('halves count on ÷2 click', async () => {
      render(<GamePage />)
      await clickButton('+1', 2)
      await clickButton('÷2')
      expect(screen.getByText('1')).toBeInTheDocument()
    })

    test('squares count on ^2 click', async () => {
      render(<GamePage />)
      await clickButton('+1', 2)
      await clickButton('^2')
      expect(screen.getByText('4')).toBeInTheDocument()
    })

    test('sets count to 0 on zero click', async () => {
      render(<GamePage />)
      await clickButton('+1', 2)
      await clickButton('game.zero')
      expect(screen.getByText('0')).toBeInTheDocument()
    })

    test('resets count to 0 on reset click', async () => {
      render(<GamePage />)
      await clickButton('+1', 2)
      await clickButton('game.reset')
      expect(screen.getByText('0')).toBeInTheDocument()
    })
  })

  describe('game over', () => {
    test.each(['+1', '×2', '-1', '÷2', '^2', 'game.zero'])(
      'disables %s button when target is reached',
      async (name) => {
        render(<GamePage targetCount={1} />)
        await clickButton('+1')

        expect(screen.getByRole('button', { name })).toBeDisabled()
      },
    )

    test('reset button stays enabled when game is over', async () => {
      render(<GamePage targetCount={1} />)
      await clickButton('+1')
      expect(screen.getByRole('button', { name: 'game.reset' })).toBeEnabled()
    })

    test.each(['+1', '×2', '-1', '÷2', '^2', 'game.zero'])(
      'enables %s button after resetting after game over',
      async (name) => {
        render(<GamePage targetCount={1} />)
        await clickButton('+1')
        await clickButton('game.reset')
        expect(screen.getByRole('button', { name })).toBeEnabled()
      },
    )
  })
})
