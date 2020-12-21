#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LIST *createList(int bigram)
{
    LIST *list = NULL;
    list = (LIST *)malloc(sizeof(LIST));
    if (list)
    {
        list->bigram = bigram;
        list->head = NULL;
        list->size = 0;
    }
    return list;
}

void destoryList(LIST *pList)
{
    NODE *pCur = pList->head, *pPre = NULL;
    while (pCur)
    {
        free(pCur->pData);
        pPre = pCur;
        pCur = pCur->next;
        free(pPre);
    }
    free(pList);
}

bool insertData(LIST *pList, char *str)
{
    NODE *newNode = NULL;
    newNode = (NODE *)malloc(sizeof(NODE));
    if (newNode)
    {
        newNode->pData = strdup(str);
        newNode->next = pList->head;
        pList->size++;
        pList->head = newNode;
        return true;
    }
    else
        return false;
}

void printList(LIST *list, FILE *fp)
{
    NODE *pCur = list->head;
    if (pCur)
    {
        while (pCur)
        {
            fprintf(fp, "%s ", pCur->pData);
            pCur = pCur->next;
        }
        fprintf(fp, "\n");
    }
    else
    {
        fprintf(fp, "NONE\n");
    }
}
