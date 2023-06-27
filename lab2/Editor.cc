#include <iostream>
#include <sstream>
#include <stdio.h>
#include "Editor.h"

using namespace std;

Editor::Editor()
{
    buffer = new Buffer();
    currentLine = 0;
}
Editor::~Editor()
{
    // TODO: Implement destructor
    delete buffer;
}

void Editor::run()
{
    string cmd;
    while (true)
    {
        cout << "cmd> ";
        cout.flush();
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        try {
            dispatchCmd(cmd);
        } catch (const char *e) {
            cout << "? " << e << endl;
        }
    }
}
void Editor::cmdAppend()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    // TODO: finish cmdAppend.
    char text[30000];
    while(true){
        cin.getline(text,30000);
        if(text[0] == '.'&&strlen(text)==1)
            break;
        buffer->appendLine(text);
    }
    currentLine = buffer->getcurrentLine();
}

void Editor::cmdInsert()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    bool firstLine = true;
    while (true)
    {
        char text[30000];
        cin.getline(text,30000);
        if (text[0] == '.'&&strlen(text)==1)
            break;
        if (firstLine) {
            buffer->insertLine(text);
            firstLine = false;
        }  else {
            buffer->appendLine(text);
        }
    }
    currentLine = buffer->getcurrentLine();
}

void Editor::cmdDelete(int start, int end)
{
    buffer->deleteLines(start, end);
    currentLine = buffer->getcurrentLine();
}

void Editor::cmdNull(int line)
{
    cout << buffer->moveToLine(line) << endl;
    currentLine = buffer->getcurrentLine();
}

void Editor::cmdNumber(int start, int end)
{
    buffer->showLines(start, end);
    currentLine = buffer->getcurrentLine();
}

void Editor::cmdWrite(const string &filename)
{
    buffer->writeToFile(filename);
}

void Editor::dispatchCmd(const string &cmd)
{
    if (cmd == "a") {
        cmdAppend();
        return;
    }
    if (cmd == "i") {
        cmdInsert();
        return;
    }
    if (cmd[0] == 'w' && cmd[1] == ' ') {
        // TODO: call cmdWrite with proper arguments
        char file[30000];
        strcpy(file,cmd.c_str());
        if(strlen(file) ==2 ){
            throw "Filename not specified";
        }
        strcpy(file,file+2);
        cmdWrite(file);
        return;
    }
    // TODO: handle special case "1,$n".
    int start, end;
    char comma, type = ' ';
    stringstream ss(cmd);
    ss >> start;
    if (ss.eof()) {
        cmdNull(start);
        return;
    }
    ss >> comma >> end >> type;
    if (ss.good()) {
        char text[30000];
        strcpy(text,cmd.c_str());
        if (comma == ',' && type == 'n') {
            char *p = strchr(text,'n');
            if(strlen(p) == 1) {
                cmdNumber(start, end);
                return;
            }
        }
        else if (comma == ',' && type == 'd') {
            char *p = strchr(text,'d');
            if(strlen(p) == 1) {
                cmdDelete(start, end);
                return;
            }
        }
    }
    else if(start>=0&&start<=9&&comma == ','&&cmd[2]=='$'&&strlen(cmd.c_str()) == 4) {
        if (cmd[3] == 'n') {
            cmdNumber(start, buffer->gettotalNum());
            return;
        } else if (cmd[3] == 'd') {
            cmdDelete(start, buffer->gettotalNum());
            return;
        }
    }
    throw "Bad/Unknown command";
}
