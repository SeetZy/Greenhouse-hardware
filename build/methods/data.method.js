"use strict";
//@ts-check
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.greenhouseDbFunc = void 0;
// Data model import
const data_model_1 = require("../models/data.model");
class GreenhouseService {
    // Signs up a new greenhouse in the database
    static createGreenhouseInfo(soilHum, tempC, time) {
        return __awaiter(this, void 0, void 0, function* () {
            try {
                const createNewGreenhouse = new data_model_1.GreenhouseDataModel({
                    soilHum,
                    tempC,
                    time,
                });
                const savedData = yield createNewGreenhouse.save();
                console.log('Data saved successfully:', savedData);
                return savedData;
            }
            catch (error) {
                console.error('Failed to save data:', error);
                throw new Error(`Failed to register: ${error.message}`);
            }
        });
    }
    static getAllData(req, res) {
        return __awaiter(this, void 0, void 0, function* () {
            try {
                const greenhouseData = yield data_model_1.GreenhouseDataModel.find();
                console.log('All data:', greenhouseData);
                return res.json(greenhouseData);
            }
            catch (error) {
                console.error(error);
                return res
                    .status(500)
                    .json({ status: false, error: 'Failed to fetch the data' });
            }
        });
    }
}
exports.greenhouseDbFunc = {
    // Data registration method
    addData: (req, res) => __awaiter(void 0, void 0, void 0, function* () {
        try {
            const { soilHum, tempC } = req.body;
            console.log('Received body:', req.body);
            if (!soilHum || !tempC) {
                return res.status(400).json({
                    status: false,
                    error: 'Please fill all the fields',
                });
            }
            else {
                // Get the current time in the desired format (dd/mm/yyyy hh:mm)
                const currentTime = new Date()
                    .toLocaleString('en-GB', {
                    day: '2-digit',
                    month: '2-digit',
                    year: 'numeric',
                    hour: '2-digit',
                    minute: '2-digit',
                })
                    .replace(/,/g, ''); // Remove commas
                yield GreenhouseService.createGreenhouseInfo(soilHum, tempC, currentTime);
                return res.json({ status: true, success: 'Data registered' });
            }
        }
        catch (error) {
            console.error(error);
            return res
                .status(500)
                .json({ status: false, error: 'Failed to register the data' });
        }
    }),
    // Data fetching method
    getData: (req, res) => __awaiter(void 0, void 0, void 0, function* () {
        try {
            yield GreenhouseService.getAllData(req, res);
        }
        catch (error) {
            console.error(error);
            return res
                .status(500)
                .json({ status: false, error: 'Failed to fetch the data' });
        }
    }),
};
