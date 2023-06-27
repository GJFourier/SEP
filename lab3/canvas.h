#pragma once

#include <iostream>

using namespace std;

class Canvas {
public:
    static const int HEIGHT = 11, WIDTH = 41;
    char buffer[HEIGHT][WIDTH];
    
    void draw() const {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++)
                cout << buffer[i][j];
            cout << endl;
        }
    }
    
    void reset() {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i != HEIGHT - 1)
                    buffer[i][j] = ' ';
                else buffer[i][j] = '-';
            }
        }
        buffer[10][5] = buffer[10][20] = buffer[10][35] = '|';
    }
};