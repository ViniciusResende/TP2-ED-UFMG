#include <iostream>
#include "msgassert.h"
#include "Vector.hpp"

int Vector::_id = 0;

Vector::Vector(int size) {
  errorAssert(size > 0, "Null Vector dimension");

  this->size = size;

  this->id = this->_id;
  this->_id += 1;

  this->value = (std::string*) malloc(this->size * sizeof(std::string));
  errorAssert(this->value != NULL && this->value != nullptr, "Failed to allocate Vector memory");

  this->lastInputtedIndex = -1;
  this->sortingAlgorithmBreakpoint = 20;
  this->pivotChoiceRange = 1;
}

int Vector::length() {
  return this->size;
}

bool Vector::vectorIsFulfilled() {
  return (this->size - 1) == this->lastInputtedIndex;
}

std::string Vector::warmUpVector() {  
  std::string aux = this->value[0];

  for (int i=1; i<this->size; i++){
    aux = this->value[i];
    // READMEMLOG((long int) (&(value[i])), sizeof(std::string), this->id);
  }

  return aux;
}

void Vector::setElement(int idx, std::string value) {
  errorAssert((idx >= 0) && (idx < this->size), "Invalid Vector index while setting element");

  this->value[idx] = value;
  // WRITEMEMLOG((long int) (&(this->value[idx])), sizeof(std::string), this->id);

  if(idx > this->lastInputtedIndex)
    this->lastInputtedIndex = idx;
}

void Vector::pushBack(std::string value) {
  errorAssert(this->lastInputtedIndex < this->size, "Allocated Vector is already full");

  this->lastInputtedIndex += 1;
  this->value[this->lastInputtedIndex] = value;

  // WRITEMEMLOG((long int) (&(this->value[lastInputtedIndex])), sizeof(std::string), this->id);
}

std::string Vector::getElement(int idx) {
  errorAssert((idx >= 0) && (idx < this->size), "Invalid Vector index while getting element");

  // READMEMLOG((long int) (&(value[idx])), sizeof(std::string), this->id);
  return this->value[idx];
}

int Vector::normalizeCharacter(char c) {
  int asciiChar = (int)c;

  if(asciiChar < ASCII_OF_FIRST_LETTER)
    return asciiChar - ASCII_OF_FIRST_LETTER;
  
  if(asciiChar > ASCII_OF_LAST_LETTER)
    return asciiChar;

  for (int i = 0; i < ALPHABET_DEFAULT_SIZE; i++) {
    if(c == this->lexicographicalSortOrder->getElement(i).front())
      return i;
  }
  
  return asciiChar;
}

int Vector::getGreaterWord(std::string firstWord, std::string secondWord) {
  warnAssert((firstWord.size() > 0) && (secondWord.size() > 0), "Shouldn't be comparisons between words involving an empty word");

  if(firstWord == secondWord) 
    return NONE_OF_WORDS_IS_GREATER;

  std::string::size_type firstWordSize = firstWord.size();
  std::string::size_type secondWordSize = secondWord.size();

  std::string::size_type sizeOfSmallestWord = firstWordSize < secondWordSize 
    ? firstWordSize 
    : secondWordSize;

  for (std::string::size_type i = 0; i < sizeOfSmallestWord; i++) {
    if(normalizeCharacter(firstWord[i]) < normalizeCharacter(secondWord[i]))
      return SECOND_WORD_IS_GREATER;
    else if(normalizeCharacter(firstWord[i]) > normalizeCharacter(secondWord[i]))
      return FIRST_WORD_IS_GREATER;
  }

  if(firstWordSize > secondWordSize)
    return FIRST_WORD_IS_GREATER;

  return SECOND_WORD_IS_GREATER;
}

void Vector::insertionSort(int leftIdx, int rightIdx) {
  errorAssert(leftIdx >= 0, "Left index provided is out of boundaries of the current Vector");
  errorAssert(rightIdx < this->size, "Right index provided is out of boundaries of the current Vector");

  int i, j;
  std::string aux;

  for (i = leftIdx + 1; i < rightIdx + 1; i++) {
    aux = this->value[i];
    // READMEMLOG((long int) (&(this->value[i])), sizeof(std::string), this->id);
    j = i - 1;

    while ((j >= 0) && (getGreaterWord(aux, this->value[j]) == SECOND_WORD_IS_GREATER)) {
      // READMEMLOG((long int) (&(this->value[j])), sizeof(std::string), this->id);

      this->value[j + 1] = this->value[j];
      // WRITEMEMLOG((long int) (&(this->value[j + 1])), sizeof(std::string), this->id);
      j--;
    }
    
    this->value[j + 1] = aux;
    // WRITEMEMLOG((long int) (&(this->value[j + 1])), sizeof(std::string), this->id);
  }
}

