#include <iostream>
#include <sstream>
#include <string>
#include <climits>

#include "board.h"

using namespace std;
int a,b;
int main() {
    while (true) {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        }
        // TODO
        if(input[0]>='1'&&input[0]<='5'&&input.length()==1){
            Board board(input[0]-'0');
            board.draw();
            while(true){
                cout << "Move a disk. Format: x y" << '\n';
                getline(cin,input);
                stringstream ss(input);
                ss >> a >> b;
                if(ss.eof()) {
                    if (a == 0 && b == 0)
                        board.autoplay();
                    else
                        board.move(a, b, false);
                    if (board.win()) {
                        cout << "Congratulations! You win!" << '\n';
                        break;
                    }
                }
                else {
                    board.draw();
                    continue;
                }
            }
        }
    }
    return 0;
}