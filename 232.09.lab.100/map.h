/***********************************************************************
 * Header:
 *    map
 * Summary:
 *    Our custom implementation of a std::map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    Adam Sheridan, McClain Lindsay, and Trevaye Morris
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map() {}

   map(const map &  rhs) 
   { 
      // Copy the BST from rhs to this map
      bst = rhs.bst;
   }
   map(map && rhs) 
   {
      // Move the BST from rhs to this map
      bst = std::move(rhs.bst);
   }
   template <class Iterator>
   map(Iterator first, Iterator last) 
   {
      // Insert each pair from the range into the BST
      for (Iterator it = first; it != last; ++it)
      {
         bst.insert(*it);
      }
   }
   map(const std::initializer_list <Pairs>& il) 
   {
      // Insert each pair from the initializer list into the BST
      for (const auto& pair : il)
      {
         bst.insert(pair);
      }
   }
  ~map()         
  {
     clear();
  }

   //
   // Assign
   //
   map & operator = (const map & rhs) 
   {
      // Copy the BST from rhs to this map
      if (this != &rhs)
      {
         bst = rhs.bst;
      }
      return *this;
   }
   map & operator = (map && rhs)
   {
       // Move the BST from rhs to this map
       if (this != &rhs)
      {
         bst = std::move(rhs.bst);
      }
	   return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
       // Clear the existing BST
       clear();

       // Insert each pair from the initializer list into the BST
       for (const auto& pair : il)
       {
           bst.insert(pair);
       }
	   return *this;
   }
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin()
   {
       // return the BST begin iterator wrapped in a map iterator
       return iterator(bst.begin());
   }
   iterator end()
   {
       // return the BST end iterator wrapped in a map iterator
       return iterator(bst.end());
   }

   // 
   // Access
   //
   const V& operator [] (const K& key) const;
   V& operator [] (const K& key);
   const V& at(const K& key) const;
   V& at(const K& key);
   iterator find(const K& key);
  
   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
      auto it = bst.find(custom::pair<K, V>(rhs.first, V()));
      if (it != bst.end())
         return custom::make_pair(iterator(it), false);

      auto result = bst.insert(std::move(rhs)); // returns std::pair<BST::iterator, bool>
      return custom::make_pair(iterator(result.first), result.second);



   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
      // Try to find the key
      auto it = bst.find(custom::pair<K, V>(rhs.first, V()));

      // If found, return iterator and false (no insertion)
      if (it != bst.end())
         return custom::make_pair(iterator(it), false);

      // Otherwise, insert the new pair
      bst.insert(rhs);

      // Find again to get iterator to inserted element
      it = bst.find(custom::pair<K, V>(rhs.first, V()));
      return custom::make_pair(iterator(it), true);

   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
   }

   //
   // Remove
   //
   void clear() noexcept
   {
      bst.clear();
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept { return bst.empty(); }
   size_t size() const noexcept { return bst.size(); } 


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //
   iterator(){}
   iterator(const typename BST < pair <K, V> > :: iterator & rhs) : it(rhs) {}
   iterator(const iterator & rhs) : it(rhs.it) {}

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
	   it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const { return it == rhs.it; }
   bool operator != (const iterator & rhs) const { return it != rhs.it; }

   // 
   // Access
   //
   const pair <K, V> & operator * () const
   {
       // Dereference the BST iterator.
	   return *it;
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
       // Increment the BST iterator.
	   ++it;
      return *this;
   }
   iterator operator ++ (int)
   {
       // Increment the BST iterator.
	   iterator temp = *this;
	   ++it;
      return temp;
   }
   iterator & operator -- ()
   {
       // Decrement the BST iterator
	   --it;
      return *this;
   }
   iterator  operator -- (int)
   {
       // Decrement the BST iterator
	   iterator temp(*this);
	   --it;
      return temp;
   }

private:

   // Member variable.
   typename BST < pair <K, V >>  :: iterator it;   
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> :: operator [] (const K& key)
{
    // Try to find the key in the tree
    auto it = bst.find(custom::pair<K, V>(key, V()));

    // If not found, insert a new pair with default value
    if (it == bst.end())
        bst.insert(custom::pair<K, V>(key, V()));

    // Find again and return reference to the value
    it = bst.find(custom::pair<K, V>(key, V()));
    return const_cast<V&>((*it).second);
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> :: operator [] (const K& key) const
{
	// Try to find the key in the tree.
    auto it = bst.find(custom::pair<K, V>(key, V()));
    if (it == bst.end())
    {
        static V empty{};
        return empty;
    }
    return (*it).second;
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> ::at(const K& key)
{
   // Try to find the key in the tree.
   auto it = bst.find(custom::pair<K, V>(key, V()));
   if (it == bst.end())
      throw std::out_of_range("invalid map<K, T> key");
   return const_cast<V&>((*it).second);

}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
   // Try to find the key in the tree.
   auto it = bst.find(custom::pair<K, V>(key, V()));
   if (it == bst.end())
      throw std::out_of_range("map::at() const - key not found");
   return (*it).second;

}

/*****************************************************
 * MAP :: FIND
 * Find an element in the map
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::find(const K& key)
{
    // Use the BST’s find() to locate the key/value pair.
    return iterator(bst.find(custom::pair<K, V>(key, V())));
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
	// Swap the underlying BSTs.
	lhs.bst.swap(rhs.bst);
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
   // Try to find the key.
   auto it = bst.find(custom::pair<K, V>(k, V()));

   // If not found, return 0.
   if (it == bst.end())
      return 0;

   // Otherwise, erase the element and return 1.
   bst.erase(it);
   return 1;

}

/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   // Erase elements in the range [first, last).
   while (first != last)
      first = erase(first);
   return last;

}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   // If the iterator is at the end, return end().
   if (it == end())
      return end();

   // Get the next iterator before erasing.
   auto next = it;
   ++next;
   bst.erase(it.it);
   return next;

}

}; //  namespace custom

