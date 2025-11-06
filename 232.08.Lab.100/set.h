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
      return true;    
   }
   size_t size() const noexcept 
   { 
      return 99;     
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T& t)
   {
      auto result = bst.insert(t, true); // insert into BST, enforcing uniqueness
      return std::make_pair(iterator(result.first), result.second);

   }
   std::pair<iterator, bool> insert(T&& t)
   {
      auto result = bst.insert(std::move(t), true); // move into BST
      return std::make_pair(iterator(result.first), result.second);

   }
   void insert(const std::initializer_list <T>& il)
   {
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
   }


   //
   // Remove
   //
   void clear() noexcept 
   { 
		bst.clear();
   }
   iterator erase(iterator &it)
   { 
      return iterator(); 
   }
   size_t erase(const T & t) 
   {
      return 99;
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
      return iterator();
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



