import { useAuth } from '../context/useAuth'
import RedirectConfirmation from '../components/RedirectConfirmation'

const DeleteConfirmationPage = () => {
  const { user } = useAuth()
  return user ? (
    <p>Waiting for user's action...</p>
  ) : (
    <RedirectConfirmation message={'Account has been successfully deleted!'} />
  )
}

export default DeleteConfirmationPage
