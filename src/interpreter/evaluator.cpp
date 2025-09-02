#include "./evaluator.hpp"
#include <memory>
#include <string>
#include <vector>

using unique_ptr = std::unique_ptr<PyObject>;

unique_ptr *Evaluator::visitProgramNode(ProgramNode *node) {
  // Push a new scope for the global environment
  scopes.push_back(std::map<std::string, unique_ptr *>());

  // Begin the traversal by visiting the main block of statements
  unique_ptr *result = node->body->accept(this);

  // After execution, pop the global scope
  scopes.pop_back();

  return result;
};

unique_ptr *Evaluator::visitBlockNode(BlockNode *node) {
  for (AstNode *stmt : node->statements) {
    // Evaluate each statement in the block
    stmt->accept(this);
  }
  return nullptr; // Blocks don't return a value
};

unique_ptr *Evaluator::visitPrintNode(PrintNode *node) {
  unique_ptr *result = node->args->accept(this);

  // You'll need a way to convert PyObject to a string.
  // Assuming PyObject has a `toString()` method.
  std::cout << result->toString() << std::endl;

  return nullptr;
};

unique_ptr *Evaluator::visitBinaryOpNode(BinaryOpNode *node) {
  unique_ptr *left = node->left->accept(this);
  unique_ptr *right = node->right->accept(this);

  switch (node->op.type) {
  case TokenType::OP_Plus:
    return left->add(right); // Assumes PyObject has operator methods
  case TokenType::OP_Minus:
    return left->subtract(right);
  // ... Implement other operators
  default:
    throw std::runtime_error("Unsupported binary operator.");
  }
};

unique_ptr *Evaluator::visitAssignNode(AssignNode *node) {
  // Evaluate the value on the right-hand side
  unique_ptr *value = node->value->accept(this);

  // Get the variable name from the name node on the left
  NameNode *nameNode = node->name->unwrap_name_node();
  std::string varName = nameNode->get_lexeme();

  // Store the value in the current scope
  scopes.back()[varName] = value;

  return value; // Assignment expressions can return the assigned value
};

unique_ptr *Evaluator::visitNameNode(NameNode *node) {
  std::string varName = node->get_lexeme();

  // Search for the variable in the current scope and then in parent scopes
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    if (it->count(varName)) {
      return (*it)[varName];
    }
  }

  throw std::runtime_error("Undefined variable '" + varName + "'");
};
