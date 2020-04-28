#include "core_tests.h"
#include "../core/flux_core.h"

int main() {
  if (!runDataStructuresTests() || !runCoreTests())
    exit(EXIT_FAILURE);

  flux::FluxCore flux_core;
  flux_core.run();
}