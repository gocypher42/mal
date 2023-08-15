#pragma once

#include <string>
#include <vector>
#include <memory>

#include "types.h"

using std::size_t;
using std::string;
using std::vector;
using std::unique_ptr;

class Reader {
public:
  Reader(const vector<string> &tokens);

  string peek() const;
  string next();

  bool eof() const;

private:
  vector<string> m_tokens;
  size_t m_position;
};

vector<string> tokenize(const string &input);
unique_ptr<MalValue> read_str(const string &input);
unique_ptr<MalValue> read_form(Reader &reader);
unique_ptr<MalValue> read_list(Reader &reader, const string& end);
unique_ptr<MalValue> read_atom(Reader &reader);
