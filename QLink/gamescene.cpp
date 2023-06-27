#include "gamescene.h"

const int WIDTH = 2000;
const int HEIGHT = 1200;
const int LABEL_WIDTH = 500;
const int LABEL_HEIGHT = 100;

Gamescene::Gamescene(QWidget *parent) : QMainWindow(parent)
{
    this->resize(WIDTH, HEIGHT);
    this->setWindowTitle("QLink");

    //初始化
    timer_id = 0;
    timer_flag = false;
    mode_flag = false;
    link_flag1 = false;
    link_kind1 = 0;
    link_flag2 = false;
    link_kind2 = 0;
    QPixmap img;
    img.load(":/res/cha1.png");
    cha1 = new Character(0, 0, img);
    img.load(":/res/cha2.png");
    cha2 = new Character(n+1, m+1, img);
    setColor();

    //显示时间、得分和暂停文本框
    time_screen = new QLabel(this);
    time_screen->setAlignment(Qt::AlignCenter); //居中对齐
    time_screen->show();

    score_screen1 = new QLabel(this);
    score_screen1->show();
    score_screen2 = new QLabel(this);

    pause = new QLabel(this);
    pause->show();

    //设置随机数种子以及创建子窗口对象
    srand((int)time(0));
    game_over_scene = new Gameoverscene;
    pause_scene = new Pausescene;

    //信号和槽的连接
    connect(pause_scene->continue_btn, &QPushButton::clicked, [=](){
        pause_flag = false;
        pause_scene->hide();
        this->show();
    });
    connect(pause_scene->save_btn, &QPushButton::clicked, [=](){
        saveFile();
    });
    connect(pause_scene->load_btn, &QPushButton::clicked, [=](){
        if(!mode_flag){
            loadFile("../demo01/singlefile.txt");
        }
        else{
            loadFile("../demo01/doublefile.txt");
        }
        for (int i = 0; i <= n + 1; ++i)
            for (int j = 0; j <= m + 1; ++j) {
                canReach[i][j] = false;
            }
        reachUpdate(0, 0);
        countUpdate();
        pause_scene->hide();
        pause_flag = false;
    });
    connect(pause_scene, &Pausescene::closed, [=](){
        pause_flag = false;
    });
}

//地图的初始化
void Gamescene::mapInit()
{
    queue.clear();
    if(load_flag){
        if(!mode_flag){
            loadFile("../demo01/singlefile.txt");
        }
        else{
            loadFile("../demo01/doublefile.txt");
        }
        for (int i = 0; i <= n + 1; ++i)
            for (int j = 0; j <= m + 1; ++j) {
                canReach[i][j] = false;
            }
        reachUpdate(0,0);
        countUpdate();
        if(!hint()){
            gameOver(1);
        }
        repaint();
    }
    else{
        box_flag1 = false;
        box_kind1 = 0;
        selected_box1[0] = 0;
        selected_box1[1] = 0;
        cha1->x = 0;
        cha1->y = 0;
        score1 = 0;
        if(mode_flag){
            box_flag2 = false;
            box_kind2 = 0;
            selected_box2[0] = 0;
            selected_box2[1] = 0;
            cha2->x = m + 1;
            cha2->y = n + 1;
            score2 = 0;
        }
        hint_flag = false;
        hint_box[0][0] = hint_box[0][1] = hint_box[1][0] = hint_box[1][1] = 0;
        hint_time = 0;    
        timer = 90;

        for(int i = 0;i <= boxNum;++i){
            count[i] = 0;
        }
        for (int i = 0; i <= n + 1; ++i)
            for (int j = 0; j <= m + 1; ++j) {
                map[i][j] = 0;
                canReach[i][j] = false;
            }


        // 随机填入一个类型的box
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= m; ++j) {
                map[i][j] = ((i + j + std::rand())) % (boxNum + 1);
                ++count[map[i][j]];
            }
        reachUpdate(0,0);
        if(!hint()){
            gameOver(1);
        }
        repaint();
    }
}

// 为地图随机生成颜色
void Gamescene::setColor()
{
    color[1] = QColor(255,127,80); // 珊瑚色
    color[2] = QColor(135,38,87); // 草莓色
    color[3] = QColor(138,43,226); // 紫罗兰色
    color[4] = QColor(124,252,0); // 草地绿
    color[5] = QColor(210,105,30); // 巧克力色
    color[6] = QColor(51,161,201); // 孔雀蓝
    color[7] = QColor(255,0,0); // 红色
}

// 绘图
void Gamescene::paintEvent(QPaintEvent *)
{ 
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //设置视口比例
    painter.setWindow(0,0,m + 2,n + 2);
        if (((double)(width())/(double)(height())) > double(WIDTH) / double(HEIGHT)) {
            heightSide = height() * 2 / 3;
            widthSide = heightSide;
        } else {
            widthSide = width() * 2 / 5;
            heightSide = widthSide;
        }
    painter.setViewport((width() - widthSide) / 2, (height() - heightSide) / 2, widthSide, heightSide);
    drawMap(&painter); //画地图
    drawCharacter(&painter);
    drawFrame(&painter);
    drawLink(&painter);
    time_screen->resize(double(width()) * 1 / 4, double(height()) * 1 / 12);
    time_screen->move((width() - time_screen->width()) / 2, ((height() - heightSide) / 2 - time_screen->height()) / 2);
    QFont time_font("Times", min(int(50 * double(time_screen->width()) / double(LABEL_WIDTH)), int(50 * double(time_screen->height()) / double(LABEL_HEIGHT))), QFont::Bold);
    time_screen->setFont(time_font);
    score_screen1->resize(double(width()) * 1 / 4, double(height()) * 1 / 12);
    score_screen1->move(0, height()-score_screen1->height());
    QFont score_font("Times", min(int(20 * double(score_screen1->width()) / double(LABEL_WIDTH)), int(20 * double(score_screen1->height()) / double(LABEL_HEIGHT))), QFont::Bold);
    score_screen1->setFont(score_font);
    score_screen2->resize(double(width()) * 1 / 4, double(height()) * 1 / 12);
    score_screen2->move(width() - score_screen2->width(), height() - score_screen1->height());
    score_screen2->setFont(score_font);
    pause->resize(double(width()) * 1 / 4, double(height()) * 1 / 12);
    pause->move(width() - pause->width(), 0);
    QFont pause_font("Times", min(int(30 * double(score_screen1->width()) / double(LABEL_WIDTH)), int(30 * double(score_screen1->height()) / double(LABEL_HEIGHT))), QFont::Bold);
    pause->setFont(pause_font);
    pause->setText("Press \'P\' to pause");
}

