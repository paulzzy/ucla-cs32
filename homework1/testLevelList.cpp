#include "LevelList.h"
#include "newSequence.h"
#include <cassert>
#include <iostream>

int main() {
  LevelList test;
  assert(test.add(300));
  assert(!test.add(29));
  assert(test.add(40));
  assert(test.add(100));
  assert(test.add(200));
  assert(test.remove(40));
  assert(!test.remove(99));
  assert(test.minimum() == 100);

  std::cerr << "ya passed wooo" << std::endl;
}