void Vector::quickSortRecursive(int leftIdx, int rightIdx) {
  errorAssert(leftIdx >= 0, "Left index provided is out of boundaries of the current Vector");
  errorAssert(rightIdx < this->size, "Right index provided is out of boundaries of the current Vector");

  std::string pivot;
  int pivotIdx;
  if((this->pivotChoiceRange != 1) && (rightIdx - leftIdx >= this->pivotChoiceRange)) {
    this->insertionSort(leftIdx, leftIdx + this->pivotChoiceRange);

    pivotIdx = (leftIdx + (leftIdx + this->pivotChoiceRange)) / 2;
    pivot = this->value[pivotIdx];
    // READMEMLOG((long int) (&(this->value[pivotIdx])), sizeof(std::string), this->id);
  } else {
    pivotIdx = (leftIdx + rightIdx) / 2;
    pivot = this->value[pivotIdx];
    // READMEMLOG((long int) (&(this->value[pivotIdx])), sizeof(std::string), this->id);
  }

  int i = leftIdx, j = rightIdx;

  while (i <= j) {
    while(this->getGreaterWord(pivot, this->value[i]) == FIRST_WORD_IS_GREATER && i < rightIdx) {
      // READMEMLOG((long int) (&(this->value[i])), sizeof(std::string), this->id);
      i++;
    };
    while(this->getGreaterWord(pivot, this->value[j]) == SECOND_WORD_IS_GREATER && j > leftIdx) {
      // READMEMLOG((long int) (&(this->value[j])), sizeof(std::string), this->id);
      j--;
    };
    
    if(i <= j) {
      swap(this->value[i], this->value[j]);

      // READMEMLOG((long int) (&(this->value[i])), sizeof(std::string), this->id);
      // READMEMLOG((long int) (&(this->value[j])), sizeof(std::string), this->id);
      // WRITEMEMLOG((long int) (&(this->value[i])), sizeof(std::string), this->id);
      // WRITEMEMLOG((long int) (&(this->value[j])), sizeof(std::string), this->id);

      i++;
      j--;
    }
  }

  if(rightIdx - leftIdx <= sortingAlgorithmBreakpoint) {
    this->insertionSort(leftIdx, rightIdx);
    return;
  }

  if(j > leftIdx)
    this->quickSortRecursive(leftIdx, j);
  
  if(i < rightIdx)
    this->quickSortRecursive(i, rightIdx);  
}

void Vector::quickSort() {
  quickSortRecursive(0, this->size - 1);
}

void Vector::sortVector() {
  this->quickSort();
}

void Vector::setLexicographicalSortOrder(Vector* lexicographicalSortOrder) {
  errorAssert(lexicographicalSortOrder->length() == ALPHABET_DEFAULT_SIZE, 
    "Invalid size provided to Vector lexicographical sort order");
  this->lexicographicalSortOrder = lexicographicalSortOrder;
}

void  Vector::setSortingAlgorithmBreakpoint(int sortingAlgorithmBreakpoint) {
  bool isValueValid = sortingAlgorithmBreakpoint >= 1;
  warnAssert(isValueValid, "Invalid value for sorting algorithm breakpoint, keeping default of 20");

  if(isValueValid) 
    this->sortingAlgorithmBreakpoint = sortingAlgorithmBreakpoint;
}

void  Vector::setPivotChoiceRange(int pivotChoiceRange) {
  bool isValueValid = pivotChoiceRange >= 1;
  warnAssert(isValueValid, "Invalid value for pivot choice range, keeping default of 1");

  if(isValueValid) 
    this->pivotChoiceRange = pivotChoiceRange;
}

Vector::~Vector() {
  warnAssert(this->size > 0, "Vector has already been destroyed");

  this->id = this->size = -1;

  free(this->value);
}