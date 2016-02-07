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

int insert_node(RBtree *tree, const void *key, const void *data);
int delete_node(RBtree *tree, const void *key);
int find_node(RBtree *root, const void *key, void **data);
void init_tree(RBtree *tree, int (*compr)(const void *key1, const void *key2), void (*destroy)(void *key, void *data), void (*tostr)(const void *key, const void *data));
void print_tree(RBtree *tree, RBnode *node);
void destroy_tree(RBtree *tree, RBnode *n);
void left_rotate(RBnode *node);
void right_rotate(RBnode *node);
void insert_case1(RBnode *n);
void insert_case2(RBnode *n);
void insert_case3(RBnode *n);
void insert_case4(RBnode *n);
void insert_case5(RBnode *n);
void delete_case1(RBnode *n);
void delete_case2(RBnode *n);
void delete_case3(RBnode *n);
int tree_height(RBnode *n);
RBnode* uncle(RBnode *n);
