#ifndef _MIME_H_
#define _MIME_H_

#include "trie.h"

typedef struct _mime_pair mime_pair;
struct _mime_pair {
  char *key;
  char *value;
};

static trie *mime_trie;

int mime_table_init();
void mime_table_free();
char *mime_lookup(char *extension);

#endif
