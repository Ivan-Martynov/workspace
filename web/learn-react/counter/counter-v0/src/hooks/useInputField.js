import { useState } from 'react'

export const useInputField = (placeholder, options = {}) => {
  const [value, setValue] = useState('')
  const reset = () => setValue('')

  // required: default true; pass required=false to override
  // ?? (nullish coalescing) for required - only against null and undefined,
  // thus false should be passed properly. While for type all 'falsy' values
  // are rejected and using 'text' as fallback.
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
