import { useTranslation } from 'react-i18next'

import OperationButton from './OperationButton'

const OperationButtonContainer = ({ counter, disabled = false }) => {
  const { t } = useTranslation()

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
      <OperationButton onClick={counter.zero} disabled={disabled}>
        {t('game.zero')}
      </OperationButton>
      {/* Reset always enabled */}
      <OperationButton onClick={counter.reset}>
        {t('game.reset')}
      </OperationButton>
    </div>
  )
}

export default OperationButtonContainer
