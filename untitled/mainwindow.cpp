#include "mainwindow.h"
#include <QCoreApplication>
#include <QPainter>
#include <QDebug>
//构造函数就写一个初始化函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //初始化
    init();
}
//默认析构
MainWindow::~MainWindow()
{
}
//初始化函数内容
void MainWindow::init()
{
    //初始化窗口大小
    this->setFixedSize(342,608);
    //初始化坐标以及相关数据
    mmX1=342;
    mmX2=342+270;
    androidX=0.5*(this->width());
    androidY=0.5*(608-42);
    score=0;
    isScoreMm1=isScoreMm2=false;
    //初始化背景
    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pix(":/back/images/1.png");
    pal.setBrush(QPalette::Background,QBrush(pix));
    setPalette(pal);
    //时间
    timer=new QTimer(this);
    //开始游戏
    startGame();
    //第一组棉花糖的随机数
    h1=GlobalUtils::getRandomNum(608-200-220);
    h2=GlobalUtils::getRandomNum(608-200-220);

    sunX=GlobalUtils::getRandomNum(20,322);
    sunY=GlobalUtils::getRandomNum(20,588);
    //山的数量，随机产生，上限为6
    mountainNumber=GlobalUtils::getRandomNum(6)+1;
    for(int i=1;i<=mountainNumber;i++)
    {
        mountainType[i]=GlobalUtils::getRandomNum(3);
        mountainSpeed[i]=GlobalUtils::getRandomNum((double)3.1);
        x[i]=GlobalUtils::getRandomNum(400);
        w[i]=GlobalUtils::getRandomNum(100,250);
        //初始化图像、宽度、横坐标，注意一定是先初始化宽度
        mountainInstance[i].setWidth(w[i]);
        mountainInstance[i].setMoun(mountainType[i]);
        mountainInstance[i].setX(x[i]);
    }

    //保持update
    connect(timer,SIGNAL(timeout()),this,SLOT(loopPaint()));
    //检测画面方向，每死一次改变一次方向，防止玩家视觉疲劳
    isFlipped=!isFlipped;
    initSpeed();
}

void MainWindow::drawMountain()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(342, 0, -342, 608);
    }
    for(int i=1;i<=mountainNumber;i++)
    {
        mountainInstance[i].draw(painter);
        mountainInstance[i].x-=mountainSpeed[i];
    }
    painter.setViewport(0, 0, 342, 608);
}

void MainWindow::drawSun()
{
    QPainter painter(this);
    //翻转画面
    if(isFlipped)
    {
        //线性代数知识，旋转矩阵
        painter.setViewport(342, 0, -342, 608);
    }
    QPixmap sun(":/back/images/sun.png");
    painter.drawPixmap(sunX,sunY,sun);
    //重置画面
    painter.setViewport(0, 0, 342, 608);
}
//鼠标点击一次刷新初速度
void MainWindow::mousePressEvent(QMouseEvent *)
{
    //记录鼠标按住的状态
    isPressed=true;
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    isPressed=false;
    update();
}
//绘制事件
void MainWindow::paintEvent(QPaintEvent *)
{
    //背景图片
    QPixmap pixmap=QPixmap(":/back/images/1.PNG").scaled(this->size());
    QPainter painterBackground(this);
    painterBackground.drawPixmap(this->rect(),pixmap);
    //画图，先画的显示在底层
    drawSun();
    drawMountain();
    drawAndroid();
    drawMm1();
    drawMm2();
    //屏幕正中间的分数图标
    drawScore();
}
//绘制事件中的绘制安卓小人
void MainWindow::drawAndroid()
{
    //鼠标按住时候的一系列事件
    if(isPressed)
    {
        androidStatus=AndroidStatus::UP;
        initSpeed();
        //如果游戏结束，重置游戏，可以改进
        if(gameStatus==STOPING)
            init();
        //旋转角度重置为45度
        imageAngle=45;
        //重置角加速度
        a=0;
    }
    //绘制事件
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(342, 0, -342, 608);
    }
    QPixmap android(":/back/images/androidBlue.png");
    android=android.scaled(42,42);
    a+=0.1;
    imageAngle+=a;

    if(imageAngle>180)
        imageAngle=180;

    painter.translate(androidX+21,androidY+21);
    painter.rotate(imageAngle);
    painter.drawPixmap(-21,-21,android);
    painter.resetTransform();
    //下降
    if(androidStatus==AndroidStatus::DOWN)
    {
        androidUpSpeed-=0.35;
        androidY-=androidUpSpeed;
    }
    //上升
    if(androidStatus == AndroidStatus::UP)
    {
        androidY-=androidUpSpeed;
        androidUpSpeed-=0.35;
        //速度降为0后开始下落
        if(androidUpSpeed<=0)
        {
            androidStatus=AndroidStatus::DOWN;
        }
    }
    //碰撞了就结束游戏
    if(isCrush())
        stopGame();
    //不能出天花板
    if(androidY<0)
        androidY=0;
    //重置
    painter.setViewport(0, 0, 342, 608);
}
//第一个循环棉花糖
void MainWindow::drawMm1()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(342, 0, -342, 608);
    }
    //上面的棉花糖
    QPixmap m1(":/back/images/m_platlogo.PNG");
    QMatrix matrix;
    matrix.rotate(180);
    m1=m1.transformed(matrix);
    m1=m1.scaled(100,100);
    if(gameStatus==RUNNING)
        mmX1-=0.8;
    //下面的棉花糖
    QPixmap m2(":/back/images/m_platlogo.PNG");
    m2=m2.scaled(100,100);
    if(gameStatus==RUNNING)
        mmX1-=0.8;
    //draw函数绘制杆子
    QPen pen;
    pen.setColor(QColor(182,163,157));
    painter.setPen(pen);
    QBrush brush(QColor(182,163,157),Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect((100-6)/2+mmX1,0,6,h1);
    painter.drawRect((100-6)/2+mmX1,h1+220+100,6,608-(h1+220-100));

    painter.drawPixmap(mmX1,h1,m1);
    painter.drawPixmap(mmX1,h1+220,m2);

    if(mmX2<-100)
    {
        h2=GlobalUtils::getRandomNum(608-200-220); //获取随机数
        mmX2=440;
        isScoreMm2=false;
    }

    if(mmX1+100<androidX+42 && !isScoreMm1)
    {
        score++;
        isScoreMm1=true;
    }
    painter.setViewport(0, 0, 342, 608);
}
//第二个棉花糖
void MainWindow::drawMm2()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(342, 0, -342, 608);
    }
    QPixmap m1(":/back/images/m_platlogo.PNG");
    QMatrix matrix;
    matrix.rotate(180);
    m1=m1.transformed(matrix);
    m1=m1.scaled(100,100);
    if(gameStatus==RUNNING)
        mmX2-=0.8;

    QPixmap m2(":/back/images/m_platlogo.PNG");
    m2=m2.scaled(100,100);
    if(gameStatus==RUNNING)
        mmX2-=0.8;
    //出屏之后重置
    if(mmX1<-100)
    {
        h1=GlobalUtils::getRandomNum(608-200-220); //获取随机数
        mmX1=440;
        isScoreMm1=false;
    }
    //draw函数绘制杆子
    QPen pen;
    pen.setColor(QColor(182,163,157));
    painter.setPen(pen);
    QBrush brush(QColor(182,163,157),Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect((100-6)/2+mmX2,0,6,h2);
    painter.drawRect((100-6)/2+mmX2,h2+220+100,6,608-(h2+220-100));
    //绘制棉花糖
    painter.drawPixmap(mmX2,h2,m1);
    painter.drawPixmap(mmX2,h2+220,m2);
    //增加分数
    if(mmX2+100<androidX+42 && !isScoreMm2)
    {
        score++;
        isScoreMm2=true;
    }
    painter.setViewport(0, 0, 342, 608);
}

