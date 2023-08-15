#include <iostream>
#include <memory>
#include <string>

#include "external/linenoise.hpp"

#include "reader.h"
#include "types.h"

using std::string;
using std::unique_ptr;

unique_ptr<MalValue> READ(const string &input) { return read_str(input); }

unique_ptr<MalValue> EVAL(unique_ptr<MalValue> input) { return input; }

string PRINT(unique_ptr<MalValue> input) { return input->to_string(); }

string rep(const string &input) { return PRINT(EVAL(READ(input))); }

int main() {

  const string history_path = ".mal_history.txt";
  linenoise::LoadHistory(history_path.c_str());

  while (true) {
    const string prompt = "user> ";

    string user_input;
    bool quit = linenoise::Readline(prompt.c_str(), user_input);

    if (quit) {
      break;
    }

    try {
      std::cout << rep(user_input) << std::endl;
    } catch (const string &e) {
      std::cout << e << std::endl;
    }
  }

  return 0;
}
