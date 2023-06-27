#ifndef PROGRAM_H
#define PROGRAM_H
#include "expression.h"
#include "parser.h"
#include <QList>
#include <QString>
#include <QStringList>
#include <QChar>
#include <QDebug>


class Program
{
public:
    Program();
    void add(QString);
    void clear();
    QString show();
    struct Sentence{
        int num;
        QString cmd;
        QString exp;
        bool flag;
        Sentence (int a,QString b, QString c, bool d = true):num(a), cmd(b), exp(c), flag(d){}
    };
    EvaluationContext context;
    QList<Sentence> body;
    void sort();
    QListIterator<Sentence> jump(int a);

};

QString dealWithString(QString);
bool isNotNum(QString);
#endif // PROGRAM_H
