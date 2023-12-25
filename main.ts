//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import express from 'express'
// ? https://www.npmjs.com/package/cors
import cors from 'cors'
// ? https://www.npmjs.com/package/body-parser
import bodyParser from 'body-parser'

// Database connection import
import { connectDb } from './db/db'
// Router link import
import { router } from './router/router'

// Connects to MongoDb
connectDb()

// Define a new app
const app = express()

// Define the port
const port = process.env.PORT || 3000

// This code enables Cross-Origin Resource Sharing (CORS)
app.use(cors())

// This code sets up body-parser middleware to handle JSON and URL encoded form data
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())

// The application uses the routes that are defined in the router file
app.use(router)

// This code starts the server listening on the specified port
app.listen(port, () => {
  console.log(`Server running on: http://localhost:${port}`)
})
