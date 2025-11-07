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
   set() {}
   set(const set& rhs) : bst(rhs.bst) {}

   set(set && rhs) 
   { 
	  // Move constructor.
      bst = std::move(rhs.bst);
   }
   set(const std::initializer_list <T> & il) 
   {
	  // Initializer list constructor.
      for (const auto& item : il)
      {
         insert(item);
      }
   }
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
	  // Range constructor.
      for (; first != last; ++first)
         insert(*first);
   }

   ~set() { clear(); }

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
	  // Copy assignment operator.
      if (this != &rhs)
      {
         clear();
         bst = rhs.bst;
      }
      return *this;



   }
   set & operator = (set && rhs)
   {
	  // Move assignment operator.
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
	  // Initializer list assignment operator.
      for (const auto& item : il)
      {
         insert(item);
      }
      return *this;

   }
   void swap(set& rhs) noexcept
   {
	  // Swap function.
      std::swap(bst, rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept 
   {
	  // Return iterator to beginning of bst.
      return iterator(bst.begin());
   }
   iterator end() const noexcept 
   {
	  // Return iterator to end of bst.
      return iterator(bst.end());
   }

   //
   // Access
   //
   iterator find(const T& t) 
   {
      // Find t in bst.
      auto it = bst.find(t);

	  return iterator(it);
   }

   //
   // Status
   //
   bool   empty() const noexcept 
   { 
	   // Return true if bst is empty.
       return bst.empty();
   }
   size_t size() const noexcept 
   { 
	   // Return the size of bst.
       return bst.size();
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T& t)
   {
	   // Insert t into bst.
       auto result = bst.insert(t, true);
	   return std::pair<iterator, bool>(iterator(result.first), result.second);
   }
   std::pair<iterator, bool> insert(T&& t)
   {
	  // Insert by move.
      auto result = bst.insert(std::move(t), true);
	  return std::pair<iterator, bool>(iterator(result.first), result.second);
   }
   void insert(const std::initializer_list <T>& il)
   {
	   // Insert each item from initializer list into bst.
	   for (auto& item : il)
		   bst.insert(item, true);
	   
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
	   // Insert each item from range into bst.
	   for (auto it = first; it != last; ++it)
		   bst.insert(*it, true);
   }


   //
   // Remove
   //
   void clear() noexcept 
   {
	   // Clear the bst.
	   bst.clear();
   }
   iterator erase(iterator &it)
   { 
	   // Erase the item at iterator it from bst.
	   auto result = bst.erase(it.it);

	   return iterator(result);
   }
   size_t erase(const T& t)
   {
	   // Erase t from bst.
       auto it = bst.find(t);

	   // If found, erase it and return 1.
       if (it != bst.end())
       {
           bst.erase(it);
           return 1;
       }
       return 0;
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
	   // Erase the range from itBegin to itEnd from bst.
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
   friend class ::TestSet; // Give unit tests access to the privates.
   friend class custom::set<T>;

public:
   // Constructors, destructors, and assignment operator.
   iterator() 
   { 
   }
   iterator(const typename custom::BST<T>::iterator& itRHS) : it(itRHS)
   {
   }
   iterator(const iterator& rhs) : it(rhs.it)
   {

   }
   iterator & operator = (const iterator & rhs)
   {

      it = rhs.it;
      return *this;
   }

   // Equals, not equals operator.
   bool operator != (const iterator & rhs) const 
   { 
      return it != rhs.it;
   }

   // Equals operator.
   bool operator == (const iterator & rhs) const 
   { 
      return it == rhs.it;
   }

   // Dereference operator: by-reference so we can modify the Set.
   const T & operator * () const 
   { 
      return *it;
   }

   // Prefix increment.
   iterator & operator ++ ()
   {
      ++it;
      return *this;
   }

   // Postfix increment.
   iterator operator++ (int postfix)
   {
      iterator temp = *this;
      ++it;
      return temp;

   }
   
   // Prefix decrement.
   iterator & operator -- ()
   {
      --it;
      return *this;
   }
   
   // Postfix decrement.
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



