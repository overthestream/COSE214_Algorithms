#define capacity 1000000

#define INSERT_COST 1
#define DELETE_COST 1
#define SUBSTITUTE_COST 1
#define TRANSPOSE_COST 1

#include <stdbool.h>

typedef struct
{
    char *str;
    int weight;
} DATA;

typedef struct heap
{
    DATA **data;
    int size;
} HEAP;

typedef struct node
{
    struct node *child[26]; // child 포인터 배열
    bool isTail;
    int weight;
} TRIE_NODE;

typedef struct
{
    TRIE_NODE *root[26];
} TRIE;

/********************************************/
HEAP *heapCreate();
DATA *dataCreate(char *str, int weight);
void heapDestroy(HEAP *heap);

void heapInsert(char *str, int weight, HEAP *heap);
void heapDelete(HEAP *heap);

void reHeapUp(HEAP *heap, int pos);
void reHeapDown(HEAP *heap, int pos);
/*******************************************/
TRIE *trieCreate();

static TRIE_NODE *__newNode();
void trieInsert(TRIE *trie, char *str);

void trieDestroy(TRIE *trie);
static void __destroy(TRIE_NODE *node);

void trieTraverse(TRIE *trie, HEAP *heap);
static void __traverse(TRIE_NODE *node, int level, char *str, HEAP *heap);
/******************************************/
int min_editdistance(char *str1, char *str2);
static int __GetMin3(int a, int b, int c);
static int __GetMin4(int a, int b, int c, int d);
