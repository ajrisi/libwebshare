/* file should contain all of the page posting functions */
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

#include "wspagetable.h"
#include "wsfs.h"
#include "wsserver.h"
#include "wserror.h"

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

  //need to do a deep copy of the buffer if the post item is a buffer
  if(old_item->postitem_type == POSTITEM_BUFFER) {
    new_item->value.buffer = strdup(old_item->value.buffer);
    if(new_item->value.buffer == NULL) {
      free(new_item);
      return NULL;
    }
  }

  //do a deep copy of the file path if it is a file being served
  if(old_item->postitem_type == POSTITEM_FILE) {
    new_item->value.file_path = strdup(old_item->value.file_path);
    if(new_item->value.file_path == NULL) {
      free(new_item);
      return NULL;
    }
  }

  //we do not do a deep copy of a function pointer, there is no need to

  return (void*)new_item;
}

static void webpage_table_item_free(void *it)
{
  webpage_table_item *myitem = it;
  
  free(myitem->key);

  if(myitem->postitem_type == POSTITEM_BUFFER)
    free(myitem->value.buffer);
  else if(myitem->postitem_type == POSTITEM_FILE) 
    free(myitem->value.file_path);
  
  free(myitem);
}

static unsigned long webpage_table_item_hash(void *it)
{
  webpage_table_item *myitem = it;
  return hshstrhash_sdbm(myitem->key);
}

static unsigned long webpage_table_item_rehash(void *it) 
{
  webpage_table_item *myitem = it;   
  return hshstrhash_rs(myitem->key);
}

int webpage_table_init(hshtbl **webpage_table)
{
  hshtbl *new_table = hshinit(webpage_table_item_hash, webpage_table_item_rehash, 
			      webpage_table_item_compare, webpage_table_item_dup,
			      webpage_table_item_free, 0);
  *webpage_table = new_table;

  return (*webpage_table == NULL);
  
}

static void webpage_table_free( hshtbl *webpage_table )
{
  hshkill(webpage_table);
}

void webshare_pagetable_free(WEBSHARE *ws)
{
  webpage_table_free(ws->pagetable);
}

webpage_table_item *webpage_table_find(hshtbl *webpage_table, char *path)
{
 
  /* build a webpage_table_item with the right key to search for */
  webpage_table_item lookfor;
  lookfor.key = path;

  return hshfind(webpage_table, &lookfor); 
}

static int webshare_post_postitem(WEBSHARE *ws, int posttype, char *path, postitem *pi)
{
  wserror(ws, MESSAGE, "Adding a path to server: %s\n", path);
  webpage_table_item toinsert = {0};
  toinsert.postitem_type = posttype;
  toinsert.key = path;
  toinsert.value = *pi;
  toinsert.mime = NULL;

  /* if it is a file, then i should auto-determine the mime here, if it is a buffer,
     then it will be assumed that it is text/plain */
  if(posttype == POSTITEM_BUFFER) {
    toinsert.mime = mime_lookup("txt");
  } else if(posttype == POSTITEM_FILE) {
    toinsert.mime = mime_auto(pi->file_path);
    printf("determined that the mime for %s was %s\n", pi->file_path, toinsert.mime);
  }

  //make sure the item is not already present
  if(webpage_table_find(ws->pagetable, path) != NULL) {
    wserror(ws, MESSAGE, "webshare: collission detected on %s, already present in share?\n", path);
  } else {
    /* return 1 on failure, 0 on success */
    return hshinsert(ws->pagetable, &toinsert) == NULL;
  }
}

int webshare_post_function(WEBSHARE *ws, char *path, void*(*callback)(void *data)) 
{
  postitem pi = {0};
  pi.callback = callback;
  return webshare_post_postitem(ws, POSTITEM_FUNCTION, path, &pi);
}

int webshare_post_file(WEBSHARE *ws, char *path, char *file_path) 
{
  wserror(ws, MESSAGE, "adding file located at %s at %s\n", file_path, path);

  postitem pi = {0};
  pi.file_path = file_path;
  return webshare_post_postitem(ws, POSTITEM_FILE, path, &pi);
}

int webshare_post_buffer(WEBSHARE *ws, char *path, char *buffer) 
{
  postitem pi = {0};
  pi.buffer = buffer;
  return webshare_post_postitem(ws, POSTITEM_BUFFER, path, &pi);
}

int webshare_post_directory(WEBSHARE *ws, char *path_prefix, char *dir_path) 
{
  wserror(ws, MESSAGE, "sharing %s with prefix %s\n", dir_path, path_prefix);

  //to post a directory, we make recursive calls to post a file
  DIR *dp;
  struct dirent *entry;

  dp = opendir(dir_path);
  if(dp == NULL) return 1;

  while((entry = readdir(dp)) != NULL) {
    /* 1024 is a hard coded limitation here */
    char path[1024] = {0};
    sprintf(path, "%s/%s", dir_path , entry->d_name);
    if(path_is_regular(path) && !path_is_directory(path)) {
      char sharepath[1024] = {0};
      sprintf(sharepath, "%.800s/%.223s", path_prefix, entry->d_name);
      if(webshare_post_file(ws, sharepath, path) != 0) {
	wserror(ws, MESSAGE, "webshare: unable to post item at %s\n", sharepath);
	//return 1;
      }
    } 
    if(path_is_directory(path)) {
      char sharepath[1024] = {0};
      if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ) {
	sprintf(sharepath, "%.800s/%.223s", path_prefix, entry->d_name);
	if(webshare_post_directory(ws, sharepath, path) != 0) {
	  wserror(ws, MESSAGE, "webshare: unable to post item at %s\n", sharepath);
	  //return 1;
	}
      }
    }
  }

  closedir(dp);  
  return 0;
}


static void webpage_table_drop(hshtbl *webpage_table, char *path)
{
  /* build a webpage_table_item with the right key to search for */
  webpage_table_item lookfor;
  lookfor.key = path;

  webpage_table_item *item = hshfind(webpage_table, &lookfor);
  if(item != NULL)
    webpage_table_item_free(item);
}

webpage_table_stats webpage_table_status(hshtbl *webpage_table) 
{
  webpage_table_stats stat = {0};
  hshstats raw_stats = hshstatus(webpage_table);

  stat.probes = raw_stats.probes;
  stat.entries = raw_stats.hentries;
  stat.misses = raw_stats.misses;
  stat.deletions = raw_stats.hdeleted;

  return stat;
}
