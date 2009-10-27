#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "pagetable.h"

/**
 * Private Functions
 * 
 */
static int pagetable_post(pagetable *pages, int posttype, char *path, postitem *pi);
static int webpage_table_item_compare( void *li, void *ri);
static void *webpage_table_item_dup(void *it);
static void webpage_table_item_free(void *it);
static unsigned long webpage_table_item_hash(void *it);
static unsigned long webpage_table_item_rehash(void *it);


pagetable *pagetable_new()
{
  pagetable *new_table = (pagetable*)hashtable_new(webpage_table_item_hash, webpage_table_item_rehash, 
						   webpage_table_item_compare, webpage_table_item_dup,
						   webpage_table_item_free);

  return new_table;
  
}

void pagetable_free(pagetable *pages)
{
  hashtable_free(pages);
}

webpage_table_item *pagetable_find(pagetable *pages, char *path)
{
  if(pages == NULL) {
    return NULL;
  }

  /* build a webpage_table_item with the right key to search for */
  webpage_table_item lookfor;
  lookfor.key = path;

  return hashtable_find(pages, &lookfor); 
}


int pagetable_post_function(pagetable *pages, char *path, void*(*callback)(void *data)) 
{
  postitem pi = {0};
  pi.callback = callback;
  return pagetable_post(pages, POSTITEM_FUNCTION, path, &pi);
}

int pagetable_post_file(pagetable *pages, char *path, char *file_path) 
{
  /*   wserror(ws, MESSAGE, "adding file located at %s at %s\n", file_path, path); */

  postitem pi = {0};
  pi.file_path = file_path;
  return pagetable_post(pages, POSTITEM_FILE, path, &pi);
}

int pagetable_post_buffer(pagetable *pages, char *path, char *buffer) 
{
  postitem pi = {0};
  pi.buffer = buffer;
  return pagetable_post(pages, POSTITEM_BUFFER, path, &pi);
}

void pagetable_remove(hashtable *webpage_table, char *path)
{
  /* build a webpage_table_item with the right key to search for */
  webpage_table_item lookfor;
  lookfor.key = path;

  webpage_table_item *item = hashtable_find(webpage_table, &lookfor);
  if(item != NULL)
    webpage_table_item_free(item);
}

/**
 * Private Functions
 * 
 */

static int webpage_table_item_compare( void *li, void *ri) 
{
  webpage_table_item *left = li;
  webpage_table_item *right = ri;
  return strcmp(left->key, right->key);
}

static void *webpage_table_item_dup(void *it) 
{
  webpage_table_item *old_item = it;
  webpage_table_item *new_item = malloc(sizeof(webpage_table_item));
  if(new_item == NULL) return NULL;
  new_item->key = strdup(old_item->key);
  if(new_item->key == NULL) {
    free(new_item);
    return NULL;
  }
  new_item->postitem_type = old_item->postitem_type;
  new_item->value = old_item->value;

  /* need to do a deep copy of the buffer if the post item is a buffer */
  if(old_item->postitem_type == POSTITEM_BUFFER) {
    new_item->value.buffer = strdup(old_item->value.buffer);
    if(new_item->value.buffer == NULL) {
      free(new_item);
      return NULL;
    }
  }

  /* do a deep copy of the file path if it is a file being served */
  if(old_item->postitem_type == POSTITEM_FILE) {
    new_item->value.file_path = strdup(old_item->value.file_path);
    if(new_item->value.file_path == NULL) {
      free(new_item);
      return NULL;
    }
  }

  /* we do not do a deep copy of a function pointer, there is no need to */
  
  return (void*)new_item;
}

static void webpage_table_item_free(void *it)
{
  webpage_table_item *myitem = it;
  
  free(myitem->key);

  if(myitem->postitem_type == POSTITEM_BUFFER) {
    free(myitem->value.buffer);
  } else if(myitem->postitem_type == POSTITEM_FILE) {
    free(myitem->value.file_path);
  }
  
  free(myitem);
}

static unsigned long webpage_table_item_hash(void *it)
{
  webpage_table_item *myitem = it;
  return hshstrhash(myitem->key);
}

static unsigned long webpage_table_item_rehash(void *it) 
{
  webpage_table_item *myitem = it;   
  return hshstrehash(myitem->key);
}

static int pagetable_post(pagetable *pages, int posttype, char *path, postitem *pi)
{
  /*   wserror(ws, MESSAGE, "Adding a path to server: %s\n", path); */
  webpage_table_item toinsert = {0};
  toinsert.postitem_type = posttype;
  toinsert.key = path;
  toinsert.value = *pi;
  toinsert.mime = NULL;

  /* if it is a file, then i should auto-determine the mime here, if it is a buffer,
     then it will be assumed that it is text/plain */
  /*   if(posttype == POSTITEM_BUFFER) { */
  /*     toinsert.mime = mime_lookup("txt"); */
  /*   } else if(posttype == POSTITEM_FILE) { */
  /*     toinsert.mime = mime_auto(pi->file_path); */
  /* /\*     printf("determined that the mime for %s was %s\n", pi->file_path, toinsert.mime); *\/ */
  /*   } */
  toinsert.mime = "text/plain";

  if(pagetable_find(pages, path) != NULL) {
    /*     wserror(ws, MESSAGE, "webshare: collission detected on %s, already present in share?\n", path); */
    return -1;
  }
  
  return hashtable_insert((hashtable*)pages, &toinsert) == NULL ? -1 : 0;
}

