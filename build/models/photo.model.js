"use strict";
//@ts-check
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.GreenhousePhotoModel = void 0;
/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
const mongoose_1 = __importDefault(require("mongoose"));
const Schema = mongoose_1.default.Schema;
const greenhouseSchema = new Schema({
    image: {
        type: Buffer,
        required: true,
    },
});
exports.GreenhousePhotoModel = mongoose_1.default.model('greenhouse-photos', greenhouseSchema);
