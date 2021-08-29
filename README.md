# cpplox

A C++ implementation of Lox, the language from [Bob Nystrom's Crafting Interpreters book](https://github.com/munificent/craftinginterpreters). This is a bytecode VM, aiming to match the `clox` test suite.

# Setting up dev tooling

## Compilation database setup

1. Install [bear](https://github.com/rizsotto/Bear).
1. Run `bear make debug` to generate compile_commands.json, used by Clang tooling such as clang-tidy.

# Running Tests

## Clox Test Suite

`cpplox` uses the `clox` test suite, copied from the Crafting Interpreters repo as of commit [6c2ea6f](https://github.com/munificent/craftinginterpreters/commit/6c2ea6f7192910053a78832f0cc34ad56b17ce7c). This test suite is run through the Dart test harness. To set this up:

1. Download and install Dart from https://dart.dev/get-dart.
2. Run `cd tool && dart pub get` to get the Dart dependendencies.

The tests can then be run with `make lox_tests`.
