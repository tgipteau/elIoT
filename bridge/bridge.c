#include <stdio.h>
#include <stdlib.h>
#include "mongoose.h"

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "hello\n");
  }
}

int main(void){

	struct mg_mgr mgr;
	mg_mgr_init(&mgr);

	mg_http_listen(&mgr, "http://192.168.1.190", http_ev_handler, NULL);

	for(;;){

		mg_mgr_poll(&mgr, 100);
	}
	return EXIT_SUCCESS
}
