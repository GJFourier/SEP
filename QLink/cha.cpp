#include "cha.h"

character::character(int a,int b,QObject *parent) : QObject(parent)
{
    x = a;
    y = b;

}
