#ifndef UTIL_H
#define UTIL_H

#include <string.h>

char *chop(char *string);

char *chomp(char *string);

char *strip_eol(char *string);

char *replace_char(char *str, char oldc, char newc);

#endif