void MainWindow::startGame()
{
    gameStatus=RUNNING;
    androidStatus=DOWN;
    androidUpSpeed=0;
    timer->start(16);
}

void MainWindow::stopGame()
{
    //一旦游戏结束，时间停止
    timer->stop();
    gameStatus=STOPING;
}

void MainWindow::drawScore()
{
    QColor color(65,105,225);
    QPainter painter(this);
    QPen pen(color, 10);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRectF((342-32)/2, 16, 32, 36), 0.5, 0.5);
    painter.fillPath(path, color);
    painter.drawPath(path);

    QFont font( "Microsoft YaHei", 13, 70);
    painter.setFont(font);

    painter.setPen(Qt::white);
    painter.drawText(QRectF((342-32)/2, 16, 32, 36),Qt::AlignCenter,QString::number(score));
}
//保持持续的paint事件
void MainWindow::loopPaint()
{
    update();
}
//向上的初速度
void MainWindow::initSpeed()
 {
     androidUpSpeed=6;
 }
//自己写碰撞检测
bool MainWindow::isCrush()
{
    if(androidY>608-42)
        return true;
    //与第一对循环棉花糖碰撞
    if(androidX+42>mmX1+10 && androidX<mmX1+90 && androidY+42>h1+20 && androidY<h1+70)
        return true;
    if(androidX+42>mmX1+10 && androidX<mmX1+90 && androidY+42>h1+220+30 && androidY<h1+220+70)
        return true;
    //第二对
    if(androidX+42>mmX2+10 && androidX<mmX2+90 && androidY+42>h2+20 && androidY<h2+70)
        return true;
    if(androidX+42>mmX2+10 && androidX<mmX2+90 && androidY+42>h2+220+30 && androidY<h2+220+70)
        return true;
    //第一对的杆子相碰
    if(androidX+42>mmX1+(100-10)/2 && androidX<mmX1+(100+10)/2 && androidY<h1)
        return true;
    if(androidX+42>mmX1+(100-10)/2 && androidX<mmX1+(100+10)/2 && androidY+42>h1+220+100)
        return true;
    //第二对
    if(androidX+42>mmX2+(100-10)/2 && androidX<mmX2+(100+10)/2 && androidY<h2)
        return true;
    if(androidX+42>mmX2+(100-10)/2 && androidX<mmX2+(100+10)/2 && androidY+42>h2+220+100)
        return true;
    return false;
}
