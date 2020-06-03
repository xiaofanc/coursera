Binary Search Tree (BST)
/* note for BST folder */

// evry node in the left subtree is smaller than the root, and every node in the right subtree is larger than the root

// dictionary as our implementation structure of the binary search tree
// Dictionary ADT: find, insert, remove, empty
// A BST used to implement a dictionary will store both a key and data at every node
// ADT: find, insert, remove, empty

// Dictionary.h
#pragma once
// We include <stdexcept> so we can throw std::runtime_error in some cases.
#include <stdexcept>
// We include <utility> for the std::swap function
#include <utility>
// We'll add a "printInOrder" function to help us inspect the results.
// This will require std::cout from <iostream>.
#include <iostream>
template <typename K, typename D>
class Dictionary {
  public:
    // Let the constructor just initialize the head pointer to null.
    // We'll skip implementing other constructors for this example.
    Dictionary() : head_(nullptr) { }

    // find, insert, remove: Please see Dictionary.hpp for comments on these.
    const D& find(const K& key);
    void insert(const K& key, const D& data);
    const D& remove(const K& key);

  private:
    class TreeNode {
      public:
        // *See note 1 below about how references are being used here.
        const K& key;
        const D& data;

        // Note that you can declare multiple pointers on the same line as
        // shorthand, like this:
        //   TreeNode *left, *right;
        // But since this requires you to write the "*" with each variable
        // name, it can be a little confusing, or prone to making a mistake.
        // Instead, you can declare the pointers on separate lines like this:
        TreeNode* left;
        TreeNode* right;

        // **See note 2 below about how this initialization list is styled.
        TreeNode(const K& key, const D& data) // constructor
          : key(key), data(data), left(nullptr), right(nullptr) { }
    };

    TreeNode *head_;

// find in BST

worst case: O(h)

template <typename K, typename D>
const D& Dictionary<K, D>::find(const K& key) {
  // Find the key in the tree starting at the head.
  // If found, we receive the tree's actual stored pointer to that node
  //   through return-by-reference.
  // If not found, then the node returned has a value of nullptr.
  TreeNode*& node = _find(key, head_);

  if (node == nullptr) { throw std::runtime_error("error: key not found"); }
  // We found the node, so return the actual data there, by reference.
  return node->data;
}

// The fully-qualified return type of the below function is:
// Dictionary<K, D>::TreeNode*&
// That is a pointer to a Dictionary<K, D>::TreeNode, returned by reference.

template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_find(
  const K& key, TreeNode*& cur) const {
  if (cur == nullptr) { return cur; }
  else if (key == cur->key) { return cur; }
  else if (key < cur->key) { return _find(key, cur->left); }
  else { return _find(key, cur->right); }
}

// insert into a BST

template <typename K, typename D>
void Dictionary<K, D>::insert(const K& key, const D& data) {
  // Find the place where the item should go.
  // _find returns a pointer by reference to the exact location where we need to insert it !!
  TreeNode *& node = _find(key, head_);
  // For the sake of this example, let's disallow duplicates. If the node
  // found isn't a nullptr, then the key already exists, so report an error.
  // (We could also do something nicer than this, like remove the old key and
  // then insert the new item to replace it.)
  if (node) { throw std::runtime_error("error: insert() used on an existing key"); }
  node = new TreeNode(key, data);
}

// remove from BST

case 1: 0 children, delete the node
case 2: 1 child, works like a linked list
case 3: 2 children:
        -> find the IOP of the node to be removed (cases!!)
        -> swap with the IOP (recursion until the node was swapped into the leaf node)
        -> remove the node in its new position 
 
// In-Order Predecessor (IOP): The IOP of a node will always be the right-most node in the node's left subtree
// 2 4 11 19 20 37 51 55

template <typename K, typename D>
const D& Dictionary<K, D>::remove(const K& key) {
  // First, find the actual pointer to the node containing this key.
  // If not found, then the pointer returned will be equal to nullptr.
  TreeNode*& node = _find(key, head_);
  if (!node) { throw std::runtime_error("error: remove() used on non-existent key"); }
  return _remove(node);
}

template <typename K, typename D>
const D& Dictionary<K, D>::_remove(TreeNode*& node) {

  if (!node) { throw std::runtime_error("error: _remove() used on non-existent key"); }
  // Zero child remove:
  if (node->left == nullptr && node->right == nullptr) {
    const D& data = node->data;    // return data by reference
    delete node;
    node = nullptr;
    return data;
  }
  // One-child (left) remove
  else if (node->left != nullptr && node->right == nullptr) {
    // Similar to the previous case, except that we need to remap the "node"
    // pointer to point to the node's child, so that the parent of the node
    // being deleted will retain its connection to the rest of the tree
    // below this point.
    const D& data = node->data;
    TreeNode* temp = node;
    node = node->left;  // like linked list
    delete temp;
    return data;
  }
  // One-child (right) remove
  else if (node->left == nullptr && node->right != nullptr) {
    // This case is symmetric to the previous case.
    const D& data = node->data;
    TreeNode* temp = node;
    node = node->right;
    delete temp;
    return data;
  }
  // Two-child remove
  else {
    // Find the IOP (in-order predecessor) of the current node.
    TreeNode*& iop = _iop_of(node);

    // Since this is the two-child remove case, we know that some in-order
    // predecessor does exist, so the _iop_of lookup should not have failed.
    if (!iop) {
      throw std::runtime_error("error in two-child remove: IOP not found");
    }

    // Swap the node to remove and the IOP.
    TreeNode*& moved_node = _swap_nodes(node, iop);

    // Recurse to remove the original targeted node at its updated position.
    return _remove(moved_node); // 0 or left child
  }
}
