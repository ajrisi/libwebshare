#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "hashtable.h"

typedef hashtable pagetable;

enum {
  POSTITEM_FILE,
  POSTITEM_BUFFER,
  POSTITEM_FUNCTION
};

typedef union postitem_u postitem;
union postitem_u {
  char *file_path;
  char *buffer;
  void*(*callback)(void *data);
};

typedef struct webpage_table_item_s webpage_table_item;
struct webpage_table_item_s {
  int postitem_type;
  char *mime;
  char *key;
  postitem value;
};

pagetable *pagetable_new();
void pagetable_free(pagetable *pages);
int pagetable_post_function(pagetable *pages, char *path, void*(*callback)(void *data));
int pagetable_post_file(pagetable *pages, char *path, char *file_path);
int pagetable_post_buffer(pagetable *pages, char *path, char *buffer);
webpage_table_item *pagetabletable_find(hashtable *webpage_table, char *path);
void pagetable_remove(hashtable *webpage_table, char *path);

#endif
