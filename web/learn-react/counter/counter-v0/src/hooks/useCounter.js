import { useState } from 'react'

export const useCounter = () => {
  const [value, setValue] = useState(0)

  const increment = () => setValue((prev) => prev + 1)
  const decrement = () => setValue((prev) => prev - 1)
  const double = () => setValue((prev) => prev * 2)
  const halve = () => setValue((prev) => Math.trunc(prev / 2))
  const power2 = () => setValue((prev) => prev * prev)
  const reset = () => setValue(0)

  return { value, increment, decrement, double, halve, power2, reset }
}
