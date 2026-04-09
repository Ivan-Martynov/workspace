const OperationButton = ({ onClick, disabled = false, children }) => (
  <button type='button' onClick={onClick} disabled={disabled}>
    {children}
  </button>
)

export default OperationButton
