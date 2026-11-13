// A VOIR
//
// Programmation asynchrone
// Gestion des JSON en js
// Promesses / then

const http = require("http");
const fs = require("fs").promises

const host = 'localhost';
const port = 8000;

let indexFile;

const requestListener = function (req, res) {

 console.log(req.method, req.url);   // diagnostic : à retirer ensuite

	switch (`${req.method} ${req.url}`) {
		case ("GET /"):
			res.setHeader("Content-Type", "text/html");
			res.writeHead(200);
			res.end(indexFile);
			break;

		case ("POST /command"):
			let body = "";
			req.on("data", (chunk) => {
				body += chunk;
			})
			req.on("end", () => {
				const result = JSON.parse(body);
				console.log("Reçu : ", result.room, result.device, result.action);
			});
			break;
	}

}

const server = http.createServer(requestListener);

fs.readFile(__dirname + "/../html/index.html")
	.then(contents => {
		indexFile = contents;
		server.listen(port, host, () => {
			console.log(`Server is running on http://${host}:${port}`);
		});
	})
	.catch(err => {
		console.error(`Could not read index.html file: ${err}`);
		process.exit(1);
	});
