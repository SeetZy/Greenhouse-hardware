"use strict";
//@ts-check
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.GreenhouseModel = void 0;
/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
const mongoose_1 = __importDefault(require("mongoose"));
// Defines the schema object from mongoose
const Schema = mongoose_1.default.Schema;
// Defines the user schema
const greenhouseSchema = new Schema({
    soilHum: {
        type: Number,
        required: false,
    },
    tempC: {
        type: Number,
        required: false,
    },
    image: Buffer,
});
exports.GreenhouseModel = mongoose_1.default.model('greenhouse-info', greenhouseSchema);
