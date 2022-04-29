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
    h2=100;
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
    QPixmap pix(":/back/images/2.png");
    pal.setBrush(QPalette::Background,QBrush(pix));
    setPalette(pal);
    //时间
    timer=new QTimer(this);
    //开始游戏
    startGame();
    //第一组棉花糖的随机数
    uint seed_x=static_cast<uint>(clock());
    h1=GlobalUtils::getRandomNum(seed_x,608-200-220);
    //保持update
    connect(timer,SIGNAL(timeout()),this,SLOT(loopPaint()));

    initSpeed();
}
//鼠标点击一次刷新初速度
void MainWindow::mousePressEvent(QMouseEvent * )
{
     androidStatus=AndroidStatus::UP;
     initSpeed();
     if(gameStatus==STOPING)
         init();
     imageAngle=45;
     a=0;
}
//绘制事件
void MainWindow::paintEvent(QPaintEvent *)
{
    //背景图片
    QPixmap pixmap=QPixmap(":/back/images/1.PNG").scaled(this->size());
    QPainter painterBackground(this);
    painterBackground.drawPixmap(this->rect(),pixmap);
    //画图
    drawAndroid();
    drawMm1();
    drawMm2();
    drawScore();
}
//绘制事件中的绘制安卓小人
void MainWindow::drawAndroid()
{
    QPainter painter(this);
    QPixmap android(":/back/images/android.PNG");
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
}
//第一个循环棉花糖
void MainWindow::drawMm1()
{
    QPainter painter(this);
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
    pen.setColor(QColor(255,255,255));
    painter.setPen(pen);
    QBrush brush(QColor(255,255,255),Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect((100-6)/2+mmX1,0,6,h1);
    painter.drawRect((100-6)/2+mmX1,h1+220+100,6,608-(h1+220-100));

    painter.drawPixmap(mmX1,h1,m1);
    painter.drawPixmap(mmX1,h1+220,m2);

    if(mmX2<-100)
    {
        uint seed_x=static_cast<uint>(clock());
        h2=GlobalUtils::getRandomNum(seed_x,608-200-220); //获取随机数
        mmX2=440;
        isScoreMm2=false;
    }

    if(mmX1+100<androidX+42 && !isScoreMm1)
    {
        score++;
        isScoreMm1=true;
    }
}

//第二个棉花糖
void MainWindow::drawMm2()
{
    QPainter painter(this);
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
        uint seed_x=static_cast<uint>(clock());
        h1=GlobalUtils::getRandomNum(seed_x,608-200-220); //获取随机数
        mmX1=440;
        isScoreMm1=false;
    }
    //draw函数绘制杆子
    QPen pen;
    pen.setColor(QColor(255,255,255));
    painter.setPen(pen);
    QBrush brush(QColor(255,255,255),Qt::SolidPattern);
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

    QFont font( "Microsoft YaHei", 15, 70);
    painter.setFont(font);

    painter.setPen(Qt::white);
    painter.drawText(QRectF((342-32)/2, 16, 32, 36),Qt::AlignCenter,QString::number(score));
}

void MainWindow::loopPaint()
{
    update();
}

void MainWindow::initSpeed()
 {
     androidUpSpeed=7.5;
 }

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
    if(androidX+42>mmX2+10 && androidX<mmX2+90 && androidY+42>h2+220+30 && androidY<h2+2250+70)
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
