/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Adam Sheridan, McClain Lindsay, and Trevaye Morris
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair
#include <initializer_list> // for std::initializer_list

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return numElements == 0; }
   size_t size()  const noexcept { return numElements; }
   

private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode() : data(), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}
   BNode(const T &  t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}
   BNode(T && t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return true; }
   bool isLeftChild( BNode * pNode) const { return true; }

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set; 
public:
   // constructors and assignment
   iterator(BNode* p = nullptr) : pNode(p) {}
   iterator(const iterator & rhs)         
   {
	   pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
       pNode = rhs.pNode;
	   return *this;
   }

   // compare
   bool operator == (const iterator& rhs) const { return pNode == rhs.pNode; }
   bool operator != (const iterator& rhs) const { return pNode != rhs.pNode; }

   // de-reference. Cannot change because it will invalidate the BST
   const T& operator * () const { return pNode->data; }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
 /*********************************************
  * BST :: COPY CONSTRUCTOR
  * Copy one tree to another
  ********************************************/
template <typename T>
BST<T>::BST(const BST<T>& rhs)
{
    numElements = rhs.numElements;
    root = nullptr;

    if (rhs.root)
    {
        // Recursive lambda to deep-copy nodes
        std::function<BNode* (BNode*, BNode*)> copyNodes =
            [&](BNode* srcNode, BNode* parent) -> BNode*
            {
                if (!srcNode)
                    return nullptr;

                // Create a new node with the same data
                BNode* newNode = new BNode(srcNode->data);
                newNode->pParent = parent;

                // Recursively copy left and right children
                newNode->pLeft = copyNodes(srcNode->pLeft, newNode);
                newNode->pRight = copyNodes(srcNode->pRight, newNode);

                return newNode;
            };

        // Copy the entire subtree rooted at rhs.root
        root = copyNodes(rhs.root, nullptr);
    }
}


