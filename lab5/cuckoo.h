#ifndef LAB5_CUCKOO_H
#define LAB5_CUCKOO_H


class Cuckoo {
private:
    struct Data{
        int key;
        int value;
        bool state;
        Data(int a = 0,int b = 0):key(a),value(b),state(false){}
    };
    Data *H1,*H2;
    int size;
    bool Kick(Data *p,Data *q,const int &s,const int &key,const int &value,const int &index,const int &n);
    void rehash();
public:
    Cuckoo();
    ~Cuckoo();
    void Run();
    void Insert(const int &key,const int &value);
    void Lookup(const int &key);
    void Delete(const int &key);
};


#endif //LAB5_CUCKOO_H
