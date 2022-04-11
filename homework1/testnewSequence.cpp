#include "newSequence.h"
#include <cassert>
#include <iostream>

Sequence a(1000); // a can hold at most 1000 items
Sequence b(5);    // b can hold at most 5 items
Sequence c;       // c can hold at most DEFAULT_MAX_ITEMS items
ItemType v = 10;

int main() {
  // No failures inserting 5 items into b
  for (int k = 0; k < 5; k++) {
    assert(b.insert(v) != -1);
    b.dump();
  }

  // Failure if we try to insert a sixth item into b
  assert(b.insert(v) == -1);

  // When two Sequences' contents are swapped, their capacities are
  // swapped as well:
  a.swap(b);
  assert(a.insert(v) == -1 && b.insert(v) != -1);

  std::cerr << "ya passed" << std::endl;
}
