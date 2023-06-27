#pragma once

#include <cassert>

template <typename T> class Stack {
    // TODO
 private:
    struct Node{
        T data;
        Node* next;
        Node(){
            next = nullptr;
        }
        Node(T a ,Node* b = nullptr){
            data = a;
            next = b;
        }
    };
    Node *top;
    int length;

 public:
    Stack() {
        // TODO
        top = nullptr;
        length = 0;
    }
    
    ~Stack() {
        // TODO
        Node *p = top,*q = p;
        while(p != nullptr){
            q = p->next;
            delete p;
            p = q;
        }
        length = 0;
    }
    
    int size()const {
        // TODO
        return length;
    }
    
    bool empty()const {
        return size() == 0;
    }
    
    void push(T t) {
        // TODO
        Node *p = new Node(t,top);
        top = p;
        ++length;
    }
    
    T pop() {
        // TODO
        Node* p = top;
        T a = top->data;
        top = top->next;
        --length;
        delete p;
        return a;
    }
    
    T& Top() const {
        // TODO
        return top->data;
    }
};
