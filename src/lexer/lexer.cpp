// Lexer is what performs lexical analysis. It transforms sequence of characters
// (source code) into sequence of meaningful units (tokens). Tokens are the
// foundational units of synthax analysis. Each token represents a syntactic
// element, such as keywords, identifiers, literals, operators, and punctuation
// marks.
//
// ------ COMPONENTS OF A LEXER --------
// 1. Input buffer
// 2. Token
// 3. State
// 4. Regular expression
//

#include "./lexer.hpp"
#include "./inputbuffer.hpp"
#include "./tokentype.hpp"
#include "token.hpp"
#include <stdexcept>

// Constructor
Lexer::Lexer(InputBuffer &buffer)
    : input_buffer(buffer), current_state(LexerState::INITIAL),
      current_indentation(0), new_line(true) {
  indent_stack.push(0); // The base indentation level is 0
  initializeMaps();
}

void Lexer::initializeMaps() {
  // Populate the keyword map
  keyword_map["False"] = TokenType::KW_False;
  keyword_map["None"] = TokenType::KW_None;
  keyword_map["True"] = TokenType::KW_True;
  keyword_map["and"] = TokenType::KW_and;
  keyword_map["as"] = TokenType::KW_as;
  keyword_map["assert"] = TokenType::KW_assert;
  keyword_map["async"] = TokenType::KW_async;
  keyword_map["await"] = TokenType::KW_await;
  keyword_map["break"] = TokenType::KW_break;
  keyword_map["class"] = TokenType::KW_class;
  keyword_map["continue"] = TokenType::KW_continue;
  keyword_map["def"] = TokenType::KW_def;
  keyword_map["del"] = TokenType::KW_del;
  keyword_map["elif"] = TokenType::KW_elif;
  keyword_map["else"] = TokenType::KW_else;
  keyword_map["except"] = TokenType::KW_except;
  keyword_map["finally"] = TokenType::KW_finally;
  keyword_map["for"] = TokenType::KW_for;
  keyword_map["from"] = TokenType::KW_from;
  keyword_map["global"] = TokenType::KW_global;
  keyword_map["if"] = TokenType::KW_if;
  keyword_map["import"] = TokenType::KW_import;
  keyword_map["in"] = TokenType::KW_in;
  keyword_map["is"] = TokenType::KW_is;
  keyword_map["lambda"] = TokenType::KW_lambda;
  keyword_map["nonlocal"] = TokenType::KW_nonlocal;
  keyword_map["not"] = TokenType::KW_not;
  keyword_map["or"] = TokenType::KW_or;
  keyword_map["pass"] = TokenType::KW_pass;
  keyword_map["raise"] = TokenType::KW_raise;
  keyword_map["return"] = TokenType::KW_return;
  keyword_map["try"] = TokenType::KW_try;
  keyword_map["while"] = TokenType::KW_while;
  keyword_map["with"] = TokenType::KW_with;
  keyword_map["yield"] = TokenType::KW_yield;
  keyword_map["and"] = TokenType::KW_and;
  keyword_map["or"] = TokenType::KW_or;
  keyword_map["not"] = TokenType::KW_not;

  // Populate the operator map
  operator_map["+"] = OP_Plus;
  operator_map["-"] = OP_Minus;
  operator_map["*"] = OP_Star;
  operator_map["/"] = OP_Slash;
  operator_map["%"] = OP_Mod;
  operator_map["**"] = OP_DoubleStar;
  operator_map["//"] = OP_DoubleSlash;
  operator_map["=="] = OP_Equal;
  operator_map["!="] = OP_NotEqual;
  operator_map["<"] = OP_Less;
  operator_map["<="] = OP_LessEqual;
  operator_map[">"] = OP_Greater;
  operator_map[">="] = OP_GreaterEqual;
  operator_map["="] = OP_Assign;
  operator_map["+="] = OP_PlusAssign;
  operator_map["-="] = OP_MinusAssign;
  operator_map["*="] = OP_StarAssign;
  operator_map["/="] = OP_SlashAssign;
  operator_map["%="] = OP_ModAssign;
  operator_map["**="] = OP_DoubleStarAssign;
  operator_map["//="] = OP_DoubleSlashAssign;
  operator_map["|"] = OP_Pipe;
  operator_map["^"] = OP_Caret;
  operator_map["&"] = OP_Ampersand;
  operator_map["~"] = OP_Tilde;
  operator_map["<<"] = OP_LeftShift;
  operator_map[">>"] = OP_RightShift;
  operator_map["|="] = OP_PipeAssign;
  operator_map["^="] = OP_CaretAssign;
  operator_map["&="] = OP_AmpersandAssign;
  operator_map["<<="] = OP_LeftShiftAssign;
  operator_map[">>="] = OP_RightShiftAssign;
  operator_map["@"] = OP_At;
  operator_map["@="] = OP_AtAssign;
  operator_map[":"] = OP_Colon;

  // Punctuators

  punctuator_map["("] = PUNC_LeftParen;
  punctuator_map[")"] = PUNC_RightParen;
  punctuator_map["{"] = PUNC_LeftBrace;
  punctuator_map["}"] = PUNC_RightBrace;
  punctuator_map["["] = PUNC_LeftBracket;
  punctuator_map["]"] = PUNC_RightBracket;
  punctuator_map[","] = PUNC_Comma;
  punctuator_map[";"] = PUNC_Semicolon;
  punctuator_map["."] = PUNC_Dot;
}

