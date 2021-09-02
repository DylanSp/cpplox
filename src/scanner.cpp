#include "scanner.hpp"
#include <string>

using lox::Scanner;

lox::Token Scanner::scanToken() {
  skipWhitespace();
  startPosition = currentPosition;

  if (isAtEnd()) {
    return makeToken(TokenType::TOKEN_EOF);
  }

  auto c = advance();

  if (isAlpha(c)) {
    return identifierToken();
  }

  if (isDigit(c)) {
    return numberToken();
  }

  switch (c) {
  case '(':
    return makeToken(TokenType::TOKEN_LEFT_PAREN);
  case ')':
    return makeToken(TokenType::TOKEN_RIGHT_PAREN);
  case '{':
    return makeToken(TokenType::TOKEN_LEFT_BRACE);
  case '}':
    return makeToken(TokenType::TOKEN_RIGHT_BRACE);
  case ';':
    return makeToken(TokenType::TOKEN_SEMICOLON);
  case ',':
    return makeToken(TokenType::TOKEN_COMMA);
  case '.':
    return makeToken(TokenType::TOKEN_DOT);
  case '-':
    return makeToken(TokenType::TOKEN_MINUS);
  case '+':
    return makeToken(TokenType::TOKEN_PLUS);
  case '/':
    return makeToken(TokenType::TOKEN_SLASH);
  case '*':
    return makeToken(TokenType::TOKEN_STAR);
  case '!':
    return makeToken(match('=') ? TokenType::TOKEN_BANG_EQUAL
                                : TokenType::TOKEN_BANG);
  case '=':
    return makeToken(match('=') ? TokenType::TOKEN_EQUAL_EQUAL
                                : TokenType::TOKEN_EQUAL);
  case '<':
    return makeToken(match('=') ? TokenType::TOKEN_LESS_EQUAL
                                : TokenType::TOKEN_LESS);
  case '>':
    return makeToken(match('=') ? TokenType::TOKEN_GREATER_EQUAL
                                : TokenType::TOKEN_GREATER);
  case '"':
    return stringToken();
  }

  return errorToken("Unexpected character.");
}

char Scanner::advance() {
  auto currentChar = source.at(currentPosition);
  currentPosition++;
  return currentChar;
}

bool Scanner::match(char expected) {
  if (isAtEnd()) {
    return false;
  }

  if (source.at(currentPosition) != expected) {
    return false;
  }

  currentPosition++;
  return true;
}

void Scanner::skipWhitespace() {
  for (;;) {
    auto c = peek();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;
    case '\n':
      line++;
      advance();
      break;
    case '/':
      if (peekNext() == '/') {
        while (peek() != '\n' &&
               !isAtEnd()) { // Lox comment goes until the end of the line
          advance();
        }
      } else {
        return;
      }
      break;
    default:
      return;
    }
  }
}

char Scanner::peek() { return source.at(currentPosition); }

char Scanner::peekNext() {
  if (isAtEnd()) {
    return '\0';
  }

  return source.at(currentPosition + 1);
}

bool Scanner::isAtEnd() { return currentPosition >= source.length(); }

bool Scanner::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool Scanner::isDigit(char c) { return c >= '0' && c <= '9'; }

lox::Token Scanner::makeToken(lox::TokenType type) {
  return {type, source.substr(startPosition, currentPosition - startPosition),
          line};
}

lox::Token Scanner::errorToken(const std::string &message) {
  return {TokenType::TOKEN_ERROR, message, line};
}

// don't convert lexeme to runtime value here; will do later in pipeline
lox::Token Scanner::stringToken() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') {
      line++;
    }
    advance();
  }

  if (isAtEnd()) {
    return errorToken("Unterminated string");
  }

  // closing quote
  advance();
  return makeToken(TokenType::TOKEN_STRING);
}

// as with stringToken(), don't parse lexeme to runtime value here
lox::Token Scanner::numberToken() {
  while (isDigit(peek())) {
    advance();
  }

  // check for decimal part
  if (peek() == '.' && isDigit(peekNext())) {
    advance(); // consume '.'

    while (isDigit(peek())) {
      advance();
    }
  }

  return makeToken(TokenType::TOKEN_NUMBER);
}

lox::Token Scanner::identifierToken() {
  while (isAlpha(peek()) || isDigit(peek())) {
    advance();
  }

  return makeToken(identifierType());
}

// todo replace with some sort of trie data structure?
// https://www.techiedelight.com/cpp-implementation-trie-data-structure/
// https://craftinginterpreters.com/scanning-on-demand.html#tries-and-state-machines
lox::TokenType Scanner::identifierType() {
  switch (source.at(startPosition)) {
  case 'a':
    return checkKeyword(1, "nd", TokenType::TOKEN_AND);
  case 'c':
    return checkKeyword(1, "lass", TokenType::TOKEN_CLASS);
  case 'e':
    return checkKeyword(1, "lse", TokenType::TOKEN_ELSE);
  case 'f':
    if (currentPosition - startPosition > 1) {
      switch (source.at(startPosition + 1)) {
      case 'a':
        return checkKeyword(2, "lse", TokenType::TOKEN_FALSE);
      case 'o':
        return checkKeyword(2, "r", TokenType::TOKEN_FOR);
      case 'n':
        return checkKeyword(2, "n", TokenType::TOKEN_FUN);
      default:
        // intentional no-op; indicates an identifier
        // lexeme starts with 'f' but doesn't continue with 'a', 'o', or 'n'
        break;
      }
    }
    break;
  case 'i':
    return checkKeyword(1, "f", TokenType::TOKEN_IF);
  case 'n':
    return checkKeyword(1, "il", TokenType::TOKEN_NIL);
  case 'o':
    return checkKeyword(1, "r", TokenType::TOKEN_OR);
  case 'p':
    return checkKeyword(1, "rint", TokenType::TOKEN_PRINT);
  case 'r':
    return checkKeyword(1, "eturn", TokenType::TOKEN_RETURN);
  case 's':
    return checkKeyword(1, "uper", TokenType::TOKEN_SUPER);
  case 't':
    if (currentPosition - startPosition > 1) {
      switch (source.at(startPosition + 1)) {
      case 'h':
        return checkKeyword(2, "is", TokenType::TOKEN_THIS);
      case 'r':
        return checkKeyword(2, "ue", TokenType::TOKEN_TRUE);
      default:
        // intentional no-op; indicates an identifier
        // lexeme starts with 't' but doesn't continue with 'h' or 'r'
        break;
      }
    }
    break;
  case 'v':
    return checkKeyword(1, "ar", TokenType::TOKEN_VAR);
  case 'w':
    return checkKeyword(1, "hile", TokenType::TOKEN_WHILE);
  }

  return TokenType::TOKEN_IDENTIFIER;
}

// todo document this
// https://craftinginterpreters.com/scanning-on-demand.html#tries-and-state-machines
lox::TokenType Scanner::checkKeyword(std::size_t prefixLength,
                                     std::string_view suffix, TokenType type) {
  // first check - is scanned lexeme the same length as the keyword?
  // second check - is suffix of scanned lexeme equal to the keyword's suffix?
  if (currentPosition - startPosition == prefixLength + suffix.length() &&
      source.substr(startPosition + prefixLength,
                    currentPosition - startPosition - 1) == suffix) {
    return type;
  }

  return TokenType::TOKEN_IDENTIFIER;
}

lox::Scanner::Scanner(std::string_view source) {
  this->source = source;
  this->startPosition = 0;
  this->currentPosition = 0;
  this->line = 1;
};
