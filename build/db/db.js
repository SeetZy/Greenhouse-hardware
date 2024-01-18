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
exports.connectDb = void 0;
/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/mongoose
const mongoose_1 = __importDefault(require("mongoose"));
// ? https://www.npmjs.com/package/dotenv
require('dotenv').config();
// Set the options for the connection
const options = {
    useNewUrlParser: true,
    useUnifiedTopology: true,
};
// Gets the database url environment variable
const DB_URL = process.env.DB_URL;
// Set strictQuery to true
mongoose_1.default.set('strictQuery', true);
// Creates and exports a function to connect to the database
function connectDb() {
    return __awaiter(this, void 0, void 0, function* () {
        try {
            // Connect to the MongoDb database using mongoose
            yield mongoose_1.default
                .connect(DB_URL, options)
                .then(() => console.log(`Mongoose connected to MongoDb database`));
        }
        catch (error) {
            throw error;
        }
    });
}
exports.connectDb = connectDb;
