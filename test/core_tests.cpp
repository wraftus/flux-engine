#include "core_tests.h"

bool runTests() {
  bool passed = true;
  printf("Running core tests ...\n");

#if TEST_VECTORS
  passed &= testVectors();
#endif TEST_VECTORS

#if TEST_MEMORY_MANAGER
  passed &= testMemoryManager();
#endif

#if TEST_COMPONENT_ARRAY
  passed &= testComponentArray();
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

bool testVectors() {
  bool passed = true;
  printf("Testing Vectors ...\n");

  auto v2_1 = flux::Vector2D(1.0, 1.0);
  auto v2_2 = flux::Vector2D(1.0, 1.0);
  auto v2_3 = -v2_1;

  auto v3_1 = flux::Vector3D(1.0, 1.0, 1.0);
  auto v3_2 = flux::Vector3D(1.0, 1.0, 1.0);
  auto v3_3 = -v3_1;

  TEST_CONDITION(!(v2_1 == v2_2), passed, "Vector2D operator== not working properly")
  TEST_CONDITION(!(v3_1 == v3_2), passed, "Vector3D operator== not working properly")
  TEST_CONDITION(v2_1 != v2_2, passed, "Vector2D operator!= not working properly")
  TEST_CONDITION(v3_1 != v3_2, passed, "Vector3D operator!= not working properly")

 TEST_CONDITION(v2_3 != flux::Vector2D(-1.0, -1.0), passed,
   "Vector2D unary operator- not working properly")
  TEST_CONDITION(v3_3 != flux::Vector3D(-1.0, -1.0, -1.0), passed, 
    "Vector2D unary operator- not working properly")
  TEST_CONDITION(v2_1 + v2_2 != flux::Vector2D(2.0, 2.0), passed,
    "Vector2D binary operator+ not working properly")
  TEST_CONDITION(v2_1 - v2_2 != flux::Vector2D(0.0, 0.0), passed,
    "Vector2D binary operator- not working properly")
  TEST_CONDITION(v3_1 + v3_2 != flux::Vector3D(2.0, 2.0, 2.0), passed,
    "Vector3D binary operator+ not working properly")
  TEST_CONDITION(v3_1 - v3_2 != flux::Vector3D(0.0, 0.0, 0.0), passed,
    "Vector3D binary operator- not working properly")
  TEST_CONDITION((2 * v2_1 != v2_1 + v2_1) || (v2_1 * 2 != v2_1 + v2_1), passed,
    "Vector2D operator* not working properly")
  TEST_CONDITION((2 * v3_1 != v3_1 + v3_1) || (v3_1 * 2 != v3_1 + v3_1), passed,
    "Vector3D operator* not working properly")
  TEST_CONDITION(v2_1 / 2 != flux::Vector2D(0.5, 0.5), passed,
    "Vector2D operator/ not working properly")
  TEST_CONDITION(v3_1 / 2 != flux::Vector3D(0.5, 0.5, 0.5), passed,
    "Vector3D operator/ not working properly")

  v2_1 += v2_2;
  v3_1 += v3_2;
  TEST_CONDITION(v2_1 != 2 * v2_2, passed, "Vector2D operator+= not working properly")
  TEST_CONDITION(v3_1 != 2 * v3_2, passed, "Vector3D operator+= not working properly")
  v2_1 -= v2_2;
  v3_1 -= v3_2;
  TEST_CONDITION(v2_1 != v2_2, passed, "Vector2D operator-= not working properly")
  TEST_CONDITION(v3_1 != v3_2, passed, "Vector3D operator-= not working properly")
  v2_1 *= 2;
  v3_1 *= 2;
  TEST_CONDITION(v2_1 != 2 * v2_2, passed, "Vector2D operator*= not working properly")
  TEST_CONDITION(v3_1 != 2 * v3_2, passed, "Vector3D operator*= not working properly")
  v2_1 /= 2;
  v3_1 /= 2;
  TEST_CONDITION(v2_1 != v2_2, passed, "Vector2D operator/= not working properly")
  TEST_CONDITION(v3_1 != v3_2, passed, "Vector3D operator/= not working properly")

  v2_2 = flux::Vector2D(3.0, 4.0);
  v3_2 = flux::Vector3D(1.0, 2.0, 2.0);
  TEST_CONDITION(flux::magnitude(v2_2) != 5.0, passed,
    "Vector2D magnitude not working properly")
  TEST_CONDITION(flux::magnitude(v3_2) != 3.0, passed,
    "Vector3D magnitude not working properly")
  TEST_CONDITION(flux::dot(v2_1, v2_2) != 7.0, passed,
    "Vector2D dot product not working properly")
  TEST_CONDITION(flux::dot(v3_1, v3_2) != 5.0, passed,
    "Vector3D dot product not working properly")
  TEST_CONDITION(flux::cross(v3_1, v3_2) != flux::Vector3D(0.0, -1.0, 1.0), passed,
    "Vector3D cross product not working properly")

  if (passed)
    printf("Vectors passed all tests!\n");

  return passed;
}

bool testComponentArray() {
  bool passed = true;
  printf("Testing ComponentArray ...\n");

  // test array setup
  flux::MemoryManager memory_manager(sizeof(float) * 3);
  flux::ComponentArray<float> arr;
  TEST_CONDITION(arr.claimMemory(&memory_manager, 4), passed,
                 "claimMemory claimed more than it should be able to\n");
  TEST_CONDITION(!arr.claimMemory(&memory_manager, 3), passed, "claimMemory failed\n");
  TEST_CONDITION(arr.claimMemory(&memory_manager, 1), passed,
                 "claimMemory should only be able to be called once\n");
  TEST_CONDITION(arr.size() != 0, passed, "size was incorrect for empty array\n")

  // filling array
  TEST_CONDITION(!arr.emplace(2.0f), passed, "failed to emplace first element\n")
  TEST_CONDITION(!arr.emplace(3.0f), passed, "failed to emplace second element\n")
  TEST_CONDITION(!arr.insert(0, 1.0f), passed, "failed to insert element\n")
  TEST_CONDITION(arr.emplace(0.0f), passed, "emplaced an element in full array\n")
  TEST_CONDITION(arr.size() != 3, passed, "size was incorrect for full array\n")

  // check contents of array
  TEST_CONDITION(arr.buffer_[0] != 1.0, passed, "first index of array not correct\n")
  TEST_CONDITION(arr.buffer_[1] != 2.0, passed, "second index of array not correct\n")
  TEST_CONDITION(arr.buffer_[2] != 3.0, passed, "thrid index of array not correct\n")

  // remove array elements
  TEST_CONDITION(!arr.remove(1), passed, "failed to remove middle element\n")
  TEST_CONDITION(arr.buffer_[1] != 3.0, passed, "wrong second element after remove\n")
  TEST_CONDITION(!arr.remove(0), passed, "failed to remove first element\n")
  TEST_CONDITION(arr.buffer_[0] != 3.0, passed, "wrong first element after remove\n")
  TEST_CONDITION(!arr.remove(0), passed, "failed to remove final element\n")
  TEST_CONDITION(arr.remove(0), passed, "removed element from empty list\n")

  if (passed)
    printf("ComponentArray passed all tests!\n");
  return passed;
}