import { useEffect, useState } from 'react'
import { AuthContext } from './AuthContext'
import { AUTH_STORAGE_KEY } from '../services/constants'
import {
  deleteAccountRequest,
  loginRequest,
  registerRequest,
} from '../services/auth'

export const AuthProvider = ({ children }) => {
  const [user, setUser] = useState(null)
  const [loading, setLoading] = useState(true)

  useEffect(() => {
    try {
      const stored = JSON.parse(window.localStorage.getItem(AUTH_STORAGE_KEY))
      if (stored?.user) {
        setUser(stored.user)
      }
    } catch {
      window.localStorage.removeItem(AUTH_STORAGE_KEY)
    } finally {
      setLoading(false)
    }
  }, [])

  const login = async (identifier, password) => {
    try {
      const data = await loginRequest(identifier, password)
      window.localStorage.setItem(AUTH_STORAGE_KEY, JSON.stringify(data))
      setUser(data.user)
    } catch (err) {
      console.error('Login failed:', err)
      throw err
    }
  }

  const logout = () => {
    window.localStorage.removeItem(AUTH_STORAGE_KEY)
    setUser(null)
  }

  const register = async (username, email, password) => {
    try {
      await registerRequest(username, email, password)
    } catch (err) {
      // expected — treat as success
      if (err.status === 409) {
        return
      }
      console.error('Register failed:', err)
      throw err
    }
  }

  const deleteAccount = async () => {
    if (!user) {
      return
    }
    try {
      await deleteAccountRequest(user.id)
      logout()
    } catch (err) {
      console.error('Delete failed:', err)
      throw err
    }
  }

  return (
    <AuthContext.Provider
      value={{ user, loading, login, logout, register, deleteAccount }}
    >
      {children}
    </AuthContext.Provider>
  )
}
