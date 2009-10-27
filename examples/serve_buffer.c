#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <webshare.h>

WEBSHARE *ws;

char *demo_html = "libWebShare is Working!";

/**
 * This function is called when the program receives SIGINT (Ctrl-C). It shuts down the
 * webshare service and cleans up, then exits the program
 * 
 * @param sig This is the signal that shutdown_program was called with
 */
void shutdown_program(int sig) 
{
	printf("\nShutting down...\n");

	webshare_stop(ws);

	printf("Done.\n");
	exit(0);
}

int main(int argc, char **argv)
{
	signal(SIGINT, shutdown_program);

	ws = webshare_new(NULL);

	webshare_post_buffer(ws, "/", demo_html);

	/* start webshare */
	webshare_start(ws);

	/* include this here, just in case */
	shutdown_program(0);
	return 0;
}
