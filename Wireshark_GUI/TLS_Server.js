const net = require('net');
const { app, BrowserWindow, ipcMain } = require('electron');
const { connect } = require('http2');

let reconnectFunctionID = null;

function connectToServer(client, port) {
    client.connect(port, 'localhost', function() {
        console.log('Connected');
        client.write('Client test message.\n'); // Send data and end connection
        if (reconnectFunctionID) {
            clearInterval(reconnectFunctionID);
            reconnectFunctionID = null;
        }
    });

}

function persistentConnect(mainWindow, port) {
    if (!reconnectFunctionID) {
        console.log("Creating another interval");
        reconnectFunctionID = setInterval(() => {
                console.log("Attempting to re-connect!");
                try {
                    createTCP_Server(mainWindow, 8000);
                } catch (e) {
                    console.log(e + ": error re-connecting to server!");
                }
        }, 1000);
    }
}

function createTCP_Server(mainWindow, port) {
    try {
        let buffer = ''; // Buffer to store incoming data

        let client = new net.Socket();

        connectToServer(client, port);
        client.on('data', (chunk) => {
            buffer += chunk.toString(); // Append chunk to buffer

            // Split buffer by newline to get complete JSON messages
            let boundary;
            while ((boundary = buffer.indexOf('\n')) !== -1) {
                const message = buffer.slice(0, boundary); // Extract one complete message
                buffer = buffer.slice(boundary + 1);       // Remove it from buffer

                try {
                    // Parse JSON data
                    const jsonData = JSON.parse(message);
                    console.log('Received JSON:', jsonData);

                    // send it to the frontend through IPC communication.
                    mainWindow.webContents.send('tcp-data', jsonData);
                    console.log("Send tcp data");
                } catch (error) {
                    console.error('Error parsing JSON:', error);
                }
            }
        });


        client.on('close', function() {
            console.log('Connection closed');
            persistentConnect(mainWindow, port);
        });

        client.on('error', function(err) {
            console.error('Error: ' + err);
            console.log("THE ERRROR" + err.message);
        });

    } catch (e) {
        console.log(e + ": error connecting to server!");
        persistentConnect(mainWindow, port);
    }
    
}

module.exports = { createTCP_Server };