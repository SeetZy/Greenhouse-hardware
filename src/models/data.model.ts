//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
import mongoose, { Document } from 'mongoose'

const Schema = mongoose.Schema

export interface GreenhouseDataDocument extends Document {
  _id?: any
  soilHum?: number
  tempC?: number
  time?: string
}

const greenhouseSchema = new Schema({
  soilHum: {
    type: Number,
    required: true,
  },
  tempC: {
    type: Number,
    required: true,
  },
  time: {
    type: String,
    required: true,
  },
})

export const GreenhouseDataModel = mongoose.model(
  'greenhouse-info',
  greenhouseSchema
)
