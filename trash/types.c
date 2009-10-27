/*
** types.c
** 
** Made by (Adam)
** Login   <adam@toxic.rh.rit.edu>
** 
** Started on  Mon Sep 22 19:40:31 2008 Adam
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

const int yes = 1;

//convert a date to a string
char* __date_to_string(date_t* mydate) {
  printf("date to string");
  return "stub";
}

//parse a string to get a date
date_t __string_to_date(char* mystring) {
  date_t ret = {0};
  printf("string to date");
  return ret;
}

//make a date_t from an epoch
date_t __epoch_to_date(int myeoch) {
  date_t ret = {0};
  printf("epoch to date");
  return ret;
}

//convert a qvalue into a string
char* __qvalue_to_string(qvalue_t* myqvalue) {
  return "stub";
}

//convert a string to a qvalue
qvalue_t __string_to_qvalue(char* mystring) {
  qvalue_t ret = {0};
  return ret;
}

char* __level_to_string(level_t* mylevel) {
  
}

level_t __string_to_level(char* mystring) {

}

char* __host_to_string(host_t* myhost, char* mybuffer) {
  char buffer[MAX_HOST_STRING_LENGTH+1] = {0};
  strcat(buffer, myhost->host);
  if (myhost->port != 0) {
    strcat(buffer, ":");
    //we use 6 here because the max port number is 5 in length
    char mynum[6] = {0};
    //10 here is the base of the port
    sprintf(mynum, "%d", myhost->port);
    strcat(buffer, mynum);
  }
  //move the locally constructed version into the passed in character buffer
  strcat(mybuffer, buffer);
  return mybuffer;
}

host_t __string_to_host(char* myhoststring) {
  host_t ret = {0};
  ret.host = (char*)malloc(MAX_HOST_STRING_LENGTH+1);
  memset(ret.host, 0, MAX_HOST_STRING_LENGTH+1);
  ret.port = 0;
  char* ind = strchr(myhoststring, ':');
  int len = strlen(myhoststring);
  if(ind == NULL) {
    //no port was defined
  } else {
    len = (ind - myhoststring)+1;
    char* portchrs = myhoststring+len;
    ret.port = atoi(portchrs);
  }
  memcpy(ret.host, myhoststring, len-1);
  return ret;
}

//free the string allocated for the host
void __free_host(host_t* myhost) {
  free(myhost->host);
}

/*
char* __relative_uri_to_string(relative_uri_t* myruri) {
}

relative_uri_t __string_to_relative_uri(char* mystring) {
  relative_uri_t ret = {0};
  return ret;
  }*/

/*
int main(int argc, char** argv) {
  date_t mydate;
  mydate = date.parse.string("string");
  mydate = date.parse.epoch(100);
  date.to_string(&mydate);
  return 0;
}
*/
