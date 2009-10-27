#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "filesystem.h"
#include "cpt.h"
#include "rpt.h"
#include "http.h"
#include "types.h"
#include "server.h"

#ifdef DISABLED

static gchar *chr = "Thread done\n";
static struct sockaddr_in my_addr;

gpointer CPT(gpointer);
gpointer RPT(gpointer);

int construct_mainsock(int *sockfd) {
	
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(my_addr.sin_zero, 0, sizeof(my_addr.sin_zero));

	bind(*sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));

	//send to the log a message about the bound local address and port
	g_message("bound server to %s port %d", inet_ntoa(my_addr.sin_addr), MYPORT);
	
	listen(*sockfd, MAX_LISTEN);
}

void CPT_join_thread(gpointer element, gpointer passed) {
	g_thread_join(element);
}

void cpt_data_free(struct CPT_data *mydata) {
	free(mydata);
}

gpointer CPT(gpointer data) {
	//grab a pointer to the relevant data to this CPT
	struct CPT_data *mydata = (struct CPT_data*)data;
	
	GSList *mythreads = g_slist_alloc();
	
	//next, read the request headers that the client sent, and
	//compartmentalize them into a GSList of strings that can be looked at by a
	//RPT (request processing thread)
	char buffer[MAX_HEADER_LENGTH+1] = {0};
	int nbytes = 0;
	while((nbytes = recv(mydata->thissockfd, buffer, MAX_HEADER_LENGTH, 0)) > 0) {
		//g_message("read from client bytes %d: %s", nbytes, buffer);
		char *request_break = strstr(buffer, "\r\n\r\n");
		if (request_break != NULL) {
			int length_of_request = request_break - buffer;
			//4 for \r and \n
			int length_remaining = nbytes - (length_of_request+4);
			
			char *thisrequest = malloc(length_of_request+1);
			memset(thisrequest, 0, length_of_request+1);
			memcpy(thisrequest, buffer, length_of_request);
			
			//g_message("isolated request: --%s--", thisrequest);
			
			//great! its been isolated, so lets create a RPT to process it
			struct RPT_data *newrptdata = malloc(sizeof(struct RPT_data));
			newrptdata->request = thisrequest;
			mythreads = g_slist_append(mythreads, (gpointer)g_thread_create(RPT,
		    					   (gpointer)newrptdata, FALSE, NULL));
						
			
			//4 to skip the newlines and carriage returns
			//g_message("remaining in buffer: --%s--", buffer+length_of_request+4);
			
			//now move the remaining data to the beginning of the buffer and 
			//clear the rest of the buffer, so its ready to read more
			memcpy(buffer, buffer+length_of_request+4, length_remaining);
			memset(buffer+length_remaining, 0, MAX_HEADER_LENGTH - length_remaining);
			
			//g_message("now the buffer is --%s--", buffer);
			
			//printf("HEADERS ARE DONE\r\n");
			//break;
		} else {
			//this client did not comply to max http header length rules.
		}
		memset(buffer, 0 , 1024);
	}
	
	//we will need to be able to split the read data into individual requests
	
	//GAsyncQueue *requestqueue = g_async_queue_new();
	

	
	/*gint i = 0;
	for(i = 0; i < 5; i++) {
		mythreads = g_slist_append(mythreads, (gpointer)g_thread_create(RPT,
		    					   (gpointer)requestqueue, FALSE, NULL));
	}*/	 
	
	
	//for(i = 0; i < 5; i++) {
	//	gpointer retdta = g_async_queue_pop(requestqueue);
	//	printf("%s", (char*)retdta);
	//}
	
	//join all of the threads
	//g_slist_foreach(mythreads, CPT_join_thread, NULL);
	
	g_slist_free(mythreads);
	
	//final unref of the queue to clear it
	//g_async_queue_unref(requestqueue);
	
	//free the memory used by the CPT_data struct
	cpt_data_free(mydata);
	
	return NULL;
}

void rpt_data_free(struct RPT_data *mydata) {
	free(mydata);
}
	
gpointer RPT(gpointer data) {
	struct RPT_data *mydata = (struct RPT_data *)data;
	
	g_message("RPT got request of --%s--", mydata->request);
	
	
	//construct a reply
	//http_header *reply_header = http_make_default_header(REPLY);
	
	
	//GAsyncQueue *loqueue = (GAsyncQueue*)myqueue;
	//loqueue = g_async_queue_ref(loqueue);
	//start of que use zone
	
	//handle the single request
	
	
	//tell the main thread that the request has been finished
	//g_async_queue_push(loqueue, (gpointer)chr);
	
	//end of queue use zone
	//g_async_queue_unref(loqueue);
	
	rpt_data_free(mydata);
	return NULL;
}

/*int main(int argc, char **argv) {

	if (!g_thread_supported ()) g_thread_init (NULL);
	
	int mainsockfd = 0;
	socklen_t sin_size;
	
	//create the main socket
	construct_mainsock(&mainsockfd);
	
	struct sockaddr_in their_addr;
	sin_size = sizeof(their_addr);
	
	while(1) {
		int newsockfd = accept(mainsockfd, (struct sockaddr *)&(their_addr), &sin_size);
		g_message("new connection: %s", inet_ntoa(their_addr.sin_addr));
		
		//construct a new CPT data struct
		struct CPT_data *newsockdata = malloc(sizeof(struct CPT_data));
		newsockdata->mainsockfd = mainsockfd;
		newsockdata->thissockfd = newsockfd;
		
		//create a new CPT thread to handle the connection
		//false means not joinable!
		GThread *newclientthread = g_thread_create(CPT, (gpointer)newsockdata, FALSE, NULL);
		
		//struct newth_s *newpack = malloc(sizeof(struct newth_s));
		//newpack->losockfd = tso;
		//newpack->their_addr = their_addr;
		//newpack->id_number = thread_counter;
		//thr = pthread_create(&(threads[thread_counter]), NULL, new_connection, newpack);
		//pthread_detach(threads[thread_counter]);
		//thread_counter++;
	}
	
	//create a new client processing thread
	//GThread *clientthread = g_thread_create(CPT, NULL, TRUE, NULL);

	//g_thread_join(clientthread);

	return 0;
}*/
#endif
