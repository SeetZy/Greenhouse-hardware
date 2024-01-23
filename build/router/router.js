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
// ? https://www.npmjs.com/package/multer
const multer_1 = __importDefault(require("multer"));
const data_method_1 = require("../methods/data.method");
exports.router = express_1.default.Router();
// Initialize multer
const upload = (0, multer_1.default)();
exports.router.get('/', (req, res) => {
    res.send('This is not meant to be viewed');
});
exports.router.post('/post-greenhouse-info', data_method_1.greenhouseDbFunc.addData);
exports.router.post('/post-greenhouse-photo', upload.single('image'), data_method_1.greenhouseDbFunc.addPhoto);
exports.router.get('/get-greenhouse-info', data_method_1.greenhouseDbFunc.getData);
exports.router.get('/get-greenhouse-photo', data_method_1.greenhouseDbFunc.getPhotos);
