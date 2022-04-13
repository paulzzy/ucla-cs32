#include "LevelList.h"
#include "Sequence.h"

LevelList::LevelList() {}

bool LevelList::add(unsigned long level) {
  if (level < 30 || level > 400) {
    return false;
  }

  // Insert `level` such that `m_sequence` is sorted in ascending order
  return m_sequence.insert(level) != -1;
}

bool LevelList::remove(unsigned long level) {
  const int position = m_sequence.find(level);
  if (position != -1) {
    m_sequence.erase(position);
    return true;
  } else {
    return false;
  }
}

int LevelList::size() const { return m_sequence.size(); }

unsigned long LevelList::minimum() const {
  if (m_sequence.empty()) {
    return NO_LEVEL;
  }

  unsigned long min_value = 0;
  m_sequence.get(0, min_value);

  return min_value;
}

unsigned long LevelList::maximum() const {
  if (m_sequence.empty()) {
    return NO_LEVEL;
  }

  unsigned long max_value = 0;
  m_sequence.get(m_sequence.size() - 1, max_value);

  return max_value;
}
