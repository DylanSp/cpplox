#include "compiler.hpp"
#include "scanner.hpp"
#include <string>

using lox::Compiler;

// abstraction because "current chunk" gets more complicated later;
// revisit when we get to user-defined functions
lox::Chunk &Compiler::currentChunk() { return compilingChunk; }

std::optional<lox::Chunk> Compiler::compile(std::string_view source) {
  scanner = Scanner(source);
  compilingChunk = Chunk();

  advance();
  expression();
  consume(TokenType::TOKEN_EOF, "Expected end of expression");
  endCompiler();

  if (parser.hadError) {
    return std::nullopt;
  }

  return compilingChunk;
}

void Compiler::endCompiler() {
  emitReturn();

#ifdef DEBUG_PRINT_CODE
  if (!parser.hadError) {
    currentChunk().disassemble("code");
  }
#endif

}

// scan and advance to the next non-error token,
// emitting errors if scanner emits them
void Compiler::advance() {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanner.scanToken();
    if (parser.current.type != TokenType::TOKEN_ERROR) {
      break;
    }

    errorAtCurrent(parser.current.lexeme);
  }
}

void Compiler::consume(TokenType expectedType, std::string_view errorMessage) {
  if (parser.current.type == expectedType) {
    advance();
    return;
  }

  errorAtCurrent(errorMessage);
}

void Compiler::error(std::string_view message) {
  errorAt(parser.previous, message);
}

void Compiler::errorAtCurrent(std::string_view message) {
  errorAt(parser.current, message);
}

void Compiler::errorAt(Token token, std::string_view message) {
  if (parser.panicMode) {
    return;
  }

  parser.panicMode = true;

  std::cerr << "[line " << token.line << "] Error";
  if (token.type == TokenType::TOKEN_EOF) {
    std::cerr << " at end";
  } else if (token.type == TokenType::TOKEN_ERROR) {
    // intentional no-op
  } else {
    std::cerr << " at " << token.lexeme;
  }

  std::cerr << ": " << message << "\n";
  parser.hadError = true;
}

// starting at the current token,
// parse any expression of given precedence level or higher
void Compiler::parsePrecedence(lox::Precedence precedence) {
  advance();
  auto prefixRule = rules.at(parser.previous.type).prefix;
  if (!prefixRule) {
    error("Expect expression.");
    return;
  }

  prefixRule.value()(*this);

  while (precedence <= rules.at(parser.current.type).precedence) {
    advance();
    auto infixRule = rules.at(parser.previous.type).infix;
    if (!infixRule) {
      // programming error; should be unreachable
      std::cerr << "Grammar error; infix parser expected, but none found.";
      return;
    }
    infixRule.value()(*this);
  }
}

void Compiler::expression() { parsePrecedence(Precedence::PREC_ASSIGNMENT); }

void Compiler::grouping() {
  expression();
  consume(TokenType::TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::binaryOp() {
  auto operatorType = parser.previous.type;
  auto rule = rules.at(operatorType);
  auto higherPrecedence = static_cast<Precedence>(1 + static_cast<int>(rule.precedence));
  parsePrecedence(higherPrecedence);

  switch (operatorType) {
  case TokenType::TOKEN_PLUS:
    emitByte(OpCode::OP_ADD);
    break;
  case TokenType::TOKEN_MINUS:
    emitByte(OpCode::OP_SUBTRACT);
    break;
  case TokenType::TOKEN_STAR:
    emitByte(OpCode::OP_MULTIPLY);
    break;
  case TokenType::TOKEN_SLASH:
    emitByte(OpCode::OP_DIVIDE);
    break;
  default:
    return; // should be unreachable
  }
}

void Compiler::unaryOp() {
  auto operatorType = parser.previous.type;

  // compile the operand
  parsePrecedence(Precedence::PREC_UNARY);

  // emit instruction for operator
  switch (operatorType) {
  case TokenType::TOKEN_MINUS:
    emitByte(OpCode::OP_NEGATE);
    break;
  default:
    return; // should be unreachable
  }
}

void Compiler::number() {
  auto value = std::stod(std::string(parser.previous.lexeme));
  emitConstant(value);
}

void Compiler::emitByte(uint8_t byte) {
  currentChunk().write(byte, parser.previous.line);
}

// most often used for writing opcode, followed by one-byte operand
void Compiler::emitBytePair(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

void Compiler::emitReturn() { emitByte(OpCode::OP_RETURN); }

void Compiler::emitConstant(Value value) {
  emitBytePair(OpCode::OP_CONSTANT, makeConstant(value));
}

uint8_t Compiler::makeConstant(Value value) {
  auto constantIndex = currentChunk().addConstant(value);
  if (constantIndex > UINT8_MAX) {
    error("Too many constants in one chunk.");
    return 0;
  }

  return static_cast<uint8_t>(constantIndex);
}