#include "ListCell.hpp"

ListCell::ListCell() {
  this->value = std::string();
  this->next = nullptr;
}

ListCell::~ListCell() {}