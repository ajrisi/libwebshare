/**
 * @file   wsstring.c
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Mon Feb 16 21:52:46 2009
 * 
 * @brief  Extended string library for use with webshare
 * 
 * 
 */

#include <string.h>

char *chop(char *string)
{
  size_t len = strlen(string);
  if(string && *string)
    string[len-1] = '\0';
  return string;
}

char *chomp(char *string)
{
  size_t len = strlen(string);
  if(string && *string) {
    switch(string[len-1]) {
    case '\r':
    case '\n':
      string = chop(string);
      break;
    default:
      ;
    };
  }

  return string;
}

char *strip_eol(char *string) 
{
  int len = strlen(string);
  while(strlen(chomp(string)) != len)
    len = strlen(string);
  return string;
}

char *replace_char(char *str, char oldc, char newc)
{
  int i;
  char *rp;
  for (i=0;i < strlen(str); i++) {
    if (*(str + i) == oldc) {
      rp = str+i;
      *rp = newc;
    }
  }
  return str;
}
