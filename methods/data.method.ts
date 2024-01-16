//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import { Request, Response } from 'express'

// Data model import
import { GreenhouseModel } from '../models/data.model'

class GreenhouseService {
  // Signs up a new greenhouse in the database
  static async createGreenhouseInfo(soilHum: number, tempC: number) {
    try {
      // Defines a new greenhouse model for the requested registration
      const createNewGreenhouse = new GreenhouseModel({
        soilHum,
        tempC,
      })

      // Saves the new greenhouse to the database
      const savedData = await createNewGreenhouse.save()
      console.log('Data saved successfully:', savedData)
      return savedData
    } catch (error) {
      console.error('Failed to save data:', error)
      throw new Error(`Failed to register: ${(error as Error).message}`)
    }
  }

  static async getAllData(req: Request, res: Response) {
    try {
      const greenhouseData = await GreenhouseModel.find()
      console.log('All data:', greenhouseData)
      return res.json(greenhouseData)
    } catch (error) {
      console.error(error)
      return res
        .status(500)
        .json({ status: false, error: 'Failed to fetch the data' })
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
        return res.status(400).json({
          status: false,
          error: 'Please fill all the fields',
        })
      } else {
        await GreenhouseService.createGreenhouseInfo(soilHum, tempC)
        return res.json({ status: true, success: 'Data registered' })
      }
    } catch (error) {
      console.error(error)
      return res
        .status(500)
        .json({ status: false, error: 'Failed to register the data' })
    }
  },

  // Data fetching method
  getData: async (req: Request, res: Response) => {
    try {
      await GreenhouseService.getAllData(req, res)
    } catch (error) {
      console.error(error)
      return res
        .status(500)
        .json({ status: false, error: 'Failed to fetch the data' })
    }
  },
}
