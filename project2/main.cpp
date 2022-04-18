#include "Sequence.h"
#include <cassert>
#include <iostream>

int main() {
  // `insert()` two parameter
  Sequence test_1;
  assert(test_1.insert(1) == 0);
  assert(test_1.insert(0) == 0);
  assert(test_1.insert(1) == 1);
  assert(test_1.insert(0, 2) == 0);
  assert(test_1.size() == 4);
  std::cerr << "test_1: ";
  test_1.dump();

  // `insert()` one parameter ascending
  Sequence test_2;
  for (int i = 0; i < 10; i++) {
    assert(test_2.insert(i) == i);
  }
  assert(test_2.size() == 10);
  std::cerr << "test_2: ";
  test_2.dump();

  // `insert()` one parameter descending
  Sequence test_3;
  for (int i = 0; i < 10; i++) {
    assert(test_3.insert(9 - i) == 0);
  }
  assert(test_3.size() == 10);
  std::cerr << "test_3: ";
  test_3.dump();

  // `erase()`
  Sequence test_4 = test_3;
  for (int i = 0; i < 10; i++) {
    assert(test_4.erase(0));
  }
  assert(test_4.size() == 0);
  assert(test_4.erase(0) == false);
  std::cerr << "test_4: ";
  test_4.dump();

  // `remove()`
  Sequence test_5;
  for (int i = 0; i < 10; i++) {
    assert(test_5.insert(0) == 0);
  }
  assert(test_5.remove(0) == 10);
  assert(test_5.size() == 0);
  std::cerr << "test_5: ";
  test_5.dump();

  // `get()` and `set()`
  Sequence test_6 = test_3;
  for (int i = 0; i < 10; i++) {
    ItemType value;
    assert(test_6.get(i, value));
    assert(value == i);
    assert(test_6.set(i, 10));
    assert(test_6.get(i, value));
    assert(value == 10);
  }
  std::cerr << "test_6: ";
  test_6.dump();

  // `find()`
  Sequence test_7(test_6);
  assert(test_7.find(10) == 0);
  test_7.dump();

  std::cerr << "uwu passed" << std::endl;
}
