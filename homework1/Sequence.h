#ifndef SEQUENCE_H
#define SEQUENCE_H

using ItemType = unsigned long;

const int DEFAULT_MAX_ITEMS = 180;

class Sequence {
public:
  // Create an empty sequence (i.e., one whose size() is 0).
  Sequence();

  // Return true if the sequence is empty, otherwise false.
  bool empty() const;

  // Return the number of items in the sequence.
  int size() const;

  // Insert value into the sequence so that it becomes the item at
  // position pos.  The original item at position pos and those that
  // follow it end up at positions one greater than they were at before.
  // Return pos if 0 <= pos <= size() and the value could be
  // inserted.  (It might not be, if the sequence has a fixed capacity,
  // e.g., because it's implemented using a fixed-size array.)  Otherwise,
  // leave the sequence unchanged and return -1.  Notice that
  // if pos is equal to size(), the value is inserted at the end.
  int insert(int pos, const ItemType &value);

  // Let p be the smallest integer such that value <= the item at
  // position p in the sequence; if no such item exists (i.e.,
  // value > all items in the sequence), let p be size().  Insert
  // value into the sequence so that it becomes the item in position
  // p.  The original item at position p and those that follow it end
  // up at positions one greater than before.  Return p if the value
  // was actually inserted.  Return -1 if the value was not inserted
  // (perhaps because the sequence has a fixed capacity and is full).
  int insert(const ItemType &value);

  // If 0 <= pos < size(), remove the item at position pos from
  // the sequence (so that all items that followed that item end up at
  // positions one lower than they were at before), and return true.
  // Otherwise, leave the sequence unchanged and return false.
  bool erase(int pos);

  // Erase all items from the sequence that == value.  Return the
  // number of items removed (which will be 0 if no item == value).
  int remove(const ItemType &value);

  // If 0 <= pos < size(), copy into value the item at position pos
  // of the sequence and return true.  Otherwise, leave value unchanged
  // and return false.
  bool get(int pos, ItemType &value) const;

  // If 0 <= pos < size(), replace the item at position pos in the
  // sequence with value and return true.  Otherwise, leave the sequence
  // unchanged and return false.
  bool set(int pos, const ItemType &value);

  // Let p be the smallest integer such that value == the item at
  // position p in the sequence; if no such item exists, let p be -1.
  // Return p.
  int find(const ItemType &value) const;

  // Exchange the contents of this sequence with the other one.
  void swap(Sequence &other);

  void dump() const;

private:
  int m_size;
  ItemType m_sequence[DEFAULT_MAX_ITEMS];
};

#endif // SEQUENCE_H
