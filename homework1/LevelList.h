#ifndef LEVEL_LIST_H
#define LEVEL_LIST_H

#include "Sequence.h"
#include <limits>

const unsigned long NO_LEVEL = std::numeric_limits<unsigned long>::max();

class LevelList {
public:
  // Create an empty level list.
  LevelList();

  // If the level is valid (a value from 30 to 400) and the level list
  // has room for it, add it to the level list and return true.
  // Otherwise, leave the level list unchanged and return false.
  bool add(unsigned long level);

  // Remove one instance of the specified level from the level list.
  // Return true if a level was removed; otherwise false.
  bool remove(unsigned long level);

  // Return the number of levels in the list.
  int size() const;

  // Return the lowest-valued level in the level list.  If the list is
  // empty, return NO_LEVEL.
  unsigned long minimum() const;

  // Return the highest-valued level in the level list.  If the list is
  // empty, return NO_LEVEL.
  unsigned long maximum() const;

private:
  Sequence m_sequence;
};

#endif // LEVEL_LIST_H
