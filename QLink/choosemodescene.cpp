#include "choosemodescene.h"

const int WIDTH = 2000;
const int HEIGHT = 1200;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 60;
const int TITLE_WIDTH = 1000;
const int TITLE_HEIGHT = 200;

ChooseModeScene::ChooseModeScene(QWidget *parent) : QMainWindow(parent)
{
    //设置界面大小
    this->setFixedSize(WIDTH,HEIGHT);
    this->setWindowTitle("QLink");

    //设置标题
    title = new QLabel(this);
    title->resize(TITLE_WIDTH, TITLE_HEIGHT);
    title->setText(tr("Please Choose Mode"));
    title->move((width() - title->width()) / 2, (height() - title->height()) / 3);
    QFont title_font("Times",50,QFont::Bold);
    title->setFont(title_font);
    title->setAlignment(Qt::AlignCenter); //居中对齐

    //设置单人、双人、返回按钮
    single_btn = new QPushButton("1 PLAYER",this);
    single_btn->resize(BUTTON_WIDTH,BUTTON_HEIGHT);
    single_btn->move((width() - single_btn->width()) / 2, (height() - single_btn->height()) * 2 / 3 - BUTTON_HEIGHT);
    single_btn->show();
    double_btn = new QPushButton("2 PLAYER",this);
    double_btn->resize(BUTTON_WIDTH,BUTTON_HEIGHT);
    double_btn->move((width() - double_btn->width()) / 2, (height() - double_btn->height()) * 2 / 3 - BUTTON_HEIGHT + 3.0 /2*BUTTON_HEIGHT);
    double_btn->show();
    back_btn = new QPushButton("BACK",this);
    back_btn->resize(BUTTON_WIDTH,BUTTON_HEIGHT);
    back_btn->move((width() - back_btn->width()) / 2, (height()-back_btn->height()) * 2 / 3 - BUTTON_HEIGHT + 3.0 / 2 * BUTTON_HEIGHT * 2);
    back_btn->show();
    gamescene = new Gamescene;

    //实现模式选择
    connect(single_btn, &QPushButton::clicked, [=](){
        gamescene->setGeometry(this->geometry());
        gamescene->show();
        gamescene->mode_flag = 0;
        this->hide();
    });
    connect(single_btn, &QPushButton::clicked, gamescene, &Gamescene::mapInit);
    connect(single_btn, &QPushButton::clicked, gamescene, &Gamescene::timestart);
    connect(single_btn, &QPushButton::clicked, gamescene, &Gamescene::setScore);

    connect(double_btn, &QPushButton::clicked, [=](){
        gamescene->setGeometry(this->geometry());
        gamescene->show();
        gamescene->mode_flag = 1;
        this->hide();
    });
    connect(double_btn, &QPushButton::clicked, gamescene, &Gamescene::mapInit);
    connect(double_btn, &QPushButton::clicked, gamescene, &Gamescene::timestart);
    connect(double_btn, &QPushButton::clicked, gamescene, &Gamescene::setScore);
}
