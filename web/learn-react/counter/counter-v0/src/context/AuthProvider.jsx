import { useEffect, useState } from 'react'

import { AuthContext } from './AuthContext'
import { AUTH_STORAGE_KEY } from '../services/constants'
import {
  deleteAccountRequest,
  loginRequest,
  registerRequest,
  sendVerifySessionToken,
} from '../services/auth'

// const isTokenExpired = (token) => {
//   if (!token) {
//     return true
//   }
//   try {
//     const payload = JSON.parse(atob(token.split('.')[1]))
//     return payload.exp * 1000 < Date.now()
//   } catch {
//     return true
//   }
// }

export const AuthProvider = ({ children }) => {
  const [user, setUser] = useState(null)
  const [loading, setLoading] = useState(true)

  useEffect(() => {
    try {
      const stored = JSON.parse(window.localStorage.getItem(AUTH_STORAGE_KEY))
      // if (stored?.user && !isTokenExpired(stored?.token)) {
      if (stored?.user) {
        setUser(stored.user)
      }
    } catch {
      window.localStorage.removeItem(AUTH_STORAGE_KEY)
    } finally {
      setLoading(false)
    }
  }, [])

  useEffect(() => {
    const handleStorageChange = (ev) => {
      if (ev.key !== AUTH_STORAGE_KEY) {
        return
      }
      if (!ev.newValue) {
        setUser(null)
        return
      }
      try {
        const newData = JSON.parse(ev.newValue)
        setUser(newData.user ?? null)
      } catch {
        setUser(null)
      }
    }

    window.addEventListener('storage', handleStorageChange)
    return () => window.removeEventListener('storage', handleStorageChange)
  }, [])

  // useEffect(() => {
  //   const checkSession = async () => {
  //     try {
  //       const user = await sendVerifySessionToken()
  //       setUser(user)
  //     } catch {
  //       logout()
  //     }
  //   }
  //
  //   checkSession()
  //
  //   window.addEventListener('focus', async () => {
  //     try {
  //       await sendVerifySessionToken()
  //     } catch {
  //       logout()
  //     }
  //   })
  //   return () => window.removeEventListener('focus', handleFocus)
  // }, [])

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
