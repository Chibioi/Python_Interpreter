#pragma once

#include "inputbuffer.hpp"
#include "token.hpp" // Use your updated Token class
#include <stack>
#include <string>
#include <unordered_map>
/*#include <unordered_set>*/

// Define Python-specific lexer states
enum class LexerState {
  INITIAL,
  IN_IDENTIFIER,
  IN_NUMBER,
  IN_STRING,
  IN_COMMENT,
  IN_OPERATOR,
  AWAITING_NEWLINE,
  END_OF_FILE,
  ERROR
};

class Lexer {
public:
  Lexer(InputBuffer &buffer);
  Token getNextToken();

private:
  InputBuffer &input_buffer;
  LexerState current_state;
  std::stack<int> indent_stack;
  int current_indentation;
  bool new_line;

  // Helper functions for character classification
  bool isAlpha(char c) const;
  bool isDigit(char c) const;
  bool isAlnum(char c) const;
  bool isWhitespace(char c) const;
  bool isNewline(char c) const;

  // Helper to get the token type for an operator string
  TokenType getOperatorType(const std::string &lexeme) const;

  // Helper to get the token type for an identifier or keyword
  TokenType getKeywordOrIdentifierType(const std::string &lexeme) const;

  // State-based processing functions
  Token process_initial_state();
  Token process_identifier();
  Token process_number();
  Token process_string();
  Token process_operator();
  Token process_comment();
  Token process_indentation();

  // Mappings for efficient lookup
  std::unordered_map<std::string, TokenType> keyword_map;
  std::unordered_map<std::string, TokenType> operator_map;
  std::unordered_map<std::string, TokenType> punctuator_map;

  void initializeMaps();
};
