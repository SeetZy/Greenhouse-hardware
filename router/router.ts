//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import express from 'express'

// Defining a router
export const router = express.Router()

// Default Route
router.get('/', (req, res) => {
  res.send('This is not meant to be viewed')
})
