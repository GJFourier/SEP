#ifndef LOADGAMESCENE_H
#define LOADGAMESCENE_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <singlegamescene.h>

class Loadgamescene : public QMainWindow
{
    Q_OBJECT
public:
    explicit Loadgamescene(QWidget *parent = nullptr);
    QMenuBar *menubar = nullptr;
    QMenu *menuFile = nullptr;
    QAction *actionOpen = nullptr;
    QAction *actionSave = nullptr;
    QLabel *title = nullptr;
    QPushButton *single_btn = nullptr;
    QPushButton *double_btn = nullptr;
    QPushButton *back_btn = nullptr;
    Singlegamescene *single_gamescene;
signals:

};

#endif // LOADGAMESCENE_H
