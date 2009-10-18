


/* main.c
 * this is the main file for the torus daemon, it loads the configuration files,
 * sets up and starts the server, handles indexing, etc */

#include <glib.h>
#include <gmodule.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "http.h"
#include "callback_table.h"
#include "cpt.h"
#include "module_manager.h"
#include "thttpd.h"


/**
 *
 */
void http_server_init() {
    //initialize threads
    if (!g_thread_supported ()) g_thread_init (NULL);

    if (!g_module_supported()) {
        g_critical("GLIB Modules must be supported for the torus http server to run");
        exit(1);
    }

    if (!g_thread_supported()) {
        g_critical("GLib Threads must be supported for the torus http server to run");
        exit(1);
    }

    //initialize the client processing thread pool
    http_cpt_init();

    //load the server configuration data
    http_load_config();

#ifdef DEBUG
    g_message("http server will be started on port %d", HTTP_SERVER_PORT);
#endif

    //init the callback table
    http_callback_table_init();

    //initialize the module manager
    http_mm_init();

}

/* http_server_start
 *
 * This function actually opens the port and sets it up for listening for incoming
 * clients. Once this is called, the server really is running
 */
/**
 *
 */
void http_server_start() {
    //construct the http server socket
    http_server_construct_socket();

    //bind the server socket
    http_server_bind_socket();

    //start listening with server socket
    http_server_start_listening();

}

/**
 *
 */
void http_server_shutdown() {
    g_message("shutdown initialized");
    //shutdown actions would go here

    g_message("shutdown finished");

    exit(0);
}

/**
 *
 * @param argc
 * @param argv
 * @return
 */
/*
int main(int argc, char **argv) {
    http_server_init();
    g_message("torus web server initialized");

    http_server_start();
    g_message("torus web server started");

    //pass off the control of the server to the http_server_main function.
    http_server_main();

    //this statement should never be reached
    return 0;
}
*/