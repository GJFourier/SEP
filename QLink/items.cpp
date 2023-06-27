#include "items.h"

Items::Items(int a, int b, QPixmap c, QWidget *parent) : QWidget(parent)
{
    x = a;
    y = b;
    img = c;
}
