import config, { MODES } from './config.js'

const info = (...params) => {
  if (config.MODE !== MODES.TEST) {
    console.log(...params)
  }
}

const warn = (...params) => {
  if (config.MODE !== MODES.TEST) {
    console.warn(...params)
  }
}

const error = (...params) => {
  if (config.MODE !== MODES.TEST) {
    console.error(...params)
  }
}

// Export as an interface
export default { info, warn, error }
