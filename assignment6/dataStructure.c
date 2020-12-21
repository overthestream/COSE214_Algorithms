#include "dataStructure.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define INSERT_COST 1
#define DELETE_COST 1
#define SUBSTITUTE_COST 1
#define TRANSPOSE_COST 1

HEAP *heapCreate()
{
    HEAP *newHeap = (HEAP *)malloc(sizeof(HEAP));
    newHeap->data = (DATA **)malloc(sizeof(DATA *) * capacity);
    newHeap->size = 0;

    return newHeap;
}

DATA *dataCreate(char *str, int weight)
{
    DATA *newData = (DATA *)malloc(sizeof(DATA));
    newData->str = strdup(str);
    newData->weight = weight;
    return newData;
}

void heapDestroy(HEAP *heap)
{
    for (int i = 0; i < heap->size; ++i)
    {
        free(heap->data[i]->str);
        free(heap->data[i]);
    }
    free(heap->data);
    free(heap);
}

void heapInsert(char *str, int weight, HEAP *heap)
{
    if (heap->size == capacity)
    {
        if (heap->data[heap->size - 2]->weight < heap->data[heap->size - 1]->weight)
        {
            DATA *temp = heap->data[heap->size - 1];
            heap->data[heap->size - 1] = heap->data[heap->size - 2];
            heap->data[heap->size - 2] = temp;
        }
        free(heap->data[heap->size - 1]->str);
        free(heap->data[--heap->size]);
    }
    heap->data[heap->size] = dataCreate(str, weight);
    reHeapUp(heap, heap->size++);
}

void heapDelete(HEAP *heap)
{
    if (heap->size == 0)
        return;

    DATA *delPtr = heap->data[0];
    heap->data[0] = heap->data[--heap->size];

    free(delPtr->str);
    free(delPtr);
    reHeapDown(heap, 0);
}

void reHeapUp(HEAP *heap, int pos)
{
    if (pos <= 0 || pos >= heap->size)
        return;

    int parent = (pos - 1) / 2;

    if (heap->data[pos]->weight >= heap->data[parent]->weight)
    {
        DATA *temp = heap->data[pos];
        heap->data[pos] = heap->data[parent];
        heap->data[parent] = temp;
        reHeapUp(heap, parent);
    }
}
void reHeapDown(HEAP *heap, int pos)
{
    if (pos < 0 || pos >= heap->size)
        return;

    if (pos * 2 + 1 < heap->size)
    {
        int maxChild = pos * 2 + 1;
        if (maxChild + 1 < heap->size && heap->data[maxChild + 1]->weight > heap->data[maxChild]->weight)
            ++maxChild;
        if (heap->data[maxChild]->weight >= heap->data[pos]->weight)
        {
            DATA *temp = heap->data[pos];
            heap->data[pos] = heap->data[maxChild];
            heap->data[maxChild] = temp;
            reHeapDown(heap, maxChild);
        }
    }
}

/***************************************/

TRIE *trieCreate()
{
    TRIE *newTrie = (TRIE *)malloc(sizeof(TRIE));
    for (int i = 0; i < 26; ++i)
        newTrie->root[i] = NULL;

    return newTrie;
}

static TRIE_NODE *__newNode()
{
    TRIE_NODE *newNode = (TRIE_NODE *)malloc(sizeof(TRIE_NODE));
    newNode->isTail = false;
    newNode->weight = 0;
    for (int i = 0; i < 26; ++i)
        newNode->child[i] = NULL;

    return newNode;
}

void trieInsert(TRIE *trie, char *str)
{
    int len = strlen(str);
    TRIE_NODE *pCur = trie->root[str[0] - 'a'];

    if (pCur == NULL)
    {
        pCur = trie->root[str[0] - 'a'] = __newNode();
    }

    for (int i = 1; i < len; ++i)
    {
        if (!pCur->child[str[i] - 'a'])
        {
            pCur->child[str[i] - 'a'] = __newNode();
        }
        pCur = pCur->child[str[i] - 'a'];
    }

    pCur->isTail = true;
    pCur->weight++;
}

void trieDestroy(TRIE *trie)
{
    for (int i = 0; i < 26; ++i)
    {
        if (trie->root[i])
            __destroy(trie->root[i]);
    }
    free(trie);
}

static void __destroy(TRIE_NODE *node)
{
    for (int i = 0; i < 26; ++i)
    {
        if (node->child[i])
            __destroy(node->child[i]);
    }
    free(node);
}

void trieTraverse(TRIE *trie, HEAP *heap)
{
    char str[100];
    for (int i = 0; i < 26; ++i)
    {
        if (trie->root[i])
        {
            str[0] = 'a' + i;
            __traverse(trie->root[i], 0, str, heap);
        }
    }
}

static void __traverse(TRIE_NODE *node, int level, char *str, HEAP *heap)
{
    if (node->isTail)
    {
        str[level + 1] = 0;

        heapInsert(str, node->weight, heap);
    }

    for (int i = 0; i < 26; ++i)
    {
        if (node->child[i])
        {
            str[level + 1] = 'a' + i;
            __traverse(node->child[i], level + 1, str, heap);
        }
    }
}

static int __GetMin3(int a, int b, int c)
{
    int min = a;
    if (b < min)
        min = b;
    if (c < min)
        min = c;
    return min;
}
static int __GetMin4(int a, int b, int c, int d)
{
    int min = __GetMin3(a, b, c);
    return (min > d) ? d : min;
}
int min_editdistance(char *str1, char *str2)
{
    int n = strlen(str1);
    int m = strlen(str2);
    int dp[n + 1][m + 1];
    for (int i = 0; i <= n; ++i)
    {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; ++j)
    {
        dp[0][j] = j;
    }
    int cost;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            cost = str1[i - 1] == str2[j - 1] ? 0 : SUBSTITUTE_COST;
            if ((i > 1 && j > 1) && (str1[i - 2] == str2[j - 1] && str1[i - 1] == str2[j - 2])){
                dp[i][j] = __GetMin4(dp[i - 1][j] + DELETE_COST, dp[i][j - 1] + INSERT_COST, dp[i - 1][j - 1] + cost, dp[i - 2][j - 2] + TRANSPOSE_COST);
            }
            else{
                dp[i][j] = __GetMin3(dp[i - 1][j] + DELETE_COST, dp[i][j - 1] + INSERT_COST, dp[i - 1][j - 1] + cost);
            }
        }
    }
    return dp[n][m];
}
