/*
 * A general purpose C unit testing framework developed for CPE 357 and
 * provided to students to expose them to and encourage their use of unit
 * testing and test driven development (TDD). While it was provided in
 * conjunction with the hash table project and associated exercise(s), students
 * are welcome and encourage to use and/or modify it, as they see fit, on
 * subsequent assignments in CPE 357 or anywhere else it they find it useful.
 *
 * Author: Kurt Mammen
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include "unitTest.h"
#include "hashTable.h"

#define TEST_ALL -1
#define REGULAR -2 
#define SPECIAL -3

#define MAX_RANDOM_STR 72 /* Max length of any random string */

/* Prototype for all test functions. This allows the creation of an array of
 * function pointers which makes the testing code shorter and more clear. It
 * also makes it easier/faster to add new tests - NICE!
 */
typedef void (*TestFunc)();

/* Structure for a test. See initRegularTests and initSpecialTests to see
 * how it is used.
 */
typedef struct
{
   TestFunc fn;
   char *fnName;
} Test;

/* Modified hash algorithm from K&R (page 144). This version does NOT mod the
 * calculated hash value by the table size because it does not know the table
 * size - the caller (the hash table itself) does though!
 *
 * Also note the following:
 *   * This hash only works for C strings.
 *   * Not the best/fastest/most flexible hash - you'll want something better
 *     for the Word Frequency project.
 */
static unsigned hashString(const void *data)
{
   unsigned hash;
   const char *str = data;

   for (hash = 0; *str;  str++)
      hash = *str + 31 * hash;

   /* Return the raw hash value - the modulo must be done by caller */
   return hash;
}

static int compareString(const void *a, const void *b)
{
   return strcmp(a, b);
}

/* Helper function to make random strings for robust testing. Strings will be
 * a minimum of 3 characters up to MAX_RANDOM_STR characters in length.
 */
static char* randomString()
{
   char *str;
   int i;
   int length = (rand() % MAX_RANDOM_STR - 2) + 3;

   if (NULL == (str = malloc(length + 1)))
   {
      perror("randomString()");
      exit(EXIT_FAILURE);
   }

   /* Put random but visible char values in the string */
   for (i = 0; i < length; i++)
      str[i] = (rand() % (' ' - '~' + 1)) + ' ';

   str[length] = 0; /* nul-terminate it! */

   return str; 
}

/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test01. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */
static void core01()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, -987, 0.73));
}

/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test11. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */ 
static void core11()
{
   unsigned sizes[] = {31};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 0.73);

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 31);
   TEST_UNSIGNED(htUniqueEntries(ht), 0);
   TEST_UNSIGNED(htTotalEntries(ht), 0);

   htDestroy(ht);
}

/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test12. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */ 
static void core12()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   /* Single entry */
   freq = htAdd(ht, randomString());

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);
   
   htDestroy(ht);
}

static void core02()
{
   unsigned sizes[] = {11};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 0, 0.73));
}

static void core03a()
{
   unsigned sizes[] = {0, 7, 11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core03b()
{
   /* no error expected */
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 2, 0.73));
}

static void core04()
{
   /* check if the next value is equal */
   unsigned sizes[] = {7, 11, 11};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 3, 0.73));
}

static void core05a()
{
   /* check order of sizes when middle values are not sorted */
   unsigned sizes[] = {11, 31, 23, 101};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core05b()
{
   /* check order of sizes when last value is not sorted */
   unsigned sizes[] = {11, 23, 31, 7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core05c()
{
   /* check order of sizes when first value is not sorted */
   unsigned sizes[] = {31, 23, 79, 101};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core05d()
{
   /* check order of sizes when list is sorted - no error expected*/
   unsigned sizes[] = {7, 23, 79, 101};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core06()
{
   /* check if loadfactor < 0 */
   unsigned sizes[] = {7, 23, 79, 101};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, -0.73));
}

static void core07()
{
   /* loadfactor = 0 */
   unsigned sizes[] = {7, 23, 79, 101};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0));
}

static void core08()
{
   /* loadfactor > 1 */
   unsigned sizes[] = {7, 23, 79, 101};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 1.01));
}

