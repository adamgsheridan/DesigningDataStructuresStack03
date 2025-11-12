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
 *    <your names here>
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
   map() 
   {
   }
   map(const map &  rhs) 
   { 
   }
   map(map && rhs) 
   { 
   }
   template <class Iterator>
   map(Iterator first, Iterator last) 
   {
   }
   map(const std::initializer_list <Pairs>& il) 
   {
   }
  ~map()         
   {
   }

   //
   // Assign
   //
   map & operator = (const map & rhs) 
   {
      return *this;
   }
   map & operator = (map && rhs)
   {
      return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
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
      return make_pair(iterator(), false);
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
      return make_pair(iterator(), false);
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
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept { return false; }
   size_t size() const noexcept { return 99; }


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
	   return *it; // dereference the BST iterator
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
	   ++it; // increment the BST iterator
      return *this;
   }
   iterator operator ++ (int)
   {
	   iterator temp = *this;
	   ++it; // increment the BST iterator
      return temp;
   }
   iterator & operator -- ()
   {
	   --it; // decrement the BST iterator
      return *this;
   }
   iterator  operator -- (int)
   {
	   iterator temp(*this);
	   --it; // decrement the BST iterator
      return temp;
   }

private:

   // Member variable
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
    auto it = bst.find(custom::pair<K, V>(key, V()));
    if (it == bst.end())
    {
        static V empty{};
        return empty;  // return default if not found
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
    auto it = bst.find(custom::pair<K, V>(key, V()));
    assert(it != bst.end());
    return const_cast<V&>((*it).second);
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
    auto it = bst.find(custom::pair<K, V>(key, V()));
    assert(it != bst.end());
    return (*it).second;
}

/*****************************************************
 * MAP :: FIND
 * Find an element in the map
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::find(const K& key)
{
    // Use the BST’s find() to locate the key/value pair
    return iterator(bst.find(custom::pair<K, V>(key, V())));
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
   return size_t(99);
}

/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   return iterator();
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   return iterator();
}

}; //  namespace custom

