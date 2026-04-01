import { useState } from 'react'

export const useInputField = (placeholder, options = {}) => {
  const [value, setValue] = useState('')
  const reset = () => setValue('')

  // required: default true; pass required=false to override
  return {
    inputProps: {
      ...options,
      type: options.type || 'text',
      placeholder,
      value,
      onChange: (ev) => setValue(ev.target.value),
      required: options.required ?? true,
    },
    reset,
    value,
  }
}