//倒计时
void Gamescene::timerEvent(QTimerEvent *){
    if(!pause_flag)
        --timer;
    if(timer == 0){
        gameOver(0);
    }
    if(timer == hint_time){
        hint_flag = false;
        repaint();
    }
    if(timer % 30 == 0 && !pause_flag){
        itemForm();
        repaint();
    }
    setTime();
}


// 按键处理
void Gamescene::keyPressEvent(QKeyEvent *event){
    if(!pause_flag){
        if(event->key() == Qt::Key_Up){
            moveUp(1);
        }
        if(event->key() == Qt::Key_Down){
            moveDown(1);
        }
        if(event->key() == Qt::Key_Left){
            moveLeft(1);
        }
        if(event->key() == Qt::Key_Right){
            moveRight(1);
        }
        if(mode_flag){
            if(event->key() == Qt::Key_W){
                moveUp(2);
            }
            if(event->key() == Qt::Key_S){
                moveDown(2);
            }
            if(event->key() == Qt::Key_A){
                moveLeft(2);
            }
            if(event->key() == Qt::Key_D){
                moveRight(2);
            }
        }
    }
    if(event->key() == Qt::Key_P){
        if(!pause_flag){
            pause_flag = true;
            pause_scene->show();
        }
        else{
            pause_flag = false;
            pause_scene->hide();
        }
    }
}

void Gamescene::closeEvent(QCloseEvent *){
    exit(0);
}

// 绘制地图
void Gamescene::drawMap(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    for (int i = 0; i <= n + 1; ++i)
        for (int j = 0; j <= m + 1; ++j) {
            if (map[i][j] == 0) continue;
            if (map[i][j] < 0) {
               std::cout<< "Error! map should not contain value<0" << std::endl;
            }
            else if(map[i][j] > boxNum){
                if(map[i][j] == boxNum + 1){
                    QPixmap img;
                    img.load(":/res/item1.png");
                    painter->drawPixmap(i,j,1,1,img);
                }
                if(map[i][j] == boxNum + 2){
                    QPixmap img;
                    img.load(":/res/item2.png");
                    painter->drawPixmap(i,j,1,1,img);
                }
                if(map[i][j] == boxNum + 3){
                    QPixmap img;
                    img.load(":/res/item3.png");
                    painter->drawPixmap(i, j, 1, 1, img);
                }
            }
            else{ //i.e., (map[i][j] > 0)
                QLinearGradient boxGradient(i, j, i + 1, j + 1);
                boxGradient.setColorAt(0.0, Qt::white);
                boxGradient.setColorAt(0.8, color[map[i][j]]);
//                boxGradient.setColorAt(0.0, color[map[i][j]]);

                painter->setBrush(boxGradient);
                painter->drawRect(i, j, 1, 1);
            }
        }
    // 绘制边界
    painter->setPen(QPen(Qt::black, 0.1));
    painter->drawLine(0, 0, 0, m + 2);
    painter->drawLine(0, m + 2, n + 2, m + 2);
    painter->drawLine(n + 2, m + 2, n + 2, 0);
    painter->drawLine(n + 2, 0, 0, 0);
}

// 绘制人物
void Gamescene::drawCharacter(QPainter *painter){
    painter->drawPixmap(cha1->x, cha1->y, 1, 1, cha1->img);
    if(mode_flag)
        painter->drawPixmap(cha2->x, cha2->y, 1, 1, cha2->img);
}

// 绘制选中方块后的边框
void Gamescene::drawFrame(QPainter *painter){
    int x1 = selected_box1[0];
    int y1 = selected_box1[1];
    int x2 = hint_box[0][0];
    int y2 = hint_box[0][1];
    int x3 = hint_box[1][0];
    int y3 = hint_box[1][1];
    int x4 = selected_box2[0];
    int y4 = selected_box2[1];
    if(hint_flag){
        painter->setPen(QPen(Qt::yellow, 0.1));
        painter->drawLine(x2, y2, x2 + 1, y2);
        painter->drawLine(x2 + 1, y2, x2 + 1, y2 + 1);
        painter->drawLine(x2 + 1, y2 + 1, x2, y2 + 1);
        painter->drawLine(x2, y2 + 1, x2, y2);
        painter->drawLine(x3, y3, x3 + 1, y3);
        painter->drawLine(x3 + 1, y3, x3 + 1, y3 + 1);
        painter->drawLine(x3 + 1, y3 + 1, x3, y3 + 1);
        painter->drawLine(x3, y3 + 1, x3, y3);
    }
    if(box_flag1){
        painter->setPen(QPen(Qt::red, 0.1));
        painter->drawLine(x1, y1, x1 + 1, y1);
        painter->drawLine(x1 + 1, y1, x1 + 1, y1 + 1);
        painter->drawLine(x1 + 1, y1 + 1, x1, y1 + 1);
        painter->drawLine(x1, y1 + 1, x1, y1);
    }
    if(box_flag2){
        painter->setPen(QPen(Qt::blue, 0.1));
        painter->drawLine(x4, y4, x4 + 1, y4);
        painter->drawLine(x4 + 1, y4, x4 + 1, y4 + 1);
        painter->drawLine(x4 + 1, y4 + 1, x4, y4 + 1);
        painter->drawLine(x4, y4 + 1, x4, y4);
    }
}

