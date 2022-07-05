#include "List.hpp"
#include "msgassert.h"

int List::_id = 2;

List::List() {
  this->id = this->_id;
  this->_id += 1;

  this->size = 0;

  this->head = new ListCell();
  this->tail = this->head;
}

int List::length() {
  return this->size;
}

std::string List::getElement(int idx) {
  errorAssert((idx >= 0) && (idx < this->size), "Invalid List index while getting element");

  ListCell *cellPointer;

  cellPointer = this->setPosition(idx, false);

  // READMEMLOG((long int) (&(cellPointer->value)), sizeof(std::string), this->id);
  return cellPointer->value;
}

void List::pushBack(std::string value) {
  errorAssert(!value.empty(), "String value must be previously initialized");

  ListCell *newCell = new ListCell();

  newCell->value = value;
  this->tail->next = newCell;
  this->tail = newCell;

  this->size++;
  // WRITEMEMLOG((long int) (&(newCell->value)), sizeof(std::string), this->id);
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
  errorAssert(this->size > 0, "Can't create a Vector from an empty List");

  Vector *newVec = new Vector(this->size - 1);
  
  ListCell *cellPointer = this->head;
  
  for (int i = 0; i < this->size - 1; i++) {
    cellPointer = cellPointer->next;

    newVec->pushBack(cellPointer->value);
    // READMEMLOG((long int) (&(cellPointer->value)), sizeof(std::string), this->id);
  }

  return newVec;
}

ListCell* List::setPosition(int idx, bool before) {
  errorAssert((idx >= 0) && (idx < this->size), "Invalid index while setting position on List");
  
  ListCell *cellPointer = this->head;
  
  for (int i = 0; i < idx; i++) {
    cellPointer = cellPointer->next;
  }

  if(!before)
    cellPointer = cellPointer->next;

  return cellPointer;
}

List::~List() {
  warnAssert(this->size > 0, "List has already been destroyed");
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