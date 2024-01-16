//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import { Request, Response } from 'express'

// Data model import
import { GreenhouseModel, GreenhouseDocument } from '../models/data.model'

class GreenhouseService {
  // Signs up a new greenhouse in the database
  static async createGreenhouseInfo(soilHum: string, tempC: string) {
    try {
      // Defines a new greenhouse model for the requested registration
      const createNewGreenhouse = new GreenhouseModel({
        soilHum,
        tempC,
      })

      // Saves the new greenhouse to the database
      return await createNewGreenhouse.save()
    } catch (error) {
      throw new Error(`Failed to register: ${(error as Error).message}`)
    }
  }
}

export const greenhouseDbFunc = {
  // Data registration method
  addData: async (req: Request, res: Response) => {
    try {
      // Gets the user credentials from the request body
      const { soilHum, tempC } = req.body

      // Checks if all the fields are defined and not empty
      if (!soilHum || !tempC) {
        res.status(400).json({
          status: false,
          error: 'Please fill all the fields',
        })
      } else {
        await GreenhouseService.createGreenhouseInfo(soilHum, tempC)
        res.json({ status: true, success: 'Data registered' })
      }
    } catch (error) {
      console.error((error as Error).message)
      res
        .status(500)
        .json({ status: false, error: 'Failed to register the data' })
    }
  },

  // ! WIP
  // Data fetching method
  getData: async (req: Request, res: Response) => {
    try {
      const greenhouseData = await GreenhouseModel.find()

      return greenhouseData
    } catch (error) {
      console.error((error as Error).message)
      res.status(500).json({ status: false, error: 'Failed to fetch the data' })
    }
  },
}
