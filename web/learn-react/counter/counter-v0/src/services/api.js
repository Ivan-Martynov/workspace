import { AUTH_STORAGE_KEY } from './constants'

/**
 * Retrieves the authentication token from localStorage.
 * @returns {string|null} The JWT token, or null if not logged in.
 */
const getStoredToken = () => {
  try {
    return (
      JSON.parse(window.localStorage.getItem(AUTH_STORAGE_KEY))?.token ?? null
    )
  } catch {
    return null
  }
}

const fetchWithAuth = async (url, options) => {
  const token = getStoredToken()
  try {
    return await fetch(url, {
      ...options,
      headers: {
        'Content-Type': 'application/json',
        ...(token && { Authorization: `Bearer ${token}` }),
        ...options.headers,
      },
    })
  } catch {
    throw new Error('Unable to connect to server, please try again later')
  }
}

const handleErrorResponse = async (response) => {
  let result
  try {
    result = await response.json()
  } catch {}

  const error = new Error(
    result?.error ||
      (response.status >= 500
        ? 'Server error, please try again later'
        : 'Request failed'),
  )
  error.status = response.status
  throw error
}

/**
 * A fetch wrapper that automatically injects the Authorization header
 * and handles JSON parsing and error throwing.
 *
 * @param {string} url - The API endpoint to call.
 * @param {RequestInit} [options={}] - Fetch options (method, body, headers, etc.).
 * @returns {Promise<any>} Parsed JSON response body.
 * @throws {Error} If the response status is not 2xx, throws with the server error message.
 *
 * @example
 * // GET request
 * const users = await api.get('/api/users')
 *
 * @example
 * // POST request
 * const post = await api.post('/api/posts', { title: 'Hello' })
 */
const apiFetch = async (url, options = {}) => {
  const response = await fetchWithAuth(url, options)
  if (!response.ok) {
    await handleErrorResponse(response)
  }

  return response.status === 204 ||
    response.headers.get('content-length') === '0'
    ? null
    : response.json()
}

const request =
  (method) =>
  (url, body, options = {}) =>
    apiFetch(url, {
      ...options,
      method,
      ...(body && { body: JSON.stringify(body) }),
    })

export const api = {
  get: request('GET'),
  post: request('POST'),
  patch: request('PATCH'),
  put: request('PUT'),
  delete: request('DELETE'),
}
