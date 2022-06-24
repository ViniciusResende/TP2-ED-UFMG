#ifndef LISTH
#define LISTH

#include "ListCell.hpp"
#include "Vector.hpp"
#include "memlog.h"
 
class ListCell;
class List {
  public:  
    /**
     * @brief Default constructor for List class instance. Responsible for initialize
     * the List attributes.
     */
    List();

    /**
     * @brief Returns the current length of the List.
     *
     * @return Returns the value of the class attribute "size".
     */
    int length();
    /**
     * @brief Getter function responsible for returning the element stored in a certain Index of the List. 
     *
     * @param idx Index of the element that will be returned.
     * 
     * @return Returns the element stored in the List at idx position.
     */
		std::string getElement (int idx);
    /**
     * @brief Method responsible for append a new element in the last position of the List. 
     *
     * @param value Value of the element to be appended.
     */
    void pushBack(std::string value);
    /**
     * @brief Method responsible for delete the element in the first position of the List. 
     *
     * @return Returns the value stored at the deleted element.
     */
    std::string popFront();

    Vector* retrieveListAsVector();
    
    /**
     * @brief Default destructor of the class, that will clean it up when called and free the allocated memory.
     */
    ~List();

  private:
    static int _id;
    int id;
    int size;
    ListCell* head;
    ListCell* tail;

    /**
     * @brief Method responsible for returning a pointer to a cell in a certain list position. 
     *
     * @param idx Index of the position which is desirable to place a pointer to.
     * @param before Boolean condition that indicates if the returned pointer refers to idx - 1.
     * 
     * @return Returns a pointer to the list cell in the specified position.
     */
    ListCell* setPosition(int idx, bool before);
};

#endif
