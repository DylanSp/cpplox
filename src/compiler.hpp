#pragma once

#include "chunk.hpp"
#include "scanner.hpp"
#include <functional>
#include <map>
#include <optional>
#include <string_view>

namespace lox {

// Lox precedence levels from lowest to highest
enum class Precedence {
  PREC_NONE,
  PREC_ASSIGNMENT, // =
  PREC_OR,         // or
  PREC_AND,        // and
  PREC_EQUALITY,   // == !=
  PREC_COMPARISON, // < > <= >=
  PREC_TERM,       // + -
  PREC_FACTOR,     // * /
  PREC_UNARY,      // ! -
  PREC_CALL,       // . ()
  PREC_PRIMARY
};

class Compiler; // forward declaration, full definition below
using ParseFn = std::function<void(Compiler &)>;

struct ParseRule {
  std::optional<ParseFn> prefix;
  std::optional<ParseFn> infix;
  Precedence precedence;
};

struct ParserState {
  Token current;
  Token previous;
  bool hadError{false};
  bool panicMode{false};
};

class Compiler {
private:
  ParserState parser;
  Scanner scanner;
  Chunk compilingChunk;

  Chunk &currentChunk();

  void endCompiler();

  void advance();
  void consume(TokenType expectedType, std::string_view errorMessage);

  void parsePrecedence(Precedence precedence);
  void expression();
  void grouping();
  void binaryOp();
  void unaryOp();
  void number();

  uint8_t makeConstant(Value value);

  void emitByte(uint8_t byte);
  void emitBytePair(uint8_t byte1, uint8_t byte2);
  void emitReturn();
  void emitConstant(Value value);

  void error(std::string_view message);
  void errorAt(Token token, std::string_view message);
  void errorAtCurrent(std::string_view message);

  std::map<TokenType, ParseRule> rules = {
      {{TokenType::TOKEN_LEFT_PAREN},
       {&Compiler::grouping, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_RIGHT_PAREN},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_LEFT_BRACE},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_RIGHT_BRACE},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_COMMA},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_DOT},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_MINUS},
       {&Compiler::unaryOp, &Compiler::binaryOp, Precedence::PREC_TERM}},
      {{TokenType::TOKEN_PLUS},
       {std::nullopt, &Compiler::binaryOp, Precedence::PREC_TERM}},
      {{TokenType::TOKEN_SEMICOLON},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_SLASH},
       {std::nullopt, &Compiler::binaryOp, Precedence::PREC_FACTOR}},
      {{TokenType::TOKEN_STAR},
       {std::nullopt, &Compiler::binaryOp, Precedence::PREC_FACTOR}},
      {{TokenType::TOKEN_BANG},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_BANG_EQUAL},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_EQUAL},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_EQUAL_EQUAL},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_GREATER},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_GREATER_EQUAL},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_LESS},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_LESS_EQUAL},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_IDENTIFIER},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_STRING},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_NUMBER},
       {&Compiler::number, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_AND},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_CLASS},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_ELSE},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_FALSE},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_FOR},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_FUN},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_IF},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_NIL},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_OR},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_PRINT},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_RETURN},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_SUPER},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_THIS},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_TRUE},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_VAR},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_WHILE},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_ERROR},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}},
      {{TokenType::TOKEN_EOF},
       {std::nullopt, std::nullopt, Precedence::PREC_NONE}}};

public:
  std::optional<Chunk> compile(std::string_view source);
};

} // namespace lox
