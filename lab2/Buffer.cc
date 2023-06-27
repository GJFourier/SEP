#include <fstream>
#include <iostream>
#include <cstring>
#include "Buffer.h"

//TODO: your code here
//implement the functions in ListBuffer
Node::Node():next(nullptr),pre(nullptr){
    row = new char[30000];
}
Node::~Node(){
    delete []row;
}
Node* Buffer::ListBuffer::move(int i){
    Node* p = head;
    while(i--){
        p = p->next;
    }
    return p;
}
Buffer::ListBuffer::ListBuffer(){
    head = new Node;
    cur = head;
    totalNum = 0;
    currentLineNum = 0;
}
Buffer::ListBuffer::~ListBuffer(){
    clear();
}
void Buffer::ListBuffer::clear(){
    Node* p = head,*q = p;
    while(p != nullptr){
        p = p->next;
        delete q;
        q = p;
    }
}
void Buffer::ListBuffer::app( char *s){
    Node *p;
    p = new Node();
    strcpy(p->row,s);
    p->next = cur->next;
    p->pre = cur;
    if(cur->next != nullptr)
        cur->next->pre = p;
    cur->next = p;
    cur = p;
    ++currentLineNum;
    ++totalNum;
}
void Buffer::ListBuffer::ins(char *s) {
    Node *p;
    p = new Node();
    strcpy(p->row, s);
    if(cur == head){
        head->next = p;
        p->pre = head;
        p->next = nullptr;
        cur = p;
        currentLineNum = 1;
        ++totalNum;
    }
    else {
        p->next = cur;
        p->pre = cur->pre;
        cur->pre->next = p;
        cur->pre = p;
        cur = p;
        ++totalNum;
    }
}


Buffer::Buffer() {line = new ListBuffer;}

Buffer::~Buffer() {delete line;}

void Buffer::writeToFile(const string &filename) const {
    ofstream fout(filename);
    long long byte = 0;
    Node* p = line->head->next;
    while(p != nullptr){
        fout << p->row << endl;
        byte += strlen(p->row)+1;
        p = p->next;
    }
    cout << byte << " byte(s) written" << endl;
}

void Buffer::showLines(int from, int to){
    if(from>to){
        throw "Number range error";
    }
    if(from<1||to>line->totalNum){
        throw "Line number out of range";
    }
    Node *p = line->move(from);
    int i = from;
    while(i++ <= to){
        cout << i-1 << '\t' << p->row << '\n';
        p = p->next;
    }
    line->currentLineNum = to;
    line->cur = line->move(to);
}

void Buffer::deleteLines(int from, int to){
    if(from>to){
        throw "Delete range error";
    }
    if(from<1||to>line->totalNum){
        throw "Line number out of range";
    }
    Node *p = line->move(from),*q = p->pre,*r = p;
    int i = from;
    while(i++ <= to){
        p = p->next;
        --line->totalNum;
        delete r;
        r = p;
    }
    if(p == nullptr){
        q->next = nullptr;
        line->cur = q;
        line->currentLineNum = from-1;
    }
    else{
        q->next = p;
        line->cur = p;
        line->currentLineNum = from;
        p->pre = q;
    }
}

void Buffer::insertLine(char *text){
    line->ins(text);
}

void Buffer::appendLine(char *text){
    line->app(text);
}

char* Buffer::moveToLine(int idx) const {
    if(idx<1||idx>line->totalNum){
        throw "Line number out of range";
    }
    line->cur = line->move(idx);
    line->currentLineNum = idx;
    return line->cur->row;
}
int Buffer::getcurrentLine(){
    return line->currentLineNum;
}
int Buffer::gettotalNum(){
    return line->totalNum;
}
void Buffer::printall(){
    Node *p = line->head->next;
    while(p != nullptr){
        cout << p->row << endl;
        p = p->next;
    }
}
