#include <string>

#include "types.h"

using std::string;

string MalInteger::to_string() const { return std::to_string(m_value); }

string MalSymbol::to_string() const
{
  if (m_symbol == "~@")
  {
    return "splice-unquote";
  }

  char c = m_symbol.at(0);
  switch (c)
  {
    case '\'':
      return "quote";
    case '`':
      return "quasiquote";
    case '~':
      return "unquote";
    case '^':
      return "with-meta";
    case '@':
      return "deref";
    default:
      break;
  }

  return m_symbol;
}

void MalList::push_back(unique_ptr<MalValue> item)
{
  m_items.push_back(std::move(item));
}

size_t MalList::size() const { return m_items.size(); }

string MalList::to_string() const
{
  std::string content;

  if (m_items.size() == 3 && m_items.at(0)->to_string() == "with-meta")
  {
    content.append(m_items.at(0)->to_string())
        .append(" ")
        .append(m_items.at(2)->to_string())
        .append(" ")
        .append(m_items.at(1)->to_string());
  }
  else
  {
    for (const unique_ptr<MalValue>& value : m_items)
    {
      content.append(value->to_string()).append(" ");
    }

    if (!content.empty())
    {
      content.pop_back();
    }
  }

  switch (m_wrapper)
  {
    case ListWrapper::Parentheses:
      return "(" + content + ")";
    case ListWrapper::CurlyBraces:
      return "{" + content + "}";
    case ListWrapper::SquareBrackets:
      return "[" + content + "]";
  }

  return "";
}

unique_ptr<MalValue> MalList::pop_front()
{
  if (m_items.empty())
  {
    return nullptr;
  }

  unique_ptr<MalValue> value = std::move(m_items[0]);

  for (size_t i = 1; i < m_items.size(); i++)
  {
    m_items[i - 1] = std::move(m_items[i]);
  }

  m_items.resize(m_items.size() - 1);

  return value;
}
