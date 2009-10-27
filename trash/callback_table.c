/**
 * @file   callback_table.c
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Tue Feb 10 18:57:44 2009
 * 
 * @brief  
 * 
 * 
 */

/* callback_table.c */

#include <glib.h>
#include "callback_table.h"

GHashTable *callback_table;

/** 
 * This sets up the callback table for URI to function
 * 
 */
void http_callback_table_init() {
  callback_table = g_hash_table_new(g_str_hash, g_str_equal);
}

int http_callback_table_insert(char *path, void*(*func)(void*)) {
  g_hash_table_insert(callback_table, path,  func);
}

void* http_callback_table_fetch(char *path) {
  return g_hash_table_lookup(callback_table, path);
}
