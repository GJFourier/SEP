#ifndef CHOOSEMODESCENE_H
#define CHOOSEMODESCENE_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <gamescene.h>

class ChooseModeScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseModeScene(QWidget *parent = nullptr);

    QMenuBar *menubar = nullptr;
    QMenu *menuFile = nullptr;
    QAction *actionOpen = nullptr;
    QAction *actionSave = nullptr;
    QLabel *title = nullptr;
    QPushButton *single_btn = nullptr;
    QPushButton *double_btn = nullptr;
    QPushButton *back_btn = nullptr;
    Gamescene *gamescene = nullptr;
public slots:

signals:
};

#endif // CHOOSEMODESCENE_H
