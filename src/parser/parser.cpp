#include "./parser.hpp"

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

ProgramNode *Parser::parse() { return parseProgram(); }

ProgramNode *Parser::parseProgram() {
  /*
   *   program ::= (statement | NEWLINE)* $
   */
  std::vector<AstNode *> statements;
  while (!isAtEnd()) {
    if (match(TokenType::T_Newline)) {
      continue;
    } else {
      statements.push_back(parseStmt());
    }
  }
  BlockNode *block = new BlockNode(statements);
  return new ProgramNode(block);
}

AstNode *Parser::parseStmt() {
  /*
   *    statement ::= stmt_list NEWLINE
   *                  | compound_stmt
   *
   *    compound_stmt ::= if_stmt
   *                      | while_stmt
   *                      | for_stmt      TODO
   *                      | try_stmt      TODO
   *                      | with_stmt     TODO
   *                      | funcdef
   *                      | classdef
   */
  if (match(TokenType::KW_if)) {
    return parseIfStmt();

  } else if (match(TokenType::KW_while)) {
    return parseWhileStmt();

  } else if (match(TokenType::KW_def)) {
    return parseFunctionDef();

  } else if (match(TokenType::KW_class)) {
    return parseClassDef();

  } else {
    std::vector<AstNode *> stmtList = parseStmtList();
    while (match(TokenType::T_Newline))
      continue;

    if (stmtList.size() == 1) {
      return stmtList[0];
    } else {
      return new BlockNode(stmtList);
    }
  }
}

std::vector<AstNode *> Parser::parseParameterList() {
  /*
   *   parameter_list ::= identifier ("," identifier)*    // keep simple for now
   */
  std::vector<AstNode *> parameters;
  do {
    Token tk = consume(TokenType::Name);
    AstNode *pname = new NameNode(tk);
    parameters.push_back(pname);
  } while (match(TokenType::PUNC_Comma));

  return parameters;
}

AstNode *Parser::parseFunctionDef() {
  /*
   *   funcdef ::= "def" funcname "(" (parameter_list)? ")" ":" suite
   *   funcname ::= identifier
   */
  Token fname = consume(TokenType::Name);
  consume(TokenType::PUNC_LeftParen);

  std::vector<AstNode *> parameters;

  if (peek().type != TokenType::PUNC_RightParen) {
    parameters = parseParameterList();
  }
  consume(TokenType::PUNC_RightParen);
  consume(TokenType::OP_Colon);

  isInsideFunc++;
  AstNode *body = parseSuite();
  isInsideFunc--;

  return new FunctionNode(fname, parameters, body);
}

AstNode *Parser::parseClassDef() {
  /*
   *   classdef  ::= "class" classname (inheritance)? ":" suite // TODO
   * inheritance classname ::= identifier
   */
  Token kname = consume(TokenType::Name);
  consume(TokenType::OP_Colon);
  AstNode *body = parseSuite();

  return new ClassNode(kname, body);
}

std::vector<AstNode *> Parser::parseStmtList() {
  /*
   *    stmt_list ::= simple_stmt (";" simple_stmt)* (";")?
   */
  std::vector<AstNode *> stmts;

  stmts.push_back(parseSimpleStmt());

  while (match(TokenType::PUNC_Semicolon)) {
    if (peek().type == TokenType::T_Newline)
      break;
    stmts.push_back(parseSimpleStmt());
  }
  if (peek().type == TokenType::PUNC_Semicolon)
    ++current;

  return stmts;
}

AstNode *Parser::parseSimpleStmt() {
  /*
   *  simple_stmt ::= assignment_stmt
   *                   | assert_stmt     TODO
   *                   | pass_stmt       TODO
   *                   | del_stmt        TODO
   *                   | print_stmt
   *                   | return_stmt
   *                   | yield_stmt      TODO
   *                   | raise_stmt      TODO
   *                   | break_stmt
   *                   | continue_stmt
   *                   | import_stmt     TODO
   *                   | global_stmt     TODO
   *
   */
  if (match(TokenType::KW_print)) {
    return parsePrintStmt();
  } else if (match(TokenType::KW_return)) {
    return parseReturnStmt();
  } else if (match(TokenType::KW_break)) {
    return parseBreakStmt();
  } else if (match(TokenType::KW_continue)) {
    return parseContinueStmt();
  } else if (match(TokenType::KW_pass)) {
    return parsePassStmt();
  } else {
    return parseAssign();
  }
}

