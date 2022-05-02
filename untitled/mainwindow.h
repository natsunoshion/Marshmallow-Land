#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QKeyEvent>
#include "globalutils.h"
#include "mountain.h"
#include "cloud.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum mScene{DAY, NIGHT, TWILIGHT, SUNSET};
    //安卓横纵坐标
    double androidX;
    double androidY;
    //游戏分数，位于屏幕正上方
    int score;
    //两个杆子的参数
    int h1,h2;
    double mmX1;
    double mmX2;
    //普通的旋转角度用，也可以用painter的rotate代替
    QMatrix matrix;
    //游戏状态
    enum GameStatus{STOPING=1, RUNNING};
    //飞行状态
    enum AndroidStatus{DEFAULT, UP, DOWN};
    //默认的游戏状态和飞行状态
    GameStatus gameStatus = GameStatus::STOPING;
    AndroidStatus androidStatus = AndroidStatus::DOWN;
    //安卓竖直方向速度，默认向上为正
    double androidUpSpeed = 0;
    //时间
    QTimer *timer=nullptr;
    //记录分数
    bool isScoreMm1,isScoreMm2;
    //安卓小人旋转的角度
    double imageAngle=30;
    //角加速度
    double a=0;
    //鼠标按键检测，默认没按
    bool isPressed=false;
    bool isFlipped=true;
    //山的数量，默认为0
    int mountainNumber=0;
    //山对象数组
    mountain mountainInstance[100];
    int mountainType[100];
    double x[100];
    int w[100];
    double mountainSpeed[100];
    //太阳
    int sunX,sunY;
    //主题随机功能，尚未完成
    int themeColor;
    int themeItem;
    //云
    int cloudNumber=0;
    cloud cloudInstance[100];
    double xCloud[100];
    int yCloud[100];
    int widthCloud[100];
    double cloudSpeed[100];
    //棉花糖
    QPixmap m1Group1,m2Group1,m1Group2,m2Group2;
    //初始化棉花糖的触角朝向
    bool isInitMm11=true,isInitMm12=true,isInitMm21=true,isInitMm22=true;
    int test11,test12,test21,test22;

    //函数部分开始
    //鼠标按下
    void mousePressEvent(QMouseEvent *event);
    //鼠标松开
    void mouseReleaseEvent(QMouseEvent *event);
    //绘制事件
    void paintEvent(QPaintEvent *event);

    void drawBack();
    //初始化
    void init();
    //绘制里的事件，分别paint
    //山
    void drawMountain();
    //云
    void drawCloud();
    //太阳
    void drawSun();
    //棉花糖
    void drawMm1();
    void drawMm2();
    //安卓
    void drawAndroid();
    //记分板
    void drawScore();
    //初速度
    void initSpeed();
    //碰撞检测
    bool isCrush();
    //开始游戏
    void startGame();
    //结束游戏
    void stopGame();

public slots:
    //循环绘制（定时器）
    void loopPaint();
};
#endif // MAINWINDOW_H
