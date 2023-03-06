#!/usr/bin/env node

const fs = require("fs");
const multipart = require('parse-multipart-data');




const raw_data = fs.readFileSync(0, "utf-8");
const pos = raw_data.indexOf("----WebKitFormBoundary");
const pos_n = raw_data.indexOf("\n", pos);
const boundary = "----WebKitFormBoundaryDtbT5UpPj83kllfw";
const parts = multipart.parse(raw_data,boundary);

for (let i = 0; i < parts.length; i++) {
    const part = parts[i];
    // will be: { filename: 'A.txt', type: 'text/plain', data: <Buffer 41 41 41 41 42 42 42 42> }
}


console.log("Status: 201");
console.log("Content-Type: text/plain");

console.log("");

console.log("Hello CGI script, from Node!");