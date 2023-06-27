#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

class Gameoverscene : public QMainWindow
{
    Q_OBJECT
public:
    explicit Gameoverscene(QWidget *parent = nullptr);

    QLabel *text = nullptr;
    QLabel *cause = nullptr;
    QLabel *score1 = nullptr;
    QLabel *score2 = nullptr;
    QPushButton *back_btn = nullptr;

public slots:

signals:

};

#endif // GAMEOVERSCENE_H
