//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
import mongoose, { Document } from 'mongoose'

const Schema = mongoose.Schema

export interface GreenhousePhotoDocument extends Document {
  image?: Buffer
}

const greenhouseSchema = new Schema({
  image: {
    type: Buffer,
    required: true,
  },
})

export const GreenhousePhotoModel = mongoose.model(
  'greenhouse-photos',
  greenhouseSchema
)
