#include "character.h"

Character::Character(int a, int b, const QPixmap c, QWidget *parent) : QWidget(parent)
{
    x = a;
    y = b;
    img = c;
}
