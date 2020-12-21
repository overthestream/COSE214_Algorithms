#define CANDIDATE_NUM 5000 // the number of candidates that its minimum edit ditstance will be computed
#define PRINT_NUM 10       // the number of words which will be printed

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dataStructure.h"

// search candidate
void search(char *dir, char *str);

// sorting array by each element's weight
void sort(DATA **arr);

// argv[1] : directory name which 2-gram indexed file is in
int main(int argc, char **argv)
{
    // error : insufficient file
    if (argc != 2)
    {
        fprintf(stderr, "%s directory\n", argv[0]);
        return 1;
    }

    // dir : file path
    char dir[10];
    dir[0] = '.';
    dir[1] = '/';
    dir[2] = 0;
    strcat(dir, argv[1]);

    // str : query
    char str[100];
    fscanf(stdin, " %s", str);

    search(dir, str);
}

// search candidate
void search(char *dir, char *str)
{
    // declare variables
    int len = strlen(str); // length of query string
    char cur, lat;         // first, second spelling of 2-gram
    FILE *index;           // FILE pointer for index-file
    char dict[100];        // file path for index ex) "./dir/aa.txt"
    char temp[7];          // for file path ex) "aa.txt"
    char dictStr[100];     // string for index file
    bool visit[26 * 26];   // prevent overlapping

    // initialize
    memset(visit, 0, sizeof(visit));
    HEAP *heap = heapCreate(); //MAX HEAP

    // process for query whose length is 1
    // put words in MIN HEAP - strlen
    if (len == 1)
    {
        cur = str[0];
        for (int i = 0; i < 26; ++i)
        {
            lat = 'a' + i;

            memset(dict, 0, sizeof(dict));

            temp[0] = cur;
            temp[1] = lat;
            temp[2] = 0;

            strcat(dict, dir);
            strcat(dict, "/");
            strcat(temp, ".txt");
            strcat(dict, temp);

            index = fopen(dict, "rt");

            fscanf(index, "%s", dictStr);

            if (!strcmp(dictStr, "NONE"))
            {
                fclose(index);
                continue;
            }
            heapInsert(dictStr, -1 * strlen(dictStr), heap);

            while (fscanf(index, "%s", dictStr) != EOF)
            {
                heapInsert(dictStr, -1 * strlen(dictStr), heap);
            }

            fclose(index);
        }

        lat = str[0];
        for (int i = 0; i < 26; ++i)
        {
            // prevent overlapping
            if (i == str[0] - 'a')
                continue;

            cur = 'a' + i;

            memset(dict, 0, sizeof(dict));

            temp[0] = cur;
            temp[1] = lat;
            temp[2] = 0;

            strcat(dict, dir);
            strcat(dict, "/");
            strcat(temp, ".txt");
            strcat(dict, temp);

            index = fopen(dict, "rt");

            fscanf(index, "%s", dictStr);

            if (!strcmp(dictStr, "NONE"))
            {
                fclose(index);
                continue;
            }
            heapInsert(dictStr, -1 * strlen(dictStr), heap);

            while (fscanf(index, "%s", dictStr) != EOF)
            {
                heapInsert(dictStr, -1 * strlen(dictStr), heap);
            }

            fclose(index);
        }
    }
    // process ordinary words
    // put words in TRIE
    else
    {
        TRIE *trie = trieCreate();
        for (int i = 0; i < len - 1; ++i)
        {
            cur = str[i];
            lat = str[i + 1];

            if (visit[(cur - 'a') * 26 + lat - 'a'])
                continue;

            memset(dict, 0, sizeof(dict));

            temp[0] = cur;
            temp[1] = lat;
            temp[2] = 0;

            strcat(dict, dir);
            strcat(dict, "/");
            strcat(temp, ".txt");
            strcat(dict, temp);

            index = fopen(dict, "rt");

            fscanf(index, "%s", dictStr);

            if (!strcmp(dictStr, "NONE"))
            {
                fclose(index);
                continue;
            }

            trieInsert(trie, dictStr);

            while (fscanf(index, "%s", dictStr) != EOF)
            {
                trieInsert(trie, dictStr);
            }

            fclose(index);

            visit[(cur - 'a') * 26 + (lat - 'a')] = true;
        }

        // words in trie -> MAX HEAP - 2-gram weights
        trieTraverse(trie, heap);
        trieDestroy(trie);
    }

    // pop heap and compute minimum edit distance
    char *candidate[CANDIDATE_NUM];
    int minEdit[CANDIDATE_NUM];

    for (int i = 0; i < CANDIDATE_NUM; ++i)
    {
        candidate[i] = strdup(heap->data[0]->str);
        heapDelete(heap);
        minEdit[i] = min_editdistance(candidate[i], str);
    }
    heapDestroy(heap);

    // create data structure
    DATA *dataSet[CANDIDATE_NUM];
    for (int i = 0; i < CANDIDATE_NUM; ++i)
    {
        dataSet[i] = dataCreate(candidate[i], minEdit[i]);
        free(candidate[i]);
    }
    // sort data array
    sort(dataSet);

    for (int i = 0; i < PRINT_NUM; ++i)
    {
        fprintf(stdout, "%2d : %s    \t-\tminEdit : %d\n", i + 1, dataSet[i]->str, dataSet[i]->weight);
    }

    // free memory
    for (int i = 0; i < CANDIDATE_NUM; ++i)
    {
        free(dataSet[i]->str);
        free(dataSet[i]);
    }
}

// sorting array by each element's weight
void sort(DATA **arr)
{
    DATA *temp;
    for (int i = 0; i < CANDIDATE_NUM; ++i)
    {
        for (int j = 0; j < CANDIDATE_NUM - 1; ++j)
        {
            if (arr[j]->weight > arr[j + 1]->weight)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}