// 画出连线
void Gamescene::drawLink(QPainter *painter){
    if(link_flag1){
        painter->setPen(QPen(Qt::red, 0.1));
        if(link_kind1 == 0){
            painter->drawLine(link_point1[0], link_point1[1]);
        }
        if(link_kind1 == 1){
            painter->drawLine(link_point1[0], link_point1[2]);
            painter->drawLine(link_point1[2], link_point1[1]);
        }
        if(link_kind1 == 2){
            painter->drawLine(link_point1[0], link_point1[2]);
            painter->drawLine(link_point1[2], link_point1[3]);
            painter->drawLine(link_point1[3], link_point1[1]);
        }
    }
    if(link_flag2){
        painter->setPen(QPen(Qt::blue, 0.1));
        if(link_kind1 == 0){
            painter->drawLine(link_point1[0], link_point1[1]);
        }
        if(link_kind1 == 1){
            painter->drawLine(link_point1[0], link_point1[2]);
            painter->drawLine(link_point1[2], link_point1[1]);
        }
        if(link_kind1 == 2){
            painter->drawLine(link_point1[0], link_point1[2]);
            painter->drawLine(link_point1[2], link_point1[3]);
            painter->drawLine(link_point1[3], link_point1[1]);
        }
    }
}

// 更改得分
void Gamescene::addScore(int cha){
    if(cha == 1)
        score1 += 10;
    else
        score2 += 10;
    setScore();
}

// 游戏结束
void Gamescene::gameOver(const bool f){
    if(!mode_flag){
        if(f == 0){
            game_over_scene->cause->setText("TIME OUT");
        }
        else{
            game_over_scene->cause->setText("NO BOX CAN BE ELIMINATED");
            pause_flag = true;
        }
        QChar num1,num2,num3,num4;
        num1 = score1 / 1000 % 10 + '0',
        num2 = score1 / 100 % 10 + '0',
        num3 = score1 / 10 % 10 + '0';
        num4 = score1 / 1 % 10 + '0';
        QString text;
        text = QString("TOTAL SCORE:");
        text.append(num1);
        text.append(num2);
        text.append(num3);
        text.append(num4);
        game_over_scene->score1->setText(text);
        game_over_scene->setGeometry(this->geometry());
        game_over_scene->show();
        game_over_scene->score2->hide();
        this->hide();
    }
    else{
        if(f == 0){
            game_over_scene->cause->setText("TIME OUT");
        }
        else{
            game_over_scene->cause->setText("NO BOX CAN BE ELIMINATED");
            pause_flag = true;
        }
        QChar num1,num2,num3,num4;
        num1 = score1 / 1000 % 10 + '0',
        num2 = score1 / 100 % 10 + '0',
        num3 = score1 / 10 % 10 + '0';
        num4 = score1 / 1 % 10 + '0';
        QString text;
        text = QString("PLAYER1'S TOTAL SCORE:");
        text.append(num1);
        text.append(num2);
        text.append(num3);
        text.append(num4);
        game_over_scene->score1->setText(text);
        game_over_scene->setGeometry(this->geometry());
        game_over_scene->show();
        num1 = score2 / 1000 % 10 + '0',
        num2 = score2 / 100 % 10 + '0',
        num3 = score2 / 10 % 10 + '0';
        num4 = score2 / 1 % 10 + '0';
        text = QString("PLAYER2'S TOTAL SCORE:");
        text.append(num1);
        text.append(num2);
        text.append(num3);
        text.append(num4);
        game_over_scene->score2->setText(text);
        game_over_scene->setGeometry(this->geometry());
        game_over_scene->show();
        game_over_scene->score2->show();
        this->hide();
    }
}

// 开始计时
void Gamescene::timestart(){
    pause_flag = false;
    if(timer_flag){
        killTimer(timer_id);
        timer_id = startTimer(1000);
    }
    else{
        timer_flag = true;
        timer_id = startTimer(1000);
    }
    setTime();
}

// 更新时间
void Gamescene::setTime(){
    QChar num1, num2, num3;
    num1 = timer / 100 % 10 + '0',
    num2 = timer / 10 % 10 + '0',
    num3 = timer / 1 % 10 + '0';
    QString text;
    text = QString("TIME:");
    text.append(num1);
    text.append(num2);
    text.append(num3);
    time_screen->setText(text);
}

// 更新得分
void Gamescene::setScore(){
    if(!mode_flag){
        QChar num1, num2, num3, num4;
        num1 = score1 / 1000 % 10 + '0',
        num2 = score1 / 100 % 10 + '0',
        num3 = score1 / 10 % 10 + '0';
        num4 = score1 / 1 % 10 + '0';
        QString text;
        text = QString("SCORE:");
        text.append(num1);
        text.append(num2);
        text.append(num3);
        text.append(num4);
        score_screen1->setText(text);
        score_screen2->hide();
    }
    else{
        QChar num1, num2, num3, num4;
        num1 = score1 / 1000 % 10 + '0',
        num2 = score1 / 100 % 10 + '0',
        num3 = score1 / 10 % 10 + '0';
        num4 = score1 / 1 % 10 + '0';
        QString text;
        text = QString("PLAYER1'S SCORE:");
        text.append(num1);
        text.append(num2);
        text.append(num3);
        text.append(num4);
        score_screen1->setText(text);
        num1 = score2 / 1000 % 10 + '0',
        num2 = score2 / 100 % 10 + '0',
        num3 = score2 / 10 % 10 + '0';
        num4 = score2 / 1 % 10 + '0';
        text = QString("PLAYER2'S SCORE:");
        text.append(num1);
        text.append(num2);
        text.append(num3);
        text.append(num4);
        score_screen2->setText(text);
        score_screen2->show();
    }
}

int Gamescene::min(int a, int b){
    if(a < b)
        return a;
    else
        return b;
}

int Gamescene::max(int a, int b){
    if(a < b)
        return b;
    else
        return a;
}

