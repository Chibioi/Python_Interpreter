#include "./evaluator.hpp"
#include "../scope/scope.hpp"
#include "../value/primitives.hpp"
#include <memory>
#include <string>
#include <vector>

// Constructor for the evaluator class

Evaluator::Evaluator() {

  Scope *builtins = new Scope();
  pushContext(builtins); // This scope holds the predefined values (functions
                         // and constants)
  Scope *globals =
      new Scope(builtins); // global scope inherits from the builtins scope
  pushContext(globals); // holds the variables defined in the user main program

  builtins->define("input",
                   new PyBuiltin([](std::vector<PyObject *> args,
                                    Interpreter *interpreter) -> PyObject * {
                     ASSERT_ARG_SIZE(args, 0);

                     std::string buffer;
                     std::getline(std::cin, buffer);

                     return new PyStr(buffer);
                   }));
}

PyObject *Evaluator::Evaluate(ProgramNode *node) {

  PyObject *result = node->body->accept(this);

  return result;
};

PyObject *Evaluator::resolve(const std::string &method_name, PyObject *object,
                             std::vector<PyObject *> args) {
  PyObject *method_obj = object->find(method_name);

  if (method_obj && method_obj->is_fn_type()) {
    PyFunction *target = method_obj->unwrap_function_obj();
    return target->bind(object)->call(args, this);

  } else {
  }
}

PyObject *Evaluator::visitBlockNode(BlockNode *node) {
  for (AstNode *stmt : node->statements) {
    // Evaluate each statement in the block
    stmt->accept(this);
  }
  return nullptr; // Blocks don't return a value
};

PyObject *Evaluator::visitPrintNode(PrintNode *node) {
  PyObject *result = node->args->accept(this);

  // You'll need a way to convert PyObject to a string.
  // Assuming PyObject has a `toString()` method.
  std::cout << result->toString() << std::endl;

  return nullptr;
};

PyObject *Evaluator::visitBinaryOpNode(BinaryOpNode *node) {
  PyObject *left = node->left->accept(this);
  PyObject *right = node->right->accept(this);

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

PyObject *Evaluator::visitAssignNode(AssignNode *node) {
  // Evaluate the value on the right-hand side
  PyObject *value = node->value->accept(this);

  // Get the variable name from the name node on the left
  NameNode *nameNode = node->name->unwrap_name_node();
  std::string varName = nameNode->get_lexeme();

  // Store the value in the current scope
  scopes.back()[varName] = value;

  return value; // Assignment expressions can return the assigned value
};

PyObject *Evaluator::visitNameNode(NameNode *node) {
  std::string varName = node->get_lexeme();

  // Search for the variable in the current scope and then in parent scopes
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    if (it->count(varName)) {
      return (*it)[varName];
    }
  }

  throw std::runtime_error("Undefined variable '" + varName + "'");
};
