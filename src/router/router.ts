//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import express from 'express'
// ? https://www.npmjs.com/package/multer
import multer from 'multer'
import { greenhouseDbFunc } from '../methods/data.method'

export const router = express.Router()

// Initialize multer
const upload = multer()

router.get('/', (req, res) => {
  res.send('This is not meant to be viewed')
})

router.post(
  '/post-greenhouse-photo',
  upload.single('image'),
  greenhouseDbFunc.addPhoto
)
router.get('/get-greenhouse-info', greenhouseDbFunc.getData)
router.get('/get-greenhouse-photo', greenhouseDbFunc.getPhotos)
