#ifndef GAMESCENE_H
#define GAMESCENE_H

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

class Gamescene : public QMainWindow
{
    Q_OBJECT
public:
    explicit Gamescene(QWidget *parent = nullptr);
    QMenuBar *menubar = nullptr;
    QMenu *menuFile = nullptr;
    QAction *actionOpen = nullptr;
    QAction *actionSave = nullptr;
    QLabel *time_screen = nullptr;
    QLabel *score_screen1 = nullptr;
    QLabel *score_screen2 = nullptr;
    QLabel *pause = nullptr;
    QColor color[8];
    Character *cha1,*cha2;
    Gameoverscene *game_over_scene;
    Pausescene *pause_scene;
    bool box_flag1;
    bool link_flag1;
    bool box_flag2;
    bool link_flag2;
    bool timer_flag;
    bool pause_flag;
    bool load_flag;
    bool mode_flag;
    bool hint_flag;
    int box_kind1;
    int selected_box1[2];
    int box_kind2;
    int selected_box2[2];
    int hint_box[2][2];
    int hint_time;
    int link_kind1;
    int link_kind2;
    int heightSide;
    int widthSide;
    QPointF link_point1[4];
    QPointF link_point2[4];
    int score1;
    int score2;
    int timer;
    int timer_id;
    QQueue<QPoint> queue;

    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void drawMap(QPainter *painter);
    void drawCharacter(QPainter *painter);
    void drawFrame(QPainter *painter);
    void drawLink(QPainter *painter);
    void setTime();
    void setColor();
    void gameOver(const bool f);
    void addScore(const int cha);
    void saveFile();
    void loadFile(const QString str_f);
    void itemForm();
    void moveUp(const int cha);
    void moveDown(const int cha);
    void moveLeft(const int cha);
    void moveRight(const int cha);
    void effect(const int kind);
    void shuffle();
    bool hint();
    void reachUpdate(int x, int y);
    void countUpdate();
    int min(int a,int b);
    int max(int a,int b);
    bool isEliminated(const int x1, const int x2, const int y1, const int y2, const bool f, const int cha);
    bool canLink1(const int x1, const int x2, const int y1, const int y2, const bool f1, const bool f2, const int cha);
    bool canLink2(const int x1, const int x2, const int y1, const int y2, const bool f, const int cha);
    void link(const int kind, int const cha, const QPointF point1, const QPointF point2, const QPointF point3 = QPointF(0,0), const QPointF point4 = QPointF(0,0));

public slots:
    void timestart();
    void setScore();
    void mapInit();

private:
    enum {m = 20};
    enum {n = 20};
    enum {boxNum = 7};
    enum {itemNum = 3};
    int map[n + 2][m + 2];
    int count[boxNum + 1];
    bool canReach[n + 2][m + 2];
};

#endif // GAMESCEN_H
