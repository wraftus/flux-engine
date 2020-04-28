#include "core_tests.h"

bool runCoreTests() {
  bool passed = true;
  printf("Running core tests ...\n");

#if TEST_MEMORY_MANAGER
  passed &= testMemoryManager();
#endif

  if (passed)
    printf("Passed all core tests!\n");
  return passed;
}

bool testMemoryManager() {
  printf("Testing MemoryManager ...\n");
  bool passed = true;

  float a = 1.0f;
  float b = 2.0f;
  float c = 3.0f;

  flux::flux_id id1;
  flux::flux_id id2;
  flux::flux_id id3;

  // test constructor and basic get functions
  try {
    flux::MemoryManager manager(0);
    TEST_CONDITION(true, passed, "Passing in zero to constructor should fail\n")
  } catch (...) {}
  flux::MemoryManager manager(sizeof(float)*3);
  TEST_CONDITION(manager.getMaxSize() != (sizeof(float) * 3), passed,
                 "MemoryManager returned incorrect value for getMaxSize\n")
  TEST_CONDITION(manager.getAmountClaimed() != 0, passed,
                 "MemoryManager returned the wrong value for getAmountClaimed\n")

  // test claiming memory locations
  flux::flux_data_ptr ptr = manager.claimSection(sizeof(float) * 4, id1);
  TEST_CONDITION(ptr, passed, "Manager gave space when it has not enough\n")
  ptr = manager.claimSection(sizeof(float), id1);
  TEST_CONDITION(!ptr, passed, "Manager failed to give space\n")
  if (ptr)
    *static_cast<float *>(ptr) = a;
  ptr = manager.claimSection(sizeof(float), id2);
  TEST_CONDITION(!ptr, passed, "Manager failed to give space\n")
  if (ptr)
    *static_cast<float *>(ptr) = b;
  ptr = manager.claimSection(sizeof(float), id3);
  TEST_CONDITION(!ptr, passed, "Manager failed to give space\n")
  if (ptr)
    *static_cast<float *>(ptr) = c;
  TEST_CONDITION(manager.claimSection(sizeof(float), id1), passed,
                 "Manager gave space when it should not have\n")

  // test getting data locations
  TEST_CONDITION(*static_cast<float *>(manager.getSection(id1)) != a,
                 passed, "value stored with first id not correct\n")
  TEST_CONDITION(*static_cast<float *>(manager.getSection(id2)) != b,
                 passed, "value stored with second id not correct\n")
  TEST_CONDITION(*static_cast<float *>(manager.getSection(id3)) != c,
                 passed, "value stored with thrid id not correct\n")

  // test removing a section, and adding a new one
  TEST_CONDITION(!manager.freeSection(id3), passed, "failed to free a section\n")
  TEST_CONDITION(manager.getSection(id3), passed, "returned a deleted section\n")
  ptr = manager.claimSection(sizeof(float), id3);
  TEST_CONDITION(!ptr, passed, "Manager failed to give space\n")
  if (ptr)
    *static_cast<float *>(ptr) = c;
  TEST_CONDITION(manager.claimSection(sizeof(float), id1), passed,
                 "Manager gave space when it should not have\n")
  TEST_CONDITION(!manager.freeSection(id2), passed, "failed to free a section\n")
  TEST_CONDITION(manager.getSection(id2), passed, "returned a deleted section\n")
  ptr = manager.claimSection(sizeof(float), id2);
  TEST_CONDITION(!ptr, passed, "Manager failed to give space\n")
  if (ptr)
    *static_cast<float *>(ptr) = b;
  TEST_CONDITION(*static_cast<float *>(manager.getSection(id1)) != a,
                 passed, "value stored with first id not correct\n")
  TEST_CONDITION(*static_cast<float *>(manager.getSection(id2)) != b,
                 passed, "value stored with second id not correct\n")
  TEST_CONDITION(*static_cast<float *>(manager.getSection(id3)) != c,
                 passed, "value stored with thrid id not correct\n")

  // test defraging
  TEST_CONDITION(!manager.freeSection(id1), passed, "failed to free a section\n")
  TEST_CONDITION(!manager.freeSection(id3), passed, "failed to free a section\n")
  TEST_CONDITION(manager.claimSection(sizeof(float) * 2, id1), passed,
                 "manager allocated space when it was too fragmented to do so")
  manager.defrag();
  TEST_CONDITION(*static_cast<float *>(manager.getSection(id2)) != b,
                 passed, "value stored with second id not correct\n")
  TEST_CONDITION(!manager.claimSection(sizeof(float) * 2, id1), passed,
                 "manager failed to allocate space when it should be able to do so")


  if (passed)
    printf("MemoryManager passed all tests!\n");
  return passed;
}
