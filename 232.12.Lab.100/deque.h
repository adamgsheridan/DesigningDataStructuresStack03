/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *         iaFront
 * ia =   0   1   2   3   4
 *      +---+---+---+---+---+
 *      |   | A | B | C |   |
 *      +---+---+---+---+---+
 * id =       0   1   2
 * iaFront = 1
 * numElements = 3
 * numCapacity = 5
 *****************************************************/
template <class T>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque() 
   { 
   }
   deque(int newCapacity);
   deque(const deque <T> & rhs);
   ~deque()
   { 
   }

   //
   // Assign
   //
   deque<T> & operator = (const deque <T> & rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin()
   {
      return iterator(this, 0);
   }
   iterator end()
   {
      return iterator(this, numElements);
   }

   //
   // Access
   //
   T& front();
   T& back();
   const T & front() const;
   const T & back()  const;
   const T & operator[](size_t index) const;
   T& operator[](size_t index);

   // 
   // Insert
   //
   void push_front(const T& t);
   void push_back(const T& t);

   //
   // Remove
   //
   void clear()
   { 
   }
   void pop_front();
   void pop_back();

   // 
   // Status
   //
   size_t size() const 
   { 
      return 99; 
   }
   bool empty() const 
   { 
      return false; 
   }

   
private:
   
   // fetch array index from the deque index
   int iaFromID(int id) const
   {
      return -99;
   }
   void resize(int newCapacity = 0);

   // member variables
   T * data;           // dynamically allocated data for the deque
   size_t numCapacity; // the size of the data array
   size_t numElements; // number of elements in the deque
   int iaFront;        // the index of the first item in the array
};



/**********************************************************
 * DEQUE ITERATOR
 * Forward and reverse iterator through a deque, just call
 *********************************************************/
template <typename T>
class deque <T> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   //
   // Construct
   //
	iterator() : id(0), pDeque(nullptr)
   {
   }
	iterator(custom::deque<T>* pDeque, int id) : id(id), pDeque(pDeque)
   {
   }
	iterator(const iterator& rhs) : id(rhs.id), pDeque(rhs.pDeque)
   {
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
	  id = rhs.id;
	  pDeque = rhs.pDeque;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator& rhs) const 
   { 
	   return pDeque == rhs.pDeque && id == rhs.id;
   }
   bool operator != (const iterator& rhs) const 
   { 
	   return !(*this == rhs);
   }

   // 
   // Access
   //
   const T & operator * () const
   {
	   int ia = pDeque->iaFromID(id);
	   return pDeque->data[ia];
   }

   T& operator * () 
   {
	   int ia = pDeque->iaFromID(id);
	   return pDeque->data[ia];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return id - it.id;
   }
   iterator& operator += (int offset)
   {
	  id += offset;
      return *this;
   }

   iterator& operator ++ ()
   {
	  id++;
      return *this;
   }
   iterator operator ++ (int)
   {
	   iterator temp = *this;
	   id++;
      return temp;
   }
   iterator& operator -- ()
   {
	  id--;
      return *this;
   }
   iterator  operator -- (int)
   {
	   iterator temp = *this;
	   id--;
	   return temp;
   }

private:

   // Member variables
   int id;             // deque index
   deque<T> *pDeque;
};


/****************************************************
 * DEQUE : CONSTRUCTOR - non-default
 ***************************************************/
template <class T>
deque <T> :: deque(int newCapacity)
{
}

/****************************************************
 * DEQUE : CONSTRUCTOR - copy
 ***************************************************/
template <class T>
deque <T> :: deque(const deque <T> & rhs)
{
}


/****************************************************
 * DEQUE : ASSIGNMENT OPERATOR
 ***************************************************/
template <class T>
deque <T> & deque <T> :: operator = (const deque <T> & rhs)
{
   return *this;
}


/**************************************************
 * DEQUE :: FRONT
 * Fetch the item that is at the beginning of the deque
 *************************************************/
template <class T>
const T & deque <T> :: front() const 
{
	assert(numElements > 0);
	return data[iaFront];
}
template <class T>
T& deque <T> ::front()
{
	assert(numElements > 0);
	return data[iaFront];
}

/**************************************************
 * DEQUE :: BACK
 * Fetch the item that is at the end of the deque
 *************************************************/
template <class T>
const T & deque <T> :: back() const 
{
	assert(numElements > 0);
	int ia = iaFromID(numElements - 1);
	return data[ia];
   
}
template <class T>
T& deque <T> ::back()
{
	assert(numElements > 0);
	int ia = iaFromID(numElements - 1);
	return data[ia];
}
 
/**************************************************
 * DEQUE :: SUBSCRIPT
 * Fetch the item in the deque
 *************************************************/
template <class T>
const T& deque <T> ::operator[](size_t index) const
{
	assert(index < numElements);
	int ia = iaFromID(index);
	return data[ia];
}
template <class T>
T& deque <T> ::operator[](size_t index)
{
	assert(index < numElements);
	int ia = iaFromID(index);
	return data[ia];
}

/*****************************************************
 * DEQUE : POP_BACK
 *****************************************************/
template <class T>
void deque <T> :: pop_back()
{
	assert(numElements > 0);
	numElements--;
}

/*****************************************************
 * DEQUE : POP_FRONT
 *****************************************************/
template <class T>
void deque <T> :: pop_front()
{
	assert(numElements > 0);
	iaFront = (iaFront + 1) % numCapacity;
	numElements--;
}

/******************************************************
 * DEQUE : PUSH_BACK
 ******************************************************/
template <class T>
void deque <T> :: push_back(const T & t) 
{
	if (numElements == numCapacity)
		resize(numCapacity == 0 ? 1 : numCapacity * 2);

	int ia = iaFromID(numElements);
	data[ia] = t;
	numElements++;
}

/******************************************************
 * DEQUE : PUSH_FRONT
 ******************************************************/
template <class T>
void deque <T> :: push_front(const T & t) 
{
	if (numElements == numCapacity)
		resize(numCapacity == 0 ? 1 : numCapacity * 2);

	iaFront = (iaFront - 1 + numCapacity) % numCapacity;
	data[iaFront] = t;
	numElements++;
}

/****************************************************
 * DEQUE :: RESIZE
 * Resize the deque so the numCapacity matches the newCapacity
 ***************************************************/
template <class T>
void deque <T> :: resize(int newCapacity) 
{
	if (newCapacity < (int)numElements)
		newCapacity = numElements;

	T* NewData = new T[newCapacity];

	for (size_t i = 0; i < numElements; i++)
		NewData[i] = (*this)[i];

	if (data)
		delete[] data;

	data = NewData;
	numCapacity = newCapacity;
	iaFront = 0;
}

} // namespace custom
