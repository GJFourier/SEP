#pragma once

#include <cassert>

template <typename T> class Queue {
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
    Node *head,*tail;
    int length;

 public:
    Queue() {
        // TODO
        head = new Node;
        tail = head;
        length = 0;
    }
    
    ~Queue() {
        // TODO
        Node *p = head,*q = p;
        while(p != nullptr){
            q = p->next;
            delete p;
            p = q;
        }
        length = 0;
    }
    
    int size() const {
        // TODO
        return length;
    }
    
    bool empty() const {
        return size() == 0;
    }
    
    void push(T t) {
        // TODO
        Node *p = new Node(t);
        tail->next = p;
        tail = tail->next;
        ++length;

    }
    
    T pop() {
        // TODO
        head = head->next;
        --length;
        return head->data;
    }
    
    T& front() const {
        // TODO
        return head->next->data;
    }
    
    T& back() const {
        // TODO
        return tail->data;
    }
};
