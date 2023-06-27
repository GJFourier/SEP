#ifndef DOUBLEGAMESCENE_H
#define DOUBLEGAMESCENE_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QByteArray>
#include <iostream>
#include <pausescene.h>
#include <gameoverscene.h>
#include <character.h>
#include <items.h>
#include <QKeyEvent>
#include <QTimer>
#include <QTimerEvent>
#include <QFile>
#include <QQueue>

const int MAP_SIZE2 = 17;
const int BOX_NUM2 = 7;

class Doublegamescene : public QMainWindow
{
    Q_OBJECT
public:
    explicit Doublegamescene(QWidget *parent = nullptr);
    ~Doublegamescene();
    QMenuBar *menubar = nullptr;
    QMenu *menuFile = nullptr;
    QAction *actionOpen = nullptr;
    QAction *actionSave = nullptr;
    QLabel *time_screen = nullptr;
    QLabel *score_screen = nullptr;
    QLabel *pause = nullptr;
    QColor color[8];
    Character *cha1;
    Gameoverscene *game_over_scene;
    Pausescene *pause_scene;
    bool box_flag;
    bool link_flag;
    bool timer_flag;
    bool pause_flag;
    bool load_flag;
    bool hint_flag;
    int box_kind;
    int selected_box[2];
    int hint_box[2][2];
    int hint_time;
    int link_kind;
    QPointF link_point[4];
    int score;
    int timer;
    int timer_id;
    QQueue<QPoint> queue;

public slots:
    void timestart();
    void setScore();
    void mapInit();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void drawMap(QPainter *painter);
    void drawCharacter(QPainter *painter);
    void drawFrame(QPainter *painter);
    void drawLink(QPainter *painter);
    void setTime();
    void setColor();
    void gameOver(bool f);
    void addScore();
    void saveFile();
    void loadFile();
    void itemForm();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void effect(int kind);
    void shuffle();
    bool hint();
    void reachUpdate(int x,int y);
    void countUpdate();
    int min(int a,int b);
    int max(int a,int b);
    bool iseliminated(int x1,int x2,int y1,int y2,bool f);
    bool canLink1(int x1,int x2,int y1,int y2,bool f1,bool f2);
    bool canLink2(int x1,int x2,int y1,int y2,bool f);
    void link(int kind,QPointF point1,QPointF point2,QPointF point3 = QPointF(0,0),QPointF point4 = QPointF(0,0));

private:
    int n=15, m=15;
    int boxNum=7;
    int itemNum=3;
    int map[MAP_SIZE2][MAP_SIZE2];
    int count[BOX_NUM2+1];
    bool canReach[MAP_SIZE2][MAP_SIZE2];
};


#endif // DOUBLEGAMESCENE_H
