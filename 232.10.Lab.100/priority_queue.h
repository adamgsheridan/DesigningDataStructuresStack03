/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h"
#include <utility> // for std::swap
#include <stdexcept> // for std::out_of_range

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT>
   friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);
public:

   //
   // construct
   //
	priority_queue() = default;

	// copy and move construct
   priority_queue(const priority_queue &  rhs)  
   {
	   container = rhs.container;
   }
   priority_queue(priority_queue && rhs)  
   {
	   container = std::move(rhs.container);
	   heapify(); // ensure the moved container is a heap
   }

   // range construct
   template <class Iterator>
   priority_queue(Iterator first, Iterator last) 
   {
       for (auto it = first; it != last; ++it)
           container.push_back(*it);
	   heapify();
   }
   // construct from vector
   explicit priority_queue (custom::vector<T> && rhs) 
   {
       container = std::move(rhs);
	   heapify();
   }
   // construct from copy of vector
   explicit priority_queue (custom::vector<T>& rhs)
   {
       container = rhs;
	   heapify();
   }
  ~priority_queue() {}

  //
  // assign 
  //
   priority_queue & operator = (const priority_queue & rhs)
   {
       if (this != &rhs)
       {
           container = rhs.container;
       }
       return *this;
   }

   priority_queue & operator = (priority_queue && rhs)
   {
       if (this != &rhs)
       {
           container = std::move(rhs.container);
           heapify(); // ensure the moved container is a heap
       }
       return *this;
   }

   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);     

   //
   // Remove
   //
   void  pop(); 

   //
   // Status
   //
   size_t size()  const 
   { 
      return container.size();

   }
   bool empty() const 
   { 
      return container.empty();

   }
   
private:

   void heapify();                            // convert the container in to a heap
   bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

   custom::vector<T> container; 

};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T>
const T & priority_queue <T> :: top() const
{
   if (container.empty())
   {
      throw std::out_of_range("priority_queue::top() called on empty queue");
   }
   return container[0];  // root of the heap


}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T>
void priority_queue <T> :: pop()
{
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T>
void priority_queue <T> :: push(const T & t)
{
}
template <class T>
void priority_queue <T> :: push(T && t)
{
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T>
bool priority_queue <T> :: percolateDown(size_t indexHeap)
{
   size_t left = 2 * indexHeap + 1; 
   size_t right = 2 * indexHeap + 2; 
   size_t largest = indexHeap; 

   if (left < container.size() && container[left] > container[largest]) 
      largest = left; 
   if (right < container.size() && container[right] > container[largest]) 
      largest = right; 

   if (largest != indexHeap) 
   {
      std::swap(container[indexHeap], container[largest]); 
      percolateDown(largest); 
      return true; 
   }
   return false;

}

/************************************************
 * P QUEUE :: HEAPIFY
 * Turn the container into a heap.
 ************************************************/
template <class T>
void priority_queue <T> ::heapify()
{
   // Start from the last non-leaf node and percolate down
   for (int i = (int)container.size() / 2 - 1; i >= 0; --i)
   {
      percolateDown(i);
   }

}

/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T>
inline void swap(custom::priority_queue <T>& lhs,
                 custom::priority_queue <T>& rhs)
{
	using std::swap;
	swap(lhs.container, rhs.container);
}

};

