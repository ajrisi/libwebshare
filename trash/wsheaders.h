/**
 * @file   wsheaders.h
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Mon Feb 16 10:40:22 2009
 * 
 * @brief Manages HTTP headers, creation of outgoing, parsing of
 * incoming
 * 
 * 
 */

#ifndef _WSHEADERS_H_
#define _WSHEADERS_H_

#include "../hash.h"

typedef hshtbl field_table;

enum {
  MOPTIONS, MGET, MHEAD, MPOST,
  MPUT, MDELETE, MTRACE, MCONNECT,
  MUNKNOWN
};

char* wshttp_methods[] = {
  "OPTIONS", "GET", "HEAD", "POST",	
  "PUT", "DELETE", "TRACE", "CONNECT"
};

typedef struct webshare_http_headers_s webshare_http_headers;

struct webshare_http_headers_s {
  int method;
  char path[1024];
  char http_version[16];
  field_table *ft;
};

typedef struct field_table_item_s field_table_item;

struct field_table_item_s {
  char *field;
  char *value;
  int priority;
};

struct field_walk_callback_datum_s {
  field_table_item **vec;
  unsigned int index;
};

/* field table functions, for handling the http fields */

/** 
 * Compares two field_table_items by their field value
 * 
 * @param li left item
 * @param ri right item
 * 
 * @return 0 on equal, other values on non-equal
 */
int field_table_item_compare(void *li, void *ri);

/** 
 * Duplicates a field_table_item
 * 
 * @param it the item to duplicate
 * 
 * @return a duplicate of it
 */
void *field_table_item_dup(void *it);

/** 
 * Frees a field_table_item located in memory
 * 
 * @param it the item to free
 */
void field_table_item_free(void *it);

/** 
 * Generates the hash of the field name contained in 'it'. Uses SDBM
 * 
 * @param it a field_table_item pointer
 * 
 * @return the hash pf the field name
 */
unsigned long field_table_item_hash(void *it);

/** 
 * Generates the hash of the field name contained in 'it'. Uses RS
 * 
 * @param it a field_table_item pointer
 * 
 * @return the hash of the field name
 */
unsigned long field_table_item_rehash(void *it);

/** 
 * Initializes a field_table located at *table
 * 
 * @param table the address of the pointer to a field_table to init
 * 
 * @return 0 on success, other on failure
 */
int field_table_init(field_table **table);

/** 
 * Frees all of the memory being stored inside of a field table
 * 
 * @param table the table to free
 */
void field_table_free(field_table *table);

/** 
 * Sets a value of a field withing a field table. Will overwrite without complaining
 * 
 * @param table the field table to use
 * @param field the field
 * @param value the fields new value
 * @param priority the priority of the field - this determines its 
 *        location in the output vector. Low values come first
 * 
 * @return 0 on success, other on failure 
 */
int field_table_set(field_table *table, char *field, char *value, unsigned int priority);

/** 
 * Finds a value for a given field
 * 
 * @param table the table to look in
 * @param field the field to find the matching value of
 * 
 * @return either the value of the field, or NULL if it is not set
 */
char *field_table_find(field_table *table, char *field);

/** 
 * Removes an entry from the field table. This should be used instead
 * of setting the value to nothing, as it removes the entry entirely
 * 
 * @param table the table
 * @param field the field to remove
 */
void field_table_remove(field_table *table, char *field);

/** 
 * Generates a char ** vector from a field table. This vector is
 * sorted by the priorities given for each field. Vector is null
 * terminated
 * 
 * @param table the table to generate the vector of
 * 
 * @return a vector of all of the entries in the field table
 */
char **field_table_make_vector(field_table *table);

/** 
 * Called by the hshwalk, this function is private and should not be
 * called external to the library
 * 
 * @param it 
 * @param dat 
 * @param xtra 
 * 
 * @return 
 */
int field_table_make_vector_callback(void *it, void *dat, void *xtra);

/** 
 * Sets the priority of a field within a table
 * 
 * @param table the field table
 * @param field the field to change the priority of
 * @param priority the new priority of that field
 * 
 * @return 0 on success, other on failure
 */
int field_table_item_set_priority(field_table *table, char *field, unsigned int priority);

/** 
 * Called by qsort, this function is private and should not be called
 * external to the library
 * 
 * @param li 
 * @param ri 
 * 
 * @return 
 */
int field_table_item_priority_compare(const void *li, const void *ri);

/** 
 * Parses and inserts a line into the field table. Will automatically
 * set the priority of the item to the size of the table before this
 * entry was added
 * 
 * @param table the table to add to
 * @param line the full line to parse and add (an example of a line would be "Content-type: text/plain")
 * 
 * @return 0 on success, other on failure
 */
int field_table_insert(field_table *table, char *line);

#endif /* _WSHEADERS_H_ */
