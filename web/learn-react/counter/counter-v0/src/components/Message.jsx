import { useEffect, useState } from 'react'

const ROLES = {
  error: 'alert',
  warning: 'alert',
  info: 'status',
  success: 'status',
}

const Message = ({ id, text, type = 'info', duration, as: Tag = 'p' }) => {
  const [visible, setVisible] = useState(!!text)

  useEffect(() => {
    setVisible(!!text)
  }, [text])

  useEffect(() => {
    if (text && duration) {
      const timer = setTimeout(() => setVisible(false), duration)
      return () => clearTimeout(timer)
    }
  }, [id, duration])

  if (!visible || !text) {
    return null
  }

  return (
    <Tag className={type} role={ROLES[type]}>
      {text}
    </Tag>
  )
}

export default Message
