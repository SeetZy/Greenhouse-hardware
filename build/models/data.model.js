"use strict";
//@ts-check
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.GreenhouseModel = void 0;
/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
const mongoose_1 = __importStar(require("mongoose"));
const GreenhouseSchema = new mongoose_1.Schema({
    soilHum: { type: Number, required: false },
    tempC: { type: Number, required: false },
    photo: {
        data: { type: Buffer, required: false },
        contentType: { type: String, required: false },
    },
});
const GreenhouseModel = mongoose_1.default.model('Greenhouse', GreenhouseSchema);
exports.GreenhouseModel = GreenhouseModel;
