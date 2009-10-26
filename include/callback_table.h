/* callback_table.h */
#include <glib.h>

#ifndef _CALLBACK_TABLE_H_
#define _CALLBACK_TABLE_H_

extern GHashTable *callback_table;

void http_callback_table_init();
int http_callback_table_insert(char *path, void*(*func)(void*));
void* http_callback_table_fetch(char *path);

#endif
