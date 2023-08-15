all: step0_repl step1_read_print

step0_repl: step0_repl.cpp
	g++ -O3 -Wall -Werror -Wextra  -std=c++11 -o step0_repl step0_repl.cpp

step1_read_print: step1_read_print.cpp reader.cpp types.cpp printer.cpp
	g++ -O3 -Wall -Werror -Wextra  -std=c++11 -o step1_read_print step1_read_print.cpp reader.cpp types.cpp printer.cpp
