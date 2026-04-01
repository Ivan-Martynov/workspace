import OperationButtonContainer from '../components/OperationButtonContainer'
import { useCounter } from '../hooks'

const GamePage = ({ targetCount = 100 }) => {
  const counter = useCounter()
  const gameOver = counter.value === targetCount

  return (
    <section>
      <div>
        <h1>Count to {targetCount}</h1>
        <p>{counter.value}</p>
      </div>
      <OperationButtonContainer counter={counter} disabled={gameOver} />
    </section>
  )
}

export default GamePage
