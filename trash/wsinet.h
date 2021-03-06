/**
 * @file   wssock.h
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Mon Feb 16 22:26:04 2009
 * 
 * @brief  libwebshare socket operations
 * 
 * 
 */

#ifndef _WSSOCK_H_
#define _WSSOCK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

extern char yes;

typedef struct wssocket_s wssock;

struct wssocket_s {
  int sockfd;
  struct {
    char b[1500];
    char *bp;
    int cnt;
  } buffer;
  int error;
  socklen_t sin_size;
  struct sockaddr_in addr;
};

wssock wsnewsock( void );

wssock wssocket(int socket_family, int socket_type, int protocol);

wssock wssocket_inet( void );

int wssetsockopt(wssock sock, int level, int option_name, 
		 const void *option_value, socklen_t option_len);

int wssetsock_reuse(wssock sock);

int wsbind(wssock sock, int family, short port);

int wsbind_inet(wssock sock, short port);

int wslisten(wssock sock, int backlog);

wssock wsaccept(wssock sock);

int wsreadline( wssock *sock, char *bufptr, size_t len );

ssize_t writen(int fd, const void *vptr, size_t n);

ssize_t readn(int fd, void *vptr, size_t n);

#endif
