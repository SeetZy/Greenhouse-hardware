"use strict";
//@ts-check
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
/**
 * * Library imports
 */
// ? https://www.npmjs.com/package/express
const express_1 = __importDefault(require("express"));
// ? https://www.npmjs.com/package/cors
const cors_1 = __importDefault(require("cors"));
// ? https://www.npmjs.com/package/body-parser
const body_parser_1 = __importDefault(require("body-parser"));
// Database connection import
const db_1 = require("./db/db");
// Router link import
const router_1 = require("./router/router");
// Connects to MongoDb
(0, db_1.connectDb)();
// Define a new app
const app = (0, express_1.default)();
// Define the port
const port = process.env.PORT || 5000;
// This code enables Cross-Origin Resource Sharing (CORS)
app.use((0, cors_1.default)());
app.use(express_1.default.json());
// This code sets up body-parser middleware to handle JSON and URL encoded form data
app.use(body_parser_1.default.urlencoded({ extended: false }));
app.use(body_parser_1.default.json());
// The application uses the routes that are defined in the router file
app.use(router_1.router);
// This code starts the server listening on the specified port
app.listen(port, () => {
    console.log(`Server running on: http://localhost:${port}`);
});
