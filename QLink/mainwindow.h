#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <choosemodescene.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:

private:
    QMenuBar *menubar = nullptr;
    QMenu *menuFile = nullptr;
    QAction *actionOpen = nullptr;
    QAction *actionSave = nullptr;
    QLabel *title = nullptr;
    QPushButton *start_btn = nullptr;
    QPushButton *load_btn = nullptr;
    QPushButton *exit_btn = nullptr;
    ChooseModeScene * choose_mode_scene = nullptr;

signals:

};

#endif // MAINWINDOW_H
