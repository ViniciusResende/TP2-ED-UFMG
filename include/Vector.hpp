#ifndef VECTORH
#define VECTORH

#include <string>
#include "memlog.h"
#include "CONSTANTS.hpp"

#define swap(A, B) { std::string c = A; A = B; B = c; }

class Vector {
  public:  
    /**
     * @brief Default constructor for Vector class instance. 
     *
     * @param size The total number of elements that Vector can handle.
     */
    Vector(int size);

    /**
     * @brief Returns the total length of the Vector.
     *
     * @return Returns the value of the class attribute "size".
     */
    int length();
    /**
     * @brief Returns a boolean value indicating if the Vector is full filled.
     *
     * @return Returns the value true if the vector is already full filled and false if its not.
     */
    bool vectorIsFulfilled();
    /**
     * @brief Util method that is responsible for iterating over the Vector elements 
     * stacking their allocated memory.
     *
     * @return Returns the value contained in the Vector last Index.
     */
		std::string warmUpVector();
    /**
     * @brief Method responsible for changing the value of a certain element in the Vector. 
     *
     * @param idx Index of the element that will have the value changed.
     * @param value Value to change the element for.
     */
		void setElement(int idx, std::string value);
    /**
     * @brief Getter function responsible for returning the element stored in a certain Index of the Vector. 
     *
     * @param idx Index of the element that will be returned.
     * 
     * @return Returns the element stored in the Vector at idx position.
     */
		std::string getElement (int idx);
    /**
     * @brief Method responsible for append a new element in the last available position of the Vector. 
     *
     * @param value Value of the element to be appended.
     */
    void pushBack(std::string value);
    /**
     * @brief Method responsible for ordering the value of the Vector according to its lexicographical order. 
     */
    void sortVector();
    /**
     * @brief Method responsible for setting the lexicographical order used for sorting of the Vector. 
     *
     * @param lexicographicalSortOrder Vector containing the alphabet letters in the order wanted to sort the current Vector.
     */
    void setLexicographicalSortOrder(Vector* lexicographicalSortOrder);
    /**
     * @brief Method responsible for setting the breakpoint of use of sorting algorithms (Quick Sort -> Insertion Sort). 
     *
     * @param sortingAlgorithmBreakpoint Integer value containing the breakpoint of use of sorting algorithms.
     */
    void setSortingAlgorithmBreakpoint(int sortingAlgorithmBreakpoint);
    /**
     * @brief Method responsible for setting the number of elements considered in the pick of the pivot at Quicksort sorting. 
     *
     * @param pivotChoiceRange Integer value containing the number of elements considered 
     * in the pick of the pivot at Quicksort sorting.
     */
    void setPivotChoiceRange(int pivotChoiceRange);

    /**
     * @brief Default destructor of the class, that will clean it up when called and free the allocated memory.
     */
    ~Vector();

  private:
    std::string* value;
    int size;
    int lastInputtedIndex;
    int pivotChoiceRange;
    int sortingAlgorithmBreakpoint;
    static int _id;
    int id;
    Vector* lexicographicalSortOrder;

    /**
     * @brief Sorting method that implements the well known algorithm Quick Sort in the Current Vector. 
     */
    void quickSort();
    /**
     * @brief Quick Sort algorithm implemented in its recursive way. 
     *
     * @param leftIdx Index of the most left element in the partitioned Vector.
     * @param leftIdx Index of the most right element in the partitioned Vector.
     */
    void quickSortRecursive(int leftIdx, int rightIdx);
    /**
     * @brief Compare the "greatness" of two words according to the lexicographicalSortOrder attribute. 
     *
     * @param firstWord First word being compared.
     * @param secondWord Second word being compared.
     * 
     * @return Returns 1 if the first word is greater, 2 if the second is greater and 0 if they're equal.
     */
    int getGreaterWord(std::string firstWord, std::string secondWord);
    /**
     * @brief Normalize a character to an integer representing its "greatness" 
     * according to the Vector lexicographical order. 
     *
     * @param c Character to be normalized.
     * 
     * @return Returns an integer representing the character "greatness" 
     * according to the Vector lexicographical order.
     */
    int normalizeCharacter(char c);
    /**
     * @brief Sorting method that implements the well known algorithm Insertion Sort 
     * in the Current Vector, given an start and end index. 
     *
     * @param leftIdx Index of the most left element in the partitioned Vector.
     * @param leftIdx Index of the most right element in the partitioned Vector.
     */
    void insertionSort(int leftIdx, int rightIdx);
};

#endif
