#pragma once

#include "../parser/ast.hpp"
#include "./pyObject.hpp"
#include <functional>

class PyFunction : public PyObject {

public:
  explicit PyFunction(FunctionNode *, Scope *);

  // Constructor for built-in functions (C++ directly)
  explicit PyFunction(
      std::function<PyObject *(std::vector<PyObject *> &, Interpreter *)>,
      size_t arity = 0 // self
  );

  inline bool is_fn_type() const override { return true; }

  class PyMethodWrapper *bind(PyObject *);
  PyObject *call(std::vector<PyObject *> &, Interpreter * = nullptr);

private:
  std::string fname;

  // for internal functions
  std::function<PyObject *(std::vector<PyObject *> &, Interpreter *)> functor;

  // for user-defined functions
  FunctionNode *declaration = nullptr;
  Scope *closure = nullptr;

  size_t arity;

  void registerMethods() override;

  void deleteData() override {
    // delete declaration;
    // delete bound;
  }
};
