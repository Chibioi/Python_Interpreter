#include "./scope.hpp"
#include "../value/pyObject.hpp"
#include <memory>
#include <stdexcept>

Scope::~Scope() {
  for (auto &[key, value] : this->values) {
    value->decRefCount();
  }
}

bool Scope::empty() { return values.empty(); }

void Scope::define(const std::string &name, PyObject *value) {

  auto it = this->values.find(name);

  if (it != values.end()) {
    std::unique_ptr<PyObject> *tp_object = it->second;
    tp_object->decRefCount();
    it->second = value;

  } else {
    values.emplace(name, value);
  }
  value->incRefCount();
}

std::unique_ptr<PyObject> *Scope::get(const std::string &name) {

  auto it = this->values.find(name);

  if (it != values.end()) {
    return it->second;

  } else if (enclosing) {
    return enclosing->get(name);

  } else {
    throw std::runtime_error("Undeclared variable '" + name + "'.");
  }
  return nullptr;
}
