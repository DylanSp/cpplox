#pragma once

#include <string>
#include <string_view>

namespace lox {
enum class TokenType {
  // Single-character tokens.
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_COMMA,
  TOKEN_DOT,
  TOKEN_MINUS,
  TOKEN_PLUS,
  TOKEN_SEMICOLON,
  TOKEN_SLASH,
  TOKEN_STAR,

  // One or two character tokens.
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,

  // Literals.
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,

  // Keywords.
  TOKEN_AND,
  TOKEN_CLASS,
  TOKEN_ELSE,
  TOKEN_FALSE,
  TOKEN_FOR,
  TOKEN_FUN,
  TOKEN_IF,
  TOKEN_NIL,
  TOKEN_OR,
  TOKEN_PRINT,
  TOKEN_RETURN,
  TOKEN_SUPER,
  TOKEN_THIS,
  TOKEN_TRUE,
  TOKEN_VAR,
  TOKEN_WHILE,

  TOKEN_ERROR,
  TOKEN_EOF
};

struct Token {
  TokenType type;
  std::string_view lexeme;
  int line;
};

class Scanner {
private:
  std::string_view source;
  std::size_t startPosition; // beginning of lexeme being currently scanned
  std::size_t currentPosition;
  int line; // what line number current lexeme is on

  bool isAlpha(char c);
  bool isDigit(char c);
  bool isAtEnd();
  Token makeToken(TokenType type);
  Token errorToken(const std::string &message);
  char advance();
  bool match(char expected);
  void skipWhitespace();
  char peek();
  char peekNext();
  Token stringToken();
  Token numberToken();
  Token identifierToken();
  TokenType identifierType();
  TokenType checkKeyword(std::size_t prefixLength, std::string_view suffix,
                         TokenType type);

public:
  explicit Scanner(std::string_view source = "");

  Token scanToken();
};

} // namespace lox
