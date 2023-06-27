#include "cuckoo.h"
#include <iostream>
#include <sstream>

using namespace std;

Cuckoo::Cuckoo(){
    size = 8;
    H1 = new Data[size];
    H2 = new Data[size];
}

Cuckoo::~Cuckoo(){
    delete []H1;
    delete []H2;
}

bool Cuckoo::Kick(Data *p,Data *q,const int &s,const int &key,const int &value,const int &index,const int &n){
    if(n == 2 * s + 1){
        cout << "Loop Detect" << endl;
        rehash();
        Insert(key,value);
        return false;
    }
    if(n % 2 == 1) {
        int a = p[index].key;
        int b = p[index].value;
        int c = (a / size) % size;
        p[index].key = key;
        p[index].value = value;
        cout << "Kick " << a << " with " << key << " in table 0 " <<  index << endl;
        if (!q[c].state) {
            q[c].key = a;
            q[c].value = b;
            q[c].state = true;
            return true;
        }
        else
            return Kick(p,q,s,a, b, c, n + 1);
    }
    if(n % 2 == 0){
        int a = q[index].key;
        int b = q[index].value;
        int c = a % size;
        q[index].key = key;
        q[index].value = value;
        cout << "Kick " << a << " with " << key << " in table 1 " <<  index << endl;
        if (!p[c].state) {
            p[c].key = a;
            p[c].value = b;
            p[c].state = true;
            return true;
        }
        else
            return Kick(p,q,s,a, b, c, n + 1);
    }
}

void Cuckoo::rehash() {
    size *= 2;
    Data *p = new Data[size];
    Data *q = new Data[size];
    int a,b;
    bool flag = true;
    for(int i = 0;i < size / 2;++i){
        if(!H1[i].state)
            continue;
        a = H1[i].key % size;
        b = (H1[i].key / size) % size;
        if(!p[a].state){
            p[a].key = H1[i].key;
            p[a].value = H1[i].value;
            p[a].state = true;
        }
        else if(!q[b].state){
            q[b].key = H1[i].key;
            q[b].value = H1[i].value;
            q[b].state = true;
        }
        else
             flag = Kick(p,q,size,H1[i].key,H1[i].value,a,1);
        if(!flag)
            return;
    }
    for(int i = 0;i < size / 2;++i){
        if(!H2[i].state)
            continue;
        a = H2[i].key % size;
        b = (H2[i].key / size) % size;
        if(!p[a].state){
            p[a].key = H2[i].key;
            p[a].value = H2[i].value;
            p[a].state = true;
        }
        else if(!q[b].state){
            q[b].key = H2[i].key;
            q[b].value = H2[i].value;
            q[b].state = true;
        }
        else
            flag = Kick(p,q,size,H2[i].key,H2[i].value,a,1);
        if(!flag) {
            cout << "flag2" << endl;
            return;
        }
    }
    delete []H1;
    delete []H2;
    H1 = p;
    H2 = q;
}

void Cuckoo::Insert(const int &key,const int &value) {
    int a = key % size;
    int b = (key / size) % size;
    if(!H1[a].state){
        H1[a].key = key;
        H1[a].value = value;
        H1[a].state = true;
    }
    else if(H1[a].key == key){
        H1[a].value = value;
    }
    else if(!H2[b].state){
        H2[b].key = key;
        H2[b].value = value;
        H2[b].state = true;
    }
    else if(H2[b].key == key){
        H2[b].value = value;
    }
    else Kick(H1,H2,size,key,value,a,1);
}

void Cuckoo::Lookup(const int &key) {
    int a = key % size;
    int b = (key / size) % size;
    if(H1[a].state && H1[a].key == key)
        cout << H1[a].value << endl;
    else if(H2[b].state && H2[b].key == key)
        cout << H2[b].value << endl;
    else
        cout << "Key Not Found" << endl;
}

void Cuckoo::Delete(const int &key) {
    int a = key % size;
    int b = (key / size) % size;
    if(H1[a].state && H1[a].key == key)
        H1[a].state = false;
    else if(H2[b].state && H2[b].key == key)
        H2[b].state = false;
    else
        cout << "Key Not Found" << endl;
}

void Cuckoo::Run() {
    int M,key,value;
    string input,cmd;
    cin >> M;
    cin.get();
    for(int i = 1;i <= M;++i){
        getline(cin,input);
        stringstream ss(input);
        ss >> cmd;
        if(cmd == "Insert"){
            ss >> key;
            ss >> value;
            Insert(key,value);
        }
        if(cmd == "Lookup"){
            ss >> key;
            Lookup(key);
        }
        if(cmd == "Delete"){
            ss >> key;
            Delete(key);
        }
    }
}