#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QtTest/QtTest>
#include <gamescene.h>

class Test : public QObject
{
    Q_OBJECT
public:
    Gamescene gamescene;
private slots:
    void isEliminate_data();
    void isEliminate();
};

#endif // TEST_H
