var http = require('http');
var url = require('url');
var fs = require('fs');
var io = require('socket.io');
var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var sp = new SerialPort("/dev/ttyO4", {
	parser: serialport.parsers.readline("\n")
});

sp.open();

sp.on('open', function() {
	console.log("Serial port opened");
});

sp.on('data', function(data) {
	console.log("Received data : ");
	console.log(data);
});

var server = http.createServer( function (request, response) {
	
	var path = url.parse(request.url).pathname;

	switch(path) {

		case '/':
			response.writeHead(200, {'Content-Type':'text/html'});
			response.write("Welcome!");
			response.end();
			break;

		case '/bb.html':
			console.log("Inside bb html case");
			fs.readFile(__dirname+path, function(error, data) {
				if (error) {
					console.log("Inside error handling code");
					response.writeHead(404);
					response.write("Oops the page does`nt exist");
					response.end();
				} else {
					response.writeHead(200, {"Content-Type":"text/html"});
					response.write(data);
					response.end();
				}
			});
			break;

		default:
			response.writeHead(404);
			response.write("Oops resource not found!");
			response.end();
			break;
	}

});

server.listen(5000);

var listner = io.listen(server);

listner.sockets.on('connection', function(socket) {

	socket.on('start_button_pressed', function() {
		process.stdout.write("Start button is pressed\n");
		sp.write('A');
		sp.drain();
	});

	socket.on('stop_button_pressed', function() {
		process.stdout.write("Stop button is pressed\n");
		sp.write('B');
		sp.drain();
	});

	socket.on('start_degree', function(data) {
		var degreeValue = data.degree;
		process.stdout.write(degreeValue+'\n');
		sp.write('C');
		sp.write(degreeValue);
		sp.write('E');
	});

	socket.on('get_coordinates', function() {
		process.stdout.write("Get coordinates is pressed\n");
		sp.write('D');
	});

});