// 判断能否被消除
bool Gamescene::isEliminated(const int x1, const int x2, const int y1, const int y2, const bool f, const int cha){
    bool flag = true;
    //种类不同
    if(map[x1][y1] != map[x2][y2]){
        if(f)
            qDebug() << "Can't be eliminated!";
        return false;
    }
    //选中同一个方块
    if(x1 == x2 && y1 == y2){
        if(f)
            qDebug() << "Can't be eliminated!";
        return false;
    }
    //转弯次数为0连接
    if(x1 == x2){
        for(int i = min(y1, y2) + 1;i < max(y1, y2);++i){
            if(map[x1][i] != 0){
                flag = false;
                break;
            }
        }
        if(flag){
            if(f){
                qDebug() << "Successfully eliminated!";
                link(0,cha,QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5));
                reachUpdate(x1, y1);
                reachUpdate(x2, y2);
            }
            return true;
        }
    }
    if(y1 == y2){
        for(int i = min(x1, x2) + 1;i < max(x1, x2);++i){
            if(map[i][y1] != 0){
                flag = false;
                break;
            }
        }
        if(flag){
            if(f){
                qDebug() << "Successfully eliminated!";
                link(0, cha, QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5));
                reachUpdate(x1,y1);
                reachUpdate(x2,y2);
            }
            return true;
        }
    }
    // x1<=x2
    if(x1 <= x2){
        //转弯次数为1连接
        if(canLink1(x1, x2, y1, y2, true, f, cha)){
            if(f){
                reachUpdate(x1, y1);
                reachUpdate(x2, y2);
            }
            return true;
        }
        //转弯次数为2连接
        if(canLink2(x1, x2, y1, y2, f, cha)){
            if(f){
                reachUpdate(x1, y1);
                reachUpdate(x2, y2);
            }
            return true;
        }
    }
    // x2<x1
    if(x2 < x1){
        // 转弯次数为1连接
        if(canLink1(x2, x1, y2, y1, true, f, cha)){
            if(f){
                reachUpdate(x1, y1);
                reachUpdate(x2, y2);
            }
            return true;
        }
        // 转弯次数为2连接
        if(canLink2(x2, x1, y2, y1, f, cha)){
            if(f){
                reachUpdate(x1, y1);
                reachUpdate(x2, y2);
            }
            return true;
        }
    }
    qDebug() << "Can't be eliminated!";
    return false;
}

// 能否通过一次转弯连接
bool Gamescene::canLink1(const int x1, const int x2, const int y1, const int y2, const bool f1, const bool f2, const int cha){
    if(x1 == x2 || y1 == y2)
        return false;
    bool flag = true;
    for(int i = x1 + 1;i <= x2;++i){
        if(map[i][y1] != 0){
            flag = false;
            break;
        }
    }
    if(flag){
        for(int i = min(y1, y2) + 1;i < max(y1, y2);++i){
            if(map[x2][i] != 0){
                flag = false;
                break;
            }
        }
        if(flag){
            if(f1 && f2){
                qDebug() << "Successfully eliminated!";
                link(1,cha,QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5), QPointF(x2 + 0.5, y1 + 0.5));
            }
            return true;
        }
    }
    flag = true;
    for(int i = x2 - 1;i >= x1;--i){
        if(map[i][y2] != 0){
            flag = false;
            break;
        }
    }
    if(flag){
        for(int i = min(y1, y2) + 1;i < max(y1, y2);++i){
            if(map[x1][i] != 0){
                flag = false;
                break;
            }
        }
        if(flag){
            if(f1 && f2){
                qDebug() << "Successfully eliminated!";
                link(1, cha, QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5), QPointF(x1 + 0.5, y2 + 0.5));
            }
            return true;
        }
    }
    return false;
}

// 判断能否通过两次转弯连接
bool Gamescene::canLink2(const int x1, const int x2, const int y1, const int y2, const bool f, const int cha){
    //1:E 2:S 3:W 4:N
    bool direction1 = true, direction2 = true, direction3 = true, direction4 = true;
    int cor1 = x2 + 1, cor2, cor3 = x1 - 1, cor4;
    if(y1 <= y2){
        cor2 = y2 + 1;
        cor4 = y1 - 1;
    }
    if(y2 < y1){
        cor2 = y1 + 1;
        cor4 = y2 - 1;
    }
    if(x1 == x2)
        direction2 = direction4 = false;
    if(y1 == y2)
        direction1 = direction3 = false;
    if(direction1 && direction2){
        for(int i = x1 + 1;i <= x2;++i){
            if(map[i][y1] != 0){
                direction1 = false;
                break;
            }
            if(canLink1(i, x2, y1, y2, false, f, cha)){
                if(f){
                    qDebug() << "Successfully eliminated!";
                    link(2, cha, QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5), QPointF(i + 0.5, y1 + 0.5), QPointF(i + 0.5, y2 + 0.5));
                }
                return true;
            }
        }
        for(int i = (y1 <= y2) ? y1 + 1 : y1 - 1;(y1 <= y2) ? i <= y2 : i >= y2;(y1 <= y2) ? ++i : --i){
            if(map[x1][i] != 0){
                if(y1 < y2)
                    direction2 = false;
                else
                    direction4 = false;
                break;
            }
            if(canLink1(x1, x2, i, y2, false, f, cha)){
                if(f){
                    qDebug() << "Successfully eliminated!";
                    link(2, cha, QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5), QPointF(x1 + 0.5, i + 0.5), QPointF(x2 + 0.5, i + 0.5));
                }
                return true;
            }
        }
    }
    while(direction1 || direction2 || direction3 || direction4){
        if(direction1){
            if(map[cor1][y1] == 0){
                if(canLink1(x2, cor1, y2, y1, false, f, cha)){
                    if(f){
                        qDebug() << "Successfully eliminated!";
                        link(2, cha, QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5), QPointF(cor1 + 0.5, y1 + 0.5), QPointF(cor1 + 0.5, y2 + 0.5));
                    }
                    return true;
                }
                if(cor1 == n + 1)
                    direction1 = false;
                else
                    ++cor1;
            }
            else{
                direction1 = false;
            }
        }
        if(direction2){
            if(map[x1][cor2] == 0){
                if(canLink1(x1, x2, cor2, y2, false, f, cha)){
                    if(f){
                        qDebug() << "Successfully eliminated!";
                        link(2, cha, QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5), QPointF(x1 + 0.5, cor2 + 0.5), QPointF(x2 + 0.5, cor2 + 0.5));
                    }
                    return true;
                }
                if(cor2 == m + 1)
                    direction2 = false;
                else
                    ++cor2;
            }
            else{
                direction2 = false;
            }
        }
        if(direction3){
            if(map[cor3][y1] == 0){
                if(canLink1(cor3, x2, y1, y2, false, f, cha)){
                    if(f){
                        qDebug() << "Successfully eliminated!";
                        link(2, cha, QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5), QPointF(cor3 + 0.5, y1 + 0.5), QPointF(cor3 + 0.5, y2 + 0.5));
                    }
                    return true;
                }
                if(cor3 == 0)
                    direction3 = false;
                else
                    --cor3;
            }
            else{
                direction3 = false;
            }
        }
        if(direction4){
            if(map[x1][cor4] == 0){
                if(canLink1(x1, x2, cor4, y2, false, f, cha)){
                    if(f){
                        qDebug() << "Successfully eliminated!";
                        link(2, cha, QPointF(x1 + 0.5, y1 + 0.5), QPointF(x2 + 0.5, y2 + 0.5), QPointF(x1 + 0.5, cor4 + 0.5), QPointF(x2 + 0.5, cor4 + 0.5));
                    }
                    return true;
                }
                if(cor4 == 0)
                    direction4 = false;
                else
                    --cor4;
            }
            else{
                direction4 = false;
            }
        }
    }
    return false;
}

