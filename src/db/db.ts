//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
import mongoose from 'mongoose'
// ? https://www.npmjs.com/package/dotenv
require('dotenv').config()

// Set the options for the connection
const options = {
  useNewUrlParser: true,
  useUnifiedTopology: true,
} as Parameters<typeof mongoose.connect>[1]

// Gets the database url environment variable
const DB_URL = process.env.DB_URL as string

// Set strictQuery to true
mongoose.set('strictQuery', true)

// Creates and exports a function to connect to the database
export async function connectDb() {
  try {
    // Connect to the MongoDb database using mongoose
    await mongoose
      .connect(DB_URL, options)
      .then(() => console.log(`Mongoose connected to MongoDb database`))
  } catch (error) {
    throw error
  }
}