AstNode *Parser::parseReturnStmt() {
  /*
   *  return_stmt ::= "return" (expression)?
   */
  if (!isInsideFunc)
    error("Return outside Function.");

  Token keyword = previous();
  AstNode *expr = match({TokenType::T_Newline, TokenType::PUNC_Semicolon})
                      ? nullptr
                      : parseExpr();
  return new ReturnNode(keyword, expr);
}

AstNode *Parser::parseBreakStmt() {
  /*
   *  break_stmt ::= "break"
   */
  if (!isInsideLoop)
    error("Break outside Loop.");

  Token keyword = previous();
  return new BreakNode(keyword);
}

AstNode *Parser::parseContinueStmt() {
  /*
   *  continue_stmt ::= "continue"
   */
  if (!isInsideLoop)
    error("Continue outside Loop.");

  Token keyword = previous();
  return new ContinueNode(keyword);
}

AstNode *Parser::parsePassStmt() {
  /*
   *  pass_stmt ::= "pass"
   */
  Token keyword = previous();
  return new PassNode(keyword);
}

AstNode *Parser::parsePrintStmt() {
  /*
   *    print_stmt ::= "print" "(" (expression)? ")"
   */
  consume(TokenType::PUNC_LeftParen);

  AstNode *expr = nullptr;

  if (peek().type != TokenType::PUNC_RightParen) {
    expr = parseExpr();
  }
  consume(TokenType::PUNC_RightParen);

  return new PrintNode(expr);
}

AstNode *Parser::parseSuite() {
  /*
   *    suite ::= NEWLINE* INDENT statement+ DEDENT
   *              | stmt_list NEWLINE
   */
  if (match(TokenType::T_Newline)) {

    // consumes '\n'
    while (match(TokenType::T_Newline))
      continue;

    consume(TokenType::T_Indent);

    std::vector<AstNode *> stmts;
    stmts.push_back(parseStmt());

    while (peek().type != TokenType::T_Dedent) {
      AstNode *node = parseStmt();

      if (node->is_block_node()) {
        BlockNode *block = node->unwrap_block_node();

        for (auto stmt : block->statements) {
          stmts.push_back(stmt);
        }
      } else {
        stmts.push_back(node);
      }
    }
    consume(TokenType::T_Dedent);

    return new BlockNode(stmts);

  } else {
    std::vector<AstNode *> stmtList = parseStmtList();
    consume(TokenType::T_Newline);
    return new BlockNode(stmtList);
  }
}

AstNode *Parser::parseIfStmt() {
  /*
   *    if_stmt ::= "if" expression ":" suite
   *                ("elif" expression ":" suite)*
   *                ("else" ":" suite)?
   */
  AstNode *cond = parseExpr();
  consume(TokenType::OP_Colon);
  AstNode *trueBranch = parseSuite();

  std::vector<std::pair<AstNode *, AstNode *>> elifBranches;
  while (match(TokenType::KW_elif)) {
    AstNode *elifCond = parseExpr();
    consume(TokenType::OP_Colon);
    AstNode *elifSuite = parseSuite();
    elifBranches.emplace_back(elifCond, elifSuite);
  }

  AstNode *elseBranch = nullptr;
  if (match(TokenType::KW_else)) {
    consume(TokenType::OP_Colon);
    elseBranch = parseSuite();
  }

  return new IfNode(cond, trueBranch, elifBranches, elseBranch);
}

AstNode *Parser::parseWhileStmt() {
  /*
   *   while_stmt ::= "while" expression ":" suite
   *                  ("else" ":" suite)?  //TODO
   */
  AstNode *cond = parseExpr();
  consume(TokenType::OP_Colon);
  isInsideLoop++;
  AstNode *body = parseSuite();
  isInsideLoop--;
  return new WhileNode(cond, body);
}

AstNode *Parser::parseAssignSimple() {
  /*
   *  assign_simple ::= expression ("=" assign_stmt)* // TODO target
   */
  AstNode *left = parseExpr();

  if (match(TokenType::OP_Equal)) {
    Token op = previous();
    AstNode *right = parseAssignSimple();
    return new AssignNode(left, right, op);
  }
  return left;
}

