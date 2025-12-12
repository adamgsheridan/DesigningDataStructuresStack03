/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include "vector.h"   // because this->buckets is a vector
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
    /************************************************
     * UNORDERED SET
     * A set implemented as a hash
     ************************************************/
    template <typename T,
        typename Hash = std::hash<T>,
        typename EqPred = std::equal_to<T>,
        typename A = std::allocator<T> >
    class unordered_set
    {
        friend class ::TestHash;   // give unit tests access to the privates
    public:
        //
        // Construct
        //
        unordered_set() : numElements(0), maxLoadFactor(1.0f)
        {
            buckets = vector<list<T, A>>(10);
        }
        unordered_set(size_t numBuckets) : numElements(0), maxLoadFactor(1.0f)
        {
            if (numBuckets == 0)
                numBuckets = 1;
            buckets = vector<list<T, A>>(numBuckets);
        }
        unordered_set(const unordered_set& rhs)
        {
            numElements = rhs.numElements;
            maxLoadFactor = rhs.maxLoadFactor;
            buckets = rhs.buckets;
        }
        unordered_set(unordered_set&& rhs)
        {
            numElements = rhs.numElements;
            maxLoadFactor = rhs.maxLoadFactor;
            buckets = std::move(rhs.buckets);

            rhs.numElements = 0;
        }
        template <class Iterator>
        unordered_set(Iterator first, Iterator last) : numElements(0), maxLoadFactor(1.0f)
        {
            buckets = vector<list<T, A>>(10);
            while (first != last)
            {
                insert(*first);
                ++first;
            }
        }

        //
        // Assign
        //
        unordered_set& operator=(const unordered_set& rhs)
        {
            if (this != &rhs)
            {
                numElements = rhs.numElements;
                maxLoadFactor = rhs.maxLoadFactor;
                buckets = rhs.buckets;
            }
            return *this;
        }
        unordered_set& operator=(unordered_set&& rhs)
        {
            if (this != &rhs)
            {
                numElements = rhs.numElements;
                maxLoadFactor = rhs.maxLoadFactor;
                buckets = std::move(rhs.buckets);
                rhs.numElements = 0;
            }
            return *this;
        }
        unordered_set& operator=(const std::initializer_list<T>& il)
        {
            clear();
            reserve(il.size());
            for (const T& t : il)
                insert(t);
            return *this;
        }
        void swap(unordered_set& rhs)
        {
            std::swap(numElements, rhs.numElements);
            std::swap(maxLoadFactor, rhs.maxLoadFactor);
            buckets.swap(rhs.buckets);
        }

        //
        // Iterator
        //
        class iterator;
        class local_iterator;

        iterator begin();
        iterator end();
        local_iterator begin(size_t iBucket);
        local_iterator end(size_t iBucket);


        
        //
        // Access
        //
        size_t bucket(const T& t)
        {
            return (size_t)99;
        }
        iterator find(const T& t);


        //   
        // Insert
        //
        custom::pair<iterator, bool> insert(const T& t);

        void insert(const std::initializer_list<T>& il)
        {
            for (const auto& x : il)
                insert(x);
        }

        void reserve(size_t count)
        {
            size_t needed = static_cast<size_t>(std::ceil(count / maxLoadFactor));
            rehash(needed);
        }

        void rehash(size_t numBuckets);

        // 
        // Remove
        //
        void clear() noexcept
        {
        }
        iterator erase(const T& t);


        //
        // Status
        //
        size_t size() const { return numElements; }
        bool empty() const { return numElements == 0; }

        size_t bucket_count() const { return buckets.size(); }
        size_t bucket_size(size_t i) const { return buckets[i].size(); }

        float load_factor() const noexcept
        {
            return buckets.size() ? (float)numElements / buckets.size() : 0.0f;
        }

        float max_load_factor() const noexcept { return maxLoadFactor; }

        void max_load_factor(float m) { maxLoadFactor = m; }

private:
    custom::vector<custom::list<T, A>> buckets;
    size_t numElements;
    float maxLoadFactor;
};

        

/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   iterator() 
   {
   }
   iterator(const typename custom::vector<custom::list<T> >::iterator& itVectorEnd,
            const typename custom::vector<custom::list<T> >::iterator& itVector,
            const typename custom::list<T>::iterator &itList)
   {
   }
   iterator(const iterator& rhs) 
   { 
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   { 
      return true; 
   }
   bool operator == (const iterator& rhs) const 
   { 
      return true;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *(new T);
   }

   //
  // Arithmetic
  //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
       return *this;
   }

private:
    typename vector<list<T>>::iterator itVectorEnd;
    typename list<T>::iterator itList;
    typename vector<list<T>>::iterator itVector;
};

