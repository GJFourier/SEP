#include <vector>//
#include <iostream>
#include <fstream>
#include "Class.h"
#include "Student.h"

using namespace std;

class AppX {
private:
    vector<Student *> studentVec;
    vector<Class *> classVec;

    void loadFiles();
    void inputScore();
    void printAvgScore();
    void printGpa();

public:
    AppX();
    ~AppX();
    int run();
};

AppX::~AppX()
{
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it) {
        if (*it) delete (*it);
    }
    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec) {
        if (s) delete (s);
    }
}

AppX::AppX()
{
    loadFiles();
    /*for(vector<Student*>::iterator it = studentVec.begin();
        it != studentVec.end();
        ++it){
        cout << (*it)->id;
    }*/
}

void AppX::loadFiles()
{
    string line;
    size_t pos1, pos2;
    vector<string> bufv;
    Student *st = nullptr;
    string clsname;
    int point;
    Class *cl = nullptr;

    // Open a file as a input stream.
    // use ../Students.txt instead of ./Students.txt because the working directory is under cmake-build-debug
    ifstream stfile("../Students.txt");//???????

    while (getline(stfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each columnes in the line.
        bufv.clear();
        // Split the line into columes.
        //   pos1: begining the the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(';', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }

        // TODO: uncomment next lines after implementing class Undergraduate
        // and Graduate.
        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else
            st = new Graduate(bufv[0], bufv[1], bufv[2]);

        studentVec.push_back(st);
    }
    stfile.close();//??????

    // TODO: load data from ./Classes.txt and push objects to the vector.
    // Hint: how is student information read?
    ifstream clfile("../Classes.txt");
    while (getline(clfile, line)) {
        if (line[0] == '#') // It's a comment line.
            continue;
        // The bufv vector stores each columnes in the line.
        bufv.clear();
        // Split the line into columes.
        //   pos1: begining the the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(':', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }
        if(bufv[0]=="Class")
            clsname=bufv[1];
        if(bufv[0]=="Points") {
            point=bufv[1][0]-'0';
            cl=new Class(clsname,point);
            while(getline(clfile,line)){
                if(line.empty())
                    break;
                for(std::vector<Student*>::iterator it = studentVec.begin();it != studentVec.end();++it){
                    if(line==(*it)->id){
                        (*it)->addClass(cl);
                        cl->addStudent(**it);
                        break;
                    }
                }
            }
            classVec.push_back(cl);
        }
    }
    clfile.close();
}

void AppX::inputScore()
{
    // TODO: implement inputScore.
    // Hint: Take a look at printAvgScore().
    string sbuf,id;
    double score=0;
    string temp;
    Class* cl=nullptr;
    while(true){
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if(sbuf == "q")
            break;
        cl = nullptr;
        for (vector<Class *>::iterator it = classVec.begin();
        it!=classVec.end();++it){
            if((*it)->name==sbuf){
                cl=*it;
                break;
            }
        }
        if(!cl) {
            cout << "No match class!\n";
            continue;
        }
        while(true){
            cout << "Please input the student id (or input q to quit): ";
            cin >> id;
            if(id == "q")
                break;
            try {
                cl->getStudentWrapper(id);
            }
            catch (char const* er){//???????
                cout << er << '\n';
                continue;
            }
            cout << cl->getStudentWrapper(id).toString();
            cout << "please input the score: ";
            cin >> score;
            if(cin.fail())
            {
                cin.clear();
                cin >> temp;
                if((temp[0]!='q')||(temp[0]=='q'&&temp.length()>1))
                    cout << "Wrong score!\n";
                continue;
            }
            // scanf

            try {
                cl->getStudentWrapper(id).setScore(score);
            }
            catch (char const* er){
                cout << er << '\n';
            }
        }
    }
}

void AppX::printAvgScore()
{
    string sbuf;
    Class *cl = nullptr;
    double avg;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class *>::iterator it = classVec.begin();
             it != classVec.end();
             ++it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cout << "No match class!\n";
            continue;
        }

        avg = cl->getAvgScore();
        printf("The average score is: %.2f\n",avg);
    }
}

void AppX::printGpa()
{
    // TODO: implement printGpa.
    // Hint: Take a look at printAvgScore().
    string sbuf;
    Student* st = nullptr;
    while (true) {
        cout << "Please input the student id (or input q to quit): ";
        cin >> sbuf;
        if(sbuf == "q")
            break;
        st=NULL;
        for(vector<Student*>::iterator it = studentVec.begin();
            it != studentVec.end();
            ++it){
            if((*it)->id == sbuf) {
                st = *it;
                break;
            }
        }
        if(!st){
            cout << "No match student!\n";
            continue;
        }
        cout << st->toString();
        printf("GPA = %.2f\n",st->getGrade());
    }
}

int AppX::run()
{
    char cmd;
    while (true) {
        cout << "Command menu:\n"
            << "\ti: Input score\n"
            << "\ta: Compute average score of a class\n"
            << "\tg: Compute grade of a student\n"
            << "\tq: Quit\n"
            << "Please input the command: ";
        cin >> cmd;
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printAvgScore();
        } else if (cmd == 'g') {
            printGpa();
        } else if (cmd == 'q') {
            break;
        } else {
            cout << "Invalid command!" << endl;
        }
    }
    return 0;
}

int main()
{
    AppX app;
    return app.run();
}
