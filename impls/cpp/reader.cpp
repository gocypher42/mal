#include <memory>
#include <string>
#include <vector>

#include "reader.h"
#include "types.h"

using std::size_t;
using std::string;
using std::unique_ptr;
using std::vector;

Reader::Reader(const vector<string>& tokens) : m_tokens(tokens), m_position(0)
{
}

string Reader::peek() const { return m_tokens[m_position]; }

string Reader::next()
{
  string token = peek();
  m_position++;
  return token;
}

bool Reader::eof() const { return m_position + 1 > m_tokens.size(); }

unique_ptr<MalValue> read_str(const string& input)
{
  vector<string> tokens = tokenize(input);
  // for (auto& token : tokens)
  // {
  //   std::cout << "[" << token << "]\n";
  // }
  Reader reader(tokens);
  if (reader.eof())
  {
    return nullptr;
  }
  return read_form(reader);
}

vector<string> tokenize(const string& input)
{
  vector<string> tokens;

  bool done = false;
  size_t index = 0;
  while (!done && index < input.size())
  {
    switch (input.at(index))
    {
      case ' ':
      case '\t':
      case '\n':
      case ',':
        break;
      case '~':
        if (input.at(index + 1) == '@')
        {
          tokens.push_back(input.substr(index, 2));
          index++;
        }
        else
        {
          tokens.push_back(input.substr(index, 1));
        }
        break;
      case '(':
      case ')':
      case '[':
      case ']':
      case '{':
      case '}':
      case '\'':
      case '`':
      case '^':
      case '@':
        tokens.push_back(input.substr(index, 1));
        break;
      case '"': {
        string token;
        size_t count = 1;

        while (true)
        {
          if (index + count >= input.size())
          {
            throw string("unbalanced");
          }

          if (input.at(index + count) == '\\')
          {
            token += input.at(index + count);
            count++;
            token += input.at(index + count);
            count++;
            continue;
          }

          if (input.at(index + count) == '"')
          {
            break;
          }

          token += input.at(index + count);
          count++;
        }

        tokens.push_back('"' + token + '"');
        index += count;
        break;
      }
      case ';':
        tokens.push_back(input.substr(index, input.size() - index));
        done = true;
        break;
      default: {
        size_t count = 1;

        bool end_str = false;
        while (!end_str && index + count < input.size())
        {
          switch (input.at(index + count))
          {
            case ' ':
            case '\t':
            case '\n':
            case ',':
            case '~':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case '`':
            case '\'':
            case '^':
            case '@':
            case '"':
            case ';':
              end_str = true;
              break;
            default:
              count++;
              break;
          }
        }

        tokens.push_back(input.substr(index, count));

        index += count;
        index--;
        break;
      }
    }
    index++;
  }

  return tokens;
}

unique_ptr<MalValue> read_form(Reader& reader)
{
  string token = reader.peek();

  switch (token.at(0))
  {
    case '[':
      reader.next();
      return read_list(reader, "]");
      break;
    case '{':
      reader.next();
      return read_list(reader, "}");
      break;
    case '(':
      reader.next();
      return read_list(reader, ")");
    default:
      break;
  }

  return read_atom(reader);
}

unique_ptr<MalValue> read_list(Reader& reader, const string& end)
{
  ListWrapper wrapper;
  switch (end.at(0))
  {
    case ']':
      wrapper = ListWrapper::SquareBrackets;
      break;
    case '}':
      wrapper = ListWrapper::CurlyBraces;
      break;
    case ')':
    default:
      wrapper = ListWrapper::Parentheses;
      break;
  }
  auto list = unique_ptr<MalList>(new MalList(wrapper));
  while (true)
  {
    if (reader.eof())
    {
      throw string("unbalanced");
    }

    if (reader.peek() == end)
    {
      reader.next();
      break;
    }

    list->push_back(read_form(reader));
  }
  return list;
}

#include <iostream>

unique_ptr<MalValue> read_atom(Reader& reader)
{
  const string token = reader.next();

  if (token.at(0) == '^')
  {
    unique_ptr<MalList> list(new MalList(ListWrapper::Parentheses));
    list->push_back(unique_ptr<MalSymbol>(new MalSymbol(token)));
    list->push_back(read_form(reader));
    list->push_back(read_form(reader));
    return list;
  }

  if (token == "'" || token == "`" || token == "~" || token == "~@" ||
      token == "@")
  {
    unique_ptr<MalList> list(new MalList(ListWrapper::Parentheses));
    list->push_back(unique_ptr<MalSymbol>(new MalSymbol(token)));
    list->push_back(read_form(reader));

    return list;
  }

  unique_ptr<MalValue> atom;
  try
  {
    const long value = std::stol(token);
    atom.reset(new MalInteger(value));
  }
  catch (std::invalid_argument const& e)
  {
    atom.reset(new MalSymbol(token));
  }

  return atom;
}
