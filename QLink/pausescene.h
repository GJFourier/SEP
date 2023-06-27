#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>

class Pausescene : public QMainWindow
{
    Q_OBJECT
public:
    explicit Pausescene(QWidget *parent = nullptr);
    QPushButton *continue_btn = nullptr;
    QPushButton *save_btn = nullptr;
    QPushButton *load_btn = nullptr;
    QPushButton *back_btn = nullptr;
    void closeEvent(QCloseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;

public slots:

signals:
    void closed();

};

#endif // PAUSESCENE_H
