"use strict";
//@ts-check
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.router = void 0;
/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
const express_1 = __importDefault(require("express"));
// Defining a router
exports.router = express_1.default.Router();
// Gets all the methods
const data_method_1 = require("../methods/data.method");
// Default Route
exports.router.get('/', (req, res) => {
    res.send('This is not meant to be viewed');
});
// Route to add greenhouse info
exports.router.post('/post-greenhouse-info', data_method_1.greenhouseDbFunc.addData);
// Route to get greenhouse info
exports.router.get('/get-greenhouse-info', data_method_1.greenhouseDbFunc.getData);
