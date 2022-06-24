#include <iostream>
#include "msgassert.h"
#include "Vector.hpp"

int Vector::_id = 0;

Vector::Vector(int size) {
  errorAssert(size > 0, "Null Vector dimension");

  this->size = size;

  this->_id += 1;
  this->id = this->_id;

  this->value = (std::string*) malloc(this->size * sizeof(std::string));

  this->lastInputtedIndex = -1;
  this->sortingAlgorithmBreakpoint = 3;
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
  }

  return aux;
}

void Vector::setElement(int idx, std::string value) {
  errorAssert((idx >= 0) && (idx < this->size), "Invalid Vector index");

  this->value[idx] = value;

  if(idx > this->lastInputtedIndex)
    this->lastInputtedIndex = idx;
}

void Vector::pushBack(std::string value) {
  errorAssert(this->lastInputtedIndex < this->size, "Allocated Vector is already full");

  this->lastInputtedIndex += 1;
  this->value[this->lastInputtedIndex] = value;
}

std::string Vector::getElement(int idx) {
  errorAssert((idx >= 0) && (idx < this->size), "Invalid Vector index");

  return this->value[idx];
}

int Vector::normalizeCharacter(char c) {
  int asciiChar = (int)c;

  if(asciiChar < ASCII_OF_FIRST_LETTER)
    return asciiChar - ASCII_OF_FIRST_LETTER;

  for (int i = 0; i < ALPHABET_DEFAULT_SIZE; i++) {
    if(c == this->lexicographicalSortOrder->getElement(i).front())
      return i;
  }
  
  return asciiChar;
}

int Vector::getGreaterWord(std::string firstWord, std::string secondWord) {
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

  if(firstWordSize == secondWordSize) 
    return NONE_OF_WORDS_IS_GREATER;

  if(firstWordSize > secondWordSize)
    return FIRST_WORD_IS_GREATER;

  return SECOND_WORD_IS_GREATER;
}

void Vector::insertionSort(int leftIdx, int rightIdx) {
  int i, j;
  std::string aux;

  for (i = leftIdx + 1; i < rightIdx + 1; i++) {
    aux = this->value[i];
    j = i - 1;

    while ((j >= 0) && (getGreaterWord(aux, this->value[j]) == SECOND_WORD_IS_GREATER)) {
      this->value[j + 1] = this->value[j];
      j--;
    }
    
    this->value[j + 1] = aux;
  }
}

void Vector::quickSortRecursive(int leftIdx, int rightIdx) {
  std::string pivot;
  int pivotIdx;
  if((this->pivotChoiceRange != 1) && (rightIdx - leftIdx >= this->pivotChoiceRange)) {
    this->insertionSort(0, this->pivotChoiceRange);

    pivotIdx = (0 + this->pivotChoiceRange) / 2;
    pivot = this->value[pivotIdx];
  } else {
    pivotIdx = (leftIdx + rightIdx) / 2;
    pivot = this->value[pivotIdx];
  }

  int i = leftIdx, j = rightIdx;

  while (i <= j) {
    while(this->getGreaterWord(pivot, this->value[i]) == FIRST_WORD_IS_GREATER && i < rightIdx) i++;
    while(this->getGreaterWord(pivot, this->value[j]) == SECOND_WORD_IS_GREATER && j > leftIdx) j--;
    
    if(i <= j) {
      swap(this->value[i], this->value[j]);

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
  warnAssert(isValueValid, "Invalid value for sorting algorithm breakpoint, keeping default of 3");

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