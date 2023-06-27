#include "parser.h"

Parser::Parser()
{

}

Expression* Parser::buildTree(QString s){ // 要求s输入时为表达式，不含行号指令
    QStringList sl = s.split(' ');
    QStack<Expression *> stackexp;
    Expression *p, *q;
    QStack<Expression*> stackval;
    for(QListIterator<QString> i(sl);i.hasNext();){
        QString read = i.next();
        // 运算符
        if(read == "+" || read == "-" || read == "*" || read == "/" || read == "**" || read == "(" || read == ")" || read == "="){
            if(stackexp.isEmpty()){
                p = new CompoundExp(read);
                stackexp.push(p);
            }
            else{
                q = stackexp.top();
                if(read == "("){
                    p = new CompoundExp(read);
                    stackexp.push(p);
                }
                if(read == "**"){
                    p = new CompoundExp(read);
                    stackexp.push(p);
                }
                if(read == "+" || read == "-"){
                    while(!(q->getOperator() == "(" ||q->getOperator() == "=")){
                        if(!out(stackexp, stackval))
                            return  nullptr;
                        if(!stackexp.isEmpty())
                            q = stackexp.top();
                        else
                            break;
                    }
                    p = new CompoundExp(read);
                    stackexp.push(p);
                }
                if(read == "*" || read == "/"){
                    while(q->getOperator() == "*" || q->getOperator() == "/" || q->getOperator() == "**"){
                        if(!out(stackexp, stackval))
                            return nullptr;
                        if(!stackexp.isEmpty())
                            q = stackexp.top();
                        else
                            break;
                    }
                    p = new CompoundExp(read);
                    stackexp.push(p);
                }
                if(read == ")"){
                    while(!(q->getOperator() == "(")){
                        if(!out(stackexp, stackval))
                            return nullptr;
                        if(!stackexp.isEmpty())
                            q = stackexp.top();
                        else{
                            throw(QString("缺少左括号"));
                            return nullptr;
                        }
                    }
                    if(q->getOperator() == "="){
                        throw(QString("缺少左括号"));
                        return nullptr;
                    }
                    q = stackexp.pop();
                }
                if(read == "="){    // =只能作为第一个运算符
                    if(stackexp.isEmpty() && stackval.size() == 1 && stackval.top()->type() == 2){
                        stackexp.push(p);
                    }
                    else{
                        throw(QString("只有变量能被“=”赋值"));
                    }
                }

            }
        }
        // 变量
        else if(read.toInt() == 0 && !(read == "0" || read == "-0")){
            p = new IdentifierExp(read);
            stackval.push(p);
        }

        // 数字
        else{
           p = new ConstantExp(read.toInt());
           stackval.push(p);
        }
    }
    while(!stackexp.isEmpty()){
        p = stackexp.top();
        if(p->getOperator() == "="){
            p = stackexp.pop();
            if(stackval.isEmpty()){
                throw(QString("缺少操作数"));
                return nullptr;
            }
            else {
                q = stackval.pop();
                p->setRHS(q);
                if(stackval.isEmpty()){
                    throw(QString("缺少操作数"));
                    return nullptr;
                }
                else{
                    q = stackval.pop();
                    p->setLHS(q);
                    stackval.push(p);
                }
            }
        }
        else{
            if(!out(stackexp, stackval))
                return nullptr;
        }
    }
    p = stackval.pop();
    return p;
}

bool Parser::out(QStack<Expression *> &stackexp, QStack<Expression*> &stackval){ // 出栈与两个操作数进行结合运算
    Expression *val1, *val2;
    Expression *q = stackexp.pop();
    if(stackval.isEmpty()){
        throw(QString("缺少操作数"));
        return false;
    }
    else {
        val2 = stackval.pop();
        if(stackval.isEmpty()){
            throw(QString("缺少操作数"));
            return false;
        }
        else{
            val1 = stackval.pop();
        }
    }
    q->setLHS(val1);
    q->setRHS(val2);
    stackval.push(q);
    return true;
}
