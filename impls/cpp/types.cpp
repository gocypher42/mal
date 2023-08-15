#include <string>

#include "types.h"

using std::string;

string MalInteger::to_string() const { return std::to_string(m_value); }

string MalSymbol::to_string() const {
  if (m_symbol == "~@") {
    return "splice-quote";
  }

  char c = m_symbol.at(0);
  switch (c) {
  case '\'':
    return "quote";
  case '`':
    return "quasiquote";
  case '~':
    return "unquote";
  default:
    break;
  }

  return m_symbol;
}

void MalList::push_back(unique_ptr<MalValue> item) {
  m_items.push_back(std::move(item));
}

size_t MalList::size() const { return m_items.size(); }

#include <iostream>

string MalList::to_string() const {
  string repr = "(";
  for (const unique_ptr<MalValue> &value : m_items) {
    repr.append(value->to_string()).append(" ");
  }
  repr.pop_back();
  repr.append(")");
  return repr;
}

unique_ptr<MalValue> MalList::pop_front() {
  if (m_items.empty()) {
    return nullptr;
  }
  unique_ptr<MalValue> value = std::move(m_items[0]);
  for (size_t i = 1; i < m_items.size(); i++) {
    m_items[i - 1] = std::move(m_items[i]);
  }
  m_items.resize(m_items.size() - 1);
  return value;
}


