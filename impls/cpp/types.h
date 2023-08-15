#pragma once

#include <memory>
#include <string>
#include <vector>

using std::size_t;
using std::string;
using std::unique_ptr;
using std::vector;

class MalValue {
public:
  virtual ~MalValue() = default;
  virtual string to_string() const = 0;
};

class MalInteger : public MalValue {
public:
  MalInteger(int value) : m_value(value) {}

  string to_string() const override;

private:
  size_t m_value;
};

class MalSymbol : public MalValue {
public:
  MalSymbol(string symbol) : m_symbol(std::move(symbol)) {}

  string to_string() const override;

private:
  string m_symbol;
};

class MalContainer : public MalValue {
public:
  virtual ~MalContainer() = default;
  virtual size_t size() const;
  virtual unique_ptr<MalValue> pop_front();
  virtual void push_back(unique_ptr<MalValue> item);
};

class MalList : public MalContainer {
public:
  MalList() = default;

  size_t size() const override;
  string to_string() const override;

  unique_ptr<MalValue> pop_front() override;
  void push_back(unique_ptr<MalValue> item) override;

private:
  vector<unique_ptr<MalValue>> m_items;
};

class MalVector : public MalContainer {
public:
  MalVector() = default;

  size_t size() const override;
  string to_string() const override;

  unique_ptr<MalValue> pop_front() override;
  void push_back(unique_ptr<MalValue> item) override;

private:
  vector<unique_ptr<MalValue>> m_items;
};
