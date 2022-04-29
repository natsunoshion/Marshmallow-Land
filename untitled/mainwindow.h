#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "globalutils.h"
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QKeyEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    double androidX;
    double androidY;
    int score;
    int h1,h2;
    double mmX1;
    double mmX2;
    QMatrix matrix;
    //游戏状态
    enum GameStatus{STOPING=1, RUNNING};
    //飞行状态
    enum AndroidStatus{DEFAULT, UP, DOWN};
    GameStatus gameStatus = GameStatus::STOPING;
    AndroidStatus androidStatus = AndroidStatus::DOWN;
    double androidUpSpeed = 0;
    QTimer *timer=nullptr;
    bool isScoreMm1,isScoreMm2;
    double imageAngle=30;
    double a=0;

    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void init();
    void inith2();
    void drawMm1();
    void drawMm2();
    void drawAndroid();
    void initSpeed();
    bool isCrush();
    void startGame();
    void stopGame();
    void drawScore();

public slots:
    //循环绘制（定时器）
    void loopPaint();
};
#endif // MAINWINDOW_H
