/* file should contain all of the page table functions */

#include <webshare.h>
#include "hash.h"

#ifndef _WSPAGETABLE_H_
#define _WSPAGETABLE_H_

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

typedef struct webpage_table_stats_s webpage_table_stats;
struct webpage_table_stats_s {
  unsigned long probes;
  unsigned long misses;
  unsigned long entries;
  unsigned long deletions;
};


/* HASH TABLE FUNCTIONS */

/** 
 * Compare two webpage post items - compares by the key. 
 * 
 * @param li left item
 * @param ri right item
 * 
 * @return 0 if they are the same, something else if they are not
 */
static int webpage_table_item_compare( void *li, void *ri);

/** 
 * Creates a deep copy of a webpage_table_item
 * 
 * @param it item to create a copy of
 * 
 * @return the address of the copy
 */
static void *webpage_table_item_dup(void *it);

/** 
 * Frees the webpage_table_item located at it
 * 
 * @param it webpage_table_item to free
 */
static void webpage_table_item_free(void *it);

/** 
 * Creates an unsigned long from a webpage_table_item key.
 * 
 * @param it the webpage_table_item
 * 
 * @return the unsigned long hash
 */
static unsigned long webpage_table_item_hash(void *it);


/** 
 * A secondary hashing algorithm for webpage_table_item keys
 * 
 * @param it the webpage_table_item
 * 
 * @return the unsigned long hash
 */
static unsigned long webpage_table_item_rehash(void *it);

/** 
 * Initializes the webpage_table hash table
 * 
 * 
 * @return 0 on success, other on failure
 */
int webpage_table_init(hshtbl **webpage_table);

/** 
 * Free the webpage table 
 * 
 * @param webpage_table table to free
 */
static void webpage_table_free(hshtbl *webpage_table);

/** 
 * Adds an item at path to the webpage_table.
 * 
 * @param ws the webshare service to post the item too
 * @param posttype the type of the post, should be a value from the posttype enum
 * @param path the path for the item to be served from
 * @param pi the item to post
 * 
 * @return 0 on success, 1 on failure
 */
static int webshare_post_postitem(WEBSHARE *ws, int posttype, char *path, postitem *pi);

/** 
 * Adds a function to the webpage_table
 * 
 * @param ws the webshare service to post the item too
 * @param path the path that the function resolves to
 * @param callback the function itself
 * 
 * @return 0 on success, other on failure
 */
int webshare_post_function(WEBSHARE *ws, char *path, void*(*callback)(void *data));

/** 
 * Adds a file to the the webpage_table. Note that this does not ever
 * look to see if the file exists
 * 
 * @param ws the webshare service to post the item too
 * @param path the path to resolve to a file
 * @param file_path the path to file file
 * 
 * @return 0 on success, other on failure
 */
int webshare_post_file(WEBSHARE *ws, char *path, char *file_path);

/** 
 * This one is a little more special. It adds a buffer to the
 * webpage_table for sharing. It does a deep copy on the buffer, so it
 * is OK to free after calling
 * 
 * @param ws the webshare service to post the item too
 * @param path the path that should resolve to a buffer
 * @param buffer the buffer to serve - it does get copied, so it is OK to free
 * 
 * @return 0 on success, other on error
 */
int webshare_post_buffer(WEBSHARE *ws, char *path, char *buffer);

/** 
 * This function posts a whole directory under a path prefix. If you
 * were posting a folder called myfolder containing file1 and file2,
 * and passed in the path prefix /prefix then file1 would be
 * available at /prefix/file1, and file2 at /prefix/file2
 * 
 * @param ws the webshare service to post the item too
 * @param path_prefix path prefix of hosted files
 * @param dir_path the path to the directory to share
 * 
 * @return 0 on success and 1 on failure
 */
int webshare_post_directory(WEBSHARE *ws, char *path_prefix, char *dir_path);

/** 
 * Searches for a webpage_table_item that has a matching path.
 * 
 * @param webpage_table the table to search
 * @param path path to match against
 * 
 * @return a pointer to the matching webpage_table_item, or NULL if one does not exist.
 */
webpage_table_item *webpage_table_find(hshtbl *webpage_table, char *path);

/** 
 * Drops the postitem from the webpage_table, safe to call if not sure
 * an item is in there. Frees the memory that was being used.
 * 
 * @param webpage_table the table to drop from
 * @param path path to drop
 */
static void webpage_table_drop(hshtbl *webpage_table, char *path);

/** 
 * Creates a structure containing the statistical information for the
 * webpage_table currently. This contains probes, misses, entries, and
 * deletions
 * 
 * @return current webpage table stats
 */
webpage_table_stats webpage_table_status();

void webshare_pagetable_free(WEBSHARE *ws);

#endif