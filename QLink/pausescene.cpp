#include "pausescene.h"

const int WIDTH = 800;
const int HEIGHT = 480;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 60;

Pausescene::Pausescene(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(WIDTH,HEIGHT);
    this->setWindowTitle("QLink");

    continue_btn = new QPushButton("CONTINUE", this);
    continue_btn->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    continue_btn->move((width() - continue_btn->width()) / 2, (height() - continue_btn->height() * 4) * 1 / 5 + BUTTON_HEIGHT * 0);
    continue_btn->show();
    save_btn = new QPushButton("SAVE", this);
    save_btn->resize(BUTTON_WIDTH,BUTTON_HEIGHT);
    save_btn->move((width() - save_btn->width()) / 2, (height() - save_btn->height() * 4) * 2 / 5 + BUTTON_HEIGHT * 1);
    save_btn->show();
    load_btn = new QPushButton("LOAD", this);
    load_btn->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    load_btn->move((width() - load_btn->width()) / 2, (height() - load_btn->height() * 4) * 3 / 5 + BUTTON_HEIGHT * 2);
    load_btn->show();
    back_btn = new QPushButton("BACK", this);
    back_btn->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    back_btn->move((width() - back_btn->width()) / 2, (height() - back_btn->height() * 4) * 4 / 5 + BUTTON_HEIGHT * 3);
    back_btn->show();
}

void Pausescene::closeEvent(QCloseEvent *){
    emit closed();
}

void Pausescene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_P){
        this->hide();
        emit closed();
    }
}
