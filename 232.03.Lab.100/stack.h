/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
//#include "vector.h"
#include <vector>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   // 
   // Construct
   // 

   stack() : container() {}
   stack(const stack<T>& rhs) : container(rhs.container) {}
   stack(stack<T>&& rhs) noexcept : container(std::move(rhs.container)) {}
   stack(const std::vector<T>& rhs) : container(rhs) {}
   stack(std::vector<T>&& rhs) noexcept : container(std::move(rhs)) {}

   //
   // Assign
   //

   stack <T> & operator = (const stack <T> & rhs)
   {
       if (this != &rhs)
         container = rhs.container;
	   return *this;
   }
   stack <T>& operator = (stack <T> && rhs)
   {
       if (this != &rhs)
         container = std::move(rhs.container);
	   return *this;
   }
   void swap(stack <T>& rhs)
   {
	   container.swap(rhs.container);
   }

   // 
   // Access
   //

   T& top()       {
      //assert(!empty());
      return container.back();
   }

   const T& top() const { 
      //assert(!empty());
      return container.back();
   }

   // 
   // Insert
   // 

   void push(const T& t) { container.push_back(t); }
   void push(T&& t) { container.push_back(std::move(t)); }

   //
   // Remove
   //

   void pop() 
   {
      if (empty())
        return;
      container.pop_back();
      if (container.empty())
        container.shrink_to_fit();
   }

   //
   // Status
   //
   size_t  size () const { return container.size(); }
   bool empty() const { return container.empty(); }
   
private:
   
  std::vector<T> container;  // underlying container
};



} // custom namespace


