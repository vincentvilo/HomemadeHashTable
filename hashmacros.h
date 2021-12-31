#ifndef HASHMACROS_H
#define HASHMACROS_H

#define CHECK_ALLOC(_ALLOC)\
{\
   if (NULL == (_ALLOC)) {\
      fprintf(stderr, "ERROR: mem alloc: %s: %d\n", __FILE__, __LINE__);\
      exit(EXIT_FAILURE);\
   }\
}

#endif
