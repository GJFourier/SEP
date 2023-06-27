#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QString>
#include <QDebug>
#include <QMap>
#include <cmath>

class EvaluationContext;

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

class Expression
{
public:
    Expression();
    virtual ~Expression(){}
    virtual int eval(EvaluationContext & context) = 0;
    virtual QString toString() = 0;
    virtual ExpressionType type() = 0;

    virtual QString getConstantValue(){return 0;}
    virtual QString getIdentifierName(){return "";}
    virtual QString getOperator(){return "";}
    virtual Expression *getLHS(){return nullptr;}
    virtual Expression *getRHS(){return nullptr;}
    virtual int getLevel(){return 0;}
    virtual void setLHS(Expression*){}
    virtual void setRHS(Expression*){}
};

class ConstantExp: public Expression {
public:
   ConstantExp(int val);
   virtual QString getConstantValue();
   virtual int eval(EvaluationContext & context);
   virtual QString toString();
   virtual ExpressionType type();

private:
   int value;

};

class IdentifierExp: public Expression {
public:
   IdentifierExp(QString name);
   virtual int eval(EvaluationContext & context);
   virtual QString toString();
   virtual ExpressionType type();
   virtual QString getIdentifierName();

private:
   QString name;
};

class CompoundExp: public Expression {
public:
   CompoundExp(QString op, int level = 0, Expression *lhs = nullptr, Expression *rhs = nullptr);
   virtual ~CompoundExp();
   virtual int eval(EvaluationContext & context);
   virtual QString toString();
   virtual ExpressionType type();
   virtual QString getOperator();
   virtual Expression *getLHS();
   virtual Expression *getRHS();
   virtual void setLHS(Expression*);
   virtual void setRHS(Expression*);
   virtual int getLevel();

private:
   QString op;
   int level;
   Expression *lhs, *rhs;

};

class EvaluationContext {
public:
   void setValue(QString var, int value);
   int getValue(QString var);
   bool isDefined(QString var);
   void clear();

private:
   QMap<QString,int> context;
};


#endif // EXPRESSION_H
