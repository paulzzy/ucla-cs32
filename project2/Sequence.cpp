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
  if (!(pos >= 0 && pos <= m_size && sentinel_head->next != sentinel_head)) {
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

int remove(const ItemType &value) {
  (void)value;
  return 0;
}

bool get(int pos, ItemType &value) {
  (void)pos;
  (void)value;
  return false;
}

bool set(int pos, const ItemType &value) {
  (void)pos;
  (void)value;
  return false;
}

int find(const ItemType &value) {
  (void)value;
  return 0;
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
