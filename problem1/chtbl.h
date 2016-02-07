#include "list.h"

typedef struct CHTbl_
{

 int buckets;
 unsigned long (*h)(const void *key);
 int (*match)(const void *key1, const void *key2);
 void (*destroy)(void *data);
 int size;
 List *table;
} CHTbl;

int chtbl_init(CHTbl *htbl, int buckets, unsigned long (*h)(const void *key), int (*match)(const void *key1, const void *key2), void (*destroy)(void *data));
void chtbl_destroy(CHTbl *htbl);
int chtbl_insert(CHTbl **htbl, const void *key, const void *data);
int chtbl_remove(CHTbl *htbl, void **key, void **data);
int chtbl_lookup(CHTbl *htbl, void *key, void **data);
#define chtbl_size(htbl) ((htbl)->size)
