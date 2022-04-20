#include "Sequence.h"
#include <iostream>
#include <sstream>

Sequence::Sequence() : m_size(0), sentinel_head(new Node) {
  sentinel_head->next = sentinel_head;
  sentinel_head->prev = sentinel_head;
}

Sequence::~Sequence() {
  Node *current_node = sentinel_head->next;

  while (current_node != sentinel_head) {
    Node *next_node = current_node->next;
    delete current_node;
    current_node = next_node;
  }

  // delete current_node;
  delete sentinel_head;
}

Sequence::Sequence(const Sequence &other)
    : m_size(other.m_size), sentinel_head(new Node) {
  Node *current_node = sentinel_head;
  Node *prev_node = sentinel_head;
  Node *other_current = other.sentinel_head;

  while (other_current->next != other.sentinel_head) {
    // Move to the next node
    other_current = other_current->next;
    current_node->next = new Node;
    current_node = current_node->next;

    // Copy value of current node in `other`
    current_node->value = other_current->value;

    // Set the `prev` node
    current_node->prev = prev_node;
    prev_node = current_node;
  }

  // Link `sentinel_head` to last node and vice versa
  // Makes the linked list circular
  current_node->next = sentinel_head;
  sentinel_head->prev = current_node;
}

Sequence &Sequence::operator=(Sequence other) {
  swap(other);
  return *this;
}

bool Sequence::empty() const { return m_size == 0; }

int Sequence::size() const { return m_size; }

int Sequence::insert(int pos, const ItemType &value) {
  if (!(pos >= 0 && pos <= m_size)) {
    return -1;
  }

  Node *current_node = sentinel_head;

  for (int i = 0; i < pos; i++) {
    current_node = current_node->next;
  }

  Node *new_node = new Node;
  new_node->value = value;

  current_node->next->prev = new_node;
  new_node->next = current_node->next;
  new_node->prev = current_node;
  current_node->next = new_node;

  m_size++;

  return pos;
}

int Sequence::insert(const ItemType &value) {
  Node *current_node = sentinel_head;

  for (int i = 0; i <= m_size; i++) {
    current_node = current_node->next;
    if (value <= current_node->value || current_node == sentinel_head) {
      return insert(i, value);
    }
  }

  return -1;
}

bool Sequence::erase(int pos) {
  if (!(pos >= 0 && pos < m_size && sentinel_head->next != sentinel_head)) {
    return false;
  }

  Node *current_node = sentinel_head;

  for (int i = 0; i <= pos; i++) {
    current_node = current_node->next;
  }

  current_node->prev->next = current_node->next;
  current_node->next->prev = current_node->prev;

  delete current_node;
  m_size--;

  return true;
}

int Sequence::remove(const ItemType &value) {
  Node *current_node = sentinel_head;
  int counter = 0;

  for (int i = 0; i < m_size; i++) {
    current_node = current_node->next;
    if (current_node->value == value) {
      current_node = current_node->prev;
      erase(i);
      counter++;
      i--; // Erasing a value replaces the current `pos` with a new value that
           // must also be checked
    }
  }

  return counter;
}

bool Sequence::get(int pos, ItemType &value) const {
  if (!(pos >= 0 && pos < m_size)) {
    return false;
  }

  Node *current_node = sentinel_head;

  for (int i = 0; i <= pos; i++) {
    current_node = current_node->next;
  }

  value = current_node->value;

  return true;
}

bool Sequence::set(int pos, const ItemType &value) {
  if (!(pos >= 0 && pos < m_size)) {
    return false;
  }

  Node *current_node = sentinel_head;

  for (int i = 0; i <= pos; i++) {
    current_node = current_node->next;
  }

  current_node->value = value;

  return true;
}

int Sequence::find(const ItemType &value) const {
  Node *current_node = sentinel_head;

  for (int i = 0; i < m_size; i++) {
    current_node = current_node->next;
    if (current_node->value == value) {
      return i;
    }
  }

  return -1;
}

void Sequence::swap(Sequence &other) {
  Node *temp_node = sentinel_head;
  sentinel_head = other.sentinel_head;
  other.sentinel_head = temp_node;

  int temp_m_size = m_size;
  m_size = other.m_size;
  other.m_size = temp_m_size;
}

void Sequence::dump() const {
  std::stringstream stringified;
  stringified << "[";

  Node *current_node = sentinel_head;

  if (current_node->next == sentinel_head) {
    stringified << "]";
  } else {
    for (int i = 0; i < m_size - 1; i++) {
      current_node = current_node->next;
      stringified << current_node->value << ", ";
    }
    stringified << current_node->next->value << "]";
  }

  std::cerr << stringified.str() << std::endl;
}

int subsequence(const Sequence &seq1, const Sequence &seq2) {
  int subseq_start = -1;
  int seq2_index = 0;

  for (int i = 0; i < seq1.size(); i++) {
    ItemType seq1_current;
    ItemType seq2_current;

    seq1.get(i, seq1_current);
    seq2.get(seq2_index, seq2_current);

    if (seq1_current == seq2_current && seq2_index == 0) {
      // Start of a new possible subsequence

      subseq_start = i;
      seq2_index++;
    } else if (seq1_current == seq2_current && seq2_index == seq2.size() - 1) {
      // When a subsequence is verified, return its starting index

      return subseq_start;
    } else if (seq1_current == seq2_current) {
      // Move to the next index of `seq2` when a possible subsequence has not
      // failed verification

      seq2_index++;
    } else {
      // Subsequence verification fails.

      subseq_start = -1;
      seq2_index = 0;
    }
  }

  return subseq_start;
}

void interleave(const Sequence &seq1, const Sequence &seq2, Sequence &result) {
  const Sequence *longer = (seq1.size() > seq2.size()) ? &seq1 : &seq2;
  const Sequence *shorter = (seq1.size() > seq2.size()) ? &seq2 : &seq1;

  // Alternate insert until the shorter Sequence is exhausted
  for (int i = 0; i < shorter->size(); i++) {
    ItemType seq1_value;
    seq1.get(i, seq1_value);
    result.insert(i * 2, seq1_value);

    ItemType seq2_value;
    seq2.get(i, seq2_value);
    result.insert(i * 2 + 1, seq2_value);
  }

  // Insert the rest of the longer Sequence
  const int offset = shorter->size();
  for (int j = shorter->size(); j < longer->size(); j++) {
    ItemType value;
    longer->get(j, value);
    result.insert(j + offset, value);
  }
}
