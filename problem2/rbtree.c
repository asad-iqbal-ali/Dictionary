#include<stdlib.h>
#include<stdio.h>
#include "rbtree.h"
/* for reference
typedef struct RBNode_
{
 int color;
 void *key;
 void *data;
 struct RBNode_ *right;
 struct RBNode_ *left;
 struct RBNode_ *parent;
} RBnode;

typedef struct RBTree_ 
{
 RBnode *root;
 int size;

 int (*compr)(const void *key1, const void *key2);
 void (*destroy)(void *key, void *data);
 void (*tostr)(const void *key, const void *data);
} RBtree;
*/
int insert_node(RBtree *tree, const void *key, const void *data)
{
 RBnode *tmp, *cnode = tree->root, *cparent = NULL;
 tmp = malloc(sizeof(RBnode));
 void *d;
 if(!find_node(tree, key, &d))
  return -1;
 tmp->key = (void *)key;
 tmp->data = (void *)data;
 tmp->color = 0;
 tmp->left = malloc(sizeof(RBnode));
 tmp->right = malloc(sizeof(RBnode));
 tmp->right->key = tmp->left->key = NULL;
 tmp->right->data = tmp->left->data = NULL;
 tmp->right->right = tmp->left->right = NULL;
 tmp->right->left = tmp->left->left = NULL;
 tmp->right->color = tmp->left->color = 1;
 tmp->right->parent = tmp->left->parent = tmp;

 if(cnode == NULL)
 {
  tree->root = tmp;
  tmp->parent = NULL;
  tree->size++;
  insert_case1(tmp);
  return 0;
 }

 while(cnode->key != NULL)
 {
  cparent = cnode;
  if(tree->compr(key, cnode->key) < 0)
   cnode = cnode->left;
  else cnode = cnode->right;
 }
 
 if(tree->compr(key, cparent->key) < 0)
 {
  free(cparent->left);
  cparent->left = tmp;
 } 
 else
 {
  free(cparent->right);
  cparent->right = tmp;
 }
 tmp->parent = cparent;
 tree->size++;
 insert_case1(tmp);
 return 0; 
 
}

int delete_node(RBtree *tree, const void *key)
{
 RBnode *tmp, *n;
 int result;
 n = tree->root;
 if(n == NULL)
  return -1;

 while(n->key != NULL)
 {
  if((result = tree->compr(key, n->key)) == 0)
  {
   if((n->left->key != NULL) && (n->right->key != NULL))
   {
    tmp = n->left;
    while(tmp->right->key != NULL)
     tmp = tmp->right;
    n->key = tmp->key;
    n->data = tmp->data;
    n = tmp;
   }
   else if(n == tree->root)
   {
    if(n->left->key == NULL)
     tmp = n->right;
    else tmp = n->left;

    if(tmp->key != NULL)
    { 
     tmp->parent = NULL;
     tmp->color = 1;
    }
    else
    {
     free(tree->root->left);
     free(tmp);
     tmp = NULL;
    }
    tree->root = tmp;
    tree->size--;
    tree->destroy(n->key, n->data);
    free(n);
    return;    
   }
   if(n->left->key == NULL)
   {
    free(n->left);
    tmp = n->right;
   }
   else
   {
    tmp = n->left;
    if(n->right->key == NULL)
     free(n->right);
   }
   if(n == n->parent->left)
    n->parent->left = tmp;
   else n->parent->right = tmp;
   tmp->parent = n->parent;
   if(n->color == 1)
   {
    tmp->color = 2;
    delete_case1(tmp);
   }
   free(n);
   return 0;
  }
  else if(result < 0)
   n = n->left;
  else n = n->right;
 }

 return -1;
}

int find_node(RBtree *tree, const void *key, void **data)
{
 RBnode *cnode = tree->root;
 int result;
 if(cnode == NULL)
  return -1;
 while(cnode->key != NULL)
 {
  if((result = tree->compr(key, cnode->key)) == 0)
  {
   *data = cnode->data;
   return 0;
  }
  else if(result < 0)
   cnode = cnode->left;
  else cnode = cnode->right;
 }

 return -1;
}

void init_tree(RBtree *tree, int (*compr)(const void *key1, const void *key2), void (*destroy)(void *key, void *data), void (*tostr)(const void *key, const void *data))
{
 tree->compr = compr;
 tree->destroy = destroy;
 tree->tostr = tostr;
 tree->root = NULL;
 tree->size = 0;

}


void print_tree(RBtree *tree, RBnode *node)
{
 if((node == NULL)||(node->key == NULL))
  return;

 print_tree(tree, node->left);
 tree->tostr(node->key, node->data);
 print_tree(tree, node->right);
}

void destroy_tree(RBtree *tree, RBnode *node)
{
 if(node == NULL)
  return;

 destroy_tree(tree, node->left);
 destroy_tree(tree, node->right);
 tree->destroy(node->key, node->data);
 free(node);
}