static void core09()
{
   /* check data != NULL */
   unsigned sizes[] = {7, 23, 79, 101};
   HTFunctions funcs = {hashString, compareString, NULL};
   void* ht = htCreate(&funcs, sizes, 4, 0.72);

   TEST_ERROR(htAdd(ht, NULL));
}

static void core10()
{
   /* check data != NULL */
   unsigned sizes[] = {7, 23, 79, 101};
   HTFunctions funcs = {hashString, compareString, NULL};
   void* ht = htCreate(&funcs, sizes, 4, 0.72);

   TEST_ERROR(htLookUp(ht, NULL));
}

static void core13()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   /* Single entry */
   freq = htAdd(ht, randomString());
   freq = htAdd(ht, randomString());

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);
   
   htDestroy(ht);
}

static char* nonRandomString()
{
   char *string;

   if (NULL == (string = malloc(6)))
   {
      perror("nonRandomString()");
      exit(EXIT_FAILURE);
   }

   string[0] = 'h';
   string[1] = 'e';
   string[2] = 'l';
   string[3] = 'l';
   string[4] = 'o';
   string[5] = 0;
   return string;
}

static void core14()
{
   int freq = 0;
   unsigned sizes[] = {23, 43};

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   char* string1 = nonRandomString();
   char* string2 = nonRandomString();
   /* entry added twice */
   freq = htAdd(ht, string1);
   freq = htAdd(ht, string2);

   TEST_UNSIGNED(freq, 2);
   TEST_UNSIGNED(htCapacity(ht), 23);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   htDestroy(ht);
   free(string2);
}

static unsigned badHash(const void *data)
{
   unsigned hash;
   const char *str = data;

   for (hash = 0; *str;  str++)
      hash = *str + 31 * hash;

   /* Return the raw hash value - the modulo must be done by caller */
   return hash * 0 + 7;
}

static void core15()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};

   HTFunctions funcs = {badHash, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   freq = htAdd(ht, randomString());
   freq = htAdd(ht, randomString());

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   htDestroy(ht);
}

static void core16()
{
   HTEntry entry;
   unsigned sizes[] = {11, 23};
   char* string = randomString();

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   entry = htLookUp(ht, string);
   TEST_BOOLEAN((entry.data == NULL), 1);
   TEST_UNSIGNED(entry.frequency, 0);

   htDestroy(ht);
   free(string);
}

static void core17()
{
   HTEntry entry;
   unsigned sizes[] = {11, 23};
   char* string = nonRandomString();

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   htAdd(ht, randomString());
   htAdd(ht, randomString());
   htAdd(ht, randomString());
   htAdd(ht, randomString());
   htAdd(ht, randomString());
   entry = htLookUp(ht, string);

   TEST_BOOLEAN((entry.data == NULL), 1);
   TEST_UNSIGNED(entry.frequency, 0);

   htDestroy(ht);
   free(string);
}

