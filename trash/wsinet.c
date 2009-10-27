/**
 * @file   wssock.c
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Mon Feb 16 22:24:55 2009
 * 
 * @brief  Socket operations done by libwebshare
 * 
 * 
 */

#include "wsstring.h"
#include "wsinet.h"

wssock wsnewsock( void )
{
  wssock ret = {0};
  ret.buffer.cnt = 0;
  ret.sin_size = sizeof(ret.addr);
  return ret;
}

wssock wssocket(int socket_family, int socket_type, int protocol)
{
  wssock ret = wsnewsock();

  ret.sockfd = socket(socket_family, socket_type, protocol);

  return ret;
}

wssock wssocket_inet( void )
{
  return wssocket(AF_INET, SOCK_STREAM, 0);
}

int wssetsockopt(wssock sock, int level, int option_name, 
		 const void *option_value, socklen_t option_len)
{
  return setsockopt(sock.sockfd, level, option_name, option_value, option_len);
}

int wssetsock_reuse(wssock sock)
{
  char yes = '1';
  return setsockopt(sock.sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(char));
}

int wsbind(wssock sock, int family, short port)
{
  sock.addr.sin_family = family;
  sock.addr.sin_port = htons(port);
  sock.addr.sin_addr.s_addr = INADDR_ANY;
  memset(sock.addr.sin_zero, 0, sizeof(sock.addr.sin_zero));
  
  return bind(sock.sockfd, (struct sockaddr*)&sock.addr, sizeof(sock.addr));
}

int wsbind_inet(wssock sock, short port)
{
  return wsbind(sock, AF_INET, port);
}

int wslisten(wssock sock, int backlog)
{
  return listen(sock.sockfd, backlog);
}

wssock wsaccept(wssock sock)
{
  wssock acc = wsnewsock();

  acc.sockfd = accept(sock.sockfd, (struct sockaddr *)&(acc.addr), &acc.sin_size);

  return acc;
}

int wsreadline( wssock *sock, char *bufptr, size_t len )
{
  char *bufx = bufptr;
  char c;
  
  while ( --len > 0 ) {
    if ( --sock->buffer.cnt <= 0 ) {
      sock->buffer.cnt = recv( sock->sockfd, sock->buffer.b, sizeof( sock->buffer.b ), 0 );
      //printf("read %d bytes\n", sock->buffer.cnt);
      if ( sock->buffer.cnt < 0 ) {
	if ( errno == EINTR ) {
	  len++;/* the while will decrement */
	  continue;
	}
	return -1;
      }
      if ( sock->buffer.cnt == 0 )
	return 0;
      sock->buffer.bp = sock->buffer.b;
    }
    c = *sock->buffer.bp++;
    *bufptr++ = c;
    //printf("adding %c to line\n", c);
    if ( (c == '\n') || (c == '\r') ) {
      *bufptr = '\0';
      return bufptr - bufx;
    }
  }
  sock->error = EMSGSIZE;
  return -1;
}

/** 
 * Reads n bytes from a file descriptor
 * 
 * @param fd the file descriptor
 * @param vptr the address to put the read data
 * @param n the number of bytes to read
 * 
 * @return the number of bytes read (this could be less than n if there was an error or an EOF), and -1 on error
 */
ssize_t readn(int fd, void *vptr, size_t n)
{
  size_t nleft;
  ssize_t nread;
  char *ptr;

  /* initialize the target pointer, and the number of bytes left */
  ptr = vptr;
  nleft = n;
  nread = 0;

  while(nleft > 0) {
    if((nread = read(fd, ptr, nleft)) < 0) {
      /* an error occurred, check to make sure it wasnt just an interrupt */
      if(errno == EINTR) {
	/* it was just an interrupt, try again (and make sure that nread for this iteration was 0 */
	nread = 0;
	continue;
      } else {
	return(-1);
      }
    } else if(nread == 0) {
      /* the read worked, but it returned 0, so it was an EOF */
      break;
    }
    /* normal read, got some bytes, adjust ptr and nleft */
    ptr += nread;
    nleft -= nread;
  }
  return(n - nleft);
}

/** 
 * Writes n bytes to a file handle
 * 
 * @param fd the file handle to write to
 * @param vptr the source of the data
 * @param n the number of bytes to write out
 * 
 * @return the number of bytes written, or -1 on error
 */
ssize_t writen(int fd, const void *vptr, size_t n)
{
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  /* initialize the number of bytes left to write, the number already written, and ptr */
  nleft = n;
  nwritten = 0;
  ptr = vptr;

  while(nleft > 0) {
    if((nwritten = write(fd, ptr, nleft)) <= 0) {
      /* if the number of bytes written is negative, then it might have been an interrupt */
      if(nwritten < 0 && errno == EINTR) {
	/* just try again, and make sure that nwritten was 0 for this round */
	nwritten = 0;
	continue;
      } else {
	/* there was a more serious error, so return negative */
	return(-1);
      }
    }
    
    /* adjust nleft and ptr */
    ptr += nwritten;
    nleft -= nwritten;
  }
  
  /* return the number of bytes written (not nwirtten, that is just for every while iteration */
  return(n);
}


