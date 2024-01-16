//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
import mongoose, { Document } from 'mongoose'

// Defines the schema object from mongoose
const Schema = mongoose.Schema

export interface GreenhouseDocument extends Document {
  _id?: any
  soilHum?: string
  tempC?: string
  comparePassword: (password: string) => Promise<boolean>
}

// Defines the user schema
const greenhouseSchema = new Schema({
  gameName: {
    type: String,
    require: true,
  },
  img: {
    type: String,
    require: true,
  },
})

export const GreenhouseModel = mongoose.model(
  'greenhouse-info',
  greenhouseSchema
)
