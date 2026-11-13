const http = require("http");
const fs = require("fs").promises;
const path = require("path");

const host = "192.168.1.190";
const port = 8000;

// Dossier contenant index.html et style.css
const dossierHtml = path.join(__dirname, "..", "html");


// Lit un fichier du dossier html et l'envoie avec le bon type de contenu.
async function servirFichier(res, nomFichier, type) {
	try {
		const contenu = await fs.readFile(path.join(dossierHtml, nomFichier));
		res.writeHead(200, { "Content-Type": type });
		res.end(contenu);
	} catch (err) {
		console.error(`Lecture impossible de ${nomFichier} : ${err.message}`);
		res.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
		res.end("Erreur serveur");
	}
}

// Reçoit le corps d'un POST, le valide, puis transmet la commande.
function recevoirCommande(req, res) {
	let body = "";
	req.setEncoding("utf8");

	req.on("data", (chunk) => {
		body += chunk;
	});

	req.on("end", () => {
		let cmd;
		try {
			cmd = JSON.parse(body);
		} catch (e) {
			res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
			res.end("JSON invalide");
			return;
		}

		const ligne = `${cmd.room},${cmd.device},${cmd.action}\n`;
		console.log("Envoi au serveur C :", ligne.trim());
		// client.write(ligne);   // à activer au branchement réel

		res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
		res.end("OK");
	});
}

const requestListener = function (req, res) {

	console.log(req.method, req.url);   // journal des requêtes : à retirer si inutile

	switch (`${req.method} ${req.url}`) {

		case "GET /":
			servirFichier(res, "index.html", "text/html; charset=utf-8");
			break;

		case "GET /style.css":
			servirFichier(res, "style.css", "text/css; charset=utf-8");
			break;

		case "POST /command":
			recevoirCommande(req, res);
			break;

		default:
			res.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
			res.end("Introuvable");
	}
};

const server = http.createServer(requestListener);

// Erreur au démarrage (port déjà utilisé, adresse invalide...) : on s'arrête.
server.on("error", (err) => {
	console.error("Erreur du serveur :", err.message);
	process.exit(1);
});

server.listen(port, host, () => {
	console.log(`Server is running on http://${host}:${port}`);
});
