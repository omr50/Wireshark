var net = require('net');

var client = new net.Socket();
console.log("Working");
client.connect(8000, 'localhost', function() {
  console.log('Connected');
  client.write('Client test message.\n'); // Send data and end connection
});

client.on('data', function(data) {
  console.log('Received: ' + data);
});

client.on('close', function() {
  console.log('Connection closed');
});

client.on('error', function(err) {
  console.error('Error: ' + err);
});