#ifndef ITEMS_H
#define ITEMS_H

#include <QWidget>
#include <QPixmap>

class Items : public QWidget
{
    Q_OBJECT
public:
    QPixmap img;
    int x,y;

    explicit Items(int a, int b,const QPixmap s, QWidget *parent = nullptr);

signals:

};

#endif // ITEMS_H
