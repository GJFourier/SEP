#pragma once
#include "stack.h"
#include "queue.h"
class Board {
    // TODO
private:
    Stack<int> disk[3];
    Stack<int> process;
    int stick[3] = {5,20,35};
    int num;
public:
    Board(int num_disk);
    ~Board();
    void draw();
    void move(int from, int to, bool log = true);
    bool win();
    void autoplay();
    void hanoi(int n,int a,int b,int c);
};
