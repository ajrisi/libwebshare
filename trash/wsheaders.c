/**
 * @file   wsheaders.c
 * @author adam <adam@chronos.rh.rit.edu>
 * @date   Mon Feb 16 10:39:44 2009
 * 
 * @brief Parses and tracks the HTTP header information. Allows for
 * creation of outgoing and parsing of incoming
 * 
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../hash.h"
#include "wsheaders.h"

int field_table_item_compare(void *li, void *ri)
{
  field_table_item *left = li;
  field_table_item *right = ri;
  return strcmp(left->field, right->field);
}

void *field_table_item_dup(void *it)
{
  field_table_item *old_item = it;
  field_table_item *new_item = malloc(sizeof(field_table_item));
  if(new_item == NULL) return NULL;

  new_item->priority = old_item->priority;

  new_item->field = strdup(old_item->field);
  if(new_item->field == NULL) {
    free(new_item);
    return NULL;
  }

  new_item->value = strdup(old_item->value);
  if(new_item->value == NULL) {
    free(new_item->field);
    free(new_item);
    return NULL;
  }

  return new_item;
}

void field_table_item_free(void *it)
{
  field_table_item *item = it;
  free(item->field);
  free(item->value);
  free(item);
}

unsigned long field_table_item_hash(void *it)
{
  field_table_item *item = it;
  return hshstrhash_sdbm(item->field);
}

unsigned long field_table_item_rehash(void *it)
{
  field_table_item *item = it;
  return hshstrhash_rs(item->field);
}

int field_table_init(field_table **table)
{
  *table = hshinit(field_table_item_hash, field_table_item_rehash, 
		   field_table_item_compare, field_table_item_dup,
		   field_table_item_free, 0);
  return(*table == NULL);
}

void field_table_free(field_table *table)
{
  hshkill(table);
}

int field_table_set(field_table *table, char *field, char *value, unsigned int priority)
{
  /* use the insert function */
  field_table_item item = {0};
  item.field = field;
  item.value = value;
  item.priority = priority;

  return (hshinsert(table, &item) == NULL);
}

int field_table_insert(field_table *table, char *line)
{
  /* grab the stats of the field_table */
  hshstats stats = hshstatus(table);

  /* first, parse the line and generate a field_table_item */
  field_table_item toinsert = {0};
  /* set the priority to the size of the table. This is good to keep
   * the lines in order if reading in fields for later vector output
  */
  toinsert.priority = stats.hentries;
  
  /* allocate space for the parsed field and value */
  char field[256] = {0};
  char value[1024] = {0};

  if(sscanf(line, "%255s: %1023s") != 2) return 1;

  toinsert.field = field;
  toinsert.value = value;

  /* lastly, insert into the table */
  return (hshinsert(table, &toinsert) != NULL);
}

char *field_table_find(field_table *table, char *field)
{
  /* use the find function */
  field_table_item item = {0};
  field_table_item *found = 0;

  item.field = field;

  found = hshfind(table, &item);
  if(found == NULL) return NULL;
  
  return found->value;
}

void field_table_remove(field_table *table, char *field)
{
  field_table_item lookfor = {0};
  lookfor.field = field;

  field_table_item *found;
  found = hshdelete(table, &lookfor);

  if(found != NULL) {
    free(found);
  }

}

char **field_table_make_vector(field_table *table)
{
  hshstats stats = hshstatus(table);

  /* first, we need a copy of the table to sort*/
  field_table_item **cpy;

  cpy = malloc(sizeof(field_table_item) * stats.hentries);
  if(cpy == NULL) {
    return NULL;
  }
  memset(cpy, 0, (sizeof(field_table_item) * stats.hentries));

  /* walk the table and copy into this array */
  struct field_walk_callback_datum_s datum = {cpy, 0};
  if(hshwalk(table, field_table_make_vector_callback, &datum) != 0) {
    free(cpy);
    return NULL;
  }

  printf("there are %d entries\n", stats.hentries);

  /* now, sort this copy of the table by priority */
  qsort(cpy, stats.hentries, sizeof(field_table_item), field_table_item_priority_compare);

  /* once sorted, iterate over this list and generate the vector */
  char **vec;
  /* malloc the right size of the vector */
  vec = malloc(sizeof(char*) * (stats.hentries+1));
  if(vec == NULL) {
    free(cpy);
    return NULL;
  }

  int i = 0;
  for(i=0; i < stats.hentries; i++) {
    int line_length = strlen(cpy[i]->field) + strlen(": ") + strlen(cpy[i]->value) + 1;

    char *line = malloc(line_length);
    if(line == NULL) {
      /* free this list backward */
      int j = i;
      for(j=i; j >= 0; j--) {
	free(vec[j]);
      }
      free(vec);
      free(cpy);
      return NULL;
    }
    sprintf(line, "%s: %s", cpy[i]->field, cpy[i]->value);
    vec[i] = line;
  }
  vec[stats.hentries] = NULL;

  /* now, iterate over the copy and free all of its memory */
  for(i=0; i < stats.hentries; i++) {
    field_table_item_free(cpy[i]);
  }

  /* and free the copy itself */
  free(cpy);

  return vec;
}

int field_table_make_vector_callback(void *it, void *dat, void *xtra)
{
  field_table_item *item  = it;
  struct field_walk_callback_datum_s *datum = dat;
  return (datum->vec[datum->index++] = field_table_item_dup(it)) == NULL;

}

int field_table_item_set_priority(field_table *table, char *field, unsigned int priority)
{
  field_table_item *item;
  field_table_item findme = {0};
  findme.field = field;

  item = hshfind(table, &findme);
  
  if(item != NULL) {
    item->priority = priority;
  }

  return 0;
}

int field_table_item_priority_compare(const void *li, const void *ri) 
{
  const field_table_item *left = li;
  const field_table_item *right = ri;

  /* null is greater than everything, to keep it null terminated */
  if(left == NULL) return -1;
  if(right == NULL) return 1;

  /* low value to high value */
  return (left->priority < right->priority) - (left->priority > right->priority);
}


#ifdef WSHEADERSTEST

int main(int argc, char**argv) 
{
  field_table *fields;

  if(field_table_init(&fields) != 0) {
    fprintf(stderr, "Could not initialize the fields table");
    return 1;
  }

  field_table_set(fields, "Content-type", "text/html", 1);
  field_table_set(fields, "Host", "blah.com", 0);
  field_table_set(fields, "Content-length", "90", 1);

  /* now render a vector out of it all */
  char **vec = field_table_make_vector(fields);

  int i = 0;
  while(vec[i] != NULL) {
    printf("line is %s\n", vec[i]);
    free(vec[i]);
    i++;
  }
  free(vec);

  field_table_free(fields);
  
  return 0;
}

#endif
