/***********************************************************************
* Header:
*    Set
* Summary:
*      __       ____       ____         __
*     /  |    .'    '.   .'    '.   _  / /
*     `| |   |  .--.  | |  .--.  | (_)/ /
*      | |   | |    | | | |    | |   / / _
*     _| |_  |  `--'  | |  `--'  |  / / (_)
*    |_____|  '.____.'   '.____.'  /_/
*
*    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    Adam Sheridan, McClain Lindsay, and Trevaye Morris
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   // 
   // Construct
   //
   set() 
   { 
		
   }
   set(const set& rhs) : bst(rhs.bst) {}

   set(set && rhs) 
   { 
      bst = std::move(rhs.bst);
   }
   set(const std::initializer_list <T> & il) 
   {
      for (const auto& item : il)
      {
			//std::cout << "Inserting element in initializer list constructor\n";
         insert(item);
      }
   }
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
      for (; first != last; ++first)
         insert(*first);

   }

   ~set() { clear(); }

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
      if (this != &rhs)
      {
         clear();            // Clean up current tree
         bst = rhs.bst;      // Use BST's copy assignment (which uses the copy constructor)
      }
      return *this;



   }
   set & operator = (set && rhs)
   {
      if (this != &rhs)
      {
         clear();
         bst = std::move(rhs.bst);
      }
      return *this;

   }
   set & operator = (const std::initializer_list <T> & il)
   {
      clear();
      for (const auto& item : il)
      {
			//std::cout << "Inserting element in initializer list assignment\n";
         
         insert(item);
      }
      return *this;

   }
   void swap(set& rhs) noexcept
   {
      std::swap(bst, rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept 
   { 
      return iterator(bst.begin());
   }
   iterator end() const noexcept 
   { 
      return iterator(bst.end());
   }

   //
   // Access
   //
   iterator find(const T& t) 
   { 
      return iterator(); 
   }

   //
   // Status
   //
   bool   empty() const noexcept 
   { 
	   // Return true if bst is empty
       return bst.empty();
   }
   size_t size() const noexcept 
   { 
	   // Return the size of bst
       return bst.size();
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T& t)
   {
	   // Insert t into bst
       auto result = bst.insert(t, true);
	   return std::pair<iterator, bool>(iterator(result.first), result.second);
   }
   std::pair<iterator, bool> insert(T&& t)
   {
	  // Insert by move
      auto result = bst.insert(std::move(t), true);
	  return std::pair<iterator, bool>(iterator(result.first), result.second);
   }
   void insert(const std::initializer_list <T>& il)
   {
	   for (auto& item : il) // Iterate through each item in the initializer list
		   bst.insert(item, true); // Insert each item in the initializer list
	   
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
	   for (auto it = first; it != last; ++it) // Iterate through the range
		   bst.insert(*it, true); // Insert each item in the range
   }


   //
   // Remove
   //
   void clear() noexcept 
   { 
	   bst.clear(); // Clear the bst
   }
   iterator erase(iterator &it)
   { 
	   auto result = bst.erase(it.it); // Erase
	   return iterator(result); // Return new iterator
   }
   size_t erase(const T& t)
   {
       auto it = bst.find(t); // Find t in bst
       if (it != bst.end()) // If found
       {
           bst.erase(it);
           return 1;  // one item removed
       }
       return 0;      // nothing removed
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
	   auto it = itBegin;
       while (it != itEnd)
       {
		   auto temp = it;
		   ++temp;
		   bst.erase(it.it);
		   it = temp;
       }
	   return itEnd;
   }

private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;

public:
   // constructors, destructors, and assignment operator
   iterator() 
   { 
   }
   iterator(const typename custom::BST<T>::iterator& itRHS) : it(itRHS)
   {  
   }
   iterator(const iterator & rhs) : it(rhs.it)
   { 
      

   }
   iterator & operator = (const iterator & rhs)
   {

      it = rhs.it;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      return it != rhs.it;
   }
   bool operator == (const iterator & rhs) const 
   { 
      return it == rhs.it;
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
      return *it;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      ++it;
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
      iterator temp = *this;
      ++it;
      return temp;

   }
   
   // prefix decrement
   iterator & operator -- ()
   {
      --it;
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
      iterator temp = *this;
      --it;
      return temp;

   }
   
private:

   typename custom::BST<T>::iterator it;
};


}; // namespace custom