static void core18a()
{
   HTEntry entry;
   unsigned sizes[] = {11, 23};
   char* string1 = nonRandomString();
   char* string2 = malloc(6);
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   if (string2 == NULL) {
      perror("ruh roh raggy");
      exit(EXIT_FAILURE);
   }
   strcpy(string2, string1);
   htAdd(ht, string1);

   entry = htLookUp(ht, string2);
   TEST_BOOLEAN((entry.data == string1), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   htDestroy(ht);
   free(string2);
}

static void core18b()
{
   HTEntry entry;
   unsigned sizes[] = {11, 23};
   char* string1 = randomString();
   char* string2 = randomString();

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   htAdd(ht, string1);
   htAdd(ht, string2);

   entry = htLookUp(ht, string1);
   TEST_BOOLEAN((entry.data == string1), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string2);
   TEST_BOOLEAN((entry.data == string2), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   htDestroy(ht);
}
static void core18c()
{
   HTEntry entry;
   unsigned sizes[] = {11, 23};
   char* string1 = randomString();
   char* string2 = randomString();
   char* string3 = randomString();
   char* string4 = nonRandomString();
   char* string5 = nonRandomString();

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   htAdd(ht, string1);
   htAdd(ht, string2);
   htAdd(ht, string3);
   htAdd(ht, string4);
   htAdd(ht, string5);

   entry = htLookUp(ht, string1);
   TEST_BOOLEAN((entry.data == string1), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string2);
   TEST_BOOLEAN((entry.data == string2), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string4);
   TEST_BOOLEAN((entry.data == string4), 1);
   TEST_UNSIGNED(entry.frequency, 2);

   htDestroy(ht);
   free(string5);
}

static void core19()
{
   HTEntry entry;
   unsigned sizes[] = {11, 23};
   char* string1 = nonRandomString();
   char* string2 = nonRandomString();

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   htAdd(ht, string1);
   htAdd(ht, string2);

   entry = htLookUp(ht, string1);
   TEST_BOOLEAN((entry.data == string1), 1);
   TEST_UNSIGNED(entry.frequency, 2);

   entry = htLookUp(ht, string2);
   TEST_BOOLEAN((entry.data == string1), 1);
   TEST_UNSIGNED(entry.frequency, 2);

   htDestroy(ht);
   free(string2);
}

static void feat01()
{
   HTEntry entry;
   unsigned sizes[] = {11, 23};
   char* string1 = randomString();
   char* string2 = randomString();

   HTFunctions funcs = {badHash, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   htAdd(ht, string1);
   htAdd(ht, string2);

   entry = htLookUp(ht, string1);
   TEST_BOOLEAN((entry.data == string1), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string2);
   TEST_BOOLEAN((entry.data == string2), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   htDestroy(ht);
}

static void feat02()
{
   unsigned sizes[] = {7, 11};
   char* string1 = randomString();
   char* string2 = nonRandomString();
   char* string3 = nonRandomString();
   char* string4 = randomString();
   char* string5 = randomString();

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.40);

   htAdd(ht, string1);
   htAdd(ht, string2);
   htAdd(ht, string3); /* 2 unique entries, no rehash */

   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 3);
   TEST_UNSIGNED(htCapacity(ht), 7);
   
   htAdd(ht, string4); /*3 unique entries, ratio exceeds, rehash on next add */

   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 4);

   htAdd(ht, string5); /* rehash */

   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 5);

   htDestroy(ht);
   free(string3);
}

static void feat03()
{
   HTEntry entry;
   unsigned sizes[] = {7, 11, 13};
   char* string1 = randomString();
   char* samestr2 = nonRandomString();
   char* samestr3 = nonRandomString();
   char* string4 = randomString();
   char* string5 = randomString();
   char* string6 = randomString();
   char* string7 = randomString();
   char* string8 = randomString();
   char* string9 = randomString();

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 3, 0.50);

   htAdd(ht, string1);
   htAdd(ht, samestr2);
   htAdd(ht, samestr3); /* 2 unique entries, no rehash */
   htAdd(ht, string4); /*3 unique entries, ratio exceeds, rehash on next add */

   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 4);

   htAdd(ht, string5); /* 4 unique entries */

   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 5);

   htAdd(ht, string6); /* 5 unique entries rehash */
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 6);

   htAdd(ht, string7);

   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 6);
   TEST_UNSIGNED(htTotalEntries(ht), 7);

   htAdd(ht, string8); 

   TEST_UNSIGNED(htCapacity(ht), 13);
   TEST_UNSIGNED(htUniqueEntries(ht), 7);
   TEST_UNSIGNED(htTotalEntries(ht), 8);

   htAdd(ht, string9); 

   TEST_UNSIGNED(htCapacity(ht), 13);
   TEST_UNSIGNED(htUniqueEntries(ht), 8);
   TEST_UNSIGNED(htTotalEntries(ht), 9);

   entry = htLookUp(ht, string1);
   TEST_BOOLEAN((entry.data == string1), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, samestr2);
   TEST_BOOLEAN((entry.data == samestr2), 1);
   TEST_UNSIGNED(entry.frequency, 2);

   entry = htLookUp(ht, samestr3);
   TEST_BOOLEAN((entry.data == samestr2), 1);
   TEST_UNSIGNED(entry.frequency, 2);

   entry = htLookUp(ht, string4);
   TEST_BOOLEAN((entry.data == string4), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string5);
   TEST_BOOLEAN((entry.data == string5), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string6);
   TEST_BOOLEAN((entry.data == string6), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string7);
   TEST_BOOLEAN((entry.data == string7), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string8);
   TEST_BOOLEAN((entry.data == string8), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, string9);
   TEST_BOOLEAN((entry.data == string9), 1);
   TEST_UNSIGNED(entry.frequency, 1);

   htDestroy(ht);
   free(samestr3);
}

