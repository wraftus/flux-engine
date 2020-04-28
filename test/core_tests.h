#ifndef CORE_TESTS
#define CORE_TESTS

#include "../core/memory_manager.h"
#include "../data_structres/vectors.h"
#include "../data_structres/component_array.h"

#define TEST_CONDITION(cond, flag, msg)                                        \
  if (cond) {                                                                  \
    flag = false;                                                              \
    printf(msg);                                                               \
  }                                                                            \

bool runTests();

// ----- core -----
#define TEST_MEMORY_MANAGER 1
bool testMemoryManager();

// ----- data structures
#define TEST_VECTORS 1
#define TEST_COMPONENT_ARRAY 1
bool testVectors();
bool testComponentArray();

#endif // CORE_TESTS