// 在被消除的方块间连线
void Gamescene::link(const int kind, const int cha, const QPointF point1, const QPointF point2, const QPointF point3, const QPointF point4){
    if(cha == 1)
        link_flag1 = true;
    if(cha == 2)
        link_flag2 = true;
    if(kind == 0){
        link_kind1 = 0;
        link_point1[0] = point1;
        link_point1[1] = point2;
        repaint();
    }
    if(kind == 1){
        link_kind1 = 1;
        link_point1[0] = point1;
        link_point1[1] = point2;
        link_point1[2] = point3;
        repaint();
    }
    if(kind == 2){
        link_kind1 = 2;
        link_point1[0] = point1;
        link_point1[1] = point2;
        link_point1[2] = point3;
        link_point1[3] = point4;
        repaint(); 
    }
    if(link_flag1){
        QTimer::singleShot(200, this, [=]{
            link_flag1 = false;
            repaint();
        });
    }
    if(link_flag2){
        QTimer::singleShot(200, this, [=]{
            link_flag2 = false;
            repaint();
        });
    }
}

// 保存文件
void Gamescene::saveFile(){
    if(!mode_flag){
        QFile file("../demo01/singlefile.txt");
        QString str;
        if(file.open(QIODevice::WriteOnly)){
            str = QString::number(cha1->x);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(cha1->y);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(timer);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(score1);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(box_flag1);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(box_kind1);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(selected_box1[0]);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(selected_box1[1]);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(hint_flag);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(hint_time);
            file.write(str.toUtf8());
            file.write("\n");
            for(int i = 0;i <= 1;++i){
                for(int j = 0;j <= 1;++j){
                    str = QString::number(hint_box[i][j]);
                    file.write(str.toUtf8());
                    file.write("\n");
                }
            }
            for(int i = 0;i <= n + 1;++i)
                for(int j = 0;j <= m + 1;++j){
                    str = QString::number(map[i][j]);
                    file.write(str.toUtf8());
                    file.write("\n");
                }
            qDebug() << "Successfully Saved!";
        }
        file.close();
    }
    else{
        QFile file("../demo01/doublefile.txt");
        QString str;
        if(file.open(QIODevice::WriteOnly)){
            str = QString::number(cha1->x);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(cha1->y);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(cha2->x);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(cha2->y);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(timer);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(score1);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(score2);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(box_flag1);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(box_kind1);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(selected_box1[0]);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(selected_box1[1]);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(box_flag2);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(box_kind2);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(selected_box2[0]);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(selected_box2[1]);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(hint_flag);
            file.write(str.toUtf8());
            file.write("\n");
            str = QString::number(hint_time);
            file.write(str.toUtf8());
            file.write("\n");
            for(int i = 0;i <= 1;++i){
                for(int j = 0;j <= 1;++j){
                    str = QString::number(hint_box[i][j]);
                    file.write(str.toUtf8());
                    file.write("\n");
                }
            }
            for(int i = 0;i <= n + 1;++i)
                for(int j = 0;j <= m + 1;++j){
                    str = QString::number(map[i][j]);
                    file.write(str.toUtf8());
                    file.write("\n");
                }
            qDebug() << "Successfully Saved!";
        }
        file.close();
    }
}

// 读取存档文件
void Gamescene::loadFile(const QString str_f){
    if(!mode_flag){
        QFile file(str_f);
        QString str;
        bool ok = false;
        if(file.open(QIODevice::ReadOnly)){
            str = file.readLine();
            cha1->x = str.toInt(&ok);
            str = file.readLine();
            cha1->y = str.toInt(&ok);
            str = file.readLine();
            timer = str.toInt(&ok);
            str = file.readLine();
            score1 = str.toInt(&ok);
            str = file.readLine();
            box_flag1 = str.toInt(&ok);
            str = file.readLine();
            box_kind1 = str.toInt(&ok);
            str = file.readLine();
            selected_box1[0] = str.toInt(&ok);
            str = file.readLine();
            selected_box1[1] = str.toInt(&ok);
            str = file.readLine();
            hint_flag = str.toInt(&ok);
            str = file.readLine();
            hint_time = str.toInt(&ok);
            for(int i = 0;i <= 1;++i){
                for(int j = 0;j <= 1;++j){
                    str = file.readLine();
                    hint_box[i][j] = str.toInt(&ok);
                }
            }
            for(int i = 0;i <= n + 1;++i)
                for(int j = 0;j <= m + 1;++j){
                    str = file.readLine();
                    map[i][j] = str.toInt(&ok);
                }
        }
        file.close();
        timestart();
        setScore();
    }
    else{
        QFile file(str_f);
        QString str;
        bool ok = false;
        if(file.open(QIODevice::ReadOnly)){
            str = file.readLine();
            cha1->x = str.toInt(&ok);
            str = file.readLine();
            cha1->y = str.toInt(&ok);
            str = file.readLine();
            cha2->x = str.toInt(&ok);
            str = file.readLine();
            cha2->y = str.toInt(&ok);
            str = file.readLine();
            timer = str.toInt(&ok);
            str = file.readLine();
            score1 = str.toInt(&ok);
            str = file.readLine();
            score2 = str.toInt(&ok);
            str = file.readLine();
            box_flag1 = str.toInt(&ok);
            str = file.readLine();
            box_kind1 = str.toInt(&ok);
            str = file.readLine();
            selected_box1[0] = str.toInt(&ok);
            str = file.readLine();
            selected_box1[1] = str.toInt(&ok);
            str = file.readLine();
            box_flag2 = str.toInt(&ok);
            str = file.readLine();
            box_kind2 = str.toInt(&ok);
            str = file.readLine();
            selected_box2[0] = str.toInt(&ok);
            str = file.readLine();
            selected_box2[1] = str.toInt(&ok);
            str = file.readLine();
            hint_flag = str.toInt(&ok);
            str = file.readLine();
            hint_time = str.toInt(&ok);
            for(int i = 0;i <= 1;++i){
                for(int j = 0;j <= 1;++j){
                    str = file.readLine();
                    hint_box[i][j] = str.toInt(&ok);
                }
            }
            for(int i = 0;i <= n + 1;++i)
                for(int j = 0;j <= m + 1;++j){
                    str = file.readLine();
                    map[i][j] = str.toInt(&ok);
                }
        }
        file.close();
        timestart();
        setScore();
    }
}

