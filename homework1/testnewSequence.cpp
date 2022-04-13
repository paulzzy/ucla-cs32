#include "newSequence.h"
#include <cassert>
#include <iostream>
using namespace std;

int main() {
  Sequence s;
  // empty
  assert(s.empty());

  // insert with one parameter
  for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
    assert(s.insert(DEFAULT_MAX_ITEMS - 1 - i) == 0);
  }
  assert(s.insert(0) == -1);

  // find
  for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
    assert(s.find(i) == i);
  }

  // remove
  for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
    assert(s.remove(i) == 1);
  }
  assert(s.empty());

  // insert with two parameters
  for (int i = 0; i < DEFAULT_MAX_ITEMS - 1; i++) {
    assert(s.insert(i, 100) == i);
  }
  assert(s.insert(0, 100) == 0);
  assert(s.insert(0, 100) == -1);

  // Invalid insert (both functions)
  assert(s.insert(101) == -1);
  assert(s.insert(0, 101) == -1);
  assert(s.erase(0));
  assert(s.insert(-1, 101) == -1);
  assert(s.size() == DEFAULT_MAX_ITEMS - 1);

  // swap
  Sequence s_2;
  assert(s_2.insert(1) == 0);
  s.swap(s_2);
  assert(s.size() == 1);
  assert(s_2.insert(0, 101) == 0);
  assert(s_2.remove(100) == 179);

  Sequence s_3(3);
  assert(s_3.insert(1) == 0);
  assert(s_3.insert(3) == 1);
  assert(s_3.insert(2) == 1);
  s_2.swap(s_3);

  // Copy ctor
  Sequence s_4(s_2);

  // Assignment operator
  Sequence s_5 = s_4;

  std::cerr << "s ([1]): ";
  s.dump();
  std::cerr << "s_2 ([1, 2, 3]): ";
  s_2.dump();
  std::cerr << "s_3 ([101]): ";
  s_3.dump();
  std::cerr << "s_4 ([1, 2, 3]): ";
  s_2.dump();
  std::cerr << "s_5 ([1, 2, 3]): ";
  s_2.dump();
  std::cerr << std::endl;

  std::cerr << "heck yeah u passed newSequence" << std::endl;
}
