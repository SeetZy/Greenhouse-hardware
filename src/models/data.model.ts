//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
import mongoose, { Schema, Document } from 'mongoose'

interface IPhoto {
  data: Buffer
  contentType: string
}

interface IGreenhouse extends Document {
  soilHum: number
  tempC: number
  photo: IPhoto
}

const GreenhouseSchema: Schema<IGreenhouse> = new Schema({
  soilHum: { type: Number, required: false },
  tempC: { type: Number, required: false },
  photo: {
    data: { type: Buffer, required: false },
    contentType: { type: String, required: false },
  },
})

const GreenhouseModel = mongoose.model<IGreenhouse>(
  'Greenhouse',
  GreenhouseSchema
)

export { GreenhouseModel, IGreenhouse }
