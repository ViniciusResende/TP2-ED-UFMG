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
     * @brief Returns the total lenght of the Vector.
     *
     * @return Returns the value of the class attribute "size".
     */
    int length();
    /**
     * @brief Returns a boolean value indicating if the Vector is full filled.
     *
     * @return Returns the value true if the vector is already full filled and false if its not.
     */
    bool vectorIsFullfilled();
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
     * @brief Method responsible for ordering the value of the Vector in ascending order. 
     */
    void sortVector();

    void setLexicographicalSortOrder(Vector* lexicographicalSortOrder);
    void setSortingAlgorithmBreakpoint(int sortingAlgorithmBreakpoint);

    /**
     * @brief Default destructor of the class, that will clean it up when called and free the allocated memory.
     */
    ~Vector();

  private:
    std::string* value;
    int size;
    int lastInputedIndex;
    int sortingAlgorithmBreakpoint;
    static int _id;
    int id;
    Vector* lexicographicalSortOrder;

    void quickSort();
    void quickSortRecursive(int leftIdx, int rightIdx);
    int getGreaterWord(std::string firstWord, std::string secondWord);
    int normalizeCharacter(char c);
    void insertionSort(int leftIdx, int rightIdx);
};

#endif
