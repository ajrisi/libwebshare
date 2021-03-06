#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <webshare.h>

//this is the global variable that will be used to store the webshare service
WEBSHARE *ws;

/**
 * This function is called when the program receives SIGINT (Ctrl-C). It shuts down the
 * webshare service and cleans up, then exits the program
 * 
 * @param sig This is the signal that shutdown_program was called with
 */
void shutdown_program(int sig) {
	printf("\nShutting down...\n");

	//shutdown the webshare
	webshare_stop(ws);

	printf("Done.\n");
	exit(0);
}

int main(int argc, char **argv) {
	//register the signal handler for safely shutting down the program
	signal(SIGINT, shutdown_program);

	//init the webshare service with the default settings
	ws = webshare_init(NULL);

	//post Hello World at the root of the server
	webshare_post_file(ws, "/", "./index.html"); 

	//start the webshare service
	webshare_start(ws);

	//include this here, just in case
	webshare_stop(ws);
	return 0;
}
