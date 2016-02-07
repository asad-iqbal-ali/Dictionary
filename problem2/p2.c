#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<time.h>
#include "rbtree.h"


/*error codes:

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
  8 = performance test
 
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

 else if(!strcmp(c1, "exit"))
 {
  return 5;
 }
 else if(!strcmp(c1, "test"))
 {
  if(k)
   return 8;
 }
 
 return -4;

}

 
int vstrcmp(const void *key, const void *key2)
{
 return (strcmp((char *) key, (char *) key2));
}

void find2(RBtree *tree, RBnode *node, void *k1, void *k2)
{
 if(node->key == NULL)
  return;

 find2(tree, node->left, k1, k2);
 if((tree->compr(node->key, k1) > -1) && (tree->compr(node->key, k2) < 1))
  tree->tostr((const void *)node->key, (const void *)node->data);
 find2(tree, node->right, k1, k2);
 return;
}

void printtolog(FILE *fp, RBtree *tree)
{
 fprintf(fp, "%d\n", tree_height(tree->root));
}


int readfile(RBtree *tree, char *file, FILE *wfile)
{
 FILE *fp = fopen(file, "r");
 size_t size = sizeof(char)*500;
 char *word = malloc(sizeof(char)*50);
 char *def = malloc(sizeof(char)*500);
 int i, j;
 i = fscanf(fp, " %s ", word);
 j = getline(&def, &size, fp);   


 while((i != -1)&&(j != -1))
 {
  if(!insert_node(tree, word, def))
   printtolog(wfile, tree);
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

void node_destroy(void *key, void *data)
{
 free(key);
 free(data);
}

void node_print(const void *key, const void *data)
{
 printf("%s\n%s\n\n", (char *)key, (char *)data);
}

void ptest(RBtree *tree, char *file, FILE *wfile)
{
 struct timespec start, stop;
 clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
 if(readfile(tree, file, wfile))
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
 FILE *fp = fopen("treelog.txt", "w");
 RBtree *tree = malloc(sizeof(RBtree));
 init_tree(tree, vstrcmp, node_destroy, node_print);
 fprintf(fp, "Tree Height\n");
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
    if(!insert_node(tree, command2, def))
    {
     printf("Added %s to dictionary\n", command2);
     printtolog(fp, tree);
    }
    else printf("Error - %s not added to dictionary\n", command2);
    break;
   case 2:
    free(def);
    if(!delete_node(tree, command2))
    {
     printf("Deleted %s from dictionary\n", command2);
     printtolog(fp, tree);
    }
    else printf("Error - %s not found\n", command2);
    free(command2);
    break;
   case 3:
    free(def);
    if(!find_node(tree, command2, &data))
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
    print_tree(tree, tree->root);
    break;
   case 5:
    free(command1);
    free(command2);
    free(def);
    destroy_tree(tree, tree->root);
    free(tree);
    fclose(fp);
    return 0;
    break; 
   case 6:
    if(!readfile(tree, command2, fp))
     printf("File scanned to dictionary\n");
    else printf("Error - file not scanned\n");
    free(command2);
    break;
   case 7:
    find2(tree, tree->root, command2, def);
    free(command2);
    free(def);
    break;
   case 8:
    ptest(tree, command2, fp);
    free(command2);
    break;
   }
 
 free(command1);
 }
}
