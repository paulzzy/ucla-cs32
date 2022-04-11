#include "Sequence.h"
#include <iostream>
#include <string>

Sequence::Sequence() : m_size(0) {}

bool Sequence::empty() const { return m_size == 0; }

int Sequence::size() const { return m_size; }

int Sequence::insert(int pos, const ItemType &value) {
  if (pos < 0 || pos > m_size || m_size >= DEFAULT_MAX_ITEMS) {
    return -1;
  }

  // Starting with the rightmost item, move every item after `pos` right by one
  // Note that when `pos == m_size`, nothing gets moved
  for (int i = m_size; i > pos; i--) {
    m_sequence[i + 1] = m_sequence[i];
  }

  m_sequence[pos] = value;
  m_size++;

  return pos;
}

int Sequence::insert(const ItemType &value) {
  if (m_size >= DEFAULT_MAX_ITEMS) {
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
  if (pos < 0 || pos >= m_size) {
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
  if (pos < 0 || pos >= m_size) {
    return false;
  }

  value = m_sequence[pos];

  return true;
}

bool Sequence::set(int pos, const ItemType &value) {
  if (pos < 0 || pos >= m_size) {
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
  Sequence temp = Sequence();

  // Copy `this` Sequence into the `temp` Sequence
  for (int i = 0; i < m_size; i++) {
    temp.insert(temp.size(), m_sequence[i]);
  }

  // Override `this` Sequence with the `other` Sequence
  for (int i = 0; i < other.size(); i++) {
    ItemType current_value;
    other.get(i, current_value);
    m_sequence[i] = current_value;
  }
  m_size = other.size();

  // Override `other` Sequence
  for (int j = 0; j < m_size; j++) {
    ItemType current_value;
    temp.get(j, current_value);
    other.set(j, current_value);
  }

  // Trim `other` Sequence to have the same `size()` as `temp`
  if (other.size() > temp.size()) {
    for (int i = temp.size(); i < other.size(); i++) {
      other.erase(i);
    }
  }
}

void Sequence::dump() const {
  std::string stringified = "[";

  for (int i = 0; i < m_size - 1; i++) {
    stringified.append(m_sequence[i] + ", ");
  }
  stringified.append(m_sequence[m_size] + "]");

  std::cerr << stringified << std::endl;
}