TokenType Lexer::getKeywordOrIdentifierType(const std::string &lexeme) const {
  if (keyword_map.count(lexeme)) {
    return keyword_map.at(lexeme);
  }
  return TokenType::ID_Identifier;
}

// CONFUSED HERE

Token Lexer::process_identifier() {
  int start_line = input_buffer.get_line_number();
  int start_col = input_buffer.get_column_number();
  std::string lexeme;

  while (isAlnum(input_buffer.peek_char())) {
    lexeme += input_buffer.next_char();
  }

  return Token(TokenType::ID_Identifier, lexeme, start_line, start_col);
}

Token Lexer::getNextToken() {
  while (true) {
    char current_char = input_buffer.peek_char();

    if (new_line) {
      Token indentation_token = process_indentation();
      if (indentation_token.type != TokenType::T_Newline) {
        // If it's INDENT or DEDENT, return it first
        return indentation_token;
      }
    }

    new_line = false; // Reset the flag

    // Skip whitespace
    while (isWhitespace(current_char) && !isNewline(current_char)) {
      input_buffer.next_char();
      current_char = input_buffer.peek_char();
    }

    // Check for end of file
    if (input_buffer.is_eof()) {
      // Check for un-closed indentation levels at EOF
      if (indent_stack.size() > 1) {
        indent_stack.pop();
        return Token(TokenType::T_Dedent, "", input_buffer.get_line_number(),
                     input_buffer.get_column_number());
      }
      return Token(TokenType::T_EndOfFile, "", input_buffer.get_line_number(),
                   input_buffer.get_column_number());
    }

    // Determine the token type based on the current character
    if (isAlpha(current_char) || current_char == '_') {
      return process_identifier();
    }
    if (isDigit(current_char)) {
      return process_number();
    }
    if (current_char == '"' || current_char == '\'') {
      return process_string();
    }
    if (current_char == '#') {
      return process_comment();
    }
    if (current_char == '\n') {
      input_buffer.next_char();
      new_line = true; // Mark that we're on a new line for indentation check
      return Token(TokenType::T_Newline, "\n", input_buffer.get_line_number(),
                   input_buffer.get_column_number());
    }
    // ... (Add logic for single-character punctuators)
    if (current_char == '(') {
      input_buffer.next_char();
      return Token(TokenType::PUNC_LeftParen, "(",
                   input_buffer.get_line_number(),
                   input_buffer.get_column_number());
    }
    // ... and so on for all single-char tokens

    // Fallback for operators or unknown characters
    return process_operator();
  }
}
