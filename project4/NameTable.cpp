#include "NameTable.h"
#include <cstdint>
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
  static size_t hash(const std::string &input);

  struct IdentifierData {
    std::string identifier;
    int line;
    int scope;
  };
  int m_current_scope;
  std::vector<std::vector<IdentifierData>> m_hash_table;
};

const int HASH_TABLE_SIZE = 20000;
const int INITIAL_BUCKET_SIZE = 1;

NameTableImpl::NameTableImpl()
    : m_hash_table{HASH_TABLE_SIZE,
                   std::vector<IdentifierData>{INITIAL_BUCKET_SIZE,
                                               {
                                                   "", // identifier
                                                   0,  // line
                                                   0   // scope
                                               }}},
      m_current_scope{0} {}

NameTableImpl::~NameTableImpl() {}

// Implements the Fowler–Noll–Vo-1a hash function, produces 32-bit outputs
size_t NameTableImpl::hash(const std::string &input) {
  const u_int32_t FNV_OFFSET_BASIS = 2166136261;
  const u_int32_t FNV_PRIME = 16777619;

  u_int32_t hash_value{FNV_OFFSET_BASIS};

  for (unsigned char char_byte : input) {
    hash_value ^= char_byte;
    hash_value *= FNV_PRIME;
  }

  return hash_value % HASH_TABLE_SIZE;
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
