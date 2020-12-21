#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "linkedList.h"

// indexing for bigram
void indexing(FILE *fp, LIST **listArray);

// make 2-gram index file in dirctory [dir]
void printIndex(char *dir, LIST **listArray);

// argv[1] : text file for dictionary(words.txt)
// argv[2] : directory name which 2-gram indexed file is in
int main(int argc, char **argv)
{
    // error : insufficient file
    if (argc != 3)
    {
        fprintf(stderr, "%s dictionary-file directory\n", argv[0]);
        return 1;
    }

    // open dictionary file
    FILE *infp;
    infp = fopen(argv[1], "rt");
    if (infp == NULL)
    {
        fprintf(stderr, "Error : cannot open file [%s]\n", argv[1]);
        return 1;
    }

    // declare linkedList-array for each 2-gram(aa~zz)
    LIST **listArray = NULL;
    listArray = (LIST **)malloc(sizeof(LIST *) * (26 * 26));
    for (int i = 0; i < 26 * 26; ++i)
        listArray[i] = createList(i);

    // indexing words in linkedList-array
    indexing(infp, listArray);

    fclose(infp);

    // make directory named argv[2]
    char dir[100] = {
        "./"};
    strcat(dir, argv[2]);

    int result = mkdir(dir, 0777);
    if (result == -1)
    {
        if (errno == EEXIST)
            fprintf(stderr, "Error : directory [%s] already exists!\n", dir);
        else
            fprintf(stderr, "Error : mkdir failed!\n");

        for (int i = 0; i < 26 * 26; ++i)
            destoryList(listArray[i]);
        free(listArray);
        return 1;
    }

    // make index file in directory
    printIndex(dir, listArray);

    // destroy data structure
    for (int i = 0; i < 26 * 26; ++i)
        destoryList(listArray[i]);
    free(listArray);

    return 0;
}

// make 2-gram index file in dirctory [dir]
void printIndex(char *dir, LIST **listArray)
{
    // declare variable for file path
    FILE *outfp;
    char file[100] = {
        0,
    };
    char bigram[3];

    for (int i = 0; i < 26 * 26; ++i)
    {
        // initialize file path
        // ex) ./dir/aa.txt
        memset(file, 0, sizeof(file));
        bigram[0] = 'a' + i / 26;
        bigram[1] = 'a' + i % 26;
        bigram[2] = 0;
        strcat(file, dir);
        strcat(file, "/");
        strcat(file, bigram);
        strcat(file, ".txt");

        // print linkedList in file
        outfp = fopen(file, "wt");
        if (outfp)
            printList(listArray[i], outfp);
        else
            fprintf(stderr, "Error : cannot open file [%s]\n", file);

        fclose(outfp);
    }
}

// indexing for bigram
void indexing(FILE *infp, LIST **listArray)
{
    // declare variables
    char str[100];       // word in FILE
    int len;             // length of str
    char first, last;    // first and second spelling
    bool visit[26 * 26]; // prevent overlapping

    while (fscanf(infp, "%s", str) != EOF)
    {
        len = strlen(str);

        // process word whose length is 1
        // ex) if word is a, put in linkedList for aa
        if (len == 1)
        {
            first = str[0] - 'a';
            insertData(listArray[first * 26 + first], str);
            continue;
        }

        memset(visit, 0, sizeof(visit));

        // process word
        // ex) if word is abc, put in linkedList for ab,bc
        for (int i = 0; i < len - 1; ++i)
        {
            first = str[i];
            last = str[i + 1];
            if (!visit[(first - 'a') * 26 + (last - 'a')])
                insertData(listArray[(first - 'a') * 26 + (last - 'a')], str);
            visit[(first - 'a') * 26 + (last - 'a')] = true;
        }
    }
}