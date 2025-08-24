#pragma once

#include "./tokentype.hpp"
#include <iostream>
#include <string>

class Token {

private:
  // Private members
  /*TokenType type;*/
  const std::string lexeme;
  int startline;
  int startcol;

public:
  TokenType type;
  // Constructor
  Token(TokenType type, const std::string &lexeme, int startline, int startcol)
      : type(type), lexeme(lexeme), startline(startline), startcol(startcol) {}

  friend std::ostream &operator<<(std::ostream &out, const Token &token) {

    // Use a helper function or a simple switch to print the token name
    auto print_token_type = [&out](TokenType type) {
      switch (type) {
      // Keywords
      case TokenType::KW_False:
        out << "KW_False";
        break;
      case TokenType::KW_None:
        out << "KW_None";
        break;
      case TokenType::KW_True:
        out << "KW_True";
        break;
      case TokenType::KW_and:
        out << "KW_and";
        break;
      case TokenType::KW_as:
        out << "KW_as";
        break;
      case TokenType::KW_assert:
        out << "KW_assert";
        break;
      case TokenType::KW_async:
        out << "KW_async";
        break;
      case TokenType::KW_await:
        out << "KW_await";
        break;
      case TokenType::KW_break:
        out << "KW_break";
        break;
      case TokenType::KW_class:
        out << "KW_class";
        break;
      case TokenType::KW_continue:
        out << "KW_continue";
        break;
      case TokenType::KW_def:
        out << "KW_def";
        break;
      case TokenType::KW_del:
        out << "KW_del";
        break;
      case TokenType::KW_elif:
        out << "KW_elif";
        break;
      case TokenType::KW_else:
        out << "KW_else";
        break;
      case TokenType::KW_except:
        out << "KW_except";
        break;
      case TokenType::KW_finally:
        out << "KW_finally";
        break;
      case TokenType::KW_for:
        out << "KW_for";
        break;
      case TokenType::KW_from:
        out << "KW_from";
        break;
      case TokenType::KW_global:
        out << "KW_global";
        break;
      case TokenType::KW_if:
        out << "KW_if";
        break;
      case TokenType::KW_import:
        out << "KW_import";
        break;
      case TokenType::KW_in:
        out << "KW_in";
        break;
      case TokenType::KW_is:
        out << "KW_is";
        break;
      case TokenType::KW_lambda:
        out << "KW_lambda";
        break;
      case TokenType::KW_nonlocal:
        out << "KW_nonlocal";
        break;
      case TokenType::KW_not:
        out << "KW_not";
        break;
      case TokenType::KW_or:
        out << "KW_or";
        break;
      case TokenType::KW_pass:
        out << "KW_pass";
        break;
      case TokenType::KW_raise:
        out << "KW_raise";
        break;
      case TokenType::KW_return:
        out << "KW_return";
        break;
      case TokenType::KW_try:
        out << "KW_try";
        break;
      case TokenType::KW_while:
        out << "KW_while";
        break;
      case TokenType::KW_with:
        out << "KW_with";
        break;
      case TokenType::KW_yield:
        out << "KW_yield";
        break;

      // Operators
      case TokenType::OP_Plus:
        out << "OP_Plus";
        break;
      case TokenType::OP_Minus:
        out << "OP_Minus";
        break;
      case TokenType::OP_Star:
        out << "OP_Star";
        break;
      case TokenType::OP_Slash:
        out << "OP_Slash";
        break;
      case TokenType::OP_Mod:
        out << "OP_Mod";
        break;
      case TokenType::OP_DoubleStar:
        out << "OP_DoubleStar";
        break;
      case TokenType::OP_DoubleSlash:
        out << "OP_DoubleSlash";
        break;
      case TokenType::OP_Equal:
        out << "OP_Equal";
        break;
      case TokenType::OP_NotEqual:
        out << "OP_NotEqual";
        break;
      case TokenType::OP_Less:
        out << "OP_Less";
        break;
      case TokenType::OP_LessEqual:
        out << "OP_LessEqual";
        break;
      case TokenType::OP_Greater:
        out << "OP_Greater";
        break;
      case TokenType::OP_GreaterEqual:
        out << "OP_GreaterEqual";
        break;
      case TokenType::OP_Assign:
        out << "OP_Assign";
        break;
      case TokenType::OP_PlusAssign:
        out << "OP_PlusAssign";
        break;
      case TokenType::OP_MinusAssign:
        out << "OP_MinusAssign";
        break;
      case TokenType::OP_StarAssign:
        out << "OP_StarAssign";
        break;
      case TokenType::OP_SlashAssign:
        out << "OP_SlashAssign";
        break;
      case TokenType::OP_ModAssign:
        out << "OP_ModAssign";
        break;
      case TokenType::OP_DoubleStarAssign:
        out << "OP_DoubleStarAssign";
        break;
      case TokenType::OP_DoubleSlashAssign:
        out << "OP_DoubleSlashAssign";
        break;
      case TokenType::OP_Pipe:
        out << "OP_Pipe";
        break;
      case TokenType::OP_Caret:
        out << "OP_Caret";
        break;
      case TokenType::OP_Ampersand:
        out << "OP_Ampersand";
        break;
      case TokenType::OP_Tilde:
        out << "OP_Tilde";
        break;
      case TokenType::OP_LeftShift:
        out << "OP_LeftShift";
        break;
      case TokenType::OP_RightShift:
        out << "OP_RightShift";
        break;
      case TokenType::OP_PipeAssign:
        out << "OP_PipeAssign";
        break;
      case TokenType::OP_CaretAssign:
        out << "OP_CaretAssign";
        break;
      case TokenType::OP_AmpersandAssign:
        out << "OP_AmpersandAssign";
        break;
      case TokenType::OP_LeftShiftAssign:
        out << "OP_LeftShiftAssign";
        break;
      case TokenType::OP_RightShiftAssign:
        out << "OP_RightShiftAssign";
        break;
      case TokenType::OP_At:
        out << "OP_At";
        break;
      case TokenType::OP_AtAssign:
        out << "OP_AtAssign";
        break;
      case TokenType::OP_Colon:
        out << "OP_Colon";
        break;

      // Punctuators
      case TokenType::PUNC_LeftParen:
        out << "PUNC_LeftParen";
        break;
      case TokenType::PUNC_RightParen:
        out << "PUNC_RightParen";
        break;
      case TokenType::PUNC_LeftBrace:
        out << "PUNC_LeftBrace";
        break;
      case TokenType::PUNC_RightBrace:
        out << "PUNC_RightBrace";
        break;
      case TokenType::PUNC_LeftBracket:
        out << "PUNC_LeftBracket";
        break;
      case TokenType::PUNC_RightBracket:
        out << "PUNC_RightBracket";
        break;
      case TokenType::PUNC_Comma:
        out << "PUNC_Comma";
        break;
      case TokenType::PUNC_Semicolon:
        out << "PUNC_Semicolon";
        break;
      case TokenType::PUNC_Dot:
        out << "PUNC_Dot";
        break;

      // Identifiers and Literals
      case TokenType::ID_Identifier:
        out << "ID_Identifier";
        break;
      case TokenType::LIT_Int:
        out << "LIT_Int";
        break;
      case TokenType::LIT_Float:
        out << "LIT_Float";
        break;
      case TokenType::LIT_String:
        out << "LIT_String";
        break;

      // Special Tokens
      case TokenType::T_Newline:
        out << "T_Newline";
        break;
      case TokenType::T_Indent:
        out << "T_Indent";
        break;
      case TokenType::T_Dedent:
        out << "T_Dedent";
        break;
      case TokenType::T_EndOfFile:
        out << "T_EndOfFile";
        break;

      // Error State
      case TokenType::T_Error:
        out << "T_Error";
        break;

      default:
        out << "Unknown";
        break;
      }
    };

    print_token_type(token.type);

    out << " <- " << "\'";
    // e.g., T_Plus <- '+'
    for (char c : token.lexeme) {
      if (c == '\n') {
        out << "\\n";
      } else {
        out << c;
      }
    }
    out << "\'";

    return out;
  }
};
