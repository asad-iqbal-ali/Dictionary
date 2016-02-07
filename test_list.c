#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include "list.h"

char* int2str(void *key, void *data);   
int   int_destroy(void *key); 


int main(int argc, char **argv){
  ListElmt *node;
  int i;
  int x[] = {1,5,3,4,0,-2,100};   /* create a buffer of ints */
  char y[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
  int tmp;
  char ytmp;
  int *xp = &tmp;
  char *yp = &ytmp;
  char *otherKey = "test";
  int otherVal = -121;
  int n = sizeof(x)/sizeof(int);

  /* create the list. would be nice if list_init did this */
  List *list = (List *) malloc(sizeof(List)); 
  /* pass list to list_init with destroy and tostring methods */
  list_init(list,int_destroy,NULL,int2str);
  /* insert at head of list */
  list_ins_next(list,NULL,&y[0],&x[0]);
  list_print(list);
  /* let's do a few more */
  node = list_head(list);
  for (i=1;i<n;++i){
    list_ins_next(list,node,&y[i],&x[i]);
    list_print(list);
    node = list_next(node);
  }
  /* now what if we want to, say, insert after the 3rd element of the list? */
  node = list->head;

  /* unfortunately we have to traverse the list from the head until we get the 3rd element */
  /* next we add this capability to the list class itself via a new higher level method */
  for (i=0;i<3;++i){
    node = list_next(node);
  }
  list_ins_next(list,node,otherKey,&otherVal);
  list_print(list);

  /* now let's remove some elements. let's start with head */
  list_rem_next(list,NULL,(const void **) &yp,(const void **) &xp);
  list_print(list);
  list_rem_next(list,NULL,(const void **) &yp,(const void **) &xp);
  list_print(list);
  printf("%d\n", *xp);

  return 0;
}


char * int2str(void *key, void *data){
  static char str[100];       /* a non-ephemeral data buffer. don't overuse this */
  int val = *( (int *)data);
  printf("%c - ", *((char *)key));
  sprintf(str, "%d,", val);   /* converts int to char * and stores in str */
  return str;
}

int int_destroy( void *data){
  return 0;
}
