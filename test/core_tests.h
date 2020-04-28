#ifndef CORE_TESTS
#define CORE_TESTS

#include "../core/memory_manager.h"
#include "../data_structres/vectors.h"

#define TEST_CONDITION(cond, flag, msg)                                        \
  if (cond) {                                                                  \
    flag = false;                                                              \
    printf(msg);                                                               \
  }                                                                            \

bool runCoreTests();
bool runDataStructuresTests();

// ----- core -----
#define TEST_MEMORY_MANAGER 1
bool testMemoryManager();

// ----- data structures
#define TEST_VECTORS 1
bool testVectors();

#endif // CORE_TESTS
