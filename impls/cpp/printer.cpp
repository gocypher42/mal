#include <memory>
#include <string>

#include "printer.h"
#include "types.h"

using std::string;
using std::unique_ptr;

string Printer::pr_str(unique_ptr<MalValue> value) { return value->to_string(); }
