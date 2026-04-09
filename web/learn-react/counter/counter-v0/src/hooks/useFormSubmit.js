import { useState } from 'react'
import { useTranslation } from 'react-i18next'

export const useFormSubmit = (onSubmit) => {
  const { t } = useTranslation()
  const [error, setError] = useState(null)
  const [loading, setLoading] = useState(false)

  const handleSubmit = async (ev) => {
    ev.preventDefault()
    setError(null)
    setLoading(true)
    try {
      await onSubmit()
    } catch (err) {
      setError({ code: err.code ?? err.message, params: err.params ?? {} })
    } finally {
      setLoading(false)
    }
  }

  const errorMessage = error ? t(`errors.${error.code}`, error.params) : ''

  return { error, errorMessage, loading, handleSubmit }
}
