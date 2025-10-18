/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    McClain, Adam, and Trevaye
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
 
class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:  
   // 
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list() 
   {
   }

   // 
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);
   void swap(list <T>& rhs);

   //
   // Iterator
   //

   class  iterator;
   iterator begin()  { return iterator(pHead); }
   iterator rbegin() { return iterator(); }
   iterator end()    { return iterator(); }

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   // 
   // Status
   //

   bool empty()  const { return numElements == 0; }
   size_t size() const { return numElements;   }


private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;    // pointer to the beginning of the list
   Node * pTail;    // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   // Default constructor
   Node() : data(), pNext(nullptr), pPrev(nullptr) {}

   // Copy constructor
   Node(const T& t) : data(t), pNext(nullptr), pPrev(nullptr) {}

   // Move constructor
   Node(T&& t) : data(std::move(t)), pNext(nullptr), pPrev(nullptr) {}

   T data;       // user data
   Node* pNext;  // pointer to next node
   Node* pPrev;  // pointer to previous node

};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator() : p(nullptr) {}
   iterator(typename list <T> ::Node* p) : p(p) {}
   ~iterator() {}
   iterator & operator = (const iterator & rhs)
   {
      p = rhs.p;
      return *this;
   }

   
   // equals, not equals operator
   bool operator != (const iterator & rhs) const
   {
      return p != rhs.p;
   }
   bool operator == (const iterator & rhs) const
   {
      return p == rhs.p;
   }

   // dereference operator, fetch a node
   T & operator * ()
   {
	  return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
	  iterator temp = *this;
      // Move to the next node
      if (p != nullptr)
         p = p->pNext;
	  return temp;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      // Move to the next node
      if (p != nullptr)
         p = p->pNext;
	  return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
      // Move to the previous node
      if (p != nullptr)
		  p = p->pPrev;
	  return temp;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      // Move to the previous node
      if (p != nullptr)
		  p = p->pPrev;
	  return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it,       T && data);
   friend iterator list <T> :: erase(const iterator & it);

private:
	typename list <T> ::Node* p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) 
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (size_t i = 0; i < num; ++i) {
      Node* pNew = new Node(t);
      pNew->pPrev = pTail;
      pNew->pNext = nullptr;
      if (pTail) pTail->pNext = pNew;
      else pHead = pNew;
      pTail = pNew;
      numElements++;
   }

}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list <T> ::list(Iterator first, Iterator last)
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (Iterator it = first; it != last; ++it) {
      Node* pNew = new Node(*it);
      pNew->pPrev = pTail;
      pNew->pNext = nullptr;
      if (pTail) pTail->pNext = pNew;
      else pHead = pNew;
      pTail = pNew;
      numElements++;
   }

}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il)
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (const T& item : il) {
      Node* pNew = new Node(item);
      pNew->pPrev = pTail;
      pNew->pNext = nullptr;
      if (pTail) pTail->pNext = pNew;
      else pHead = pNew;
      pTail = pNew;
      numElements++;
   }

}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num)
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (size_t i = 0; i < num; ++i) {
      Node* pNew = new Node(); // default-constructed T
      pNew->pPrev = pTail;
      pNew->pNext = nullptr;
      if (pTail) pTail->pNext = pNew;
      else pHead = pNew;
      pTail = pNew;
      numElements++;
   }

}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list() 
{
   numElements = 0;
   pHead = nullptr;
   pTail = nullptr;
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) 
{
   numElements = 0;
   pHead = nullptr;
   pTail = nullptr;

   Node* pSrc = rhs.pHead;
   while (pSrc != nullptr)
   {
      // Allocate new node with copied data
      Node* pNew = new Node(pSrc->data);
      pNew->pNext = nullptr;
      pNew->pPrev = pTail;

      if (pTail)
         pTail->pNext = pNew;
      else
         pHead = pNew;

      pTail = pNew;
      numElements++;

      pSrc = pSrc->pNext;
   }



}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs)
{
   numElements = rhs.numElements;
   pHead = rhs.pHead;
   pTail = rhs.pTail;

   rhs.numElements = 0;
   rhs.pHead = nullptr;
   rhs.pTail = nullptr;

}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
   // Self-assignment check
   if (this == &rhs)
      return *this;

   // Clean up current list
   clear();

   // Steal resources
   numElements = rhs.numElements;
   pHead = rhs.pHead;
   pTail = rhs.pTail;

   // Null out source
   rhs.numElements = 0;
   rhs.pHead = nullptr;
   rhs.pTail = nullptr;

   return *this;

}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
   // Self-assignment check
   if (this == &rhs)
      return *this;

   // Clean up current list
   clear();

   // Deep copy
   numElements = 0;
   pHead = nullptr;
   pTail = nullptr;

   for (Node* pSrc = rhs.pHead; pSrc != nullptr; pSrc = pSrc->pNext)
   {
      Node* pNew = new Node(pSrc->data);
      pNew->pPrev = pTail;
      pNew->pNext = nullptr;

      if (pTail)
         pTail->pNext = pNew;
      else
         pHead = pNew;

      pTail = pNew;
      numElements++;
   }

   return *this;

}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
   // Clean up current list
   clear();

   // Copy from initializer list
   numElements = 0;
   pHead = nullptr;
   pTail = nullptr;

   for (const T& item : rhs)
   {
      Node* pNew = new Node(item);
      pNew->pPrev = pTail;
      pNew->pNext = nullptr;

      if (pTail)
         pTail->pNext = pNew;
      else
         pHead = pNew;

      pTail = pNew;
      numElements++;
   }

   return *this;

}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{
   Node* p = pHead;
   while (p != nullptr)
   {
      Node* pNext = p->pNext;
      delete p;
      p = pNext;
   }
   pHead = pTail = nullptr;
   numElements = 0;

}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::push_back(const T& data)
{
   // Create a new node
   Node* pNew = new Node(data);
   pNew->pPrev = pTail;
   pNew->pNext = nullptr;
   // Update the tail
   if (pTail)
      pTail->pNext = pNew;
   else
      pHead = pNew;
   pTail = pNew;
   numElements++;
}

