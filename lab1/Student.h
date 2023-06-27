#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>
class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
private:
    std::string name,year;
    Degree degree;
protected:
    std::vector<Class*> classes;
public:
    const std::string id;
    Student(std::string id,std::string name,std::string year,Degree Degree):id(id),name(name),year(year),degree(Degree){}
    std::string toString() const;
    virtual double getGrade()=0;
    void addClass(Class* c);
};
// TODO: implement class Graduate.
class Graduate:public Student{
public:
    Graduate(std::string id,std::string name,std::string year):Student(id,name,year,graduate){}
    double getGrade();
};
// TODO: implement class Undergraduate.
class Undergraduate:public Student{
    double getGrade();

public:
    Undergraduate(std::string id,std::string name,std::string year):Student(id,name,year,undergraduate){}
};
class StudentWrapper {
private:
    const Student &student;
    double score = 0.0;
public:
    const std::string id;
    // TODO: fix error
    StudentWrapper(const std::string &id, const Student &student): id(id),student(student){}

    void setScore(double score)
    {
        if (score < 0 || score > 100)
            throw "Wrong score!";
        this->score = score;
    }

    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
};

#endif // STUDENT_H_
