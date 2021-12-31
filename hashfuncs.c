#include <stdio.h>
#include <stdlib.h>

#include "hashTable.h"
#include "hashfuncs.h"
#include "hashmacros.h"

HTEntry invalidEntry() {
   HTEntry entry;
   entry.data = NULL;
   entry.frequency = 0;
   return entry;
}

int initNode(HTEntry entry, HashNode *node,
   int cap, unsigned (*hash)(const void *data)) {
   /* pass in empty node to be initialized, returns hash value */
   node->entry = entry;
   node->next = NULL;
   node->listSize = 1;
   return hashData(entry.data, cap, *hash);
}

unsigned hashData(void *data, int capacity,
   unsigned (*hash)(const void *data)) {
   return (*hash)(data) % capacity;
}

int addToHashArr(HashNode **hashArr, int h, HashNode *newNode,
   int (*compare)(const void *data1, const void *data2)) {
   int i = 0, isDuplicate = 0;
   if(hashArr[h] == NULL) {
      hashArr[h] = malloc(sizeof(HashNode));
      CHECK_ALLOC(hashArr[h]);
      hashArr[h][0] = *newNode;
      return 1;
   }
   /* check if entry is a duplicate within linkedList; also get last index */
   if((isDuplicate = checkDuplicate(&i, hashArr[h], newNode, (*compare)))) {
      return isDuplicate;
   }
   /* improve for cpu performance; currently using linear allocation */
   hashArr[h][0].listSize += 1;
   hashArr[h] = realloc(hashArr[h], hashArr[h][0].listSize * sizeof(HashNode));
   CHECK_ALLOC(hashArr[h]);
   hashArr[h][i] = *newNode;
   return 1;
}

int checkDuplicate(int *i, HashNode *list, HashNode *newNode, 
   int (*compare)(const void *data1, const void *data2)) {
   while (*i < list[0].listSize) {
      if ((*compare)(list[*i].entry.data, newNode->entry.data) == 0) {
         list[*i].entry.frequency += 1;
         return list[*i].entry.frequency;
      }
      *i += 1;
   }
   return 0; 
}

void freeListData(HashNode *linkedList, void (*destroy)(const void *data)) {
   int i;
   for (i = linkedList[0].listSize - 1; i >= 0; i--) {
      if ((*destroy) != NULL)
         (*destroy)(linkedList[i].entry.data);
      else
         free(linkedList[i].entry.data);
   }
   free(linkedList);
}

void rehashValues(HashTable* ht, HashNode** newHashArr, int newCap) {
   int h, i, newHash;
   HashNode newNode;
   /* iterate through old hash table to add vals */
   for (h = 0; h < htCapacity(ht); h++) {
      if(ht->hashArr[h] == NULL)
         continue;
      for (i = 0; i < ht->hashArr[h][0].listSize; i++) {
         newHash = initNode(ht->hashArr[h][i].entry, &newNode,
            newCap, ht->funcs->hash);
         addToHashArr(newHashArr, newHash, &newNode, ht->funcs->compare);
      }
      free(ht->hashArr[h]);
   }
   free(ht->hashArr);
}