// 生成道具
void Gamescene::itemForm(){
    int flag = std::rand() % 2;
    int kind = std::rand() % itemNum + boxNum + 1;
    int randomx = (std::rand()) % (n + 2);
    int randomy = (std::rand()) % (m + 2);
    int randomx0 = randomx,randomy0 = randomy;
    if(flag == 1){
        if(randomx0 >= (n + 2) / 2){
            while(true){
                if(map[randomx][randomy] == 0){
                    map[randomx][randomy] = kind;
                    break;
                }
                randomx = (randomx + 1) % (n + 2);
                if(randomx == randomx0){
                    randomy = (randomy + 1) % (m + 2);
                    if(randomy == randomy0)
                        break;
                }
            }
        }
        else{
            while(true){
                if(map[randomx][randomy] == 0){
                    map[randomx][randomy] = kind;
                    break;
                }
                randomx = (randomx - 1 + n + 2) % (n + 2);
                if(randomx == randomx0){
                    randomy = (randomy + 1) % (m + 2);
                    if(randomy == randomy0)
                        break;
                }
            }
        }
    }
    else{
        if(randomy0 >= (m + 2) / 2){
            while(true){
                if(map[randomx][randomy] == 0){
                    map[randomx][randomy] = kind;
                    break;
                }
                randomy = (randomy + 1) % (m + 2);
                if(randomy == randomy0){
                    randomx = (randomx + 1) % (n + 2);
                    if(randomx == randomx0)
                        break;
                }
            }
        }
        else{
            while(true){
                if(map[randomx][randomy] == 0){
                    map[randomx][randomy] = kind;
                    break;
                }
                randomy = (randomy - 1 + m + 2) % (m + 2);
                if(randomy == randomy0){
                    randomx = (randomx + 1) % (n + 2);
                    if(randomx == randomx0)
                        break;
                }
            }
        }
    }
}

// 移动
void Gamescene::moveUp(const int cha){
    if(cha == 1){
        int corx = cha1->x;
        int cory = (cha1->y - 1 + m + 2) % (m + 2);
        if(map[corx][cory] > 0 && map[corx][cory] <= boxNum){
            if(!box_flag1){
                box_flag1 = true;
                box_kind1 = map[corx][cory];
                selected_box1[0] = corx;
                selected_box1[1] = cory;
            }
            else{
                if(isEliminated(selected_box1[0], corx, selected_box1[1], cory, true, cha)){
                    if((selected_box2[0] == selected_box1[0] && selected_box2[1] == selected_box1[1]) || (selected_box2[0] == corx && selected_box2[1] == cory)){
                        box_flag2 = false;
                        box_kind2 = 0;
                        selected_box2[0] = 0;
                        selected_box2[1] = 0;
                    }
                    map[selected_box1[0]][selected_box1[1]] = 0;
                    map[corx][cory] = 0;
                    count[box_kind1] -= 2;
                    addScore(1);
                    if(!hint())
                        gameOver(1);
                }
                box_flag1 = false;
                box_kind1 = 0;
                selected_box1[0] = 0;
                selected_box1[1] = 0;
            }
            repaint();
        }
        else{
            if(map[corx][cory] != 0)
            {
                effect(map[corx][cory]);
                map[corx][cory] = 0;
            }
            cha1->x = corx;
            cha1->y = cory;
            repaint();
        }
    }
    else{
        int corx = cha2->x;
        int cory = (cha2->y - 1 + m + 2) % (m + 2);
        if(map[corx][cory] > 0 && map[corx][cory] <= boxNum){
            if(!box_flag2){
                box_flag2 = true;
                box_kind2 = map[corx][cory];
                selected_box2[0] = corx;
                selected_box2[1] = cory;
            }
            else{
                if(isEliminated(selected_box2[0], corx, selected_box2[1], cory, true, cha)){
                    if((selected_box1[0] == selected_box2[0] && selected_box1[1] == selected_box2[1]) || (selected_box1[0] == corx && selected_box1[1] == cory)){
                        box_flag1 = false;
                        box_kind1 = 0;
                        selected_box1[0] = 0;
                        selected_box1[1] = 0;
                    }
                    map[selected_box2[0]][selected_box2[1]] = 0;
                    map[corx][cory] = 0;
                    count[box_kind2] -= 2;
                    addScore(2);
                    if(!hint())
                        gameOver(1);
                }
                box_flag2 = false;
                box_kind2 = 0;
                selected_box2[0] = 0;
                selected_box2[1] = 0;
            }
            repaint();
        }
        else{
            if(map[corx][cory] != 0)
            {
                effect(map[corx][cory]);
                map[corx][cory] = 0;
            }
            cha2->x = corx;
            cha2->y = cory;
            repaint();
        }
    }
}

