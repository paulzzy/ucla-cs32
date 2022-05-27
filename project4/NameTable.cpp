#include "NameTable.h"
#include <list>
#include <string>
#include <vector>

class NameTableImpl {
public:
  NameTableImpl();
  ~NameTableImpl();
  void enterScope();
  bool exitScope();
  bool declare(const std::string &id, int lineNum);
  int find(const std::string &id) const;
  // Prevent a NameTable object from being copied, assigned, or moved
  NameTableImpl(const NameTableImpl &) = delete;
  NameTableImpl &operator=(const NameTableImpl &) = delete;
  NameTableImpl(NameTableImpl &&) = delete;
  NameTableImpl &operator=(NameTableImpl &&) = delete;

private:
  std::vector<std::vector<int>> m_hash_table;
};

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable() : m_impl{new NameTableImpl} {}

NameTable::~NameTable() { delete m_impl; }

void NameTable::enterScope() { m_impl->enterScope(); }

bool NameTable::exitScope() { return m_impl->exitScope(); }

bool NameTable::declare(const std::string &id, int lineNum) {
  return m_impl->declare(id, lineNum);
}

int NameTable::find(const std::string &id) const { return m_impl->find(id); }
