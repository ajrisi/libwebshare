/**
 * @file   webshare.c
 * @author Adam Risi
 * @date   Tue Feb 10 2009
 * 
 * @brief  The main webshare source file, a better description should go here
 * 
 * 
 */

#include <webshare.h>
#include <client_thread.h>

/**
 * Private Functions
 * 
 */
static webshare_config webshare_default_config( void );


WEBSHARE *webshare_new(webshare_config *config)
{
  WEBSHARE *ws = (WEBSHARE*)malloc(sizeof(WEBSHARE));
  if(ws == NULL) {
    return NULL;
  }

  if(config != NULL) {
    ws->config = *config;
  } else {
    ws->config = webshare_default_config();
  }

  ws->pages = pagetable_new();

  return ws;
}

int webshare_start(WEBSHARE *ws)
{
  int listenfd, connfd;
  int i;
  struct sockaddr_in servaddr;

  struct sockaddr client;
  socklen_t client_size;

  connfd = -1;
  memset(&client, 0, sizeof(client));
  client_size = 0;

  listenfd = socket(PF_INET, SOCK_STREAM, 0);
  if(listenfd == -1) {
    return -1;
  }


  ws->listenfd = listenfd;

  struct linger l;

  /* fix the linger opations so that we are not left in a TIME WAIT on
     close */
  l.l_onoff = 1;
  l.l_linger = 0;
  i = setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
  if(i < 0) {
    return -1;
  }

  char yep = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yep, sizeof(char));  

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(ws->config.port);

  if(bind(listenfd, (struct sockaddr *)&servaddr, (socklen_t)sizeof(servaddr)) == -1) {
    return -1;
  }

  if(listen(listenfd, 5) == -1) {
    return -1;
  }

  while((connfd = accept(listenfd, &client, &client_size)) != -1) {

    printf("Got a client\n");

    /* create the client_thread data */
    client_data d;
    d.remote_fd = connfd;
    d.ws = ws;

    (void) client_thread((void*)&d);

  }

  return 0;
}

int webshare_stop(WEBSHARE *ws)
{
  close(ws->listenfd);
  return 0;
}

int webshare_post_buffer(WEBSHARE *ws, char *path, char *buffer)
{
  return pagetable_post_buffer(ws->pages, path, buffer);
}

int webshare_post_file(WEBSHARE *ws, char *path, char *file_path)
{
  return pagetable_post_file(ws->pages, path, file_path);
}

int webshare_post_function(WEBSHARE *ws, char *path, void*(*callback)(void *data))
{
  return pagetable_post_function(ws->pages, path, callback);
}

void webshare_post_remove(WEBSHARE *ws, char *path)
{
  pagetable_remove(ws->pages, path);
}


/**
 * Private Functions
 * 
 */
static webshare_config webshare_default_config()
{
  webshare_config ws = {0};
  ws.host = DEFAULT_HOST;
  ws.port = DEFAULT_PORT;
  
  ws.mime.unknown = DEFAULT_UNKNOWN_MIME;
  
  ws.threading.model = DEFAULT_THREADING_MODEL;
  ws.threading.pool_size = DEFAULT_THREAD_POOL_SIZE;
  
  return ws;
}

