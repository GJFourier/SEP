//QLink主界面
#include "mainwindow.h"

const int WIDTH = 2000;
const int HEIGHT = 1200;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 60;
const int TITLE_WIDTH = 500;
const int TITLE_HEIGHT = 100;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //设置界面大小
    this->setFixedSize(WIDTH, HEIGHT);
    this->setWindowTitle("QLink");

    //设置标题
    title =new QLabel(this);
    title->resize(TITLE_WIDTH, TITLE_HEIGHT);
    title->setText(tr("QLink"));
    title->move((width() - title->width()) / 2, (height() - title->height()) / 3);
    QFont title_font("Times", 50, QFont::Bold);
    title->setFont(title_font);
    title->setAlignment(Qt::AlignCenter); //居中对齐

    //设置开始、读档按钮
    start_btn = new QPushButton("START", this);
    start_btn->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    start_btn->move((width() - start_btn->width()) / 2, (height() - start_btn->height()) * 2 / 3 - BUTTON_HEIGHT);
    start_btn->show();
    load_btn = new QPushButton("LOAD",this);
    load_btn->resize(BUTTON_WIDTH,BUTTON_HEIGHT);
    load_btn->move((width() - load_btn->width()) / 2, (height() - load_btn->height()) * 2 / 3 - BUTTON_HEIGHT + 3 * BUTTON_HEIGHT / 2);
    load_btn->show();
    exit_btn = new QPushButton("EXIT",this);
    exit_btn->resize(BUTTON_WIDTH,BUTTON_HEIGHT);
    exit_btn->move((width() - exit_btn->width()) / 2, (height() - exit_btn->height()) * 2 / 3 - BUTTON_HEIGHT + 3 * BUTTON_HEIGHT / 2 * 2);
    exit_btn->show();

    //点击START切换至选择模式界面，点击choosemodescene及gameoverscene中的BACK返回
    choose_mode_scene = new ChooseModeScene;
    connect(start_btn,&QPushButton::clicked,[=](){
        choose_mode_scene->setGeometry(this->geometry());
        choose_mode_scene->show();
        choose_mode_scene->gamescene->load_flag = false;
        this->hide();
    });
    connect(choose_mode_scene->back_btn,&QPushButton::clicked, [=](){
        this->setGeometry(choose_mode_scene->geometry());
        this->show();
        choose_mode_scene->hide();
    });
    connect(choose_mode_scene->gamescene->game_over_scene->back_btn, &QPushButton::clicked, [=](){
        this->setGeometry(choose_mode_scene->gamescene->game_over_scene->geometry());
        this->show();
        choose_mode_scene->gamescene->game_over_scene->hide();
    });
    connect(choose_mode_scene->gamescene->pause_scene->back_btn, &QPushButton::clicked, [=](){
        this->setGeometry(choose_mode_scene->gamescene->geometry());
        this->show();
        choose_mode_scene->gamescene->pause_scene->hide();
        choose_mode_scene->gamescene->hide();
    });

    //点击LOAD读取上次游戏存档，可读取单人模式双人模式各一个存档,点击BACK返回
    connect(load_btn,&QPushButton::clicked, [=](){
        choose_mode_scene->setGeometry(this->geometry());
        choose_mode_scene->show();
        choose_mode_scene->gamescene->load_flag = true;
        this->hide();
    });

    //点击EXIT退出游戏
    connect(exit_btn,&QPushButton::clicked, [=](){
        this->hide();
        exit(0);
    });
}
