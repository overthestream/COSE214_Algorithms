#include <stdio.h>
#include <stdbool.h>

typedef struct node
{
	char* pData;          
	struct node* next;
} NODE;

typedef struct {
    int size;
    int bigram; // 26 * 26 (aa~zz)
    NODE* head;
}LIST; 

LIST* createList(int bigram);
void destoryList(LIST* pList);
bool insertData(LIST* pList, char* str);
void printList(LIST* list,FILE* fp);
