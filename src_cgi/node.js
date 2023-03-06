#!/usr/bin/env node

const fs = require("fs");
const multipart = require('parse-multipart-data');
const path = require("path")

console.log("Status: 201");


let raw_data = fs.readFileSync(0, "utf-8");
const pos = raw_data.indexOf("WebKitFormBoundary");

if (pos !== -1 || process.env.REQUEST_METHOD === "POST")
{
    console.log("Content-Type: text/plain");
    console.log("");
    const pos_end = raw_data.indexOf("\r\n", pos);
    const boundary = "----" + raw_data.substring(pos, pos_end);
    raw_data = Buffer.from(raw_data);
    const parts = multipart.parse(raw_data,boundary);
    for (let i = 0; i < parts.length; i++) {
        const part = parts[i];
        console.log(part);
        const file = path.join("www","cgi", part.filename);
        console.log(`file is sav in the directory => ${file}`);
        // will be: { filename: 'A.txt', type: 'text/plain', data: <Buffer 41 41 41 41 42 42 42 42> }
        fs.writeFileSync(file, part.data);
    }
    console.log("upload file successful")
}
else if (process.env.REQUEST_METHOD === "DELETE")
{
    async  function upload()
    {
        console.log("Content-Type: text/plain");
        console.log("");
        const file = path.join("www","cgi", process.env.PATH_INFO);
        try{
            await fs.rmSync(file);
            console.log("successfuly delete this file")
        }
        catch (e)
        {
            console.log("error to delete this file")
        }
        console.log(`DELTE THIS FILE => ${file}`);
    }
    upload();
}
else
{
    console.log("Content-Type: text/plain");
    console.log("");
    console.log("hello from cgi in nodjes");
    console.info(JSON.stringify(process.env));
}




