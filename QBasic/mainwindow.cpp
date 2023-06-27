#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQueue>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->CodeDisplay->setFont(QFont("SimSon", 13));
    ui->textBrowser->setFont(QFont("SimSon", 13));
    ui->treeDisplay->setFont(QFont("SimSon", 13));
    connect(ui->btnLoadCode, &QPushButton::clicked, this, &MainWindow::open);
    connect(ui->btnRunCode, &QPushButton::clicked, this, &MainWindow::run);
    connect(ui->btnClearCode, &QPushButton::clicked, this, &MainWindow::clear);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->setText("");

    // 处理输入语句
    dealWith(cmd);
}

void MainWindow::dealWith(QString s){
    bool flag = true;
    s = s.simplified();
    if(s == nullptr)
        return;
    QStringList sl = s.split(' ');

    // 判断第一个输入是否为行号
    int a = sl[0].toInt();
    if(a < 0){
        // 行号错误
        return;
    }
    if(a == 0){
        QChar c = sl[0][0];
        if(c == '0'){
            //行号错误
            return;
        }
        else
            flag = false;
    }
    if(a > 1000000){
        // 行号错误
        return;
    }
    // 处理带行号的命令
    if(flag){
        program.add(s);
        ui->CodeDisplay->setText(program.show());
    }

    // 处理直接命令
    else{
        QString s0 = sl[0];
        sl.removeAt(0);
        s = sl.join(" ");
        s = dealWithString(s);
        if(s0 == "LET"){
            try {
                Expression* exp = parser.buildTree(s);
                exp->eval(context);
                return;
            } catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
            }

        }
        if(s0 == "PRINT"){
            try {
                Expression* exp = parser.buildTree(s);
                int a = exp->eval(context);
                QString s1 = QString::number(a);
                ui->textBrowser->append(s1);
                return;
            } catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
            }

        }
        if(s0 == "INPUT"){
            try {
                Expression* exp = parser.buildTree(s);
                if(exp->type() != IDENTIFIER)
                    throw(QString("只有变量能被赋值"));
                ui->textBrowser->append(" ? ");
                QString s1 = "请输入变量" + s + "的值";
                int iAge = QInputDialog::getInt(this, "输入", s1, 0);
                s1 = s + " = " + QString::number(iAge);
                exp = parser.buildTree(s1);
                exp->eval(context);
                ui->textBrowser->insertPlainText(QString::number(iAge));
                return;
            } catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
            }

        }
        if(s0 == "RUN"){
            run();
            return;
        }
        if(s0 == "LOAD"){
            load();
            return;
        }
        if(s0 == "CLEAR"){
            clear();
            return;
        }
        if(s0 == "LIST"){
            list(s);
            return;
        }
        if(s0 == "HELP"){
            help(s);
            return;
        }
        if(s0 == "QUIT"){
            quit();
            return;
        }
        ui->textBrowser->append("错误：无效命令");
    }
}

