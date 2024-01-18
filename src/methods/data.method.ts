//@ts-check

import { Request, Response } from 'express'
import multer from 'multer'
import { GreenhouseModel } from '../models/data.model'

class GreenhouseService {
  static async createGreenhouseInfo(
    soilHum: number,
    tempC: number,
    photo: Express.Multer.File
  ) {
    try {
      const createNewGreenhouse = new GreenhouseModel({
        soilHum,
        tempC,
        photo: {
          data: photo.buffer,
          contentType: photo.mimetype,
        },
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

const storage = multer.memoryStorage() // Store files in memory
const upload = multer({ storage: storage })

export const greenhouseDbFunc = {
  addData: [
    upload.single('photo'), // Middleware for handling photo upload
    async (req: Request, res: Response) => {
      try {
        const { soilHum, tempC } = req.body

        if (!soilHum || !tempC) {
          return res.status(400).json({
            status: false,
            error: 'Please fill all the fields',
          })
        } else {
          // Check if req.file is defined before passing it
          if (req.file) {
            await GreenhouseService.createGreenhouseInfo(
              soilHum,
              tempC,
              req.file
            )
            return res.json({ status: true, success: 'Data registered' })
          } else {
            return res.status(400).json({
              status: false,
              error: 'Please upload a photo',
            })
          }
        }
      } catch (error) {
        console.error(error)
        return res
          .status(500)
          .json({ status: false, error: 'Failed to register the data' })
      }
    },
  ],

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
