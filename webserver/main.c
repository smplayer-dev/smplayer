/*  simple_web_server
    Copyright (C) 2017-2020 Ricardo Villalba <rvm@users.sourceforge.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "mongoose.h"

#ifndef _WIN32
#include <sys/types.h>
#include <sys/wait.h>
#endif

static struct mg_serve_http_opts server_opts;
static int exit_flag = 0;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
	if (ev == MG_EV_HTTP_REQUEST) {
		mg_serve_http(nc, (struct http_message *) p, server_opts);
	}
}

static void __cdecl signal_handler(int sig_num) {
	// Reinstantiate signal handler
	signal(sig_num, signal_handler);

#ifndef _WIN32
	if (sig_num == SIGCHLD) {
		do {} while (waitpid(-1, &sig_num, WNOHANG) > 0);
	} else
#endif
	{ exit_flag = sig_num; }
}

#ifdef _WIN32
static void checkCloseEvent() {
	MSG msg;
	if (PeekMessage(&msg, NULL, WM_CLOSE, WM_CLOSE, PM_NOREMOVE)) exit_flag = 1;
}
#endif

static void show_usage() {
	printf("Usage: simple_web_server [OPTIONS] \n");
	printf("%s\n", "Options:");
	printf("%s\n", "  -p\t port");
	printf("%s\n", "  -r\t document root");
	printf("%s\n", "  -j\t hide directory listing");
}

int main(int argc, char *argv[]) {
	struct mg_mgr mgr;
	struct mg_connection *nc;

	const char *http_port = "8000";
	const char *doc_root = ".";
	const char *listing = "yes";
	int i;

	// Parse command line options
	for (i = 1; i < argc && argv[i][0] == '-'; i++) {
		if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
			http_port = argv[++i];
		}
		else
		if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
			doc_root = argv[++i];
		}
		else
		if (strcmp(argv[i], "-j") == 0) {
			listing = "no";
		}
		else
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-?") == 0) {
			show_usage();
			return -1;
		}
		else {
			fprintf(stderr, "Error: unknown option %s \n", argv[i]);
			show_usage();
			return -1;
		}
	}

	// Setup signal handler: quit on Ctrl-C
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
#ifndef _WIN32
	signal(SIGCHLD, signal_handler);
#endif

	mg_mgr_init(&mgr, NULL);

	printf("Starting web server on port %s\n", http_port);
	printf("Document root: %s\n", doc_root);
	printf("Directory listing: %s\n", listing);
	fflush(stdout);

	nc = mg_bind(&mgr, http_port, ev_handler);
	if (nc == NULL) {
		printf("Failed to create listener\n");
		return 1;
	}

	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	server_opts.document_root = doc_root;
	server_opts.enable_directory_listing = listing;
	server_opts.extra_headers = "Access-Control-Allow-Origin: *";

	while (exit_flag == 0) {
		mg_mgr_poll(&mgr, 1000);
		#ifdef _WIN32
		checkCloseEvent();
		#endif
	}
	mg_mgr_free(&mgr);

	printf("Exiting on signal %d\n", exit_flag);

	return 0;
}
