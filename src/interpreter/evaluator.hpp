#include "../lexer/token.hpp"
#include "../parser/ast.hpp"
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

class Evaluator : public NodeVisitor, public Token {
public:
  Evaluator();
  std::unique_ptr<PyObject> *visitProgramNode(ProgramNode *node) override;
  std::unique_ptr<PyObject> *visitBlockNode(BlockNode *node) override;
  std::unique_ptr<PyObject> *visitPrintNode(PrintNode *node) override;
  virtual std::unique_ptr<PyObject> *visitWhileNode(WhileNode *node) override;
  virtual std::unique_ptr<PyObject> *visitBreakNode(BreakNode *node) override;
  virtual std::unique_ptr<PyObject> *
  visitContinueNode(ContinueNode *node) override;
  virtual std::unique_ptr<PyObject> *visitPassNode(PassNode *node) override;
  virtual std::unique_ptr<PyObject> *visitIfNode(IfNode *node) override;
  virtual std::unique_ptr<PyObject> *visitAssignNode(AssignNode *node) override;
  virtual std::unique_ptr<PyObject> *
  visitTernaryOpNode(TernaryOpNode *node) override;
  virtual std::unique_ptr<PyObject> *
  visitBinaryOpNode(BinaryOpNode *node) override;
  virtual std::unique_ptr<PyObject> *
  visitUnaryOpNode(UnaryOpNode *node) override;
  virtual std::unique_ptr<PyObject> *visitIntNode(IntNode *node) override;
  virtual std::unique_ptr<PyObject> *visitFloatNode(FloatNode *node) override;
  virtual std::unique_ptr<PyObject> *visitNameNode(NameNode *node) override;
  virtual std::unique_ptr<PyObject> *visitStringNode(StringNode *node) override;
  virtual std::unique_ptr<PyObject> *
  visitBooleanNode(BooleanNode *node) override;
  virtual std::unique_ptr<PyObject> *visitNullNode(NullNode *node) override;
  virtual std::unique_ptr<PyObject> *
  visitFunctionNode(FunctionNode *node) override;
  virtual std::unique_ptr<PyObject> *visitCallNode(CallNode *node) override;
  virtual std::unique_ptr<PyObject> *visitReturnNode(ReturnNode *node) override;
  virtual std::unique_ptr<PyObject> *visitClassNode(ClassNode *node) override;
  virtual std::unique_ptr<PyObject> *
  visitPropertyNode(PropertyNode *node) override;

  void pushContext(Scope *frame) { contextStack.push_back(frame); }

  void popContext() {

    if (!contextStack.empty()) {
      // delete contextStack.back();
      contextStack.pop_back();
    } else {
      throw std::runtime_error("Cannot pop context from empty stack");
    }
  }

  Scope *currentContext() {

    if (!contextStack.empty()) {
      return contextStack.back();
    } else {
      return nullptr;
    }
  }

  void defineOnContext(const std::string &name, PyObject *value) {

    if (!contextStack.empty()) {
      Scope *lastFrame = contextStack.back();
      lastFrame->define(name, value);
    } else {
      throw std::runtime_error("Cannot define variable outside of context");
    }
  }

  std::unique_ptr<PyObject> *getFromContext(const std::string &name) {

    if (!contextStack.empty()) {
      Scope *lastFrame = contextStack.back();
      return lastFrame->get(name);
    } else {
      throw std::runtime_error("Cannot access variable outside of context");
    }
  }

private:
  std::vector<std::map<std::string, std::unique_ptr<PyObject> *>> scopes;
};