void Gamescene::moveDown(const int cha){
    if(cha == 1){
        int corx = cha1->x;
        int cory = (cha1->y + 1 + m + 2) % (m + 2);
        if(map[corx][cory] > 0 && map[corx][cory] <= boxNum){
            if(!box_flag1){
                box_flag1 = true;
                box_kind1 = map[corx][cory];
                selected_box1[0] = corx;
                selected_box1[1] = cory;
            }
            else{
                if(isEliminated(selected_box1[0], corx, selected_box1[1], cory, true, cha)){
                    if((selected_box2[0] == selected_box1[0] && selected_box2[1] == selected_box1[1]) || (selected_box2[0] == corx && selected_box2[1] == cory)){
                        box_flag2 = false;
                        box_kind2 = 0;
                        selected_box2[0] = 0;
                        selected_box2[1] = 0;
                    }
                    map[selected_box1[0]][selected_box1[1]] = 0;
                    map[corx][cory] = 0;
                    count[box_kind1] -= 2;
                    addScore(1);
                    if(!hint())
                        gameOver(1);
                }
                box_flag1 = false;
                box_kind1 = 0;
                selected_box1[0] = 0;
                selected_box1[1] = 0;
            }
            repaint();
        }
        else{
            if(map[corx][cory] != 0)
            {
                effect(map[corx][cory]);
                map[corx][cory] = 0;
            }
            cha1->x = corx;
            cha1->y = cory;
            repaint();
        }
    }
    else{
        int corx = cha2->x;
        int cory = (cha2->y + 1 + m + 2) % (m + 2);
        if(map[corx][cory] > 0 && map[corx][cory] <= boxNum){
            if(!box_flag2){
                box_flag2 = true;
                box_kind2 = map[corx][cory];
                selected_box2[0] = corx;
                selected_box2[1] = cory;
            }
            else{
                if(isEliminated(selected_box2[0], corx, selected_box2[1], cory, true, cha)){
                    if((selected_box1[0] == selected_box2[0] && selected_box1[1] == selected_box2[1]) || (selected_box1[0] == corx && selected_box1[1] == cory)){
                        box_flag1 = false;
                        box_kind1 = 0;
                        selected_box1[0] = 0;
                        selected_box1[1] = 0;
                    }
                    map[selected_box2[0]][selected_box2[1]] = 0;
                    map[corx][cory] = 0;
                    count[box_kind2] -= 2;
                    addScore(2);
                    if(!hint())
                        gameOver(1);
                }
                box_flag2 = false;
                box_kind2 = 0;
                selected_box2[0] = 0;
                selected_box2[1] = 0;
            }
            repaint();
        }
        else{
            if(map[corx][cory] != 0)
            {
                effect(map[corx][cory]);
                map[corx][cory] = 0;
            }
            cha2->x = corx;
            cha2->y = cory;
            repaint();
        }
    }
}

void Gamescene::moveLeft(const int cha){
    if(cha == 1){
        int corx = (cha1->x - 1 + n + 2) % (n + 2);
        int cory = cha1->y;
        if(map[corx][cory] > 0 && map[corx][cory] <= boxNum){
            if(!box_flag1){
                box_flag1 = true;
                box_kind1 = map[corx][cory];
                selected_box1[0] = corx;
                selected_box1[1] = cory;
            }
            else{
                if(isEliminated(selected_box1[0], corx, selected_box1[1], cory, true, cha)){
                    if((selected_box2[0] == selected_box1[0] && selected_box2[1] == selected_box1[1]) || (selected_box2[0] == corx && selected_box2[1] == cory)){
                        box_flag2 = false;
                        box_kind2 = 0;
                        selected_box2[0] = 0;
                        selected_box2[1] = 0;
                    }
                    map[selected_box1[0]][selected_box1[1]] = 0;
                    map[corx][cory] = 0;
                    count[box_kind1] -= 2;
                    addScore(1);
                    if(!hint())
                        gameOver(1);
                }
                box_flag1 = false;
                box_kind1 = 0;
                selected_box1[0] = 0;
                selected_box1[1] = 0;
            }
            repaint();
        }
        else{
            if(map[corx][cory] != 0)
            {
                effect(map[corx][cory]);
                map[corx][cory] = 0;
            }
            cha1->x = corx;
            cha1->y = cory;
            repaint();
        }
    }
    else{
        int corx = (cha2->x - 1 + n + 2) % (n + 2);
        int cory = cha2->y;
        if(map[corx][cory] > 0 && map[corx][cory] <= boxNum){
            if(!box_flag2){
                box_flag2 = true;
                box_kind2 = map[corx][cory];
                selected_box2[0] = corx;
                selected_box2[1] = cory;
            }
            else{
                if(isEliminated(selected_box2[0], corx, selected_box2[1], cory, true, cha)){
                    if((selected_box1[0] == selected_box2[0] && selected_box1[1] == selected_box2[1]) || (selected_box1[0] == corx && selected_box1[1] == cory)){
                        box_flag1 = false;
                        box_kind1 = 0;
                        selected_box1[0] = 0;
                        selected_box1[1] = 0;
                    }
                    map[selected_box2[0]][selected_box2[1]] = 0;
                    map[corx][cory] = 0;
                    count[box_kind2] -= 2;
                    addScore(2);
                    if(!hint())
                        gameOver(1);
                }
                box_flag2 = false;
                box_kind2 = 0;
                selected_box2[0] = 0;
                selected_box2[1] = 0;
            }
            repaint();
        }
        else{
            if(map[corx][cory] != 0)
            {
                effect(map[corx][cory]);
                map[corx][cory] = 0;
            }
            cha2->x = corx;
            cha2->y = cory;
            repaint();
        }
    }
}

