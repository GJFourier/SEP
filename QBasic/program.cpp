#include "program.h"

Program::Program()
{

}

void Program::add(QString s){
    bool flag = false;
    QStringList sl = s.split(' ');   // 分隔行号、指令与表达式，处理空格，使运算符前后各一个空格
    int n = sl[0].toInt();
    sl.removeAt(0);
    QString s2 = "";
    QString s3 = "";
    if(sl.length() > 0){
        s2 = sl[0];
        sl.removeAt(0);
    }
    if(sl.length() > 0){
        s3 = sl.join(' ');
        if(!(s2 == "REM")){
            s3 = dealWithString(s3);
        }
    }
    int j = 0;
    for(QListIterator<Sentence> i(body);i.hasNext();){
        Sentence s = i.next();
        if(s.num < n){
            ++j;
            continue;
        }
        if(s.num == n)
            if(s2 == "")
                body.removeAt(j); // 删除句子
            else
                body.replace(j, Sentence(n, s2, s3)); // 替换句子
        else
            if(s2 == "")
                return; // 删除了不存在的行号
            else
                body.insert(j, Sentence(n, s2, s3)); // 插入句子
        ++j;
        flag = true;
        break;
    }
    if(!flag){
        if(s2 == "")
            return; // 删除了不存在的行号
        else
            body.append(Sentence(n, s2, s3));
    }
}

QString Program::show(){
    QString str;
    for(QListIterator<Sentence> i(body);i.hasNext();){
        Sentence s = i.next();
        QString b = s.cmd;
        QString c = s.exp;
        QString d = QString::number(s.num);
        str += (d + " " + b + " " + c + "\n");
    }
    return str;
}

void Program::clear(){
    body.clear();
    context.clear();
}

bool isNotNum(QString s){
    if(s == "+" || s == "-" || s == "*" || s == "/" || s == "**" || s == "(" || s == "=")
        return true;
    else
        return false;
}

QString dealWithString(QString s3){
    QStringList sl;
    s3.remove(QRegExp("\\s"));  // 写完才发现可以直接replace，就不改了，当作防伪标志用
    sl = s3.split('+');
    s3 = sl.join(" + ");
    sl = s3.split('-');
    s3 = sl.join(" - ");
    sl = s3.split('*');
    s3 = sl.join(" * ");
    sl = s3.split('/');
    s3 = sl.join(" / ");
    sl = s3.split('=');
    s3 = sl.join(" = ");
    sl = s3.split('<');
    s3 = sl.join(" < ");
    sl = s3.split('>');
    s3 = sl.join(" > ");
    sl = s3.split("THEN");
    s3 = sl.join(" THEN ");
    sl = s3.split('(');
    s3 = sl.join(" ( ");
    sl = s3.split(')');
    s3 = sl.join(" ) ");
    sl = s3.split(" *  * ");  // 前面处理*时会让中间多出两个空格
    s3 = sl.join(" ** ");
    s3 = s3.simplified();
    sl = s3.split(" ");
    int n = 0;
    for(QListIterator<QString> i(sl);i.hasNext();){
        QListIterator<QString> j = i;
        QString s = i.next();
        if(s == "-"){
            if(!j.hasPrevious()){
                if(j.hasNext()){
                    QString s0 = i.next();
                    sl.removeAt(n);
                    sl.replace(n, s+s0);
                }
            }
            else{
                QString t = j.previous();
                if(isNotNum(t)){
                    j = i;
                    if(j.hasNext()){
                        QString s0 = i.next();
                        sl.removeAt(n);
                        sl.replace(n, s+s0);
                    }
                }
            }
        }
        ++n;
    }
    s3 = sl.join(" ");
    return s3;
}

QListIterator<Program::Sentence> Program::jump(int a){
    for(QListIterator<Sentence> i(body);i.hasNext();){
        Sentence s = i.next();
        if(s.num == a){
            i.previous();
            return i;
        }
        if(s.num > a){
            throw(QString("不存在的行号"));
        }
    }
    throw(QString("不存在的行号"));
}
