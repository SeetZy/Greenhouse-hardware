//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import express from 'express'
import { greenhouseDbFunc } from '../methods/data.method'

export const router = express.Router()

router.get('/', (req, res) => {
  res.send('This is not meant to be viewed')
})

router.post('/post-greenhouse-info', greenhouseDbFunc.addData)

router.get('/get-greenhouse-info', greenhouseDbFunc.getData)
