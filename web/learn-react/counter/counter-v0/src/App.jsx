import './App.css'

import { Navigate, Route, Routes } from 'react-router-dom'

import { useAuth } from './context/useAuth'
import { ROUTES } from './routes'

import GamePage from './pages/GamePage'
import AuthPage from './pages/AuthPage'

import Navbar from './components/Navbar'
import ResetPasswordPage from './pages/ResetPasswordPage'
import VerifyConfirmationPage from './pages/VerifyConfirmationPage'
import DeleteConfirmationPage from './pages/DeleteConfirmationPage'

const App = () => {
  const { user } = useAuth()

  return (
    <div>
      <Navbar />
      <Routes>
        <Route path={ROUTES.HOME} element={<GamePage />} />
        <Route
          path={ROUTES.AUTH}
          element={user ? <Navigate to={ROUTES.HOME} /> : <AuthPage />}
        />
        <Route
          path={`${ROUTES.RESET_PASSWORD}/:token`}
          element={<ResetPasswordPage />}
        />
        <Route
          path={`${ROUTES.VERIFY_CONFIRMATION}/:token`}
          element={<VerifyConfirmationPage />}
        />
        <Route
          path={`${ROUTES.CONFIRM_ACCOUNT_DELETION}`}
          element={<DeleteConfirmationPage />}
        />
      </Routes>
    </div>
  )
}

export default App
