#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include <queue>
#include <algorithm>

#include "Tree.h"

using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/
ostream &operator<<(ostream &out, const TreeNode &b) {
    // TODO: Your code here
    out << b.data[0] << ' ' << b.data[1];
    return out;
}

bool sortX(TreeNode p,TreeNode q)
{
    return p.data[0] < q.data[0];
}

bool sortY(TreeNode p,TreeNode q)
{
    return p.data[1] < q.data[1];
}

int TreeNode::getX() {
    // TODO: Your code here
    return data[0];
}

int TreeNode::getY() {
    // TODO: Your code here
    return data[1];
}

TreeNode::~TreeNode() {
    // TODO: Your code here
}

TreeNode::TreeNode(int x,int y,int lev){
    data[0] = x;
    data[1] = y;
    key = lev;
    left = nullptr;
    right = nullptr;
}

istream &operator>>(istream &in, BinaryDimonTree &tree) {
    // TODO: Your code here
    int node_num,x,y;
    TreeNode A[5100];
    in >> node_num;
    for(int i = 0;i < node_num;++i){
        in >> x >> y;
        TreeNode p(x,y);
        A[i] = p;
    }
    tree.createTree(A,node_num);
    return in;
}

BinaryDimonTree::BinaryDimonTree() {
    // TODO: Your code here
    root = nullptr;
}

TreeNode *BinaryDimonTree::find_nearest_node(int x, int y) {
    // TODO: Your code here
    long long int distance = LONG_LONG_MAX;
    TreeNode *p = nullptr;
    recur_search(root,x,y,distance,p);
    return p;
}

void BinaryDimonTree::recur_search(TreeNode *cur, int x, int y, long long int &min_distance, TreeNode *&guess) {
    // TODO: Your code here
    if(cur == nullptr)
        return;
    if(dis(cur->data[0],cur->data[1],x,y) < min_distance){
        min_distance = dis(cur->data[0],cur->data[1],x,y);
        guess = cur;
    }
    else if(dis(cur->data[0],cur->data[1],x,y) == min_distance){
        if(cur->data[0] < guess->data[0])
            guess = cur;
        else if(cur->data[0] == guess->data[0]){
            if(cur->data[1] < guess->data[1])
                guess = cur;
        }
    }
    if(cur->key == 1){
        if(x < cur->data[0]) {
            recur_search(cur->left, x, y, min_distance, guess);
            if (dis(x, 0, cur->data[0], 0) < min_distance)
                recur_search(cur->right, x, y, min_distance, guess);
        }
        else {
            recur_search(cur->right, x, y, min_distance, guess);
            if (dis(x, 0, cur->data[0], 0) < min_distance)
                recur_search(cur->left, x, y, min_distance, guess);
        }
    }
    else if(cur->key == 0){
        if(y < cur->getY()) {
            recur_search(cur->left, x, y, min_distance, guess);
            if (dis(0, y, 0, cur->data[1]) < min_distance)
                recur_search(cur->right, x, y, min_distance, guess);
        }
        else {
            recur_search(cur->right, x, y, min_distance, guess);
            if (dis(0, y, 0, cur->data[1]) < min_distance)
                recur_search(cur->left, x, y, min_distance, guess);
        }
    }
}

BinaryDimonTree::~BinaryDimonTree()
{
    // TODO: Your code here
    queue<TreeNode*> Queue;
    Queue.push(root);
    while(!Queue.empty()){
        TreeNode *p = Queue.front();
        Queue.pop();
        if(p->left != nullptr)
            Queue.push(p->left);
        if(p->right != nullptr)
            Queue.push(p->right);
        delete p;
    }
}

long long BinaryDimonTree::dis(int a,int b,int c,int d){
    return (a-c)*(a-c)+(b-d)*(b-d);
}

void BinaryDimonTree::createTree(TreeNode A[], int length) {
    createTree(A,length,root,1);
}

void BinaryDimonTree::createTree(TreeNode A[], int length, TreeNode *&p, int key) {
    if(length == 0)
        return;
    if(key == 1) {
        sort(A, A + length, sortX);
        p = new TreeNode(A[length/2].data[0],A[length/2].data[1],1);
        createTree(A,length/2,p->left,0);
        createTree(A+length/2+1,length-1-length/2,p->right,0);
    }
    else if(key == 0){
        sort(A, A + length, sortY);
        p = new TreeNode(A[length/2].data[0],A[length/2].data[1],0);
        createTree(A,length/2,p->left,1);
        createTree(A+length/2+1,length-1-length/2,p->right,1);
    }
}
