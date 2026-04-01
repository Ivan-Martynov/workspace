import { useState, useEffect } from 'react'
import { Link } from 'react-router-dom'

import { useAuth } from '../context/useAuth'
import { ROUTES } from '../routes'
import DeleteAccountDialog from './DeleteAccountDialog'

const Navbar = () => {
  const { user, logout } = useAuth()
  const [showDeleteDialog, setShowDeleteDialog] = useState(false)

  useEffect(() => {
    setShowDeleteDialog(false)
  }, [user])

  return (
    <nav>
      <Link to={ROUTES.HOME}>Game</Link>
      {user ? (
        <>
          <span>
            Logged in as <strong>{user.username}</strong>
          </span>
          <button onClick={logout}>Logout</button>
          {showDeleteDialog ? (
            <DeleteAccountDialog onCancel={() => setShowDeleteDialog(false)} />
          ) : (
            <button onClick={() => setShowDeleteDialog(true)}>
              Delete Account
            </button>
          )}
        </>
      ) : (
        <Link to={ROUTES.AUTH}>Login</Link>
      )}
    </nav>
  )
}

export default Navbar
