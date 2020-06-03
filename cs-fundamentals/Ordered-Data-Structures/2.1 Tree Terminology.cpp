2.1 Terminology
// A tree is a rooted, directed, and acyclic structure.
// node edges
// nodes that contain no outgoing edges are called leaf nodes
// KM: edge starts from K to M; edge never contains data
// siblings: nodes that share same parents
// ancestor: root
// grandchild: second level after parent
// grandparent

2.2 Binary Trees
/* note for binary-tree folder */
A binary tree is a tree where every node has at most two children.

#pragma once

template <typename T>
class BinaryTree {
  public:
    // ...

  private:
    class TreeNode {
      public:
        // *See note below about how references are being used here.
        T & data;

        // Note that you can declare multiple pointers on the same line as
        // shorthand, like this:
        //   TreeNode *left, *right;

        // But since this requires you to write the "*" with each variable
        // name, it can be a little confusing, or prone to making a mistake.
        // Instead, you can declare the pointers on separate lines like this:
        TreeNode* left;
        TreeNode* right;

        // **See note below about how this initialization list is styled.
        TreeNode(T & data) : data(data), left(nullptr), right(nullptr) { }
    };

    TreeNode *root_;
};

Height: the number of edges in the longest path from the root to a leaf.
full binary tree: all of its nodes have 0 or 2 children.
perfect binary tree:all interior nodes have two children (except for leaf node), and leaves are at the same level. 
If the height is h, then the total node is 2^(h+1)-1
complete binary tree: if and only if the tree is perfect up until the last level and all leaf nodes on the last level are pushed to the left.

2.3 Tree Tranversal
preorder: if (cur != null) {shout(cur); preorder(cur -> left); preorder(cur -> right)}
inorder: if (cur != null) {inorder(cur -> left); shout(cur); inorder(cur -> right)}
postorder: if (cur != null) {postorder(cur -> left), postorder(cur -> right), shout(cur)}
levelorder


