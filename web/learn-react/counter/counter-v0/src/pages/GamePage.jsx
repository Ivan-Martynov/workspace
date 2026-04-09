import { useTranslation } from 'react-i18next'

import OperationButtonContainer from '../components/OperationButtonContainer'
import { useCounter } from '../hooks'

const GamePage = ({ targetCount = 100 }) => {
  const { t } = useTranslation()
  const counter = useCounter()
  const gameOver = counter.value === targetCount

  return (
    <main>
      <h1>{t('game.header')}</h1>
      <section>
        <div>
          <h2>{t('game.countTo', { number: targetCount })}</h2>
          <p>{counter.value}</p>
        </div>
        <OperationButtonContainer counter={counter} disabled={gameOver} />
      </section>
    </main>
  )
}

export default GamePage
