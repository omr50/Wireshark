const net = require('net');
const { app, BrowserWindow, ipcMain } = require('electron');

var client = new net.Socket();
console.log("Working");
client.connect(8000, 'localhost', function() {
  console.log('Connected');
  client.write('Client test message.\n'); // Send data and end connection
});

let buffer = ''; // Buffer to store incoming data

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
        } catch (error) {
            console.error('Error parsing JSON:', error);
        }
    }
});


client.on('close', function() {
  console.log('Connection closed');
});

client.on('error', function(err) {
  console.error('Error: ' + err);
});