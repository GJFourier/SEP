#ifndef CHARACTER_H
#define CHARACTER_H

#include <QWidget>
#include <QPixmap>

class Character : public QWidget
{
    Q_OBJECT
public:
    int x,y;
    QPixmap img;
    explicit Character(int a, int b,const QPixmap s, QWidget *parent = nullptr);

public slots:

signals:

};

#endif // CHARACTER_H
