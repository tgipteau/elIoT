#include <stdio.h>
#include <stdlib.h>
#include "ressources/mongoose.h"


/* Http event handler.
 * Handle GET and POST.
 * On POST /cmd, get the command as a JSON with paths "room", "device" and "action".
 * Parse that JSON and sends it as a clean PUB to the broker.
 */
static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {

	/* Retrieving the broker connection from void* pointer fn_data */
	struct mg_connection *broker_conn = (struct mg_connection*) c->fn_data;

	if (ev == MG_EV_HTTP_MSG) {  /* Http request received */
		struct mg_http_message *hm = (struct mg_http_message *) ev_data; /* Parse the request */

		/* GET */
		if (mg_match(hm->method, mg_str("GET"), NULL)){
			/* Serve static files from html folder */
		   	struct mg_http_serve_opts opts = {.root_dir = "./html", .fs = &mg_fs_posix};
		    mg_http_serve_dir(c, hm, &opts);
		}

		/* POST */
		else if (mg_match(hm->method, mg_str("POST"), NULL)){

		   	if (mg_match(hm->uri, mg_str("/cmd"), NULL)) {

				/* Parse JSON command */
				struct mg_str json_cmd = hm->body;

				printf("%s\n", json_cmd.buf);

			 	char *room = mg_json_get_str(json_cmd, "$.room");
				char *device = mg_json_get_str(json_cmd, "$.device");
				char *action =  mg_json_get_str(json_cmd, "$.action");

				/* Format PUB transmission and send to the broker*/
				mg_printf(broker_conn, "PUB %s/%s/%s", room, device, action);

				/* Cleanup and OK response */
				free(room);free(device);free(action);
				mg_http_reply(c, 200, "", "{%m:%d}\n", MG_ESC("status"), 1);
		    }
		}

		else {

		}
	}
}

int main(void){

	/* Initiate a mongoose manager.
	 * We  create a tcp connection to the broker,
	 * then initiate the bridge webserver with http listen.
	 * The broker connection is passed to the http event handler,
	 * Thus, the event handler can send data from POST requests to the broker.
	 */

	struct mg_mgr mgr;
	mg_mgr_init(&mgr);

	/* Broker connection */
	struct mg_connection *broker_conn;
	broker_conn = mg_connect(&mgr, "192.168.1.190:4242", NULL, NULL);

	/* Http webserver startup */
	mg_http_listen(&mgr, "http://192.168.1.190:8000", ev_handler, broker_conn);

	/* Mainloop */
	for(;;){

		mg_mgr_poll(&mgr, 100);
	}

	return EXIT_SUCCESS;
}
