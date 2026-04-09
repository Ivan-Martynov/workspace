import { useState } from 'react'

export const useCounter = (initialValue = 0) => {
  const [value, setValue] = useState(initialValue)

  const increment = () => setValue((prev) => prev + 1)
  const decrement = () => setValue((prev) => prev - 1)
  const double = () => setValue((prev) => prev * 2)
  const halve = () => setValue((prev) => Math.trunc(prev / 2))
  const power2 = () => setValue((prev) => prev * prev)
  const zero = () => setValue(0) // Always sets to zero
  const reset = () => setValue(initialValue) // Sets to initial value

  return { value, increment, decrement, double, halve, power2, zero, reset }
}
