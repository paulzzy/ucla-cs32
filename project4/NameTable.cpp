#include "NameTable.h"
#include <cstdint>
#include <functional>
#include <list>
#include <stack>
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
  static size_t calculate_hash(const std::string &identifier);
  static bool same_ids(const IdentifierData &id_1, const IdentifierData &id_2);

  struct IdentifierData {
    std::string identifier;
    int line;
    int scope;
  };

  int m_scope;
  std::stack<IdentifierData> m_in_scope;
  std::vector<std::vector<IdentifierData>> m_hash_table;
};

const int HASH_TABLE_SIZE = 20000;

NameTableImpl::NameTableImpl()
    : m_scope{0}, m_hash_table{HASH_TABLE_SIZE, std::vector<IdentifierData>{}} {
}

NameTableImpl::~NameTableImpl() {}

void NameTableImpl::enterScope() { m_scope++; }

bool NameTableImpl::exitScope() {
  if (m_scope == 0) {
    return false;
  }

  // Erase identifiers that go out of scope
  while (!m_in_scope.empty()) {
    auto current_id = m_in_scope.top();

    // Only erase identifiers in the current scope
    if (current_id.scope != m_scope) {
      break;
    }

    size_t hash_value{calculate_hash(current_id.identifier)};

    // Erase the out-of-scope identifier in the hash table
    auto &hash_bucket{m_hash_table.at(hash_value)};
    for (auto it_2 = hash_bucket.begin(); it_2 != hash_bucket.end(); it_2++) {
      if (same_ids(current_id, *it_2)) {
        hash_bucket.erase(it_2);
        break;
      }
    }

    m_in_scope.pop();
  }

  m_scope--;
  return true;
}

bool NameTableImpl::declare(const std::string &id, int line_num) {
  if (id.empty()) {
    return false;
  }

  IdentifierData id_data{id, line_num, m_scope};
  size_t hash_value = calculate_hash(id);

  // Check for already existing declarations in the same scope
  auto hash_bucket = m_hash_table.at(hash_value);
  for (const auto &identifier : hash_bucket) {
    if (identifier.identifier == id && identifier.scope == m_scope) {
      return false;
    }
  }

  m_in_scope.push(id_data);
  m_hash_table.at(hash_value).push_back(id_data);

  return true;
}

int NameTableImpl::find(const std::string &id) const {
  if (id.empty()) {
    return -1;
  }

  IdentifierData candidate{id, 0, m_scope};

  // Iterates over all scopes
  int closest_scope{-1};
  int line{-1};
  size_t hash_value{calculate_hash(id)};

  for (const auto &id_data : m_hash_table.at(hash_value)) {
    if (same_ids(id_data, candidate) && id_data.scope > closest_scope) {
      closest_scope = id_data.scope;
      line = id_data.line;
    }
  }

  return line;
}

size_t NameTableImpl::calculate_hash(const std::string &identifier) {
  return std::hash<std::string>{}(identifier) % HASH_TABLE_SIZE;
}

bool NameTableImpl::same_ids(const IdentifierData &id_1,
                             const IdentifierData &id_2) {
  return id_1.identifier == id_2.identifier && id_1.scope <= id_2.scope;
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
