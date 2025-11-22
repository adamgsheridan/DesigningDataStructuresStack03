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
 *    Adam Sheridan, McClain Lindsay, and Trevaye Morris
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
	   // Reserve space and copy elements
	   size_t numElements = std::distance(first, last);
       container.reserve(numElements);
	   // Copy elements
       for (Iterator it = first; it != last; ++it)
       {
           container.push_back(*it);
       }
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
   // Check for empty
   if (container.empty())
   {
      throw std::out_of_range("std:out_of_range");
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
	// Remove the top item
    if (!container.empty())
    {
		// Move the last item to the top and pop the last
        container[0] = std::move(container.back());
        container.pop_back();
		// Percolate down from the root
        if (!container.empty())
            percolateDown(1);
    }
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T>
void priority_queue <T> :: push(const T & t)
{
   // Add the new element to the end of the container
   container.push_back(t);
   // Percolate up
   size_t indexHeap = container.size() - 1; 
   size_t parent = (indexHeap - 1) / 2; 
   while (indexHeap > 0 && container[indexHeap] > container[parent]) 
   {
      std::swap(container[indexHeap], container[parent]); 
      indexHeap = parent; 
      parent = (indexHeap - 1) / 2; 
   }
}
template <class T>
void priority_queue <T> :: push(T && t)
{
	// Add the new element to the end of the container
	container.push_back(std::move(t));
	// Percolate up
	size_t indexHeap = container.size() - 1;
	size_t parent = (indexHeap - 1) / 2;
    while (indexHeap > 0 && container[indexHeap] > container[parent])
    {
        std::swap(container[indexHeap], container[parent]);
        indexHeap = parent;
        parent = (indexHeap - 1) / 2;
	}
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
	// Check for valid index
    if (indexHeap == 0 || indexHeap > container.size())
        return false;

	// Convert to 0-based index
    size_t i = indexHeap - 1;
    bool changed = false;
    size_t n = container.size();

	// Percolate down
    while (true)
    {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t largest = i;

		// Find the largest among root, left child and right child
        if (left < n && container[left] > container[largest])
            largest = left;
        
		// Check right child
        if (right < n && container[right] > container[largest])
            largest = right;

		// If the largest is still the root, we're done
        if (largest == i)
            break;

		// Swap and continue percolating down
        std::swap(container[i], container[largest]);
        changed = true;

		// Move to largest index
        i = largest;
    }

    return changed;
}

/************************************************
 * P QUEUE :: HEAPIFY
 * Turn the container into a heap.
 ************************************************/
template <class T>
void priority_queue <T> ::heapify()
{
	// Start from the last parent and percolate down to the root
    size_t n = container.size();
    if (n < 2) return;

	// (n - 2) / 2 is the last parent index in 0-based
    for (int i = (int(n) - 2) / 2 + 1; i > 0; --i)
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
	// Swap the containers
	using std::swap;
	swap(lhs.container, rhs.container);
}

};