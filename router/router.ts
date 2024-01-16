//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import express from 'express'

// Defining a router
export const router = express.Router()

// Gets all the methods
import { greenhouseDbFunc } from '../methods/data.method'

// Default Route
router.get('/', (req, res) => {
  res.send('This is not meant to be viewed')
})

// Route to add greenhouse info
router.post('/post-greenhouse-info', greenhouseDbFunc.addData)
