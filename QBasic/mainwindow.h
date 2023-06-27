#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QChar>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include "program.h"
#include "expression.h"
#include "parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Parser parser;
    EvaluationContext context;

private slots:
    void on_cmdLineEdit_editingFinished();
    void open();
    void clear();
    void run();
private:
    Ui::MainWindow *ui;
    Program program;
    void buildGrammarTree();
    void display(Expression*);
    void dealWith(QString);
    Expression* let(QString);
    void print(QString);
    void input(QString);
    void load();
    void list(QString);
    void help(QString);
    void quit();
};

struct NODE{
    Expression *exp;
    int height;
    NODE(Expression *a, int b = 0):exp(a), height(b){}
};


#endif // MAINWINDOW_H
