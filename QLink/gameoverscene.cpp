#include "gameoverscene.h"

const int WIDTH = 2000;
const int HEIGHT = 1200;
const int TITLE_WIDTH = 1000;
const int TITLE_HEIGHT = 200;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 60;

Gameoverscene::Gameoverscene(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(WIDTH, HEIGHT);
    this->setWindowTitle("QLink");
    text = new QLabel(this);
    text->resize(TITLE_WIDTH,TITLE_HEIGHT);
    text->setText(tr("GAME OVER"));
    text->move((width()-text->width())/2,(height()-text->height())/4);
    QFont text_font("Times",50, QFont::Bold);
    text->setFont(text_font);
    text->setAlignment(Qt::AlignCenter); //居中对齐
    text->show();
    cause = new QLabel(this);
    cause->resize(TITLE_WIDTH,TITLE_HEIGHT / 2);
    cause->move((width() - cause->width()) / 2, (height() - text->height()) / 4 + text->height());
    QFont cause_font("Times", 30,QFont::Bold);
    cause->setFont(cause_font);
    cause->setAlignment(Qt::AlignCenter);
    cause->show();
    score1 = new QLabel(this);
    score1->resize(TITLE_WIDTH,TITLE_HEIGHT / 2);
    score1->move((width() - cause->width()) / 2, (height() - text->height()) / 4 + text->height() + cause->height());
    QFont score_font("Times", 20);
    score1->setFont(score_font);
    score1->setAlignment(Qt::AlignCenter);
    score1->show();
    score2 = new QLabel(this);
    score2->resize(TITLE_WIDTH, TITLE_HEIGHT / 2);
    score2->move((width() - cause->width()) / 2, (height() - text->height()) / 4 + text->height() + cause->height() + score1->height());
    score2->setFont(score_font);
    score2->setAlignment(Qt::AlignCenter);

    //设置返回按钮
    back_btn = new QPushButton("BACK",this);
    back_btn->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    back_btn->move((width() - back_btn->width()) / 2,(height() - back_btn->height()) * 3 / 4);
    back_btn->show();
}
