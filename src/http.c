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

#include "http.h"
#include "pagetable.h"

int http_request_parse(http_request *r, char *buffer, unsigned int blen)
{
  char met[MAX_METHOD_LENGTH];
  int imet;
  int iurl;
  int ibuf;

  static enum PARSE_STATE {
    START,
    FIRST_WORD,
    SECOND_WORD,
    DONE,
    ERR
  } cstate;

  cstate = START;
  imet = 0;
  iurl = 0;
  ibuf = 0;

  if(buffer == NULL) {
    blen = 0;
  }

  while((ibuf < blen) &&
	(cstate != DONE) &&
	(buffer[ibuf] != '\0') &&
	((cstate == FIRST_WORD) ? (imet < MAX_METHOD_LENGTH) : 1) &&
	((cstate == SECOND_WORD) ? (iurl < MAX_URL_LENGTH) : 1)) {
    switch (cstate) {
    case START:
      /* read char, if blank, forward by one, if not, set first word */
      if(buffer[ibuf] == ' ') ibuf++;
      else cstate = FIRST_WORD;
      break;
    case FIRST_WORD: 
      if(buffer[ibuf] == ' ') cstate = SECOND_WORD;
      else met[imet++] = buffer[ibuf];
      ibuf++;
      /* if blank, set second word */
      /* if not, copy char */
      /* forward by one */
      break;
    case SECOND_WORD:
      if(buffer[ibuf] == ' ') cstate = DONE;
      else r->url[iurl++] = buffer[ibuf];
      ibuf++;
      /* if blank, set third/done */
      /* if not, copy char */
      /* fw by 1 */
      break;
    default:
      cstate = ERR;
    }
  }
  
  if(cstate != DONE) {
    return -1;
  }

  /* ensure that we have hit a \r\n\r\n */
  if(strstr(buffer, "\r\n\r\n") == NULL) {
    return -1;
  }

  r->method = 0;
  
  return 0;
}

int send_error(int fd, int code, char *desc)
{
  char buffer[2048];
  char line[256];

  memset(buffer, 0, 2048);

  sprintf(line, "HTTP/1.0 %d %.80s\r\n\r\n", code, desc);
  strcat(buffer, line);
  sprintf(line, "<HTML><HEAD><TITLE>%d - %s</TITLE></HEAD><BODY>", code, desc);
  strcat(buffer, line);
  sprintf(line, "<H1>%d - %s</H1></BODY></HTML>", code, desc);
  strcat(buffer, line);

  write(fd, buffer, strlen(buffer));

  return 0;
}

