/* note for avl folder */

// Balanced BSTs that are kept in balance through tree rotations on insert and remove are called AVL trees.
// To quickly compute the balance factor, AVL trees store the height of every node as part of the node itself.
// Maintains the height at each node
// Maintains the balance factor after insert and remove
// consider where the deepest node that is out of balance
// identify the stick or elbow that causes the node to be out of balance
/*
Generic left rotation: 
a stick imbalance: b -> c -> d
deepest point of imblance is the node with the balance factor greater than 1;
d(b) = 2; d(c) = 1; an insertion under node d caused the imbalance
node is out of balance heavily to the right:
-> bring the middle of imbalance up to the mountain and rotate everything to the left
-> if the middle node has a left node, move it to the right node of the first node in the stick

Generic right-left Rotation:
an elbow imbalance: b -> c -> d
d(b) = 2; d(c) = -1; an insert under node d caused the imbalance
-> unbend the elbow with a rotation about the bend and then we have a stick, then we do a left rotation to fix the stick

Rotation: O(1)
                         b of the deepest point    b of the node in the direction of imbalance
left rotatin              2                        1
right rotation           -2                       -1
right-left rotation       2                       -1
left-right rotation      -2		           1
*/
// AVL.hpp 
// AVL insert 
// insert(4) -> imbalance -> rotate..
// insert at proper place
// check for imbalance
// rotate if necessary
// update height

// if current node has balance factor == 2: right subtree out of balance
// - r_balance = get_balance_factor(cur->right)
// - if r_balance == 1 or 0: stick imbalance -> rotateleft(cur)
// - if r_balance == -1: elbow imbalance     -> rotaterightleft(cur)

// if current node has balance factor == -2: left subtree out of balance
// - l_balance = get_balance_factor(cur->left)
// - if l_balance == -1 or 0: stick imbalance -> rotateright(cur)
// - if l_balance == 1: elbow imbalance       -> rotateleftrightleft(cur)

template <typename K, typename D>
void AVL<K, D>::_ensureBalance(TreeNode*& cur) {

  // Calculate the balance factor for this node:
  const int initial_balance = _get_balance_factor(cur);

  if (initial_balance == -2) {   // out of balance heavily to the left
    const int l_balance = _get_balance_factor(cur->left);  // check the balance of left node
    if (l_balance == -1 || l_balance == 0) {
      _rotateRight(cur);
    }
    else if (l_balance == 1) {   // elbow imbalance
      _rotateLeftRight(cur);
    }
  }
  else if (initial_balance == 2) {   // out of balance heavily to the right
    const int r_balance = _get_balance_factor(cur->right);
    if (r_balance == 1 || r_balance == 0) {      // stick imbalance
      _rotateLeft(cur);
    }
    else if (r_balance == -1) {    // elbow imbalance
      _rotateRightLeft(cur);
    }
  }
  _updateHeight(cur);

// AVL remove
// remove(5) -> imbalance -> rotate
// _iopRomove: find the rightmost of the left subtree -> swap -> delete

template <typename K, typename D>
const D& AVL<K, D>::_iopRemove(TreeNode*& node, TreeNode*& iop) {
  if (iop->right != nullptr) {
  // iop not found, keep going deeper
  const D & d = _iopRemove(node, iop->right);
  if (iop) {
  _ensureBalance(iop); 
  }  // extra checking
  return d;  // location of iop
  }
  else {  // iop->right == nullptr
  // found iop, swap the location:
  _swap(node, iop);
  std::swap(node, iop);
  
  // remove the swapped node at iop's position
  return _remove(iop);
  }
}



