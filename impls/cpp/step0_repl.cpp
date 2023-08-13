#include <iostream>
#include <string>

#include "linenoise.hpp"

using std::string;

string READ(const string &input) { return input; }

string EVAL(const string &input) { return input; }

string PRINT(const string &input) { return input; }

string rep(const string &input) {
  string input_read = READ(input);
  string input_eval = EVAL(input_read);
  string input_print = PRINT(input_eval);
  return input_print;
}

int main() {

  const string history_path = "history.txt";
  linenoise::LoadHistory(history_path.c_str());

  while (true) {
    const string prompt = "user> ";

    string user_input;
    bool quit = linenoise::Readline(prompt.c_str(), user_input);

    if (quit) {
      break;
    }

    std::cout << rep(user_input) << std::endl;
  }

  return 0;
}
