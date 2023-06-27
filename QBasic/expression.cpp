#include "expression.h"

Expression::Expression()
{

}

//--------------ConstantExp-------------------//

ConstantExp::ConstantExp(int a){
    value = a;
}

QString ConstantExp::getConstantValue(){
    return QString::number(value);
}

int ConstantExp::eval(EvaluationContext &) {
    return value;
}

QString ConstantExp::toString(){
    return QString::number(value,10);
}

ExpressionType ConstantExp::type(){
    return CONSTANT;
}

//--------------IdentifierExp-------------------//

IdentifierExp::IdentifierExp(QString a){
    name = a;
}

QString IdentifierExp::getIdentifierName(){
    return name;
}

int IdentifierExp::eval(EvaluationContext & context) {
    QString n = name;
    bool flag = false;
    if(n[0] == '-'){
        n.remove('-');
        flag = true;
    }
    if(!context.isDefined(n)){
        throw(QString("“") + n + QString("” is undefined."));
        return 0;
    }
    else
        if(!flag){
            return context.getValue(n);
        }
        else
            return -context.getValue(n);
}

QString IdentifierExp::toString(){
    return name;
}

ExpressionType IdentifierExp::type(){
    return IDENTIFIER;
}

//--------------CompoundExp-------------------//

CompoundExp::CompoundExp(QString a, int b, Expression* d, Expression* e){
    op = a;
    level = b;
    lhs = d;
    rhs = e;
    if(a == "+" || (a == "-" && b != 4))
        level = 1;
    if(a == "*" || a == "/")
        level = 2;
    if(a == "**")
        level = 3;
}

CompoundExp::~CompoundExp(){}

QString CompoundExp::getOperator(){
    return op;
}

int CompoundExp::getLevel(){
    return level;
}

Expression* CompoundExp::getLHS(){
    return lhs;
}

Expression* CompoundExp::getRHS(){
    return rhs;
}

void CompoundExp::setLHS(Expression* a){
    lhs = a;
}

void CompoundExp::setRHS(Expression *a){
    rhs = a;
}

int CompoundExp::eval(EvaluationContext & context) {
    int right = rhs->eval(context);
    if (op == "=") {
        context.setValue(lhs->getIdentifierName(), right);
        return right;
    }
    int left = lhs->eval(context);
    if (op == "+")
        return left + right;
    if (op == "-")
        return left - right;
    if (op == "*")
        return left * right;
    if (op == "/") {
        if (right == 0){
            throw(QString("Division by 0"));
            return 0;
        }
        else return left / right;
    }
    if (op == "**"){
        return pow(left, right);
    }
    throw(QString("Illegal operator in expression"));
    return 0;
}

QString CompoundExp::toString(){
    return lhs->toString() + op + rhs->toString();
}

ExpressionType CompoundExp::type(){
    return COMPOUND;
}

//--------------EvaluationContext-------------------//

void EvaluationContext::setValue(QString var, int value){
    context.insert(var, value);
}

int EvaluationContext::getValue(QString var){
    return context[var];
}

bool EvaluationContext::isDefined(QString var){
    return context.contains(var);
}

void EvaluationContext::clear(){
    context.clear();
}




