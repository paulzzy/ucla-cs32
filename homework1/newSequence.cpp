#include "newSequence.h"
#include <iostream>
#include <sstream>

Sequence::Sequence()
    : m_size(0), m_max_size(DEFAULT_MAX_ITEMS),
      m_sequence(new ItemType[DEFAULT_MAX_ITEMS]) {}

Sequence::Sequence(int max_size)
    : m_size(0), m_max_size(max_size),
      m_sequence(max_size > 0 ? new ItemType[max_size] : new ItemType[0]) {}

Sequence::Sequence(const Sequence &that)
    : m_size(that.size()), m_max_size(that.m_max_size),
      m_sequence(new ItemType[that.size()]) {
  for (int i = 0; i < that.size(); i++) {
    that.get(i, m_sequence[i]);
  }
}

Sequence &Sequence::operator=(Sequence that) {
  swap(that);
  return *this;
}

Sequence::~Sequence() { delete[] m_sequence; }

bool Sequence::empty() const { return m_size == 0; }

int Sequence::size() const { return m_size; }

int Sequence::insert(int pos, const ItemType &value) {
  if (!(pos >= 0 && pos <= m_size && m_size < m_max_size)) {
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
  if (!(m_size < m_max_size)) {
    return -1;
  }

  if (m_size == 0) {
    return insert(0, value);
  }

  int pos = 0;
  for (; pos < m_size; pos++) {
    const ItemType current_value = m_sequence[pos];
    if (value <= current_value) {
      return insert(pos, value);
    }
  }

  // `value` is inserted at the end when it's bigger than everything else
  if (pos == m_size) {
    return insert(pos, value);
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

  for (int i = pos; i < m_size - 1; i++) {
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
      pos--; // Remain at the same `pos` to check the new value, which replaces
             // the erased one
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
  int temp_size = m_size;
  int temp_max_size = m_max_size;
  ItemType *temp_array = m_sequence;

  m_size = other.m_size;
  m_max_size = other.m_max_size;
  m_sequence = other.m_sequence;

  other.m_size = temp_size;
  other.m_max_size = temp_max_size;
  other.m_sequence = temp_array;
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