void MainWindow::run(){
    // 清空变量储存
    program.context.clear();

    this->ui->textBrowser->clear();
    this->ui->treeDisplay->clear();
    // 生成语法树
    buildGrammarTree();
    // 执行主程序
    for(QListIterator<Program::Sentence> i(program.body);i.hasNext();){
        Program::Sentence s = i.next();
        if(s.cmd == "REM"){

        }
        else if(s.cmd == "LET"){
            try {
                Expression* exp = parser.buildTree(s.exp);
                exp->eval(program.context);
            }
            catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
            }

        }
        else if(s.cmd == "PRINT"){
            try{
                Expression* exp = parser.buildTree(s.exp);
                int a = exp->eval(program.context);
                QString s1 = QString::number(a);
                ui->textBrowser->append(s1);
                continue;
            }
            catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
            }
        }
        else if(s.cmd == "INPUT"){
            try {
                Expression* exp = parser.buildTree(s.exp);
                if(exp->type() != IDENTIFIER)
                    throw(QString("只有变量能被赋值"));
                ui->textBrowser->append(" ? ");
                QString s1 = "请输入变量" + s.exp + "的值";
                int iAge = QInputDialog::getInt(this, "输入", s1, 0);
                s1 = s.exp + " = " + QString::number(iAge);
                exp = parser.buildTree(s1);
                exp->eval(program.context);
                ui->textBrowser->insertPlainText(QString::number(iAge));
                continue;
            } catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
            }
        }
        else if(s.cmd == "GOTO"){
            try {
                int a = s.exp.toInt();
                if(a == 0 && s.exp != "0")
                    throw(QString("只能跳转到数字"));
                i = program.jump(a);
                continue;
            }
            catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
            }
        }
        else if(s.cmd == "IF"){
            try {
                Expression* exp;
                QStringList sl = s.exp.split("THEN");
                if(sl.size() != 2)
                    throw(QString("指令错误"));
                if(sl[1].toInt() == 0 && sl[1].simplified() !="0")
                    throw(QString("只能跳转到常量"));
                QStringList sl1 = sl[0].split("<");
                if(sl1.size() > 2)
                    throw(QString("指令错误"));
                else if(sl1.size() == 2){
                    sl1[0] = sl1[0].simplified();
                    sl1[1] = sl1[1].simplified();
                    exp = parser.buildTree(sl1[0]);
                    Expression* exp2 = parser.buildTree(sl1[1]);
                    int a = exp->eval(program.context);
                    int b = exp2->eval(program.context);
                    if(a < b)
                        i = program.jump(sl[1].toInt());
                }
                else if(sl1.size() == 1){
                    sl1 = sl[0].split(">");
                    if(sl1.size() > 2)
                        throw(QString("指令错误"));
                    else if(sl1.size() == 2){
                        sl1[0] = sl1[0].simplified();
                        sl1[1] = sl1[1].simplified();
                        exp = parser.buildTree(sl1[0]);
                        Expression* exp2 = parser.buildTree(sl1[1]);
                        int a = exp->eval(program.context);
                        int b = exp2->eval(program.context);
                        if(a > b)
                            i = program.jump(sl[1].toInt());
                    }
                    else if(sl1.size() == 1){
                        sl1 = sl[0].split("=");
                        if(sl1.size() > 2)
                            throw(QString("指令错误"));
                        else if(sl1.size() == 2){
                            sl1[0] = sl1[0].simplified();
                            sl1[1] = sl1[1].simplified();
                            exp = parser.buildTree(sl1[0]);
                            Expression* exp2 = parser.buildTree(sl1[1]);
                            int a = exp->eval(program.context);
                            int b = exp2->eval(program.context);
                            if(a == b)
                                i = program.jump(sl[1].toInt());
                        }
                    }
                }
                continue;
            }
            catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
                return;
            }
        }
        else if(s.cmd == "END"){
            try {
                if(s.exp != "")
                    throw(QString("指令错误"));
                else
                    return;
            }
            catch (QString err) {
                ui->textBrowser->append("错误：" + err);
                return;
            }
        }
    }
}

void MainWindow::load(){
    open();
}

void MainWindow::clear(){
    this->ui->CodeDisplay->clear();
    this->ui->textBrowser->clear();
    this->ui->treeDisplay->clear();
    program.clear();
}

void MainWindow::list(QString){

}

void MainWindow::help(QString){
    QMessageBox::information(this, "帮助", "在下方输入代码，点击Run运行.\n点击clear清除.\n可通过load载入已有代码.");
}

void MainWindow::quit(){
    exit(0);
}

void MainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open");
    clear();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        return;
    }
    QTextStream stream(&file);
    while(!stream.atEnd()){
        QString text = stream.readLine();
        dealWith(text);
    }
}

