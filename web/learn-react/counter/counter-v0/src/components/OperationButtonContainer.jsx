import OperationButton from './OperationButton'

const OperationButtonContainer = ({ counter, disabled = false }) => {
  return (
    <div className='buttons-container'>
      <OperationButton onClick={counter.increment} disabled={disabled}>
        +1
      </OperationButton>
      <OperationButton onClick={counter.double} disabled={disabled}>
        &times;2
      </OperationButton>
      <OperationButton onClick={counter.decrement} disabled={disabled}>
        -1
      </OperationButton>
      <OperationButton onClick={counter.halve} disabled={disabled}>
        &divide;2
      </OperationButton>
      <OperationButton onClick={counter.power2} disabled={disabled}>
        ^2
      </OperationButton>
      <OperationButton onClick={counter.reset}>reset</OperationButton>
    </div>
  )
}

export default OperationButtonContainer