void Gamescene::moveRight(const int cha){
    if(cha == 1){
        int corx = (cha1->x + 1 + n + 2) % (n + 2);
        int cory = cha1->y;
        if(map[corx][cory] > 0 && map[corx][cory] <= boxNum){
            if(!box_flag1){
                box_flag1 = true;
                box_kind1 = map[corx][cory];
                selected_box1[0] = corx;
                selected_box1[1] = cory;
            }
            else{
                if(isEliminated(selected_box1[0], corx, selected_box1[1], cory, true, cha)){
                    if((selected_box2[0] == selected_box1[0] && selected_box2[1] == selected_box1[1]) || (selected_box2[0] == corx && selected_box2[1] == cory)){
                        box_flag2 = false;
                        box_kind2 = 0;
                        selected_box2[0] = 0;
                        selected_box2[1] = 0;
                    }
                    map[selected_box1[0]][selected_box1[1]] = 0;
                    map[corx][cory] = 0;
                    count[box_kind1] -= 2;
                    addScore(1);
                    if(!hint())
                        gameOver(1);
                }
                box_flag1 = false;
                box_kind1 = 0;
                selected_box1[0] = 0;
                selected_box1[1] = 0;
            }
            repaint();
        }
        else{
            if(map[corx][cory] != 0)
            {
                effect(map[corx][cory]);
                map[corx][cory] = 0;
            }
            cha1->x = corx;
            cha1->y = cory;
            repaint();
        }
    }
    else{
        int corx = (cha2->x + 1 + n + 2) % (n + 2);
        int cory = cha2->y;
        if(map[corx][cory] > 0 && map[corx][cory] <= boxNum){
            if(!box_flag2){
                box_flag2 = true;
                box_kind2 = map[corx][cory];
                selected_box2[0] = corx;
                selected_box2[1] = cory;
            }
            else{
                if(isEliminated(selected_box2[0], corx, selected_box2[1], cory, true, cha)){
                    if((selected_box1[0] == selected_box2[0] && selected_box1[1] == selected_box2[1]) || (selected_box1[0] == corx && selected_box1[1] == cory)){
                        box_flag1 = false;
                        box_kind1 = 0;
                        selected_box1[0] = 0;
                        selected_box1[1] = 0;
                    }
                    map[selected_box2[0]][selected_box2[1]] = 0;
                    map[corx][cory] = 0;
                    count[box_kind2] -= 2;
                    addScore(2);
                    if(!hint())
                        gameOver(1);
                }
                box_flag2 = false;
                box_kind2 = 0;
                selected_box2[0] = 0;
                selected_box2[1] = 0;
            }
            repaint();
        }
        else{
            if(map[corx][cory] != 0)
            {
                effect(map[corx][cory]);
                map[corx][cory] = 0;
            }
            cha2->x = corx;
            cha2->y = cory;
            repaint();
        }
    }
}

// 道具生效
void Gamescene::effect(const int kind){
    if(kind == boxNum + 1){
        timer += 30;
        hint_time += 30;
        setTime();
    }
    if(kind == boxNum + 2){
        shuffle();
    }
    if(kind == boxNum + 3){
        hint_time = timer - 10;
        hint_flag = true;
    }
}

// shuffle道具实现
void Gamescene::shuffle(){
    for(int i = 1;i <= n;++i){
        for(int j = 1;j <= m;++j){
            if(map[i][j] > 0 && map[i][j] <= boxNum)
                map[i][j] = 0;
        }
    }
    for(int i = 1;i <= boxNum;++i){
        int num = count[i];
        while(num!=0){
            int x = std::rand() % n + 1;
            int y = std::rand() % m + 1;
            if(map[x][y] == 0 && !(x == cha1->x && y == cha1->y)){
                map[x][y] = i;
                --num;
            }
        }
    }
    for(int i = 0;i <= n + 1;++i){
        for(int j = 0;j <= m + 1;++j){
            canReach[i][j] = false;
        }
    }
    reachUpdate(0,0);
    selected_box1[0] = 0;
    selected_box1[1] = 0;
    box_flag1 = false;
    box_kind1 = 0;
    selected_box2[0] = 0;
    selected_box2[1] = 0;
    box_flag2 = false;
    box_kind2 = 0;
    if(!hint())
        gameOver(1);
    repaint();
}

// hint高亮方块的寻找逻辑
bool Gamescene::hint(){
    bool flag = false;
    for(int i = 1;i <= n;++i){
        for(int j = 1;j <= m;++j){
            if(0 < map[i][j] && map[i][j] <= boxNum){
                for(int k = i;k <= n;++k){
                    for(int l = (k == i ? j : 0);l <= m;++l){
                        if(map[i][j] != map[k][l])
                            continue;
                        if(isEliminated(i, k, j, l, false, 1)){
                            if((canReach[i - 1][j] || canReach[i + 1][j] || canReach[i][j - 1] || canReach[i][j + 1]) && (canReach[k - 1][l] || canReach[k + 1][l] || canReach[k][l - 1] || canReach[k][l + 1])){
                                hint_box[0][0] = i;
                                hint_box[0][1] = j;
                                hint_box[1][0] = k;
                                hint_box[1][1] = l;
                                flag = true;
                                break;
                            }
                        }
                    }
                    if(flag)
                        break;
                }
            }
            if(flag)
                break;
        }
        if(flag)
            break;
    }
    return flag;
}

// 更新能到达的位置
void Gamescene::reachUpdate(int x, int y){
    canReach[x][y] = true;
    queue.enqueue(QPoint(x, y));
    while(!queue.empty()){
        QPoint point1 = queue.dequeue();
        x = point1.x();
        y = point1.y();
        if(y != 0 && (map[x][y - 1] == 0 || map[x][y - 1] > boxNum) && canReach[x][y - 1] == false){
            canReach[x][y - 1] = true;
            queue.enqueue(QPoint(x, y - 1));
        }
        if(y != m + 1 && (map[x][y + 1] == 0 || map[x][y + 1] > boxNum) && canReach[x][y + 1] == false){
            canReach[x][y + 1] = true;
            queue.enqueue(QPoint(x, y + 1));
        }
        if(x != 0 && (map[x - 1][y] == 0 || map[x - 1][y] > boxNum) && canReach[x - 1][y] == false){
            canReach[x - 1][y] = true;
            queue.enqueue(QPoint(x - 1, y));
        }
        if(x != n + 1 && (map[x + 1][y] == 0 || map[x + 1][y] > boxNum) && canReach[x + 1][y] == false){
            canReach[x + 1][y] = true;
            queue.enqueue(QPoint(x + 1, y));
        }
    }
}

void Gamescene::countUpdate(){
    for(int i = 1;i <= boxNum;++i)
        count[i] = 0;
    for(int i = 0;i <= n + 1;++i){
        for(int j = 0;j <= m + 1;++j){
            if(map[i][j] > 0 && map[i][j] <= boxNum)
                ++count[map[i][j]];
            else
                continue;
        }
    }
}
