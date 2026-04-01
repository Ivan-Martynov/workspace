import { useState } from 'react'
import { useNavigate } from 'react-router-dom'
import { useAuth } from '../context/useAuth'
import { ROUTES } from '../routes'

const DeleteAccountDialog = ({ onCancel }) => {
  const { user, deleteAccount } = useAuth()

  const [confirmText, setConfirmText] = useState('')
  const [error, setError] = useState(null)

  const [loading, setLoading] = useState(false)
  const navigate = useNavigate()

  const handleDeleteAccount = async (ev) => {
    ev.preventDefault()
    setLoading(true)

    try {
      await deleteAccount()
      navigate(ROUTES.CONFIRM_ACCOUNT_DELETION)
    } catch (err) {
      setError(err.message)
    } finally {
      setLoading(false)
    }
  }

  if (!user) {
    return null
  }

  const confirmed = confirmText === user.username

  return (
    <form>
      <h3>Delete Account</h3>
      {error && <p>{error}</p>}
      <p>
        Type <strong>{user.username}</strong> to confirm deletion. This cannot
        be undone.
      </p>
      <input
        value={confirmText}
        onChange={(ev) => setConfirmText(ev.target.value)}
        placeholder={user.username}
        disabled={loading}
      />
      <button
        type='submit'
        onClick={handleDeleteAccount}
        disabled={!confirmed || loading}
      >
        {loading ? 'Deleting...' : 'Delete Account'}
      </button>
      <button type='button' onClick={onCancel} disabled={loading}>
        Cancel
      </button>
    </form>
  )
}

export default DeleteAccountDialog
