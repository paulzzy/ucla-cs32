#include "Sequence.h"

Sequence::Sequence() : sentinel_head(new Node), size(0) {
  sentinel_head->next = sentinel_head;
  sentinel_head->prev = sentinel_head;
}

Sequence::~Sequence() {
  Node *current = sentinel_head;
  Node *next_node = sentinel_head;

  while (current->next != current->next) {
    next_node = current->next;
    delete current;
    current = next_node;
  }

  delete current;
}

Sequence::Sequence(const Sequence &that)
    : sentinel_head(new Node), size(that.size) {
  sentinel_head = that.sentinel_head;
  Node *current = that.sentinel_head->next;
  Node *prev_node = that.sentinel_head->next;
  Node *current_that = that.sentinel_head->next;

  while (current_that->next != current_that->next) {
    current->value = current_that->value;

    current = current_that->next;
    current_that = current_that->next;

    current->prev = prev_node;
    prev_node = current;
  }

  current->next = sentinel_head;
  sentinel_head->prev = current;
}

Sequence &Sequence::operator=(Sequence that) {
  swap(that);
  return *this;
}

void Sequence::swap(Sequence &other) {
  Node *temp_node = sentinel_head;
  sentinel_head = other.sentinel_head;
  other.sentinel_head = temp_node;

  int temp_size = size;
  size = other.size;
  other.size = temp_size;
}
