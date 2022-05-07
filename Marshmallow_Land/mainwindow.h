#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QKeyEvent>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include "globalutils.h"
#include "mountain.h"
#include "cloud.h"
#include "cactus.h"
#include "building.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPushButton * play;
    bool startAndroid=false,startMm=false;

    //主题
    int mTimeOfDay;
    int mScene;
    int DAY=0, NIGHT=1, TWILIGHT=2, SUNSET=3;
    int SCENE_ZRH=0,SCENE_TX=1,SCENE_CITY=2;
    bool isMountain=0,isCactus=0,isBuilding=0;
    bool isSun=0,isMoon=0,isStar=0,isCloud=0;

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
    double imageAngle=45;

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
    int sunOrMoonX,sunOrMoonY;

    //随机主题功能
    int themeColor;
    int Theme_Blue=0,Theme_Yellow=1,Theme_Green=2,Theme_Purple=3,Theme_Grey=4,Theme_Red=5;

    //仙人掌
    int cactusNumber=0;
    int cactusType[100];
    cactus cactusInstance[100];
    double xCactus[100];
    int widthCactus[100];
    double cactusSpeed[100];

    //建筑物
    int buildingNumber=0;
    building buildingInstance[100];
    int heightBuilding[100];
    double xBuilding[100];
    int widthBuilding[100];
    double buildingSpeed[100];

    //云
    int cloudNumber=0;
    cloud cloudInstance[100];
    double xCloud[100];
    int yCloud[100];
    int widthCloud[100];
    double cloudSpeed[100];

    //星星的数量，这里偷个懒，没有新建类
    int starNumber=0;
    int starX[100],starY[100];

    //棉花糖
    QPixmap m1Group1,m2Group1,m1Group2,m2Group2;

    //物体的重置
    int isReset=true;

    //开始倒计时
    bool isStartCount=false;

    //倒计时
    int countNumber=3;

    //表情
    QPixmap mouth11,mouth12,mouth21,mouth22 ,eyes11,eyes12,eyes21,eyes22;

    //初始化棉花糖的触角朝向
    bool isInitMm11=true,isInitMm12=true,isInitMm21=true,isInitMm22=true;

    //触须种类
    int test11,test12,test21,test22;
    int isMouth11,isMouth12,isMouth21,isMouth22;
    int isEyes11,isEyes12,isEyes21,isEyes22;

    //嘴巴种类
    int mouthType11,mouthType12,mouthType21,mouthType22;

    //眼睛种类
    int eyesType11,eyesType12,eyesType21,eyesType22;
    bool isFirstInit=true;


    //函数部分开始
    //鼠标按下
    void mousePressEvent(QMouseEvent *event);

    //鼠标松开
    void mouseReleaseEvent(QMouseEvent *event);

    //绘制事件
    void paintEvent(QPaintEvent *event);

    //背景
    void drawBack();

    //数位计算
    int getDigit(int x)
    {
        if(x==0)
            return 1;
        int a;
        for(a=0;;)
        {
            if(x>0)
                a++;
            x/=10;
            if(x==0)
                break;
        }
        return a;
    }

    //初始化
    void init();

    //绘制里的事件，分别paint
    //山
    void drawMountain();

    //仙人掌
    void drawCactus();

    //云
    void drawCloud();

    //星星
    void drawStar();

    //太阳
    void drawSun();

    //月亮
    void drawMoon();

    //建筑物
    void drawBuilding();

    //棉花糖
    void drawMm1();
    void drawMm2();

    //安卓
    void drawAndroid();

    //暂停键的白色圈
    void drawCircle();

    //暂停时的阴影
    void drawShadowPause();

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

    //真正意义上的开始游戏
    void initAndroid();
    void initMm();

    //倒计时
    void drawCountNumber();

public slots:
    //循环绘制（定时器）
    void loopPaint();
    void hideButton();

    //3 2 1开始游戏函数
    void startCount();
};
#endif // MAINWINDOW_H
