#include "NameTable.h"
#include <cstdint>
#include <functional>
#include <list>
#include <string>
#include <vector>

class NameTableImpl {
public:
  NameTableImpl();
  ~NameTableImpl();
  void enterScope();
  bool exitScope();
  bool declare(const std::string &id, int line_num);
  int find(const std::string &id) const;
  // Prevent a NameTable object from being copied, assigned, or moved
  NameTableImpl(const NameTableImpl &) = delete;
  NameTableImpl &operator=(const NameTableImpl &) = delete;
  NameTableImpl(NameTableImpl &&) = delete;
  NameTableImpl &operator=(NameTableImpl &&) = delete;

private:
  struct IdentifierData;
  static size_t calculate_hash(const std::string &identifier, int scope);

  struct IdentifierData {
    std::string identifier;
    int line;
    int scope;
  };
  int m_current_scope;
  std::vector<std::vector<IdentifierData>> m_hash_table;
};

const int HASH_TABLE_SIZE = 20000;

NameTableImpl::NameTableImpl()
    : m_current_scope{0}, m_hash_table{HASH_TABLE_SIZE,
                                       std::vector<IdentifierData>{}} {}

NameTableImpl::~NameTableImpl() {}

void NameTableImpl::enterScope() { m_current_scope++; }

bool NameTableImpl::exitScope() {
  if (m_current_scope == 0) {
    return false;
  }

  // No need to remove identifiers that go out of scope, since hashing is
  // performed with both the identifier and scope.

  m_current_scope--;
  return true;
}

bool NameTableImpl::declare(const std::string &id, int line_num) {
  if (id.empty()) {
    return false;
  }

  IdentifierData id_data{id, line_num, m_current_scope};
  size_t hash_value = calculate_hash(id, m_current_scope);

  m_hash_table.at(hash_value).push_back(id_data);

  return true;
}

int NameTableImpl::find(const std::string &id) const {
  if (id.empty()) {
    return -1;
  }

  for (int scope{m_current_scope}; scope >= 0; scope--) {
    size_t hash_value{calculate_hash(id, scope)};

  for (const IdentifierData &id_data : m_hash_table.at(hash_value)) {
    if (id_data.identifier == id && id_data.scope == m_current_scope) {
      return id_data.line;
    }
  }

  return -1; // Identifier has no active declaration
}

size_t NameTableImpl::calculate_hash(const std::string &identifier, int scope) {
  std::string key{identifier + " " + std::to_string(scope)};

  return std::hash<std::string>{}(key) % HASH_TABLE_SIZE;
}

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
