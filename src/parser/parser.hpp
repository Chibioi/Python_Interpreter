// ----------------- COMPONENTS OF A PARSER ---------------
// 1. Grammar
// 2. Parser working memory (stack)
// 3. Abstract synthax tree
// 4. Error handling

#pragma once

#include "../lexer/token.hpp"
#include "./ast.hpp"
#include <initializer_list>
#include <string>
#include <vector>

class Parser {

public:
  Parser(const std::vector<Token> &tokens);
  ProgramNode *parse();

private:
  ProgramNode *parseProgram();
  AstNode *parseStmt();
  std::vector<AstNode *> parseStmtList();
  AstNode *parseSimpleStmt();
  AstNode *parsePrintStmt();
  AstNode *parseSuite();
  AstNode *parseIfStmt();
  AstNode *parseWhileStmt();
  AstNode *parseBreakStmt();
  AstNode *parseContinueStmt();
  AstNode *parsePassStmt();
  AstNode *parseReturnStmt();
  AstNode *parseAssignSimple();
  AstNode *parseAssign();
  AstNode *parseExpr();
  AstNode *parseConditionalExpr();
  AstNode *parseDisjunction();
  AstNode *parseConjunction();
  AstNode *parseInversion();
  AstNode *parseComparison();
  AstNode *parseBitwiseOr();
  AstNode *parseBitwiseXor();
  AstNode *parseBitwiseAnd();
  AstNode *parseShiftExpr();
  AstNode *parseFactor();
  AstNode *parseTerm();
  AstNode *parsePower();
  AstNode *parseUnary();
  AstNode *parseCall(AstNode *func_name);
  AstNode *parsePrimary();
  AstNode *parseAtom();
  std::vector<AstNode *> parseParameterList();
  AstNode *parseFunctionDef();
  AstNode *parseClassDef();

  const std::vector<Token> &tokens;
  int current = 0;

  bool isAtEnd() const;
  Token consume(TokenType type);
  bool match(TokenType type);
  bool match(std::initializer_list<TokenType> types);
  void error(const std::string &message);
  Token peek() const;
  Token previous() const;
  Token advance();

  int isInsideLoop = 0;
  int isInsideFunc = 0;
};
