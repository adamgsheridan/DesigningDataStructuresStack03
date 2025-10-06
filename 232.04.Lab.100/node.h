/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:

   //
   // Construct
   //

  // Default constructor
   Node()
      : data(), pNext(nullptr), pPrev(nullptr)
   {
   }


   // Copy constructor
   Node(const T& data) : data(data)
   {
      pPrev = pNext = NULL;
   }

   // Move constructor
   Node(T&& data) : data(std::move(data))
   {
      pPrev = pNext = NULL;
   }


   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
   if (!pSource)
      return nullptr;

   Node<T>* pHead = new Node<T>(pSource->data);
   Node<T>* pTail = pHead;
   pSource = pSource->pNext;

   while (pSource)
   {
      Node<T>* newNode = new Node<T>(pSource->data);
      pTail->pNext = newNode;
      newNode->pPrev = pTail;
      pTail = newNode;
      pSource = pSource->pNext;
   }

   return pHead;

}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   Node<T>* pDestWalker = pDestination;
   Node<T>* pPrev = nullptr;

   // Case 0: Source is empty — delete all destination nodes
   if (!pSource)
   {
      while (pDestWalker)
      {
         Node<T>* temp = pDestWalker;
         pDestWalker = pDestWalker->pNext;
         delete temp;
      }
      pDestination = nullptr;
      return;
   }

   // Case 1: Destination is empty — clone the source list
   if (!pDestWalker)
   {
      pDestination = new Node<T>(pSource->data);
      pPrev = pDestination;
      pSource = pSource->pNext;

      while (pSource)
      {
         Node<T>* newNode = new Node<T>(pSource->data);
         pPrev->pNext = newNode;
         newNode->pPrev = pPrev;
         pPrev = newNode;
         pSource = pSource->pNext;
      }
      return;
   }

   // Case 2: Destination has nodes — reuse and extend
   while (pSource && pDestWalker)
   {
      pDestWalker->data = pSource->data;
      pPrev = pDestWalker;
      pDestWalker = pDestWalker->pNext;
      pSource = pSource->pNext;
   }

   // Source has more nodes — allocate and link
   while (pSource)
   {
      Node<T>* newNode = new Node<T>(pSource->data);
      if (pPrev)
      {
         pPrev->pNext = newNode;
         newNode->pPrev = pPrev;
      }
      else
      {
         pDestination = newNode;
      }
      pPrev = newNode;
      pSource = pSource->pNext;
   }

   // Destination has extra nodes — delete them
   while (pDestWalker)
   {
      Node<T>* temp = pDestWalker;
      pDestWalker = pDestWalker->pNext;
      delete temp;
   }

   // Finalize the tail
   if (pPrev)
      pPrev->pNext = nullptr;
}


/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{

}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   return new Node <T>;
}

/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   return new Node <T>;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   return 99;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   return out;
}

/*****************************************************
 * CLEAR
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{

}