/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) 
{
   numElements = rhs.numElements;
   root = rhs.root;
   rhs.numElements = 0;
   rhs.root = nullptr;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST<T>::BST(const std::initializer_list<T>& il)
{
    numElements = 0;
    root = nullptr;
    for (const T& t : il)       // iterate as const T& so insert(const T&) is used
        insert(t);
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST<T>& BST<T>::operator=(const BST<T>& rhs)
{
    if (this == &rhs) return *this;

    if (!rhs.root) { clear(); return *this; }
    if (!root)
    {
        struct NodePair { BNode* dest; BNode* src; };
        NodePair stack[128]; int top = 0;
        root = new BNode(rhs.root->data); root->pParent = nullptr;
        stack[top++] = { root, rhs.root };
        while (top > 0)
        {
            NodePair pair = stack[--top];
            BNode* d = pair.dest; BNode* s = pair.src;

            if (s->pLeft)
            {
                d->pLeft = new BNode(s->pLeft->data);
                d->pLeft->pParent = d;
                stack[top++] = { d->pLeft, s->pLeft };
            }
            if (s->pRight) { d->pRight = new BNode(s->pRight->data); d->pRight->pParent = d; stack[top++] = { d->pRight, s->pRight }; }
        }
        numElements = rhs.numElements;
        return *this;
    }

    root->data = rhs.root->data;
    struct NodePair { BNode* dest; BNode* src; };
    NodePair stack[128]; int top = 0;
    stack[top++] = { root, rhs.root };

    while (top > 0) {
        NodePair pair = stack[--top];
        BNode* d = pair.dest; BNode* s = pair.src;

        for (int i = 0; i < 2; ++i) {
            BNode*& childDest = (i == 0 ? d->pLeft : d->pRight);
            BNode* childSrc = (i == 0 ? s->pLeft : s->pRight);

            if (childSrc && childDest) { childDest->data = childSrc->data; stack[top++] = { childDest, childSrc }; }
            else if (childSrc && !childDest) {
                childDest = new BNode(childSrc->data); childDest->pParent = d;
                stack[top++] = { childDest, childSrc };
            }
            else if (!childSrc && childDest) {
                BNode* toDelete = childDest;
                BNode* delStack[128]; int delTop = 0; delStack[delTop++] = toDelete;
                while (delTop > 0) {
                    BNode* n = delStack[--delTop];
                    if (n->pLeft) delStack[delTop++] = n->pLeft;
                    if (n->pRight) delStack[delTop++] = n->pRight;
                    delete n;
                }
                childDest = nullptr;
            }
        }
    }

    numElements = rhs.numElements;
    return *this;
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   clear();
   for (const T& t : il)
      insert(t);
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   if (this == &rhs)
      return *this;
   clear();
   numElements = rhs.numElements;
   root = rhs.root;
   rhs.numElements = 0;
   rhs.root = nullptr;
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
    std::swap(root, rhs.root);
	std::swap(numElements, rhs.numElements);

}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST<T>::iterator, bool> BST<T>::insert(const T& t, bool keepUnique)
{
    BNode* newNode = new BNode(t);
    if (!root)
    {
        root = newNode;
        numElements++;
        return { iterator(root), true };
    }

    BNode* current = root;
    BNode* parent = nullptr;

    while (current)
    {
        parent = current;
        if (t < current->data)
            current = current->pLeft;
        else if (keepUnique && !(t < current->data) && !(current->data < t))
            return { iterator(current), false };
        else
            current = current->pRight;
    }

    newNode->pParent = parent;
    if (newNode->data < parent->data)
        parent->pLeft = newNode;
    else
        parent->pRight = newNode;

    numElements++;
    return { iterator(newNode), true };
}



template <typename T>
std::pair<typename BST<T>::iterator, bool> BST<T>::insert(T&& t, bool keepUnique)
{
    BNode* newNode = new BNode(std::move(t));
    if (!root)
    {
        root = newNode;
        numElements++;
        return { iterator(root), true };
    }

    BNode* current = root;
    BNode* parent = nullptr;

    while (current)
    {
        parent = current;
        if (newNode->data < current->data)
            current = current->pLeft;
        else if (keepUnique && !(newNode->data < current->data) && !(current->data < newNode->data))
            return { iterator(current), false };
        else
            current = current->pRight;
    }

    newNode->pParent = parent;
    if (newNode->data < parent->data)
        parent->pLeft = newNode;
    else
        parent->pRight = newNode;

    numElements++;
    return { iterator(newNode), true };
}



/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator& it)
{
    // If iterator is invalid or at end, do nothing
    if (it == end() || !it.pNode)
        return end();

    BNode* node = it.pNode;       // node to be deleted
    BNode* parent = node->pParent;  // parent of the node
    BNode* next = nullptr;        // node to return after deletion

    // ===== CASE 1: Node has NO children =====
    if (!node->pLeft && !node->pRight)
    {
        if (!parent)
        {
            // Node is root, tree becomes empty
            root = nullptr;
        }
        else if (parent->pLeft == node)
        {
            parent->pLeft = nullptr;
        }
        else
        {
            parent->pRight = nullptr;
        }

        next = parent;  // The iterator after erase could point to parent
        delete node;
    }

    // ===== CASE 2: Node has ONE child =====
    else if (!node->pLeft || !node->pRight)
    {
        // Determine which child exists
        BNode* child = (node->pLeft) ? node->pLeft : node->pRight;
        child->pParent = parent;

        if (!parent)
        {
            // Node is root
            root = child;
        }
        else if (parent->pLeft == node)
        {
            parent->pLeft = child;
        }
        else
        {
            parent->pRight = child;
        }

        next = child;   // Move iterator to the promoted child
        delete node;
    }

    // ===== CASE 3: Node has TWO children =====
    else
    {
        // Find in-order successor (smallest in right subtree)
        BNode* successor = node->pRight;
        while (successor->pLeft)
            successor = successor->pLeft;

        // Copy successor's data into current node
        node->data = successor->data;

        // Recursively erase the successor
        iterator temp(successor);
        erase(temp);

        // Iterator remains pointing at original node
        return it;
    }

    numElements--;
    return iterator(next ? next : nullptr);
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   std::function<void(BNode*)> deleteNodes = [&](BNode* node)
   {
      if (!node)
         return;
      deleteNodes(node->pLeft);
      deleteNodes(node->pRight);
      delete node;
   };
   deleteNodes(root);
   root = nullptr;
   numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::begin() const noexcept
{
    BNode* current = root;
    if (!current)
        return iterator(nullptr);
    while (current->pLeft)
        current = current->pLeft;
    return iterator(current);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::find(const T& t)
{
    BNode* current = root;
    while (current)
    {
        if (t == current->data)
            return iterator(current);
        else if (t < current->data)
            current = current->pLeft;
        else
            current = current->pRight;
    }
    return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{

}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{

}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{

}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{

}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{

}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{

}



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   return *this;  
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   return *this;

}


} // namespace custom


