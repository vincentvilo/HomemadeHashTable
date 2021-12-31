#ifndef HASHFUNCS_H
#define HASHFUNS_H

#include "hashTable.h"

typedef struct node
{
   HTEntry entry;
   struct node *next;
   unsigned listSize;
}  HashNode;

typedef struct
{
   HashNode **hashArr;
   HTFunctions *funcs;
   unsigned *sizes;
   float rehashLoadFactor;
   int *nums;
   float *rehashFactor;
}  HashTable;


HTEntry invalidEntry();
int initNode(HTEntry entry, HashNode *node,
   int cap, unsigned (*hash)(const void *data));
int getLastIndex(HTEntry* newEntry, int *h, int* isUnique, HashNode** hashArr);
int addToHashArr(HashNode **hashArr, int h, HashNode *newNode,
   int (*compare)(const void *data1, const void *data2));
int checkDuplicate(int *i, HashNode *list, HashNode *newNode, 
   int (*compare)(const void *data1, const void *data2));
unsigned hashData(void *data, int capacity, unsigned (*hash)(const void *data));
void rehashValues(HashTable* ht, HashNode** newHashArr, int newCap);
void freeListData(HashNode *linkedList, void (*destroy)(const void *data));

#endif
