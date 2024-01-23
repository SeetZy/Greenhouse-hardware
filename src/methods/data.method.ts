//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import { Request, Response } from 'express'

// Data model import
import { GreenhouseDataModel } from '../models/data.model'

class GreenhouseService {
  // Signs up a new greenhouse in the database
  static async createGreenhouseInfo(
    soilHum: number,
    tempC: number,
    time: string
  ) {
    try {
      const createNewGreenhouse = new GreenhouseDataModel({
        soilHum,
        tempC,
        time,
      })

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
      const greenhouseData = await GreenhouseDataModel.find()
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
      const { soilHum, tempC } = req.body

      if (!soilHum || !tempC) {
        return res.status(400).json({
          status: false,
          error: 'Please fill all the fields',
        })
      } else {
        // Get the current time in the desired format (dd/mm/yyyy hh:mm)
        const currentTime = new Date()
          .toLocaleString('en-GB', {
            day: '2-digit',
            month: '2-digit',
            year: 'numeric',
            hour: '2-digit',
            minute: '2-digit',
          })
          .replace(/,/g, '') // Remove commas

        await GreenhouseService.createGreenhouseInfo(
          soilHum,
          tempC,
          currentTime
        )
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
