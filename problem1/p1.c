#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<time.h>
#include "chtbl.h"

//global variables for switching hashing function on each rehash
unsigned long  g1(const void *k);
unsigned long  g2(const void *k);
unsigned long  g3(const void *k);

int func_count = 0;
int *fcptr = &func_count;
unsigned long (*f[3])(const void *k) = {g1, g2, g3};

//two functions for lists within the hash table

int list_dest_elm( void *key, void *data)
{
 free(key);
 free(data);
 return 0;
}

char* list_print_elm(void *key, void *data)
{
 printf("%s\n%s\n", (char *)key, (char *)data);
 return key;
}

//function to read lines of input for commands and parse into strings


/*return codes:

  -1 = addword error
  -2 = delete error
  -3 = find error
  -4 = command not recognized
  -5 = read error

  1 = add
  2 = delete
  3 = find
  4 = print
  5 = exit
  6 = read
  7 = find2
 
*/

int parseline( char* c1, char* c2, char* def)
{

 size_t i = 1000;
 int j, k, l, m;
 int end = 0;
 char c;
 j = k = l = m = 0;
 
 if(j = fscanf(stdin, " %s", c1))
 {
  while(isspace(c = getchar()))
   {
    if(c == '\n')
     {
      end = 1;
      break;
     }
   }
  m = ungetc(c, stdin);
  if(!end && (k = fscanf(stdin, " %s", c2)))
  { 
   while(isspace(c = getchar()))
   {
    if(c == '\n')
     {
      end = 1;
      break;
     }
   }
   if(!end)
   {
    m = ungetc(c, stdin);
    l = getline(&def, &i, stdin);
    def[l-1] = '\0';
   }
  }
 }


 if(!strcmp(c1, "add"))
 {
  if(k&&l)
  {
    return 1;
  }
  else return -1;
 } 

 else if(!strcmp(c1, "find"))
 {
  if(k)
  {
   if(l)
    return 7;
   else
    return 3;   
  }
  else return -3;
 } 
 

 else if(!strcmp(c1, "delete"))
 {
  if(k)
  {
   return 2;   
  }
  else return -2;
 } 

 
 else if(!strcmp(c1, "print"))
 {
  return 4;
 }
 

 else if(!strcmp(c1, "read"))
 {
  if(k)
  {
   return 6;   
  }
  else return -5;
 } 
 else if (!strcmp(c1, "test"))
 {
  if(k)
  {
   return 8;
  }
 }

 else if(!strcmp(c1, "exit"))
 {
  return 5;
 }
 
 return -4;

}

//prehashing algorithms

//naive ascii character value-adding prehash algortihm
unsigned long g1(const void *key)
{
 char *nkey = (char *)key;
 int i;
 unsigned long sum = 0;
 for(i = 0; nkey[i] != '\0'; ++i)
  sum += (unsigned long)nkey[i];

 return sum;
}

//djb2 prehashing algorithm taken from http://www.cse.yorku.ca/~oz/hash.html
unsigned long g2(const void *key)
{
 unsigned long hash = 5381;
 int c;

 while(c = *(char *)key++)
  hash = ((hash << 5) + hash) + c;

 return hash;
}

//prehashing taken from lecture in class
unsigned long g3(const void *key)
{
 unsigned long sum = 0;
 unsigned long  B = 1;
 int i;

 while(i = *(char *)key++)
 {
  sum += (i*B);
  B << 8;
 }
 return sum;
}

 
//comparison algorithm for the hash table
int vstrcmp(const void *key, const void *key2)
{
 return (strcmp((char *) key, (char *) key2));
}

//function to remove an item with the key "key" from the hash table
int htbl_remove(CHTbl *htbl, void *key)
{
 List *clist;
 ListElmt *element, *parent = NULL;
 int bucket;
 void *nkey, *data;

 bucket = htbl->h(key)%htbl->buckets;
 clist = &htbl->table[bucket];
 for(element = list_head(clist); element != NULL; element = list_next(element))
 {
  if(!htbl->match(key, list_key(element)))
  {
   list_rem_next(clist, parent, (const void **) &nkey, (const void **)&data);
   clist->destroy(nkey, data);
   htbl->size--;
   return 0;
  }
  parent = element;
 }

 return -1;
}

//for use with qsort to compare *ListElmts
int elemcompare(const void *a, const void *b)
{
 return strcmp((char *)(((*(ListElmt **)a))->key),(char *)(((*(ListElmt **)b))->key));
}

//print the table in alphabetical order
void print_table(CHTbl *htbl)
{
 ListElmt **fulllist = malloc(sizeof(ListElmt)*(htbl->size));
 ListElmt *element;
 int i = 0;
 int j = 0;

 for(i = 0; i < htbl->buckets; ++i)
 {
  for(element = list_head(&htbl->table[i]); element != NULL; element = list_next(element))
   fulllist[j++] = element;
 }
 
 qsort(fulllist, htbl->size, sizeof(ListElmt*), elemcompare);
 
 for(j = 0; j < htbl->size; ++j)
  printf("%s\n%s\n\n", (char *)fulllist[j]->key, (char *)fulllist[j]->data);

 free(fulllist);  
}


//find and print all items between k1 and k2, inclusive
void find2(CHTbl *htbl, void *k1, void *k2)
{
 ListElmt **fulllist = malloc(sizeof(ListElmt)*(htbl->size));
 ListElmt *element;
 int i = 0;
 int j = 0;

 for(i = 0; i < htbl->buckets; ++i)
 {
  for(element = list_head(&htbl->table[i]); element != NULL; element = list_next(element))
  {
   if((htbl->match(list_key(element), k1) > -1) && htbl->match(list_key(element), k2) < 1)
    fulllist[j++] = element;
  }
 }
 
 qsort(fulllist, j, sizeof(ListElmt*), elemcompare);
 
 for(i = 0; i < j; ++i)
  printf("%s\n%s\n\n", (char *)fulllist[i]->key, (char *)fulllist[i]->data);

 free(fulllist);  
}

