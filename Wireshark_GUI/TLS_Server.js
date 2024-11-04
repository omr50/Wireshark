var net = require('net');

var client = new net.Socket();
console.log("Working");
client.connect(8000, 'localhost', function() {
  console.log('Connected');
  client.write('Client test message.\n'); // Send data and end connection
});

let buffer = ''; // Buffer to store incoming data

client.on('data', (chunk) => {
    buffer += chunk.toString(); // Append chunk to buffer
    console.log(buffer) 
    try {
        // Attempt to parse the JSON data
        const jsonData = JSON.parse(buffer);
        console.log('Received JSON:', jsonData);
        
        // Clear the buffer if parsing is successful
        buffer = '';
        
    } catch (error) {
        // If error is a SyntaxError, it likely means the JSON is incomplete
        if (error instanceof SyntaxError) {
            console.log('Incomplete JSON data, waiting for more...');
        } else {
            console.error('Error parsing JSON:', error);
            buffer = ''; // Clear buffer on error to prevent repeated failures
        }
    }
});

client.on('close', function() {
  console.log('Connection closed');
});

client.on('error', function(err) {
  console.error('Error: ' + err);
});