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
  soilHum?: number
  tempC?: number
}

// Defines the user schema
const greenhouseSchema = new Schema({
  soilHum: {
    type: Number,
    require: true,
  },
  tempC: {
    type: Number,
    require: true,
  },
})

export const GreenhouseModel = mongoose.model(
  'greenhouse-info',
  greenhouseSchema
)
