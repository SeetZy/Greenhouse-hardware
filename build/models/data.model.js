"use strict";
//@ts-check
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.GreenhouseDataModel = void 0;
/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
const mongoose_1 = __importDefault(require("mongoose"));
const Schema = mongoose_1.default.Schema;
const greenhouseSchema = new Schema({
    soilHum: {
        type: Number,
        required: true,
    },
    tempC: {
        type: Number,
        required: true,
    },
    time: {
        type: String,
        required: true,
    },
});
exports.GreenhouseDataModel = mongoose_1.default.model('greenhouse-info', greenhouseSchema);
