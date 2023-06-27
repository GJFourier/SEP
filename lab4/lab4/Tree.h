//
// Created by Smile on 2019/4/8.
//

#ifndef C_BINARYDIMEN_TREE_H
#define C_BINARYDIMEN_TREE_H

#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/

class TreeNode
{
  friend ostream &operator<<(ostream &out, const TreeNode &b);
  friend class BinaryTree;
  friend class BinaryDimonTree;
  friend bool sortX(TreeNode,TreeNode);
  friend bool sortY(TreeNode,TreeNode);

private:
  /* data */
  int data[2];
  int key;
  TreeNode* left;
  TreeNode* right;

public:
  /* methods */
  TreeNode(int x = 0,int y = 0,int lev = 0);
  int getX();  /* DO NOT CHANGE */
  int getY();  /* DO NOT CHANGE */
  ~TreeNode(); /* DO NOT CHANGE */

};


class BinaryDimonTree
{
friend istream &operator>>(istream &in, BinaryDimonTree &tree); /* DO NOT CHANGE */

private:
  /* data */
  TreeNode *root;

public:
  /* methods */
  BinaryDimonTree();          /* DO NOT CHANGE */
  TreeNode *find_nearest_node(int x, int y);  /* DO NOT CHANGE */

  void recur_search(TreeNode *cur, int x, int y, long long int &min_distance, TreeNode *&guess);
  void createTree(TreeNode A[],int length);
  void createTree(TreeNode A[],int length,TreeNode *&p,int key);
  long long dis(int a,int b,int c,int d);
  ~BinaryDimonTree();
};

#endif //C_BINARYDIMEN_TREE_H
