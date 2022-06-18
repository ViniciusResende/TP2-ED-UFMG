#include "List.hpp"
#include "msgassert.h"

int List::_id = 0;

List::List() {
  this->_id += 1;
  this->id = this->_id;

  this->size = 0;

  this->head = new ListCell();
  this->tail = this->head;
}

int List::length() {
  return this->size;
}

std::string List::getElement(int idx) {
  ListCell *cellPointer;

  cellPointer = this->setPosition(idx, false);

  return cellPointer->value;
}

void List::pushBack(std::string value) {
  errorAssert(!value.empty(), "String value must be previously initialized");

  ListCell *newCell = new ListCell();

  newCell->value = value;
  this->tail->next = newCell;
  this->tail = newCell;

  this->size++;
}

std::string List::popFront() {
  errorAssert(this->size > 0, "Can't pop empty list");

  std::string aux;
  ListCell *cellPointer = this->head->next;

  this->head->next = cellPointer->next;
  this->size--;

  if(this->head->next == nullptr)
    this->tail = this->head;

  aux = cellPointer->value;
  delete cellPointer;  

  return aux;
}

Vector* List::retrieveListAsVector() {
  errorAssert(this->size > 0, "Can't create a Vector from an empty list");

  Vector *newVec = new Vector(this->size);
  
  ListCell *cellPointer = this->head;
  
  for (int i = 0; i < this->size; i++) {
    cellPointer = cellPointer->next;

    newVec->pushBack(cellPointer->value);
  }

  return newVec;
}

ListCell* List::setPosition(int idx, bool before) {
  errorAssert((idx >= 0) && (idx < this->size), "Invalid index while seting position on List");
  
  ListCell *cellPointer = this->head;
  
  for (int i = 0; i < idx; i++) {
    cellPointer = cellPointer->next;
  }

  if(!before)
    cellPointer = cellPointer->next;

  return cellPointer;
}

List::~List() {
  warnAssert(this->size > 0, "List has already been destoyed");
  ListCell *cellPointer = this->head->next;

  while (cellPointer != nullptr) {
    this->head->next = cellPointer->next;
    delete cellPointer;
    cellPointer = this->head->next;
  }
  
  this->tail = this->head;
  this->size = 0;
  
  delete this->head;
}