void left_rotate(RBnode *n)
{
 RBnode *tmp_node;
 void *tmp_key, *tmp_data;
 int tmp_color;
 tmp_node = n->left;
 tmp_key = n->key;
 tmp_data = n->data;
 tmp_color = n->color;
 n->left = n->right;
 n->key = n->right->key;
 n->data = n->right->data;
 n->color = n->right->color;
 n->right = n->left->right;
 if(n->right != NULL)
  n->right->parent = n;
 n->left->right = n->left->left;
 if(n->left->right != NULL)
  n->left->right->parent = n->left;
 n->left->left = tmp_node;
 if(n->left->left !=NULL)
  n->left->left->parent = n->left;
 n->left->key = tmp_key;
 n->left->data = tmp_data;
 n->left->color = tmp_color;
}

void right_rotate(RBnode *n)
{
 RBnode *tmp_node;
 void *tmp_key, *tmp_data;
 int tmp_color;
 tmp_node = n->right;
 tmp_key = n->key;
 tmp_data = n->data;
 tmp_color = n->color;
 n->right = n->left;
 n->key = n->left->key;
 n->data = n->left->data;
 n->color = n->left->color;
 n->left = n->right->left;
 if(n->left != NULL)
  n->left->parent = n;
 n->right->left = n->right->right;
 if(n->right->left !=NULL)
  n->right->left->parent = n->right;
 n->right->right = tmp_node;
 if(n->right->right != NULL)
  n->right->right->parent = n->right;
 n->right->key = tmp_key;
 n->right->data = tmp_data;
 n->right->color = tmp_color;
}


void insert_case1(RBnode *n)
{
 if(n->parent == NULL)
  n->color = 1;
 else insert_case2(n);
}

void insert_case2(RBnode *n)
{
 if(n->parent->color == 1)
  return;
 else insert_case3(n);
}

void insert_case3(RBnode *n)
{
 RBnode *g, *u = uncle(n);

 if((u != NULL) && (u->color == 0))
 {
  n->parent->color = 1;
  u->color = 1;
  g = n->parent->parent;
  g->color = 0;
  insert_case1(g);
 }
 else insert_case4(n);
}

void insert_case4(RBnode *n)
{
 RBnode *g = n->parent->parent;
 RBnode *p = n->parent;
 if((n == n->parent->right) && (n->parent == g->left))
 {
  left_rotate(n->parent);
  n = g->left->left;
 }
 else if((n == n->parent->left) && (n->parent == g->right))
 {
  right_rotate(n->parent);
  n = g->right->right;
 }

 insert_case5(n);
}


void insert_case5(RBnode *n)
{
 RBnode *g = n->parent->parent;
 n->parent->color = 1;
 g->color = 0;
 if(n == n->parent->left)
  right_rotate(g);
 else left_rotate(g);
}

void delete_case1(RBnode *n)
{
 int nisleft = 0;
 RBnode *p = n->parent, *s, *neph;

 if(p == NULL)
 {
  n->color = 1;
  return;
 }


 if(n == p->left)
 {
  nisleft = 1;
  s = p->right;
 }
 else s = p->left;

 if(s->color == 0)
 {
  delete_case3(n);
  return;
 }

 if((s->left != NULL) && (s->left->color == 0))
 {
  neph = s->left;
  neph->color = 1;
  n->color = 1;
  if(nisleft)
  {
   right_rotate(s);
   left_rotate(p);
  }
  else
  {
   right_rotate(p);
  }
  return;
 }
 else if((s->right != NULL) && (s->right->color == 0))
 {
  neph = s->right;
  neph->color = 1;
  n->color = 1;
  if(!nisleft)
  {
   left_rotate(s);
   right_rotate(p);
  }
  else
  {
   left_rotate(p);
  }
  return;
 }
 else delete_case2(n);
 
}
void delete_case2(RBnode *n)
{
 RBnode *p = n->parent, *s;
 if(n == p->left)
  s = p->right;
 else s = p->left;
 
 if(p->color == 0)
 {
  n->color = p->color = 1;
  s->color = 0;
  return;
 } 
 else
 {
  n->color = 1;
  p->color = 2;
  s->color = 0;
  delete_case1(p);

 }
}

void delete_case3(RBnode *n)
{
 int nisleft = 0;
 RBnode *p = n->parent, *s;
 if(n == p->left)
 {
  nisleft = 1;
  s = p->right;
 }
 else s = p->left;
 p->color = 0;
 s->color = 1;
 if(nisleft)
 {
  left_rotate(p);
 }
 else 
 {
  right_rotate(p);
 }
 p = n->parent;
 s = p->parent;
 delete_case1(n);

}


int tree_height(RBnode *n)
{
 int left, right;
 if(n->key == NULL)
  return 0;

 left = tree_height(n->left);
 right = tree_height(n->right);
 if(left > right)
  return (left+1);
 else return (right+1);
 
}


RBnode* uncle(RBnode *n)
{
 RBnode *g;
 if((n->parent == NULL) || ((g = n->parent->parent) == NULL))
  return NULL;
 else if(n->parent == g->left)
  return g->right;
 else return g->left;
}