static void feat04()
{
   unsigned sizes[] = {3, 5};
   char* string1 = randomString();
   char* samestr2 = nonRandomString();
   char* samestr3 = nonRandomString();
   char* string4 = randomString();
   char* string5 = randomString();

   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.5);

   htAdd(ht, string1);
   htAdd(ht, samestr2); /* 2 unique entries, rehash on next addition */
   htAdd(ht, samestr3); /* 2 unique entries, no rehash */

   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   htAdd(ht, string4); /*3 unique entries, rehash */

   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 4);

   htAdd(ht, string5); /* 4 unique entries */

   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 5);

   htDestroy(ht);
   free(samestr3);
}

static void feat06() {
   HTEntry *entry;
   unsigned size = INT_MAX;
   unsigned sizes[] = {7, 11};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.5);
   entry = htToArray(ht, &size);

   TEST_BOOLEAN((entry == NULL), 1);
   TEST_UNSIGNED(size, 0);
   free(entry);
   htDestroy(ht);

}

static void feat07() {
   int i;
   unsigned size = INT_MAX;
   HTEntry* entries;
   HTEntry entry;
   char* string1 = nonRandomString();
   char* string2 = nonRandomString();
   char* string3 = nonRandomString();
   char* string4 = nonRandomString();
   unsigned sizes[] = {11, 23, 31};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 3, 0.7);

   htAdd(ht, string1);
   for (i = 0; i < 30; i++) {
      if (i == 12)
         htAdd(ht, string2);
      else if (i == 24)
         htAdd(ht, string3);
      else if (i == 36)
         htAdd(ht, string4);
      else
         htAdd(ht, randomString());
   }
   
   entries = htToArray(ht, &size);   

   TEST_UNSIGNED(size, htUniqueEntries(ht));
   TEST_BOOLEAN((entries != NULL), 1);

   for (i = 0; i < htUniqueEntries(ht); i++) {
      entry = htLookUp(ht, entries[i].data);
      TEST_BOOLEAN(entry.data == entries[i].data, 1);
      TEST_UNSIGNED(entries[i].frequency, entry.frequency);
   }

   free(entries);
   free(string2);
   free(string3);
   free(string4);
   htDestroy(ht);

}

static void feat08() {
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTMetrics metrics = htMetrics(ht);
   TEST_UNSIGNED(metrics.numberOfChains, 0);
   TEST_UNSIGNED(metrics.maxChainLength, 0);
   TEST_REAL((double)(metrics.avgChainLength), 0, FLT_EPSILON);
   htDestroy(ht);
   
}

