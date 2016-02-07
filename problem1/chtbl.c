#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include "chtbl.h"

int list_dest_elm(void *key, void *data);
char* list_print_elm(void *key, void *data);
int vstrcmp(void *key1, void *key2);

int chtbl_init(CHTbl *htbl, int buckets, unsigned long (*h)(const void *key), int (*match)(const void *key1, const void *key2), void (*destroy)(void *key))
{
 int i;
 
 if((htbl->table = (List *)malloc(buckets*sizeof(List))) == NULL)
  return -1;

 htbl->buckets = buckets;

 for(i = 0; i < htbl->buckets; ++i)
  list_init(&htbl->table[i], list_dest_elm, NULL, list_print_elm);
 
 htbl->h = h;
 htbl->destroy = destroy;
 htbl->match = match;
 htbl->size = 0;
 return 0;
}

int chtbl_insert(CHTbl **htbl, const void *key, const void *data)
{

 int error_code;
 int bucket;
 
 if(chtbl_lookup(*htbl, (void *)key, (void **) &data) == 0)
  return 1;

 bucket = (*htbl)->h(key) % (*htbl)->buckets;
 if((error_code = list_append(&((*htbl)->table[bucket]), (const void*)key, (const void*)data)) == 0)
  (*htbl)->size++;
 if((((float)(*htbl)->size)/((float)(*htbl)->buckets)) > 1)
 {
  if(((*htbl)->table = realloc((*htbl)->table, sizeof(List)*((*htbl)->buckets)*2)) == NULL)
   return -1; 
  error_code = rehash(*htbl);
 }
     

 return error_code;
}

int chtbl_lookup(CHTbl *htbl, void *key, void **data)
{
 ListElmt *element;
 int bucket;

 bucket = htbl->h(key)%htbl->buckets;
 for(element = list_head(&htbl->table[bucket]); element != NULL; element = list_next(element))
 {
  if(!htbl->match(key, list_key(element)))
  {
   *data = list_data(element);
   return 0;
  }
 }

 return -1;
}

/* 
int vstrcmp(void *key, void *key2)
{
 return (strcmp((char *) key, (char *) key2));
}
*/