/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::local_iterator
{
    friend class ::TestHash;   // give unit tests access to the privates

    template <typename TT, typename HH, typename EE, typename AA>
    friend class custom::unordered_set;
public:
    // 
    // Construct
    //
    local_iterator()
    {
    }
    local_iterator(const typename custom::list<T>::iterator& itList)
    {
    }
    local_iterator(const local_iterator& rhs)
    {
    }

    //
    // Assign
    //
    local_iterator& operator = (const local_iterator& rhs)
    {
        return *this;
    }

    // 
    // Compare
    //
    bool operator != (const local_iterator& rhs) const
    {
        return true;
    }
    bool operator == (const local_iterator& rhs) const
    {
        return true;
    }

    // 
    // Access
    //
    T& operator * ()
    {
        return *(new T);
    }

    // 
    // Arithmetic
    //
    local_iterator& operator ++ ()
    {
        return *this;
    }
    local_iterator operator ++ (int postfix)
    {
        return *this;
    }

private:
    typename list<T>::iterator itList;
};

/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
typename unordered_set <T, Hash, E, A> ::iterator
unordered_set<T, Hash, E, A>::erase(const T& t)
{
    Hash hashFunction;
    E equal;

    // find bucket
    size_t index = hashFunction(t) % buckets.size();

    // search for element
    for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it)
    {
        if (equal(*it, t))
        {
            // erase returns iterator to next element in bucket
            auto itNext = buckets[index].erase(it);
            --numElements;

            // case 1: next element in the same bucket exists
            if (itNext != buckets[index].end())
            {
                return iterator(buckets.begin() + index, buckets.end(), itNext);
            }

            // case 2: find next non-empty bucket
            auto itBucket = buckets.begin() + index;
            ++itBucket;

            while (itBucket != buckets.end() && itBucket->empty())
                ++itBucket;

            // case 2a: no more buckets
            if (itBucket == buckets.end())
                return end();

            // case 2b: return iterator to first element of next nonempty bucket
            return iterator(itBucket, buckets.end(), itBucket->begin());
        }
    }

    // element not found
    return end();
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T, typename H, typename E, typename A>
custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool>
unordered_set<T, H, E, A>::insert(const T& t)
{
    // rehash if load factor exceeded
    if (load_factor() > maxLoadFactor)
        rehash(buckets.size() * 2);

    H hashFunction;
    E equal;

    size_t index = hashFunction(t) % buckets.size();

    // check if element already exists
    for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it)
    {
        if (equal(*it, t))
        {
            return custom::pair<iterator, bool>(
                iterator(buckets.begin() + index, buckets.end(), it),
                false
            );
        }
    }

    // insert at end of bucket
    buckets[index].push_back(t);
    ++numElements;

    auto it = buckets[index].end();
    --it; // now points to newly inserted element

    return custom::pair<iterator, bool>(
        iterator(buckets.begin() + index, buckets.end(), it),
        true
    );
}

template <typename T, typename H, typename E, typename A>
void unordered_set<T, H, E, A>::insert(const std::initializer_list<T>& il)
{
    for (const T& t : il)
        insert(t);
}

/*****************************************
 * UNORDERED SET :: REHASH
 * Re-Hash the unordered set by numBuckets
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
void unordered_set<T, Hash, E, A>::rehash(size_t numBuckets)
{
    if (numBuckets < 1)
        numBuckets = 1;

    // create new bucket array
    vector<list<T, A>> newBuckets(numBuckets);

    Hash hashFunction;

    // redistribute all existing elements
    for (size_t i = 0; i < buckets.size(); ++i)
    {
        for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
        {
            size_t newIndex = hashFunction(*it) % numBuckets;
            newBuckets[newIndex].push_back(*it);
        }
    }

    // swap into place
    buckets.swap(newBuckets);
}

/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator
unordered_set<T, H, E, A>::find(const T& t)
{
    H hashFunction;
    E equals;

    // determine which bucket the value should be in
    size_t index = hashFunction(t) % buckets.size();

    // search within the bucket list
    auto itList = buckets[index].begin();
    auto itListEnd = buckets[index].end();

    for (; itList != itListEnd; ++itList)
    {
        if (equals(*itList, t))
        {
            // build correct iterator pointing to the element
            auto itVector = buckets.begin() + index;
            auto itVectorEnd = buckets.end();

            return iterator(itVector, itVectorEnd, itList);
        }
    }

    // not found
    return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A>::iterator&
unordered_set<T, H, E, A>::iterator::operator++()
{
   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T, typename H, typename E, typename A>
void swap(unordered_set<T,H,E,A>& lhs, unordered_set<T,H,E,A>& rhs)
{
}

}// namespace custom