//frees up all memory from the table
void destroy_table(CHTbl *htbl)
{
 int i;
 ListElmt *element, *parent;
 List *clist;
 void *key, *data;
 for(i = 0; i < htbl->buckets; ++i)
 {
  clist = &htbl->table[i];
  parent = NULL;
  for(element = list_head(clist); element != NULL; element = list_next(element))
  {
   list_rem_next(clist, parent, (const void **) &key, (const void **)&data);
   clist->destroy(key, data);
   htbl->size--;
   parent = element;
  }
 }
}


//prints load factor and table size to fp
void printtolog(FILE *fp, CHTbl *htbl)
{
 fprintf(fp, "%.2f\t\t%d\n", (float)(htbl->size)/(float)(htbl->buckets), htbl->size);
}

//read words and definitions from a file and add them to the table
int readfile(CHTbl *htbl, char *file, FILE *wfile)
{
 FILE *fp; 
 if((fp = fopen(file, "r")) == NULL)
  return -1;
 size_t size = sizeof(char)*500;
 char *word = malloc(sizeof(char)*50);
 char *def = malloc(sizeof(char)*500);
 int i, j;
 i = fscanf(fp, " %s ", word);
 j = getline(&def, &size, fp);   


 while((i != -1)&&(j != -1))
 {
  if(!chtbl_insert(&htbl, word, def))
   printtolog(wfile, htbl);
  if((word = malloc(sizeof(char)*50))==NULL)
  {
   fclose(fp);
   return -1;
  }
  if((def = malloc(sizeof(char)*500))==NULL)
  {
   fclose(fp);
   return -1;
  }
  i = fscanf(fp, " %s ", word);
  j = getline(&def, &size, fp); 
 }

 free(word);
 free(def);
 fclose(fp);

 return 0;  
}

//rehashing function
int rehash(CHTbl *htbl)
{
 void *key;
 void *data;
 int i, j, oldbuckets, size;
 
 oldbuckets = htbl->buckets;
 htbl->buckets *= 2;
 for(i = oldbuckets; i < htbl->buckets; ++i)
  list_init(&htbl->table[i], list_dest_elm, NULL, list_print_elm);

 htbl->h = f[((*fcptr)++)%3];

 for(i = 0; i < oldbuckets; ++i)
 {
  size = (&(htbl->table[i]))->size;
  for(j = 0; j < size; ++j)
  {
   list_rem_next(&(htbl->table[i]), NULL, (const void**) &key, (const void**) &data);
   htbl->size--;
   chtbl_insert(&htbl, key, data);
  }
 }
 return 0;
}

void ptest(CHTbl *htbl, char *file, FILE *wfile)
{
 struct timespec start, stop;
 clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
 if(readfile(htbl, file, wfile))
 {
  printf("file not found\n");
  return;
 }
 clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

 printf("Time to read: %ld nsecs\n", stop.tv_nsec-start.tv_nsec);

}

int main()
{
 void *data;
 char *command1, *command2, *def;
 int result;
 FILE *fp = fopen("hashlog.txt", "w");
 CHTbl *htbl = malloc(sizeof(CHTbl));
 chtbl_init(htbl, 4, g3, vstrcmp, NULL);
 fprintf(fp, "Load factor\tOccupancy\n", (float)(htbl->size/htbl->buckets), htbl->buckets);
 while(1)
 {
  printf(">");
  command1 = malloc(sizeof(char)*10);
  command2 = malloc(sizeof(char)*50);
  def = malloc(sizeof(char)*1000);
  result = (parseline(command1, command2, def));


  switch(result){
   case -1:
    printf("Error: invalid use of 'add'\n");
    free(command2);
    free(def);
    break;
   case -2:
    printf("Error: invalid use of 'delete'\n");
    free(command2);
    free(def);
    break;
   case -3:
    printf("Error: invalid use of 'find'\n");
    free(command2);
    free(def);
    break;
   case -4:
    printf("Error: command not recognized\n");
    free(command2);
    free(def);
    break;
   case -5:
    printf("Error: no filename given");
    free(command2);
    free(def);
   case 1:
    if(!(chtbl_insert(&htbl, command2, def)))
    {
     printf("Added %s to dictionary\n", command2);
     printtolog(fp, htbl);
    }
    else printf("Error - %s not added to dictionary\n", command2);
    break;
   case 2:
    free(def);
    if(!htbl_remove(htbl, command2))
    {
     printf("Deleted %s from dictionary\n", command2);
     printtolog(fp, htbl);
    }
    else printf("Error - %s not found\n", command2);
    free(command2);
    break;
   case 3:
    free(def);
    if(!chtbl_lookup(htbl, command2, &data))
    {
     printf("%s\n", command2);
     printf("%s\n", (char *)data);
    }
    else printf("%s not found in dictionary\n", command2);
    free(command2);
    break;
   case 4:
    free(command2);
    free(def);
    print_table(htbl);
    break;
   case 5:
    free(command1);
    free(command2);
    free(def);
    destroy_table(htbl);
    free(htbl);
    fclose(fp);
    return 0;
    break; 
   case 6:
    if(!readfile(htbl, command2, fp))
     printf("File scanned to dictionary\n");
    else printf("Error - file not scanned\n");
    free(command2);
    break;
   case 7:
    find2(htbl, command2, def);
    free(command2);
    free(def);
    break;
   case 8:
    ptest(htbl, command2, fp);
    break;
   }
 
 free(command1);
 }
}
