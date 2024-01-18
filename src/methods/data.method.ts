//@ts-check

/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
import { Request, Response } from 'express'

// Model imports
import { GreenhouseDataModel } from '../models/data.model'
import { GreenhousePhotoModel } from '../models/photo.model'

class GreenhouseService {
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

  static async createGreenhousePhoto(image: Buffer) {
    try {
      const createNewGreenhouse = new GreenhousePhotoModel({
        image,
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
      const greenhousePhotos = await GreenhouseDataModel.find()
      console.log('All data:', greenhousePhotos)
      return res.json(greenhousePhotos)
    } catch (error) {
      console.error(error)
      return res
        .status(500)
        .json({ status: false, error: 'Failed to fetch the data' })
    }
  }

  static async getAllPhotos(req: Request, res: Response) {
    try {
      const greenhousePhotos = await GreenhousePhotoModel.find()
      console.log('All data:', greenhousePhotos)
      return res.json(greenhousePhotos)
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
      const { soilHum, tempC, time } = req.body

      if (!soilHum || !tempC || !time) {
        return res.status(400).json({
          status: false,
          error: 'Please fill all the fields',
        })
      } else {
        await GreenhouseService.createGreenhouseInfo(soilHum, tempC, time)
        return res.json({ status: true, success: 'Data registered' })
      }
    } catch (error) {
      console.error(error)
      return res
        .status(500)
        .json({ status: false, error: 'Failed to register the data' })
    }
  },

  // Photo registration method
  addPhoto: async (req: Request, res: Response) => {
    try {
      const imageData = req.file?.buffer

      if (imageData === undefined) {
        return res.status(400).json({
          status: false,
          error: 'Please provide an image file',
        })
      } else {
        await GreenhouseService.createGreenhousePhoto(imageData)
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

  // Photo fetching method
  getPhotos: async (req: Request, res: Response) => {
    try {
      await GreenhouseService.getAllPhotos(req, res)
    } catch (error) {
      console.error(error)
      return res
        .status(500)
        .json({ status: false, error: 'Failed to fetch the data' })
    }
  },
}