void MainWindow::buildGrammarTree(){
    Expression* exp;
    ui->treeDisplay->clear();
    for(QListIterator<Program::Sentence> i(program.body);i.hasNext();){
        Program::Sentence s = i.next();
        if(s.cmd == "REM"){
            ui->treeDisplay->append(QString::number(s.num) + " REM");
            ui->treeDisplay->append("\t" + s.exp);
            continue;
        }
        if(s.cmd == "LET"){
            try {
                exp = parser.buildTree(s.exp);
                exp->eval(program.context);
                ui->treeDisplay->append(QString::number(s.num) + " LET =");
                display(exp->getLHS());
                display(exp->getRHS());
            }
            catch (QString err) {
                ui->treeDisplay->append(QString::number(s.num) + " ERROR");
            }
            continue;
        }
        if(s.cmd == "INPUT"){
            try {
                exp = parser.buildTree(s.exp);
                if(!(exp->type() == IDENTIFIER))
                    throw(QString("只有变量能被赋值"));
//                exp->eval(program.context);
                ui->treeDisplay->append(QString::number(s.num) + " INPUT");
                display(exp);
            }
            catch (QString err) {
                ui->treeDisplay->append(QString::number(s.num) + " ERROR");
            }
            continue;
        }
        if(s.cmd == "PRINT"){
            try {
                exp = parser.buildTree(s.exp);
                ui->treeDisplay->append(QString::number(s.num) + " PRINT");
                display(exp);
            }
            catch (QString err) {
                ui->treeDisplay->append(QString::number(s.num) + " ERROR");
            }
            continue;
        }
        if(s.cmd == "GOTO"){
            try {
                exp = parser.buildTree(s.exp);
                if(!(exp->type() == CONSTANT))
                    throw(QString("只能跳转到常量"));
                ui->treeDisplay->append(QString::number(s.num) + " GOTO");
                display(exp);
            }
            catch (QString err) {
                ui->treeDisplay->append(QString::number(s.num) + " ERROR");
            }
            continue;
        }
        if(s.cmd == "IF"){
            try {
                QStringList sl = s.exp.split("THEN");
                if(sl.size() != 2)
                    throw(QString("指令错误"));
                if(sl[1].toInt() == 0 && sl[1].simplified() !="0")
                    throw(QString("只能跳转到常量"));
                QStringList sl1 = sl[0].split("<");
                if(sl1.size() > 2)
                    throw(QString("指令错误"));
                else if(sl1.size() == 2){
                    sl1[0] = sl1[0].simplified();
                    sl1[1] = sl1[1].simplified();
                    exp = parser.buildTree(sl1[0]);
                    Expression* exp2 = parser.buildTree(sl1[1]);
//                    exp->eval(program.context);
//                    exp2->eval(program.context);
                    ui->treeDisplay->append(QString::number(s.num) + " IF THEN");
                    display(exp);
                    ui->treeDisplay->append("    <");
                    display(exp2);
                    ui->treeDisplay->append("    " + sl[1].simplified());
                }
                else if(sl1.size() == 1){
                    sl1 = sl[0].split(">");
                    if(sl1.size() > 2)
                        throw(QString("指令错误"));
                    else if(sl1.size() == 2){
                        sl1[0] = sl1[0].simplified();
                        sl1[1] = sl1[1].simplified();
                        exp = parser.buildTree(sl1[0]);
                        Expression* exp2 = parser.buildTree(sl1[1]);
//                        exp->eval(program.context);
//                        exp2->eval(program.context);
                        ui->treeDisplay->append(QString::number(s.num) + " IF THEN");
                        display(exp);
                        ui->treeDisplay->append("    >");
                        display(exp2);
                        ui->treeDisplay->append("    " + sl[1].simplified());
                    }
                    else if(sl1.size() == 1){
                        sl1 = sl[0].split("=");
                        if(sl1.size() > 2)
                            throw(QString("指令错误"));
                        else if(sl1.size() == 2){
                            sl1[0] = sl1[0].simplified();
                            sl1[1] = sl1[1].simplified();
                            exp = parser.buildTree(sl1[0]);
                            Expression* exp2 = parser.buildTree(sl1[1]);
//                            exp->eval(program.context);
//                            exp2->eval(program.context);
                            ui->treeDisplay->append(QString::number(s.num) + " IF THEN");
                            display(exp);
                            ui->treeDisplay->append("    =");
                            display(exp2);
                            ui->treeDisplay->append("    " + sl[1].simplified());
                        }
                    }
                }
            }
            catch (QString err) {
                ui->treeDisplay->append(QString::number(s.num) + " ERROR");
                qDebug() << err;
            }
            continue;
        }
        if(s.cmd == "END"){
            try {
                if(s.exp != "")
                    throw(QString("END后不能有其他命令"));
                ui->treeDisplay->append(QString::number(s.num) + " END");
            }
            catch (QString err) {
                ui->treeDisplay->append(QString::number(s.num) + " ERROR");
            }
            continue;
        }
        ui->treeDisplay->append(QString::number(s.num) + " ERROR");
    }
}

void MainWindow::display(Expression *exp){
    QQueue<NODE> queue;
    queue.enqueue(NODE(exp, 1));
    while(!queue.isEmpty()){
        NODE n = queue.dequeue();
        if(n.exp->getLHS() != nullptr)
            queue.enqueue(NODE(n.exp->getLHS(), n.height + 1));
        if(n.exp->getRHS() != nullptr)
            queue.enqueue(NODE(n.exp->getRHS(), n.height + 1));
        QString s = "";
        int i = n.height * 4;
        while(i-- > 0){
            s.append(' ');
        }
        if(n.exp->type() == COMPOUND)
            s.append(n.exp->getOperator());
        else if(n.exp->type() == IDENTIFIER)
            s.append(n.exp->getIdentifierName());
        else
            s.append(n.exp->getConstantValue());
        ui->treeDisplay->append(s);
    }
}