AstNode *Parser::parseAssign() {
  /*
   *  assign_stmt ::= assign_simple | augmented_assignment_stmt
   *  augmented_assignment_stmt ::= expression augop expression // TODO target,
   * expression_list augop ::= "+="  | "-=" ¦ "*=" | "/=" ¦ "%="  | "&=" | "|="
   * ¦ "^=" | "<<=" | ">>="
   */
  AstNode *left = parseAssignSimple();

  if (match({TokenType::PlusEqual, TokenType::MinusEqual, TokenType::StarEqual,
             TokenType::SlashEqual, TokenType::ModEqual, TokenType::AndEqual,
             TokenType::OrEqual, TokenType::XorEqual, TokenType::LeftShiftEqual,
             TokenType::RightShiftEqual})) {
    Token op = previous();
    AstNode *right = parseExpr();

    if (left->is_name_node() or left->is_property_node()) {
      return new AssignNode(left, right, op);
    } else {
      error("Invalid compound assignment.");
      return nullptr;
    }
  }
  return left;
}

AstNode *Parser::parseExpr() {
  /*
   *  expression ::= conditional_expression
   *                 | lambda_form // TODO
   */
  return parseConditionalExpr();
}

AstNode *Parser::parseConditionalExpr() {
  /*
   *  conditional_expression ::= disjunction ("if" disjunction "else"
   * expression)?
   */
  AstNode *left = parseDisjunction();

  if (match(TokenType::KW_if)) {
    AstNode *condition = parseDisjunction();
    consume(TokenType::KW_else);
    AstNode *right = parseExpr();
    left = new TernaryOpNode(condition, left, right);
  }
  return left;
}

