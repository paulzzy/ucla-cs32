#include "Sequence.h"
#include <iostream>
#include <sstream>

Sequence::Sequence() : m_size(0) {}

bool Sequence::empty() const { return m_size == 0; }

int Sequence::size() const { return m_size; }

int Sequence::insert(int pos, const ItemType &value) {
  if (!(pos >= 0 && pos <= m_size && m_size < DEFAULT_MAX_ITEMS)) {
    return -1;
  }

  // Starting with the rightmost value, move every value after `pos` right by
  // one to its new position
  // Note that when `pos` equals the previous `m_size`, nothing gets moved
  for (int i = pos; i < m_size; i++) {
    const int new_index = m_size - (i - pos);
    const int prev_index = new_index - 1;

    m_sequence[new_index] = m_sequence[prev_index];
  }
  m_size++;

  m_sequence[pos] = value;

  return pos;
}

int Sequence::insert(const ItemType &value) {
  if (!(m_size < DEFAULT_MAX_ITEMS)) {
    return -1;
  }

  if (m_size == 0) {
    return insert(0, value);
  }

  for (int pos = 0; pos < m_size; pos++) {
    const ItemType current_value = m_sequence[pos];
    if (value <= current_value) {
      return insert(pos, value);
    }
  }

  return -1;
}

bool Sequence::erase(int pos) {
  if (!(pos >= 0 && pos < m_size)) {
    return false;
  }

  if (pos == m_size - 1) {
    m_size--;

    return true;
  }

  for (int i = pos; i < m_size - 2; i++) {
    m_sequence[i] = m_sequence[i + 1];
  }
  m_size--;

  return true;
}

int Sequence::remove(const ItemType &value) {
  int counter = 0;
  for (int pos = 0; pos < m_size; pos++) {
    const ItemType current_value = m_sequence[pos];
    if (value == current_value) {
      erase(pos);
      counter++;
    }
  }

  return counter;
}

bool Sequence::get(int pos, ItemType &value) const {
  if (!(pos >= 0 && pos < m_size)) {
    return false;
  }

  value = m_sequence[pos];

  return true;
}

bool Sequence::set(int pos, const ItemType &value) {
  if (!(pos >= 0 && pos < m_size)) {
    return false;
  }

  m_sequence[pos] = value;

  return true;
}

int Sequence::find(const ItemType &value) const {
  for (int i = 0; i < m_size; i++) {
    const ItemType current_value = m_sequence[i];
    if (value == current_value) {
      return i;
    }
  }

  return -1;
}

void Sequence::swap(Sequence &other) {
  const bool this_is_shorter = m_size < other.size();
  const int original_other_size = other.size();
  const int common_size = this_is_shorter ? m_size : original_other_size;

  // Swap items up to the common size of each Sequence. For example, if
  // `this.size()` is 5 and `other.size()` is 10, 5 items will be swapped.
  for (int i = 0; i < common_size; i++) {
    ItemType temp = m_sequence[i];
    other.get(i, m_sequence[i]);
    other.set(i, temp);
  }

  // Extends and fills the shorter Sequence with items from the longer Sequence
  if (this_is_shorter) {
    m_size = original_other_size;

    for (int j = common_size; j < original_other_size; j++) {
      other.get(j, m_sequence[j]);
    }
  } else {
    for (int j = common_size; j < m_size; j++) {
      other.set(j, m_sequence[j]);
    }

    m_size = original_other_size;
  }
}

void Sequence::dump() const {
  std::stringstream stringified;
  stringified << "[";

  if (m_size == 0) {
    stringified << "]";
  } else {
    for (int i = 0; i < m_size - 1; i++) {
      stringified << m_sequence[i] << ", ";
    }
    stringified << m_sequence[m_size - 1] << "]";
  }

  std::cerr << stringified.str() << std::endl;
}