static void feat09() {
   int i;
   unsigned sizes[] = {4};
   char* string1 = nonRandomString();
   char* string2 = nonRandomString();
   char* string3 = nonRandomString();
   HTMetrics metrics;
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 1);

   htAdd(ht, string1);
   for (i = 0; i < 5; i++) {
      htAdd(ht, randomString());
   }
   htAdd(ht, string2);
   htAdd(ht, string3);
   
   metrics = htMetrics(ht);
   TEST_UNSIGNED(metrics.numberOfChains, 4);
   TEST_UNSIGNED(metrics.maxChainLength, 3);
   TEST_REAL((double)(metrics.avgChainLength), 1.5, FLT_EPSILON);

   free(string2);
   free(string3);
   htDestroy(ht);
   
}

static void cpu02() {
   unsigned i = 0;
   unsigned sizes[] = {2000000};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 0.72);
   for (i = 0; i < 100000; i++) {
      htAdd(ht, randomString());
   }

   htDestroy(ht);
   
}
static void heap01() {
   unsigned i = 0;
   char* string1;
   unsigned sizes[] = {6969, 7777, 42069};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 3, 0.72);
   for (i = 0; i < 10000; i++) {
      string1 = randomString();
      /* ensure no duplicates */
      if(htLookUp(ht, string1).data == NULL)
         htAdd(ht, string1);
      else {
         free(string1);
      }
   }
   htDestroy(ht);
}

static void testAll(Test* tests)
{
   int i;

   for (i = 0; tests[i].fn != NULL; i++)
   {
      printf("Running regular %s\n", tests[i].fnName);

      /* Call the test function via function pointer in the array */
      tests[i].fn();
   }
}

static void findAndCall(Test* tests, const char *type, char *fnName)
{
   int i;

   for (i = 0; tests[i].fn != NULL; i++)
   {
      if (0 == strcmp(tests[i].fnName, fnName))
      {
         /* Found it, call the function via function pointer... */
         printf("Running %s %s\n", type, fnName);
         tests[i].fn();
         return;
      }
   }

   fprintf(stderr, "ERROR %s %s: Function not found\n", type, fnName);
   exit(EXIT_FAILURE);
}

static void runTests(Test *tests, const char *type, char *fnName)
{
   if (fnName == NULL)
      testAll(tests);
   else
      findAndCall(tests, type, fnName);

   /* Free the tests (allocated in initTests) */
   free(tests);
}

