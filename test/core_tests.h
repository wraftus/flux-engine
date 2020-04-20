#ifndef CORE_TESTS
#define CORE_TESTS

#include "../core/memory_manager.h"

#define TEST_CONDITION(cond, flag, msg)                                        \
  if (cond) {                                                                  \
    flag = false;                                                              \
    printf(msg);                                                               \
  }                                                                            \

#define TEST_MEMORY_MANAGER 1

bool runCoreTests();

bool testMemoryManager();

#endif // CORE_TESTS
