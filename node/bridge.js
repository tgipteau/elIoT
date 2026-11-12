const net = require('net');
const http = require('http');

const client = net.createConnection(
	{ port: 4242, host: '192.168.1.190' }, () => {

		console.log("Connected to server.");
		client.pipe(process.stdout);
		process.stdin.pipe(client);

});

client.on('error', (err) => {
	console.log("error :", err.message);
});

const server = http.createServer((req, res) => {
	if (req.method === 'GET') {
        res.writeHead(200, { 'Content-Type': 'text/plain' });
        res.end('Handling GET Request');
    } else if (req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            res.writeHead(200, { 'Content-Type': 'text/plain' });
            res.end(`Received data: ${body}`);
        });
    } else {
        res.writeHead(405, { 'Content-Type': 'text/plain' });
        res.end('Method Not Allowed');
    }
});

server.listen(4243, '192.168.1.190', () => {
    console.log('Server running at http://192.168.1.190:4243');
});
