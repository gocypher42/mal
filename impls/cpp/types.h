#pragma once

#include <memory>
#include <string>
#include <vector>

using std::size_t;
using std::string;
using std::unique_ptr;
using std::vector;

class MalValue
{
public:
  virtual ~MalValue() = default;
  virtual string to_string() const = 0;
};

class MalInteger : public MalValue
{
public:
  MalInteger(long value) : m_value(value) {}

  string to_string() const override;

private:
  long m_value;
};

class MalSymbol : public MalValue
{
public:
  MalSymbol(string symbol) : m_symbol(std::move(symbol)) {}

  string to_string() const override;

private:
  string m_symbol;
};

enum class ListWrapper
{
  Parentheses,
  CurlyBraces,
  SquareBrackets,
};

class MalList : public MalValue
{
public:
  MalList(ListWrapper wrapper) : m_wrapper(wrapper) {}

  size_t size() const;
  string to_string() const;

  unique_ptr<MalValue> pop_front();
  void push_back(unique_ptr<MalValue> item);

private:
  ListWrapper m_wrapper;
  vector<unique_ptr<MalValue>> m_items;
};
