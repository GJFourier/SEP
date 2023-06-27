#ifndef PARSER_H
#define PARSER_H
#include "expression.h"
#include <QStack>
#include <QString>
#include <QStringList>

class Parser
{
public:
    Parser();
    Expression* buildTree(QString);
    bool out(QStack<Expression*> &, QStack<Expression*> &);
};

#endif // PARSER_H
