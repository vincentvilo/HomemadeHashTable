#include <limits.h>
#include <assert.h>

#include "hashTable.h"
#include "hashmacros.h"
#include "hashfuncs.h"

#define NUMS_SIZE 5

#define NUM_SIZES 0
#define CAP  1
#define TOT_ENTRS 2
#define UNI_ENTRS 3
#define CUR_SIZE_INDEX 4

void assertSizes(unsigned sizes[], int numSizes)
{
   int i;
   assert(numSizes > 0);
   assert(sizes[0] != 0);
   for (i = 0; i < numSizes - 1; i++)
      assert(sizes[i] < sizes[i + 1]);
}

void* htCreate(
   HTFunctions *functions,
   unsigned sizes[],
   int numSizes,
   float rehashLoadFactor)
{
   int i;
   HashTable *ht = malloc(sizeof(HashTable));

   assertSizes(sizes, numSizes);
   assert(rehashLoadFactor > 0.0 && rehashLoadFactor <= 1.0);
   
   ht->sizes = calloc(numSizes, sizeof(unsigned));
   ht->funcs = malloc(sizeof(HTFunctions));
   ht->hashArr = calloc(sizes[0], sizeof(HashNode*));
   ht->nums = calloc(NUMS_SIZE, sizeof(int));
   ht->rehashFactor = malloc(sizeof(float));

   CHECK_ALLOC(ht);
   CHECK_ALLOC(ht->sizes);
   CHECK_ALLOC(ht->hashArr);
   CHECK_ALLOC(ht->funcs);
   CHECK_ALLOC(ht->nums);

   for (i = 0; i < numSizes; i++) {
      ht->sizes[i] = sizes[i];
   }

   *(ht->funcs) = *functions;
   ht->nums[NUM_SIZES] = numSizes;
   ht->nums[CAP] = sizes[0];
   ht->nums[TOT_ENTRS] = 0;
   ht->nums[UNI_ENTRS] = 0;
   ht->nums[CUR_SIZE_INDEX] = 0;
   *(ht->rehashFactor) = rehashLoadFactor;
   return ht;
}

void htDestroy(void *hashTable)
{
   int h;
   HashTable *ht = hashTable;
   /* free data alloc'd by htAdd */
   for (h = 0; h < htCapacity(ht); h++) {
      if (ht->hashArr[h] == NULL)
         continue;
      freeListData(ht->hashArr[h], ht->funcs->destroy);
   }
   
   /* free data alloc'd by htCreate */
   free(ht->rehashFactor);
   free(ht->sizes);
   free(ht->hashArr);
   free(ht->funcs);
   free(ht->nums);
   free(ht);
}

int hashCondition(HashTable *ht) {
   return ((*(ht->rehashFactor) != 1.0 &&
      ht->nums[CUR_SIZE_INDEX] + 1 != ht->nums[NUM_SIZES] &&
      ((double)(htUniqueEntries(ht))) / htCapacity(ht) > *(ht->rehashFactor)));
}

void rehash(HashTable *ht) { 
   int newCap;
   HashNode** newHashArr;
   if (!hashCondition(ht))
      return;
   ht->nums[CUR_SIZE_INDEX] = ht->nums[CUR_SIZE_INDEX] + 1;
   newCap = ht->sizes[ht->nums[CUR_SIZE_INDEX]];
   newHashArr = calloc(newCap, sizeof(HashNode*));
   CHECK_ALLOC(newHashArr);
   rehashValues(ht, newHashArr, newCap);
   ht->hashArr = newHashArr;
   ht->nums[CAP] = newCap;
}

unsigned htAdd(void *hashTable, void *data)
{
   int h, ret;
   HTEntry newEntry;
   HashNode newNode; 
   HashTable *ht = (HashTable*)(hashTable);
   unsigned (*hash)(const void *data) = ht->funcs->hash;
   assert(data != NULL);

   rehash(ht);

   newEntry.frequency = 1;
   newEntry.data = data;
   h = initNode(newEntry, &newNode, htCapacity(ht), (*hash));
   if ((ret=addToHashArr(ht->hashArr, h, &newNode, ht->funcs->compare)) == 1)
      ht->nums[UNI_ENTRS] += 1;
   ht->nums[TOT_ENTRS] += 1;
   return ret;
}

HTEntry htLookUp(void *hashTable, void *data)
{
   HashTable *ht = hashTable;
   unsigned h, i, (*hash)(const void *data) = ht->funcs->hash;
   int (*compare)(const void *data1, const void *data2) = ht->funcs->compare;
   assert(data != NULL);
   h = hashData(data, htCapacity(ht), *hash);
   if (ht->hashArr[h] == NULL)
      return invalidEntry();
   for (i = 0; i < ht->hashArr[h][0].listSize; i++) {
      if ((*compare)(ht->hashArr[h][i].entry.data, data) == 0) {
         HTEntry *entry = &(ht->hashArr[h][i].entry);
         return *entry;
      }
   }
   return invalidEntry();
}

void convertToArr(HashTable *ht, HTEntry **entries, unsigned *allocSize,
   unsigned *size, unsigned h) {
   int i;
   for (i = 0; i < ht->hashArr[h][0].listSize; i++) {
      (*entries)[*size] = ht->hashArr[h][i].entry;
      *size += 1;
      if (*size >= *allocSize) {
         *allocSize *= 2;
         *entries = realloc(*entries, (*allocSize) * sizeof(HTEntry));
         CHECK_ALLOC(*entries);
      }
   }
}

HTEntry* htToArray(void *hashTable, unsigned *size)
{
   unsigned h, allocSize = 4;
   HashTable *ht = hashTable;
   HTEntry *entries;
   *size = 0;
   if (!htTotalEntries(ht)) {
      return NULL;
   }
   entries = malloc(sizeof(HTEntry) * allocSize);
   CHECK_ALLOC(entries);
   for (h = 0; h < htCapacity(ht); h++) {
      if (ht->hashArr[h] == NULL)
         continue;
      convertToArr(ht, &entries, &allocSize, size, h);
   }
   entries = realloc(entries, *size);
   CHECK_ALLOC(entries);
   return entries;
}

unsigned htCapacity(void *hashTable)
{
   return ((HashTable*)(hashTable))->nums[CAP];
}

unsigned htUniqueEntries(void *hashTable)
{
   return ((HashTable*)(hashTable))->nums[UNI_ENTRS];
}

unsigned htTotalEntries(void *hashTable)
{
   return ((HashTable*)(hashTable))->nums[TOT_ENTRS];
}

HTMetrics htMetrics(void *hashTable)
{
   unsigned h;
   double totalLength;
   HashTable *ht = hashTable;
   HTMetrics met;
   met.numberOfChains = 0;
   met.maxChainLength = 0;
   met.avgChainLength = 0;
   
   for (h = 0; h < htCapacity(ht); h++) {
      if (ht->hashArr[h] == NULL)
         continue;
      met.numberOfChains++;
      met.maxChainLength = (met.maxChainLength > ht->hashArr[h][0].listSize) ?
         met.maxChainLength : ht->hashArr[h][0].listSize;
      totalLength += ht->hashArr[h][0].listSize;
   }
   met.avgChainLength = totalLength / met.numberOfChains;
   return met;
}
