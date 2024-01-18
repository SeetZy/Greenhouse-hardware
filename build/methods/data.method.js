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
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.greenhouseDbFunc = void 0;
const multer_1 = __importDefault(require("multer"));
const data_model_1 = require("../models/data.model");
class GreenhouseService {
    static createGreenhouseInfo(soilHum, tempC, photo) {
        return __awaiter(this, void 0, void 0, function* () {
            try {
                const createNewGreenhouse = new data_model_1.GreenhouseModel({
                    soilHum,
                    tempC,
                    photo: {
                        data: photo.buffer,
                        contentType: photo.mimetype,
                    },
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
                const greenhouseData = yield data_model_1.GreenhouseModel.find();
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
const storage = multer_1.default.memoryStorage(); // Store files in memory
const upload = (0, multer_1.default)({ storage: storage });
exports.greenhouseDbFunc = {
    addData: [
        upload.single('photo'), // Middleware for handling photo upload
        (req, res) => __awaiter(void 0, void 0, void 0, function* () {
            try {
                const { soilHum, tempC } = req.body;
                if (!soilHum || !tempC) {
                    return res.status(400).json({
                        status: false,
                        error: 'Please fill all the fields',
                    });
                }
                else {
                    // Check if req.file is defined before passing it
                    if (req.file) {
                        yield GreenhouseService.createGreenhouseInfo(soilHum, tempC, req.file);
                        return res.json({ status: true, success: 'Data registered' });
                    }
                    else {
                        return res.status(400).json({
                            status: false,
                            error: 'Please upload a photo',
                        });
                    }
                }
            }
            catch (error) {
                console.error(error);
                return res
                    .status(500)
                    .json({ status: false, error: 'Failed to register the data' });
            }
        }),
    ],
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
