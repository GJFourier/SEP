#pragma once

#include <cstring>
#include <string>
using namespace std;
struct Node{
    char* row;
    Node* next,*pre;
    Node();
    ~Node();
};
class Buffer {
private:
    // TODO: add a List here for storing the input lines
    struct ListBuffer{
        Node* head,*cur;
        int currentLineNum;
        int totalNum;
        Node* move(int i);
        ListBuffer();
        ~ListBuffer();
        void clear();
        void app( char *s);
        void ins(char *s);
    };
    ListBuffer *line;
public:
    Buffer();
    ~Buffer();
    void writeToFile(const string &filename) const;

    char *moveToLine(int idx) const;

    void showLines(int from, int to);

    void deleteLines(int from, int to);
    void insertLine(char *text);
    void appendLine(char *text);
    int getcurrentLine();
    int gettotalNum();
    void printall();
};