static char* checkArgs(int argc, char *argv[], int *testType)
{
   char *testName;

   if (argc == 1)
   {
      *testType = REGULAR;      
      testName = NULL;
   }
   else if (argc == 2)
   {
      *testType = REGULAR; 
      testName = argv[1];
   }
   else if (argc == 3)
   {
      if (0 != strcmp(argv[1], "-special"))
      {
         fprintf(stderr, "Invalid option '%s'\n", argv[1]);
         exit(EXIT_FAILURE);
      }
      
      *testType = SPECIAL;
      testName = argv[2];
   }
   else
   {
      fprintf(stderr, "Usage: %s [testName | -special testName]\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   return testName;
}

Test* initTests(Test tests[], int size)
{
   Test *dynamicMemory = malloc(size);

   if (dynamicMemory == NULL)
   {
      fprintf(stderr, "FAILURE in %s at %d: ", __FILE__, __LINE__);
      perror(NULL);
      exit(EXIT_FAILURE);
   }

   return memcpy(dynamicMemory, tests, size);
}

/* Allocates, initializes, and returns the array of regular test functions.
 * Regular test functions are those that are expected to pass or report failure
 * BUT NOT terminate the test driver.
 *
 * By default, the test driver runs ALL of the regular tests. Alternatively, you
 * can run one at a time by simply specifying its name when you invoke the
 * test driver.
 *
 * See initSpecialTests for tests that you always want to run individually.
 *
 * NOTE: The last structure in the array must have NULL values as this indicates
 *    the array's end.
 *
 * IMPORTANT SUBTLETY: You can only use sizeof to obtain an array's size in
 *    the scope where the array is declared, otherwise you will just get
 *    the size of the pointer to the array.
 */
Test* initRegularTests()
{
   Test local[] = {
      {core11, "core11"},
      {core12, "core12"},
      {core13, "core13"},
      {core14, "core14"},
      {core15, "core15"},
      {core16, "core16"},
      {core17, "core17"},
      {core18a, "core18a"},
      {core18b, "core18b"},
      {core18c, "core18c"},
      {core19, "core19"},
      {feat01, "feat01"},
      {feat02, "feat02"},
      {feat03, "feat03"},
      {feat04, "feat04"},
      {feat06, "feat06"},
      {feat07, "feat07"},
      {feat08, "feat08"},
      {feat09, "feat09"},
      {cpu02, "cpu02"},
      {heap01, "heap01"},
      {NULL, NULL}
   };

   /* See IMPORTANT SUBTLETY above regarding the use of sizeof on arrays */
   return initTests(local, sizeof(local));
}

/* Allocates, initializes, and returns the array of special test functions.
 * Special test functions are those that you want to run individually for one
 * reason or another. For example, a test to see if a function asserts failure
 * when it is supposed to. Or, a test intended to measure CPU or memory
 * performance using tools like the Unix time command or Valgrind - for
 * tests like these you only want to run the single test and then observe the
 * results.
 *
 * See initRegularTests for tests that can together.
 *
 * NOTE: The last structure in the array must have NULL values as this indicates
 *    the array's end.
 *
 * IMPORTANT SUBTLETY: You can only use sizeof to obtain an array's size in
 *    the scope where the array is declared, otherwise you will just get
 *    the size of the pointer to the array.
 */
Test* initSpecialTests()
{
   Test local[] = {
      {core01, "core01"},
      {core02, "core02"},
      {core03a, "core03a"},
      {core03b, "core03b"},
      {core04, "core04"},
      {core05a, "core05a"},
      {core05b, "core05b"},
      {core05c, "core05c"},
      {core05d, "core05d"},
      {core06, "core06"},
      {core07, "core07"},
      {core08, "core08"},
      {core09, "core09"},
      {core10, "core10"},
      {NULL, NULL}
   };

   /* See IMPORTANT SUBTLETY above regarding the use of sizeof on arrays */
   return initTests(local, sizeof(local)); 
}

/* Test driver for Hash Table Project.
 *
 * Usage: testHashTable [functionName | -special functionName]
 *
 * When no option is specified ALL of the regular tests are run.
 * When "functionName" is specified that single regular test is run.
 * When "-special functionName" is specified that single special test is run.
 *
 * What TODO: As provided with the Unit Test Hash Table exercise, this unit test
 * driver provides several working examples of actual Evaluation System tests
 * for the Hash Table project. The exercise asks you to develop a few more so
 * that you can experience how easy it is to develop unit tests BEFORE you even
 * implement any actual code for the solution being tested. The hope is that
 * this will encourage you to develop tests early and often as you develop your
 * Hash Table project solution thereby adding unit testing and Test Driven
 * Development (TDD) to your personal repertoire of programming skills! 
 *    
 * To add a new test you must:
 * 
 *    1) Write a test function.
 *    2) Add its name to the appropriate array of test functions, see
 *       initRegularTests and initSpecialTests for details.
 *    3) Compile and test your test by verifying:
 *          1. It report failure when run on code known to be incorrect.
 *          2. It passes (does NOT report failure) when run on code believed to
 *             be correct.
 *    4) Be sure use Valgrind, where appropriate, to check for memory errors
 *       and/or leaks too!
 */
int main(int argc, char *argv[])
{
   char *testName;
   int testType;

   /* Random numbers used to produce "interesting" strings for testing */
   srand(182955);

   /* Make stdout unbuffered so that test output is synchronous on signals */
   setbuf(stdout, NULL);

   /* Get the test name type */
   testName = checkArgs(argc, argv, &testType);
 
   /* Run the test(s)... */
   if (testType == REGULAR)
      runTests(initRegularTests(), "regular", testName);
   else
      runTests(initSpecialTests(), "special", testName);
   
   return 0;
}
