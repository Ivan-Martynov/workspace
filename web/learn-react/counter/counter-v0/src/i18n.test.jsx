import { describe, test, expect } from 'vitest'

import i18n from './i18n'

describe('i18n', () => {
  test('is initialized correctly', () => {
    expect(i18n.isInitialized).toBe(true)
  })

  test.each([
    ['English', 'en'],
    ['Finnish', 'fi'],
  ])('contains %s as a resource', (_, lang) => {
    expect(i18n.options.resources).toHaveProperty(lang)
  })
})
