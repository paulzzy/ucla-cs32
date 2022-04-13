#include "LevelList.h"
#include "Sequence.h"
#include <cassert>
#include <iostream>

int main() {
  LevelList l;
  assert(!l.add(29));
  assert(!l.add(401));

  for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
    assert(l.add(400 - i));
  }

  assert(l.minimum() == 400 - DEFAULT_MAX_ITEMS + 1);
  assert(l.maximum() == 400);
  assert(l.size() == DEFAULT_MAX_ITEMS);

  for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
    assert(l.remove(400 - i));
  }
  assert(l.size() == 0);

  std::cerr << "ya passed LevelList wooo" << std::endl;
}
