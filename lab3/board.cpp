#include "board.h"
#include "canvas.h"

Board::Board(int num_disk) {
    // TODO
    num = num_disk;
    while(num_disk>0){
        disk[0].push(num_disk);
        --num_disk;
    }
}

Board::~Board() {
    // TODO
    num = 0;
}

void Board::draw() {
    Canvas canvas;
    canvas.reset();
    // TODO
    Stack<int> temp;
    int a=1,m,i,j;
    for(i = 0;i < 3;++i){
        m = 1;
        while(!disk[i].empty()){
            a = disk[i].pop();
            temp.push(a);
        }
        while(!temp.empty()){
            a = temp.pop();
            for(j = stick[i]-a;j <= stick[i]+a;++j){
                canvas.buffer[11-2*m][j] = '*';
            }
            canvas.buffer[10-2*m][stick[i]] = '|';
            ++m;
            disk[i].push(a);
        }
        for(int k = 11-2*m;k>=0;--k){
            canvas.buffer[k][stick[i]] = '|';
        }
    }
    canvas.draw();
}

void Board::move(int from, int to, bool log) {
    // TODO
    if(from<1||from>3||to<1||to>3) {
        draw();
        return;
    }
    if(log)
        cout << "Auto moving:";
    if(disk[from-1].empty()||((!disk[to-1].empty())&&disk[from-1].Top()>disk[to-1].Top())) {
        draw();
        return;
    }
    else {
        cout << from << "->" << to << '\n';
        disk[to-1].push(disk[from-1].pop());
        draw();
        if(!log) {
            process.push(from);
            process.push(to);
        }
    }
}

bool Board::win() {
    // TODO
    if(disk[0].empty()&&disk[2].empty())
        return true;
    else return false;
}
void Board::hanoi(int n,int a,int b,int c){
    if(n == 1) {
        move(a, b);
    }
    else {
        hanoi(n-1,a,c,b);
        move(a,b);
        hanoi(n-1,c,b,a);
    }

}
void Board::autoplay() {
    // TODO
    int a,b;
    while(!process.empty()){
        a = process.pop();
        b = process.pop();
        move(a,b);
    }
    hanoi(num,1,2,3);
}
