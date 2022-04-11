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
  if (m_size < other.size()) {
    for (int i = 0; i < m_size; i++) {
      ItemType temp = m_sequence[i];
      other.get(i, m_sequence[i]);
      other.set(i, temp);
    }

    for (int j = m_size; j < other.size(); j++) {
      other.erase(j);
    }
  } else {
    int other_size = other.size();

    for (int i = 0; i < other.size(); i++) {
      ItemType temp = m_sequence[i];
      other.get(i, m_sequence[i]);
      other.set(i, temp);
    }

    m_size = other_size;
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
