/* Provided stubbed test macros for the Unit Test Macros exercise. Your job
 * is to complete them AND develop unit tests for them in the provided
 * partially complete unit test driver (testUnitTestMacros.c).
 */

#ifndef UNITTEST_H
#define UNITTEST_H

#include <stdio.h>
#include <string.h>
#include <math.h>

/*
 * The TEST_SIGNED is given to you in the assigned reading so you should find
 * it and copy it here. Be sure to test your copy to make sure it works as
 * expected!
 *
 * AND, remember that the provided tests are not sufficient - you'll need to
 * write more!
 */

#define TEST_SIGNED(_ACTUAL,_EXPECT)\
{\
   long _actual = _ACTUAL, _expect = _EXPECT;\
   if (_actual != _expect) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value %ld, expected %ld\n",\
         #_ACTUAL, _actual, _expect);\
   }\
}

/*
 * Stubbed macros you must complete.
 */

#define TEST_UNSIGNED(_ACTUAL, _EXPECT)\
{\
   unsigned long _actual = _ACTUAL, _expect = _EXPECT;\
   if (_actual != _expect) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value %lu, expected %lu\n",\
         #_ACTUAL, _actual, _expect);\
   }\
}

#define TEST_REAL(_ACTUAL, _EXPECT, _EPSILON)\
{\
   double  _actual = _ACTUAL, _expect = _EXPECT, _epsilon = _EPSILON;\
   double  _epsilonAbs, _expectLow, _expectHigh;\
   _epsilonAbs = fabs(_epsilon);\
   _expectLow = _expect - _epsilonAbs;\
   _expectHigh = _expect + _epsilonAbs;\
   if (!(_expectLow <= _actual && _actual <= _expectHigh)) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value %g, expected "\
         "%g +/-%g\n", #_ACTUAL, _actual, _expect, _epsilonAbs);\
   }\
}

#define TEST_BOOLEAN(_ACTUAL, _EXPECT)\
{\
   long _actual = _ACTUAL, _expect = _EXPECT;\
   char _actualStr[6], _expectStr[6];\
   if (_actual)\
      strcpy(_actualStr, "true");\
   else\
      strcpy(_actualStr, "false");\
   if (_expect)\
      strcpy(_expectStr, "true");\
   else\
      strcpy(_expectStr, "false");\
   if ((_actual == 0) != (_expect == 0)) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value %s, expected %s\n",\
         #_ACTUAL, _actualStr, _expectStr);\
   }\
}

#define TEST_CHAR(_ACTUAL, _EXPECT)\
{\
   char _actual = _ACTUAL, _expect = _EXPECT;\
   if (_actual != _expect) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value '%c', expected '%c'\n",\
         #_ACTUAL, _actual, _expect);\
   }\
}
   
#define TEST_STRING(_ACTUAL, _EXPECT)\
{\
   char *_actual = _ACTUAL, *_expect = _EXPECT;\
   int isEqual;\
   isEqual = !(strcmp(_actual, _expect));\
   if(!isEqual) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value \"%s\", expected"\
         " \"%s\"\n", #_ACTUAL, _actual, _expect);\
   }\
}

#define TEST_ERROR(_FUNCTION_CALL)\
{\
   _FUNCTION_CALL;\
   fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
   fprintf(stderr, "Expected error detection did not occur\n");\
}
#endif
