#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const
{
    // TODO: uncomment the following code after implementing class Student.
    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();
    return "";
}
// TODO: implement functions which are declared in Student.h.
void Student::addClass(Class* c){
    classes.push_back(c);
}
double Graduate::getGrade(){
    long double sum = 0,g = 0;
    int i = 0,P = 0;
    std::vector<long double> GPA;
    std::vector<int> point;
    for(std::vector<Class*>::iterator it = classes.begin();
        it != classes.end();
        ++it){
        g=(*it)->getStudentWrapper(id).getScore();
        if(g<60)
            GPA.push_back(2.0);
        else if(g<70)
            GPA.push_back(2.5);
        else if(g<80)
            GPA.push_back(3.0);
        else if(g<90)
            GPA.push_back(3.5);
        else if(g<=100)
            GPA.push_back(4.0);
        point.push_back((*it)->point);
        P += ((*it)->point);
        ++i;
    }
    for(int j=0;j<i;++j){
        sum+=GPA[j]*point[j]/P;
    }
    return sum;
}
double Undergraduate::getGrade(){
    long double sum = 0;
    int i = 0,P=0;
    std::vector<long double> GPA;
    std::vector<int> point;
    for(std::vector<Class*>::iterator it = classes.begin();
        it != classes.end();
        ++it){
        GPA.push_back((*it)->getStudentWrapper(id).getScore()/20);
        point.push_back((*it)->point);
        P+=point[i];
        ++i;
    }
    for(int j=0;j<i;++j){
        sum+=GPA[j]*point[j]/P;
    }
    return sum;
}

