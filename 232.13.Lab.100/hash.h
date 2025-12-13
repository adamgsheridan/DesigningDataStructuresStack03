/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <utility>  // for std::pair
#include <functional> // for std::hash
#include "pair.h"     // for custom::pair
#include <initializer_list> // for std::initializer_list
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T>
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
public:
   //
   // Construct
   //
	unordered_set() : numElements(0)
	{
	}

	unordered_set(unordered_set& rhs) : numElements(0)
	{
		// copy each bucket using iterators (NO list copy constructor)
		for (size_t i = 0; i < 10; ++i)
		{
			for (auto it = rhs.buckets[i].begin();
				it != rhs.buckets[i].end();
				++it)
			{
				buckets[i].push_back(*it);
				++numElements;
			}
		}
	}

	unordered_set(unordered_set&& rhs) : numElements(0)
	{
		// steal each bucket
		for (size_t i = 0; i < 10; ++i)
			buckets[i].swap(rhs.buckets[i]);
		// steal the element count
		numElements = rhs.numElements;
		rhs.numElements = 0;
	}

	template <class Iterator>
	unordered_set(Iterator first, Iterator last) : numElements(0)
	{
		for (; first != last; ++first)
		{
			size_t i = bucket(*first);

			// INSERT AT BACK (required by unit tests)
			buckets[i].push_back(*first);
			++numElements;
		}
	}


   //
   // Assign
   //
   unordered_set& operator=(unordered_set& rhs)
   {
	   // copy-and-swap idiom
	   if (this != &rhs)
	   {
		   unordered_set temp(rhs); // uses copy constructor
		   swap(temp);
	   }
	   return *this;
   }

   unordered_set& operator=(unordered_set&& rhs)
   {
	   // move-and-swap idiom
	   if (this != &rhs)
	   {
		   swap(rhs);
	   }
	   return *this;
   }

   unordered_set& operator=(const std::initializer_list<T>& il)
   {
	   clear();
	   for (const T& t : il)
		   insert(t);
	   return *this;
   }

   void swap(unordered_set& rhs)
   {
	   for (size_t i = 0; i < 10; ++i)
		   buckets[i].swap(rhs.buckets[i]);

	   std::swap(numElements, rhs.numElements);
   }


   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
	   // find the first non-empty bucket
	   for (size_t i = 0; i < 10; ++i)
		   if (!buckets[i].empty())
			   // return iterator to the first element of that bucket
			   return iterator(buckets + i, buckets + 10, buckets[i].begin());

	   // all buckets are empty: return end()
	   return end();
   }
   iterator end()
   {
	   // end iterator: pBucket and pBucketEnd both point past the last bucket
	   return iterator(buckets + 10, buckets + 10,
		   typename custom::list<T>::iterator());
   }
   local_iterator begin(size_t iBucket)
   {
	   // iterator to the first element in the specified bucket
	   return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
	   // iterator past the last element in the specified bucket
	   return local_iterator(buckets[iBucket].end());
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
	   return std::hash<T>()(t) % 10;
   }
   iterator find(const T& t);

   //   
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);


   // 
   // Remove
   //
   void clear() noexcept
   {
	   for (size_t i = 0; i < 10; ++i)
		   buckets[i].clear();
	   numElements = 0;
   }
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const 
   { 
	  return numElements;
   }
   bool empty() const 
   { 
	  return numElements == 0;
   }
   size_t bucket_count() const 
   { 
	  return 10;
   }
   size_t bucket_size(size_t i) const
   {
	  return buckets[i].size();
   }

private:

   custom::list<T> buckets [10];   // exactly 10 buckets
   int numElements;                // number of elements in the Hash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   iterator()  : pBucket(nullptr),
	   pBucketEnd(nullptr)
   {  
   }
   iterator(typename custom::list<T>* pBucket,
            typename custom::list<T>* pBucketEnd,
            typename custom::list<T>::iterator itList) : pBucket(pBucket),
                                                        pBucketEnd(pBucketEnd),
	   itList(itList)
   {
   }
   iterator(const iterator& rhs) : pBucket(rhs.pBucket),
       pBucketEnd(rhs.pBucketEnd),
	   itList(rhs.itList)
   { 
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
	   pBucket = rhs.pBucket;
	   pBucketEnd = rhs.pBucketEnd;
       itList = rhs.itList;
	   return *this;

   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   {
	   return (pBucket != rhs.pBucket || itList != rhs.itList);
   }
   bool operator == (const iterator& rhs) const 
   { 
	   return (pBucket == rhs.pBucket && itList == rhs.itList);
   }

   // 
   // Access
   //
   T& operator * ()
   {
	   return *itList;
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
	   iterator temp = *this;
	   ++(*this);
	   return temp;
   }

private:
   custom::list<T> *pBucket;
   custom::list<T> *pBucketEnd;
   typename list<T>::iterator itList;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
	local_iterator() : itList()
   {
   }
   local_iterator(const typename custom::list<T>::iterator& itList) : itList(itList)
   {
   }
   local_iterator(const local_iterator& rhs) : itList(rhs.itList)
   { 
   }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
	   itList = rhs.itList;
	   return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
	   return itList != rhs.itList;
   }
   bool operator == (const local_iterator& rhs) const
   {
	   return itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
	   return *itList;
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
	   ++itList;
	   return *this;
   }
   local_iterator operator ++ (int postfix)
   {
	   local_iterator temp = *this;
	   ++(*this);
	   return temp;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::erase(const T& t)
{
	// determine which bucket the value belongs to
	size_t i = bucket(t);

	// search the bucket for the value
	for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
		if (*it == t)
		{
			// iterator to the element after the one being erased
			auto next = it;
			++next;

			// remove the element from the bucket
			buckets[i].erase(it);
			--numElements;

			// if there is another element in the same bucket, return it
			if (next != buckets[i].end())
				return iterator(buckets + i, buckets + 10, next);

			// otherwise, find the next non-empty bucket
			for (size_t j = i + 1; j < 10; ++j)
				if (!buckets[j].empty())
					return iterator(buckets + j, buckets + 10, buckets[j].begin());

			// no more elements anywhere: return end()
			return end();
		}

	// value not found: return end()
	return end();
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T>
custom::pair<typename custom::unordered_set<T>::iterator, bool>
unordered_set<T>::insert(const T& t)
{
	// determine which bucket the value belongs to
	size_t i = bucket(t);

	// search the bucket to prevent duplicates
	for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
		if (*it == t)
			// element already exists
			return custom::pair<iterator, bool>(
				iterator(buckets + i, buckets + 10, it),
				false);

	// INSERT AT FRONT (required by unit tests)
	buckets[i].push_front(t);
	++numElements;

	// iterator must point to begin() of that bucket
	return custom::pair<iterator, bool>(
		iterator(buckets + i, buckets + 10, buckets[i].begin()),
		true);
}

template <typename T>
void unordered_set<T>::insert(const std::initializer_list<T>& il)
{
	// insert each element from the initializer list
	for (const T& t : il)
		insert(t);
}

/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::find(const T& t)
{
	size_t i = bucket(t);

	for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
		if (*it == t)
			return iterator(buckets + i, buckets + 10, it);

	return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator & unordered_set<T>::iterator::operator ++ ()
{
	// move forward within the current bucket
	++itList;

	// if we reach the end of this bucket,
	// move to the next non-empty bucket
	while (pBucket != pBucketEnd && itList == pBucket->end())
	{
		++pBucket;
		if (pBucket != pBucketEnd)
			itList = pBucket->begin();
	}

   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T>
void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
{
	lhs.swap(rhs);
}

}