AstNode *Parser::parseDisjunction() {
  /*
   *  disjunction ::= conjunction ("or" conjunction)*
   */
  AstNode *left = parseConjunction();

  while (match(TokenType::KW_or)) {
    Token op = previous();
    AstNode *right = parseConjunction();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

AstNode *Parser::parseConjunction() {
  /*
   *  conjunction ::= inversion ("and" inversion)*
   */
  AstNode *left = parseInversion();

  while (match(TokenType::KW_and)) {
    Token op = previous();
    AstNode *right = parseInversion();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

AstNode *Parser::parseInversion() {
  /*
   *   inversion ::= "not" inversion
   *                | comparison
   */
  if (match(TokenType::KW_not)) {
    Token op = previous();
    AstNode *right = parseInversion();
    return new UnaryOpNode(op, right);
  } else {
    return parseComparison();
  }
}

AstNode *Parser::parseComparison() {
  /*
   *   comparison ::= factor (comp_operator factor)*
   *   comp_operator ::= "<" | ">" | "==" | ">=" | "<=" | "!="
                         | "is" ("not")? | ("not")? "in"  // TODO
  */
  AstNode *left = parseBitwiseOr();

  while (match({TokenType::OP_Less, TokenType::OP_Greater,
                TokenType::OP_EqualEqual, TokenType::OP_GreaterEqual,
                TokenType::OP_LessEqual, TokenType::OP_NotEqual})) {
    Token op = previous();
    AstNode *right = parseBitwiseOr();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

AstNode *Parser::parseBitwiseOr() {
  /*
   *    bitwise_or ::= bitwise_xor ("|" bitwise_xor)*
   */
  AstNode *left = parseBitwiseXor();

  while (match(TokenType::OP_Pipe)) {
    Token op = previous();
    AstNode *right = parseBitwiseXor();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

AstNode *Parser::parseBitwiseXor() {
  /*
   *    bitwise_xor ::= bitwise_and ("^" bitwise_and)*
   */
  AstNode *left = parseBitwiseAnd();

  while (match(TokenType::OP_Caret)) {
    Token op = previous();
    AstNode *right = parseBitwiseAnd();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

AstNode *Parser::parseBitwiseAnd() {
  /*
   *   bitwise_and ::= shift_expr ("&" shift_expr)*
   */
  AstNode *left = parseShiftExpr();

  while (match(TokenType::OP_Ampersand)) {
    Token op = previous();
    AstNode *right = parseShiftExpr();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

AstNode *Parser::parseShiftExpr() {
  /*
   *   shift_expr ::= factor (("<<" | ">>") factor)*
   */
  AstNode *left = parseFactor();

  while (match({TokenType::OP_LeftShift, TokenType::OP_RightShift})) {
    Token op = previous();
    AstNode *right = parseFactor();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

AstNode *Parser::parseFactor() {
  /*
   *   factor ::= term (("+" | "-") term)*
   */
  AstNode *left = parseTerm();

  while (match({TokenType::OP_Plus, TokenType::OP_Minus})) {
    Token op = previous();
    AstNode *right = parseTerm();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

AstNode *Parser::parseTerm() {
  /*
   *   term ::= unary (("*" | "/" | "%") unary)*
   */
  AstNode *left = parseUnary();

  while (match({TokenType::OP_Star, TokenType::OP_Slash, TokenType::OP_Mod})) {
    Token op = previous();
    AstNode *right = parseUnary();
    left = new BinaryOpNode(left, op, right);
  }
  return left;
}

/*AstNode* Parser::parsePower() {
    /*
     *  power ::= unary ("**" power)*

    auto left = parseUnary();

    if (match(TokenType::DoubleStar)) {
        auto op = previous();
        auto right = parsePower();
        left = new BinaryOpNode(left, op.lexeme, right);
    }
    return left;
}*/

AstNode *Parser::parseUnary() {
  /*
   *   unary ::= (("+" | "-" | "~") unary) | primary
   */
  if (match({TokenType::OP_Minus, TokenType::OP_Tilde})) {
    Token op = previous();
    AstNode *right = parseUnary();
    return new UnaryOpNode(op, right);

  } else if (match(TokenType::OP_Plus)) {
    return parseUnary();

  } else {
    return parsePrimary();
  }
}

AstNode *Parser::parseCall(AstNode *callee) {
  /*
   *  call ::= primary "(" (argument_list)? ")"
   */
  std::vector<AstNode *> args;

  if (peek().type != TokenType::PUNC_RightParen) {
    do {
      args.push_back(parseExpr());
    } while (match(TokenType::PUNC_Comma));
  }
  consume(TokenType::PUNC_RightParen);

  return new CallNode(callee, args);
}

AstNode *Parser::parsePrimary() {
  /*
   *   primary ::= atom | attributeref
   *               | subscription | slicing | call
   *   attributeref ::= primary "." identifier
   *   subscription ::= primary "[" expression_list "]"    // TODO
   *   slicing ::= simple_slicing | extended_slicing       // TODO
   */
  AstNode *left = parseAtom();

  while (true) {
    if (match(TokenType::PUNC_LeftParen)) {
      left = parseCall(left);

    } else if (match(TokenType::PUNC_Dot)) {
      AstNode *right = new NameNode(consume(TokenType::Name));
      left = new PropertyNode(left, right);

    } else {
      break;
    }
  }
  return left;
}

AstNode *Parser::parseAtom() {
  /*
   *  atom ::= identifier | literal | enclosure
   *  literal ::= stringliteral | integer | floatnumber
   *             | "None" | "True" | "False"
   *  enclosure ::= "(" expression ")"
   */
  if (match(TokenType::PUNC_LeftParen)) {
    AstNode *expr = parseExpr();
    consume(TokenType::PUNC_RightParen);
    return expr;
  } else {
    Token tk = advance();

    switch (tk.type) {
    case TokenType::Int:
      return new IntNode(tk);
    case TokenType::Float:
      return new FloatNode(tk);
    case TokenType::KW_True:
      return new BooleanNode(true);
    case TokenType::KW_False:
      return new BooleanNode(false);
    case TokenType::KW_None:
      return new NullNode(tk);
    case TokenType::String:
      return new StringNode(tk);
    case TokenType::Name: {
      return new NameNode(tk);
    }
    default: {
      error("Expected a primary expression");
      return nullptr;
    }
    }
  }
}

Token Parser::consume(TokenType type) {
  if (match(type))
    return previous();
  else {
    error("Expected " + std::to_string(type) + "\n");
    // cout << "But got  " <<  peek().type <<  endl;
    return Token(TokenType::Error, "", 0, 0);
  }
}

bool Parser::match(TokenType type) {
  if (isAtEnd() || peek().type != type) {
    return false;
  } else {
    ++current;
    return true;
  }
}

bool Parser::match(std::initializer_list<TokenType> types) {

  if (isAtEnd())
    return false;

  for (const auto &type : types) {
    if (peek().type == type) {
      ++current;
      return true;
    }
  }
  return false;
}

bool Parser::isAtEnd() const { return peek().type == TokenType::T_EndOfFile; }

Token Parser::peek() const { return tokens[current]; }

Token Parser::previous() const {
  return tokens[(current > 0) ? current - 1 : 0];
}

Token Parser::advance() { return tokens[current++]; }

void Parser::error(const std::string &message) {
  std::cerr << "Error at line " << peek().startline << ": " << message
            << std::endl;
  exit(EXIT_FAILURE);
}
