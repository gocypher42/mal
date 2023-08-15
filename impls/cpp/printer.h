#pragma once

#include <memory>
#include <string>

#include "types.h"

using std::string;
using std::unique_ptr;

class Printer {
public:
  static string pr_str(unique_ptr<MalValue> value);

private:
};