template <typename T>
void list <T> ::push_back(T && data)
{
   // Create a new node
   Node* pNew = new Node(std::move(data));
   pNew->pPrev = pTail;
   pNew->pNext = nullptr;
   // Update the tail
   if (pTail)
      pTail->pNext = pNew;
   else
      pHead = pNew;
   pTail = pNew;
   numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{
   // Create a new node
   Node* pNew = new Node(data);
   pNew->pNext = pHead;
   pNew->pPrev = nullptr;
   // Update the head
   if (pHead)
      pHead->pPrev = pNew;
   else
      pTail = pNew;
   pHead = pNew;
   numElements++;
}

template <typename T>
void list <T> ::push_front(T && data)
{
   // Create a new node
   Node* pNew = new Node(std::move(data));
   pNew->pNext = pHead;
   pNew->pPrev = nullptr;
   // Update the head
   if (pHead)
      pHead->pPrev = pNew;
   else
      pTail = pNew;
   pHead = pNew;
   numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
	// If the list is empty, do nothing
    if (pTail == nullptr)
		return;
    // Store the current tail
	Node* pOldTail = pTail;
	// Update the tail to the previous node
	pTail = pTail->pPrev;
	// If the new tail is not null, set its next pointer to null
    if (pTail)
        pTail->pNext = nullptr;
    else
		pHead = nullptr; // List is now empty
	// Delete the old tail node
	delete pOldTail;
	numElements--;
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
	// If the list is empty, do nothing
    if (pHead == nullptr)
		return;
   // Store the current head
   Node* pOldHead = pHead;
   // Update the head to the next node
   pHead = pHead->pNext;
   // If the new head is not null, set its previous pointer to null
   if (pHead)
      pHead->pPrev = nullptr;
   else
	   pTail = nullptr; // List is now empty
   // Delete the old head node
   delete pOldHead;
   numElements--;
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: front()
{
	// Empty list case
	if (pHead == nullptr)
    {
        throw "ERROR: unable to access data from an empty list";
	}
	return pHead->data;
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
	// Empty list case
    if (pTail == nullptr)
    {
        throw "ERROR: unable to access data from an empty list";
	}
	return pTail->data;
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
   // Empty list case
   if (pHead == nullptr && pTail == nullptr)
   {
      return iterator(nullptr);
   }
   // Node to be deleted
   Node* pToDelete = it.p;
   // Update pointers
   if (pToDelete->pPrev)
      pToDelete->pPrev->pNext = pToDelete->pNext;
   else
      pHead = pToDelete->pNext; // Deleting head
   if (pToDelete->pNext)
      pToDelete->pNext->pPrev = pToDelete->pPrev;
   else
      pTail = pToDelete->pPrev; // Deleting tail
   // Iterator to the next node
   iterator itNext(pToDelete->pNext);
   // Delete the node
   delete pToDelete;
   numElements--;
   return itNext;
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data) 
{
	// Create a new node
	Node* pNew = new Node(data);
    // Empty list case
    if (pHead == nullptr && pTail == nullptr)
    {
        pHead = pNew;
        pTail = pNew;
        pNew->pNext = nullptr;
        pNew->pPrev = nullptr;
    }
   // Insertion at the head
    else if (it.p == pHead)
    {
        pNew->pNext = pHead;
        pNew->pPrev = nullptr;
        pHead->pPrev = pNew;
        pHead = pNew;
    }
    // Insertion at the tail
    else if (it.p == nullptr)
    {
        pNew->pNext = nullptr;
        pNew->pPrev = pTail;
        pTail->pNext = pNew;
        pTail = pNew;
    }
    // Insertion in the middle
    else
    {
        pNew->pNext = it.p;
        pNew->pPrev = it.p->pPrev;
        if (it.p->pPrev)
            it.p->pPrev->pNext = pNew;
		else
			pHead = pNew;
        it.p->pPrev = pNew;
	}
	numElements++;
	return iterator(pNew);
}



template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
	// Create a new node
	Node* pNew = new Node(std::move(data));
	// Empty list case
    if (pHead == nullptr && pTail == nullptr)
    {
        pHead = pNew;
        pTail = pNew;
        pNew->pNext = nullptr;
        pNew->pPrev = nullptr;

    }
   // Insertion at the head
    else if (it.p == pHead)
    {
        pNew->pNext = pHead;
        pNew->pPrev = nullptr;
        pHead->pPrev = pNew;
        pHead = pNew;
	}
    // Insertion at the tail
    else if (it.p == nullptr)
    {
        pNew->pNext = nullptr;
        pNew->pPrev = pTail;
        pTail->pNext = pNew;
        pTail = pNew;
	}
    // Insertion in the middle
    else
    {
        pNew->pNext = it.p;
        pNew->pPrev = it.p->pPrev;
        if (it.p->pPrev)
            it.p->pPrev->pNext = pNew;
        else
			pHead = pNew;
        it.p->pPrev = pNew;
    }
	numElements++;
	return iterator(pNew);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
   lhs.swap(rhs);

}

template <typename T>
void list<T>::swap(list <T>& rhs)
{
   std::swap(numElements, rhs.numElements);
   std::swap(pHead, rhs.pHead);
   std::swap(pTail, rhs.pTail);

}

//#endif
}; // namespace custom
