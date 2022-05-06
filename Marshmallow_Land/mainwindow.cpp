#include "mainwindow.h"
#include <QCoreApplication>
#include <QPainter>
#include <QDebug>
//构造函数就写一个初始化函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //每次打开游戏，主题颜色只生成一次，之后不会变化
    themeColor=GlobalUtils::getRandomNum(6);
    //游戏一打开，播放音乐
    QMediaPlaylist *musicList = new QMediaPlaylist(this);  //添加音乐列表
    musicList->addMedia(QUrl("qrc:/data/music/The History.mp3"));  //添加音乐列表

    QMediaPlayer *soundPlayer = new QMediaPlayer(this);  //创建音乐播放器
    soundPlayer->setPlaylist(musicList);  //设置音乐列表
    soundPlayer->play();
    musicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);  //循环播放
    //这样就可以单首音乐循环播放
    //初始化
    init();
}
//析构
MainWindow::~MainWindow()
{
    delete play;
}
//初始化函数内容
void MainWindow::init()
{
    //播放按钮
    play=new QPushButton(this);
    play->setFixedSize(48,48);
    QPixmap iconPixmap(":/back/images/play.png");
    QIcon icon(iconPixmap);
    play->setIconSize(QSize(48, 48));
    play->setStyleSheet("QPushButton{color:white; background-color:transparent;}");
    //设置图标
    play->setIcon(icon);
    play->setLayoutDirection(Qt::LeftToRight);
    play->setFlat(true);
    //移动到中心
    play->move((360-48)/2,(780-48)/2);
    //生成主题
    //时间
    mTimeOfDay=GlobalUtils::getRandomNum(4);
    //场景
    mScene=GlobalUtils::getRandomNum(4);
    isMoon=GlobalUtils::getRandomNum(2);
    isSun=GlobalUtils::getRandomNum(2);
    isStar=GlobalUtils::getRandomNum(2);
    isCloud=GlobalUtils::getRandomNum(2);
    //白天没有星星，但是可以有月亮（咦？
    if(mTimeOfDay==DAY)
        isStar=false;
    //晚上没有太阳
    if(mTimeOfDay==TWILIGHT || mTimeOfDay==NIGHT)
        isSun=false;
    //太阳和月亮不同时出现
    if(isSun)
        isMoon=false;
    //深夜一定有星星
    if(mTimeOfDay==NIGHT)
        isStar=true;
    if(mScene==SCENE_ZRH)
    {
        isMountain=true;
        //清除上次的状态
        isCactus=false;
        isBuilding=false;
    }
    if(mScene==SCENE_TX)
    {
        isCactus=true;
        isMountain=false;
        isBuilding=false;
    }
    if(mScene==SCENE_CITY)
    {
        isBuilding=true;
        isCactus=false;
        isMountain=false;
    }
    //初始化窗口大小
    this->setFixedSize(360,780);
    //初始化坐标以及相关数据
    mmX1=360;
    mmX2=360+301;
    androidX=0.5*(this->width());
    androidY=0.5*(780-42);
    score=0;
    isScoreMm1=isScoreMm2=false;
    isInitMm11=isInitMm12=isInitMm21=isInitMm22=true;
    //时间
    timer=new QTimer(this);
    //开始游戏
    startGame();
    //第一组棉花糖的随机数
    h1=GlobalUtils::getRandomNum(780-200-220);
    h2=GlobalUtils::getRandomNum(780-200-220);
    //初始星星坐标
    //星星一般不会太少
    starNumber=GlobalUtils::getRandomNum(3,10);
    for(int i=1;i<=starNumber;i++)
    {
        starX[i]=GlobalUtils::getRandomNum(360);
        starY[i]=GlobalUtils::getRandomNum(780);
    }
    //初始化太阳/月亮坐标
    sunOrMoonX=GlobalUtils::getRandomNum(20,340);
    sunOrMoonY=GlobalUtils::getRandomNum(20,760);
    //初始化山
    //山的数量，随机产生，上限为8
    mountainNumber=GlobalUtils::getRandomNum(6)+3;
    for(int i=1;i<=mountainNumber;i++)
    {
        mountainType[i]=GlobalUtils::getRandomNum(3);
        mountainSpeed[i]=GlobalUtils::getRandomNum((double)1.7);
        x[i]=GlobalUtils::getRandomNum(720);
        w[i]=GlobalUtils::getRandomNum(100,300);
        //初始化图像、宽度、横坐标，注意一定是先初始化宽度
        mountainInstance[i].setWidth(w[i]);
        mountainInstance[i].setMoun(mountainType[i]);
        mountainInstance[i].setX(x[i]);
    }
    //初始化云
    //云不会很多
    cloudNumber=GlobalUtils::getRandomNum(12)+2;
    for(int i=1;i<=cloudNumber;i++)
    {
        cloudSpeed[i]=GlobalUtils::getRandomNum((double)0.8)+0.5;
        xCloud[i]=GlobalUtils::getRandomNum(720);
        //云不能太低
        yCloud[i]=GlobalUtils::getRandomNum(780/3*2);
        widthCloud[i]=GlobalUtils::getRandomNum(50,100);
        cloudInstance[i].setX(xCloud[i]);
        cloudInstance[i].setY(yCloud[i]);
        cloudInstance[i].setWidth(widthCloud[i]);
        cloudInstance[i].setCloud();
    }
    //初始化仙人掌
    //仙人掌比较多
    cactusNumber=GlobalUtils::getRandomNum(7,10);
    for(int i=1;i<=cactusNumber;i++)
    {
        cactusType[i]=GlobalUtils::getRandomNum(3);
        cactusSpeed[i]=GlobalUtils::getRandomNum((double)1.7)+0.1;
        xCactus[i]=GlobalUtils::getRandomNum(720);
        //仙人掌比较小
        widthCactus[i]=GlobalUtils::getRandomNum(70,150);
        //初始化图像、宽度、横坐标，注意一定是先初始化宽度
        cactusInstance[i].setWidth(widthCactus[i]);
        cactusInstance[i].setCact(cactusType[i]);
        cactusInstance[i].setX(xCactus[i]);
    }
    //初始化建筑物
    buildingNumber=GlobalUtils::getRandomNum(13,17);
    for(int i=1;i<=buildingNumber;i++)
    {
        buildingSpeed[i]=GlobalUtils::getRandomNum((double)1.7)+0.1;
        xBuilding[i]=GlobalUtils::getRandomNum(720);
        widthBuilding[i]=GlobalUtils::getRandomNum(60,240);
        heightBuilding[i]=GlobalUtils::getRandomNum(120);

        buildingInstance[i].setWidth(widthBuilding[i]);
        buildingInstance[i].setHeight(heightBuilding[i]);
        buildingInstance[i].setX(xBuilding[i]);
    }
    //保持update
    connect(timer,SIGNAL(timeout()),this,SLOT(loopPaint()));
    //按下按键开始游戏
    connect(play,SIGNAL(clicked()),this,SLOT(startCount()));
    connect(play,SIGNAL(clicked()),this,SLOT(hideButton()));
    //检测画面方向，每死一次改变一次方向，防止玩家视觉疲劳
    isFlipped=!isFlipped;
    //开局先跳一下
    initSpeed();
}
//山
void MainWindow::drawMountain()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(360, 0, -360, 780);
    }
    for(int i=1;i<=mountainNumber;i++)
    {
        mountainInstance[i].draw(painter);
        mountainInstance[i].drawShadow(painter,(double)(1.0-(double)(i-1)/(double)(mountainNumber-1.0)));
        if(gameStatus==RUNNING)
            mountainInstance[i].x-=mountainSpeed[i];
        //重置透明度，防止下次实例被设置为透明
        painter.setOpacity(1.0);

        if(isReset)
        {
            if(mountainInstance[i].x<-mountainInstance[i].width)
            {
                mountainType[i]=GlobalUtils::getRandomNum(3);
                mountainSpeed[i]=GlobalUtils::getRandomNum((double)1.7);
                x[i]=GlobalUtils::getRandomNum(360,720);
                w[i]=GlobalUtils::getRandomNum(100,300);
                //初始化图像、宽度、横坐标，注意一定是先初始化宽度
                mountainInstance[i].setWidth(w[i]);
                mountainInstance[i].setMoun(mountainType[i]);
                mountainInstance[i].setX(x[i]);
            }
        }
    }
    painter.setViewport(0, 0, 360, 780);
}
//仙人掌
void MainWindow::drawCactus()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(360, 0, -360, 780);
    }
    for(int i=1;i<=cactusNumber;i++)
    {
        cactusInstance[i].draw(painter);
        cactusInstance[i].drawShadow(painter,(double)(1.0-(double)(i-1.0)/(double)(cactusNumber-1.0)));
        if(gameStatus==RUNNING)
            cactusInstance[i].x-=cactusSpeed[i];
        painter.setOpacity(1.0);

        if(isReset)
        {
            if(cactusInstance[i].x<-cactusInstance[i].width)
            {
                cactusType[i]=GlobalUtils::getRandomNum(3);
                cactusSpeed[i]=GlobalUtils::getRandomNum((double)1.7)+0.1;
                xCactus[i]=GlobalUtils::getRandomNum(360,720);
                //仙人掌比较小
                widthCactus[i]=GlobalUtils::getRandomNum(70,150);
                //初始化图像、宽度、横坐标，注意一定是先初始化宽度
                cactusInstance[i].setWidth(widthCactus[i]);
                cactusInstance[i].setCact(cactusType[i]);
                cactusInstance[i].setX(xCactus[i]);
            }
        }
    }
    painter.setViewport(0, 0, 360, 780);
}
//云
void MainWindow::drawCloud()
{
    QPainter painter(this);
    //设置透明度
    painter.setOpacity(0.5);
    if(isFlipped)
    {
        painter.setViewport(360, 0, -360, 780);
    }
    for(int i=1;i<=cloudNumber;i++)
    {
        cloudInstance[i].draw(painter);
        if(gameStatus==RUNNING)
            cloudInstance[i].x-=cloudSpeed[i];

        if(isReset)
        {
            if(cloudInstance[i].x<-cloudInstance[i].width)
            {
                cloudSpeed[i]=GlobalUtils::getRandomNum((double)0.8)+0.5;
                //在外边重置
                xCloud[i]=GlobalUtils::getRandomNum(360,720);
                //云不能太低
                yCloud[i]=GlobalUtils::getRandomNum(780/3*2);
                widthCloud[i]=GlobalUtils::getRandomNum(50,100);
                cloudInstance[i].setX(xCloud[i]);
                cloudInstance[i].setY(yCloud[i]);
                cloudInstance[i].setWidth(widthCloud[i]);
                cloudInstance[i].setCloud();
            }
        }
    }
    painter.setViewport(0, 0, 360, 780);
}

void MainWindow::drawStar()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(360, 0, -360, 780);
    }
    for(int i=1;i<=starNumber;i++)
    {
        QPixmap star(":/back/images/star.png");
        star=star.scaled(5,5);
        painter.drawPixmap(starX[i],starY[i],star);
    }
    painter.setViewport(0, 0, 360, 780);
}

void MainWindow::drawSun()
{
    QPainter painter(this);
    //翻转画面
    if(isFlipped)
    {
        //线性代数知识，翻转矩阵
        painter.setViewport(360, 0, -360, 780);
    }
    QPixmap sun;
    if(mTimeOfDay==SUNSET)
        sun.load(":/back/images/sunset.png");
    else
        sun.load(":/back/images/sun.png");
    painter.drawPixmap(sunOrMoonX,sunOrMoonY,sun);
    //重置画面
    painter.setViewport(0, 0, 360, 780);
}

void MainWindow::drawMoon()
{
    QPainter painter(this);
    //如果是在白天或者日落，减小月亮的透明度
    if(mTimeOfDay==DAY || mTimeOfDay==SUNSET)
        painter.setOpacity(0.75);
    //翻转画面
    if(isFlipped)
    {
        //线性代数知识，翻转矩阵
        painter.setViewport(360, 0, -360, 780);
    }
    QPixmap moon(":/back/images/moon.png");
    painter.drawPixmap(sunOrMoonX,sunOrMoonY,moon);
    //重置画面
    painter.setViewport(0, 0, 360, 780);
}

void MainWindow::drawBuilding()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(360, 0, -360, 780);
    }
    for(int i=1;i<=buildingNumber;i++)
    {
        buildingInstance[i].draw(painter);
        buildingInstance[i].drawShadow(painter,(double)(1.0-(double)(i-1.0)/(double)(buildingNumber-1.0)));
        if(gameStatus==RUNNING)
            buildingInstance[i].x-=buildingSpeed[i];

        if(isReset)
        {
            if(buildingInstance[i].x<-buildingInstance[i].width)
            {
                buildingSpeed[i]=GlobalUtils::getRandomNum((double)1.7)+0.1;
                xBuilding[i]=GlobalUtils::getRandomNum(360,720);
                //仙人掌比较小
                widthBuilding[i]=GlobalUtils::getRandomNum(70,150);
                //初始化图像、宽度、横坐标，注意一定是先初始化宽度
                buildingInstance[i].setWidth(widthBuilding[i]);
                buildingInstance[i].setX(xBuilding[i]);
            }
        }
    }
    painter.setViewport(0, 0, 360, 780);
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
    //画图，先画的显示在底层
    drawBack();
    //画不画太阳
    if(isSun)
        drawSun();
    //画不画月亮
    if(isMoon)
        drawMoon();
    //画！
    if(isStar)
        drawStar();
    if(isMountain)
        drawMountain();
    if(isCactus)
        drawCactus();
    if(isBuilding)
        drawBuilding();
    if(isCloud)
        drawCloud();
    if(startAndroid)
        drawAndroid();
    if(startMm)
    {
        drawMm1();
        drawMm2();
    }
    if(!startAndroid)
    {
        drawShadowPause();
        drawCircle();
    }
    drawCountNumber();
    //屏幕正中间的分数图标
    drawScore();
}
//直接暴力用draw函数画渐变背景
void MainWindow::drawBack()
{
    QPainter painter(this);
    //白天
    if(mTimeOfDay==DAY)
    {
        QLinearGradient linearGrad(QPointF(180, 0), QPointF(180, 780));
        linearGrad.setColorAt(0, QColor(160,161,254));
        linearGrad.setColorAt(1, QColor(192,192,254));
        QBrush brush(linearGrad);
        painter.setBrush(brush);
        //直接画矩形填充渐变背景
        painter.drawRect(-1,-1,361,781);
    }
    if(mTimeOfDay==SUNSET)
    {
        QLinearGradient linearGrad(QPointF(180, 0), QPointF(180, 780));
        linearGrad.setColorAt(0, QColor(33,64,128));
        linearGrad.setColorAt(1, QColor(161,128,33));
        QBrush brush(linearGrad);
        painter.setBrush(brush);
        //直接画矩形填充渐变背景
        painter.drawRect(-1,-1,361,781);
    }
    if(mTimeOfDay==TWILIGHT)
    {
        QLinearGradient linearGrad(QPointF(180, 0), QPointF(180, 780));
        linearGrad.setColorAt(0, QColor(0,0,0));
        linearGrad.setColorAt(1, QColor(1,0,16));
        QBrush brush(linearGrad);
        painter.setBrush(brush);
        //直接画矩形填充渐变背景
        painter.drawRect(-1,-1,361,781);
    }
    if(mTimeOfDay==NIGHT)
    {
        QLinearGradient linearGrad(QPointF(180, 0), QPointF(180, 780));
        linearGrad.setColorAt(0, QColor(1,0,16));
        linearGrad.setColorAt(1, QColor(0,0,62));
        QBrush brush(linearGrad);
        painter.setBrush(brush);
        //直接画矩形填充渐变背景
        painter.drawRect(-1,-1,361,781);
    }

}
//绘制事件中的绘制安卓小人
void MainWindow::drawAndroid()
{
    //鼠标按住时候的一系列事件
    if(isPressed)
    {
        androidStatus=AndroidStatus::UP;
        initSpeed();
        //保持能够按鼠标复活
        if(gameStatus==GameStatus::STOPING)
            startCount();
        //旋转角度重置为45度
        imageAngle=45;
    }
    //绘制事件
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(360, 0, -360, 780);
    }
    QPixmap android;
    if(themeColor==Theme_Blue)
        android.load(":/back/images/androidBlue.png");
    if(themeColor==Theme_Yellow)
        android.load(":/back/images/androidYellow.png");
    if(themeColor==Theme_Green)
        android.load(":/back/images/androidGreen.png");
    if(themeColor==Theme_Purple)
        android.load(":/back/images/androidPurple.png");
    if(themeColor==Theme_Grey)
        android.load(":/back/images/androidGrey.png");
    if(themeColor==Theme_Red)
        android.load(":/back/images/androidRed.png");
    android=android.scaled(42,42);

    //下降
    if(androidStatus==AndroidStatus::DOWN && gameStatus==RUNNING)
    {
        androidY-=androidUpSpeed;
        androidUpSpeed-=0.375;
        if(androidY>780-42)
            androidY=780-42;
    }
    //上升
    if(androidStatus == AndroidStatus::UP && gameStatus==RUNNING)
    {
        androidY-=androidUpSpeed;
        androidUpSpeed-=0.375;
        //速度降为0后开始下落
        if(androidUpSpeed<=0)
        {
            androidStatus=AndroidStatus::DOWN;
        }
    }
    if(gameStatus==RUNNING)
        imageAngle+=2.5;

    if(imageAngle>180)
        imageAngle=180;

    painter.translate(androidX+21,androidY+21);
    painter.rotate(imageAngle);
    painter.drawPixmap(-21,-21,android);
    painter.resetTransform();

    //碰撞了就结束游戏
    if(isCrush())
        stopGame();
    //不能出天花板
    if(androidY<0)
        androidY=0;
    //重置
    painter.setViewport(0, 0, 360, 780);
}

void MainWindow::drawCircle()
{
    QPainter painter(this);
    QPainterPath path;
    path.addEllipse(QRect((360-72)/2,(780-72)/2,72,72));
    QBrush brush(QColor(170,170,170));
    painter.fillPath(path,brush);
}

void MainWindow::drawShadowPause()
{
    QPainter painter(this);
    painter.setOpacity(0.63);
    QPixmap shadow(":/back/images/shadow.png");
    painter.drawPixmap(0,0,shadow);
    //重置painter
    painter.setOpacity(1.0);
}
//第一个循环棉花糖
void MainWindow::drawMm1()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(360, 0, -360, 780);
    }
    //上面的棉花糖
    //随机生成两种触须的一种
    if(isInitMm11)
    {
        test11=GlobalUtils::getRandomNum(2);

        isMouth11=GlobalUtils::getRandomNum(2);
        isEyes11=GlobalUtils::getRandomNum(2);
        //有眼睛才能有嘴巴
        if(!isEyes11)
            isMouth11=false;
        //嘴巴
        if(isMouth11)
        {
            //四种嘴巴
            mouthType11=GlobalUtils::getRandomNum(4);
        }
        if(isEyes11)
        {
            //两种眼睛
            eyesType11=GlobalUtils::getRandomNum(2);
        }

        //触须
        if(test11==0)
        {
            m1Group1.load(":/back/images/mm1.png");
            QMatrix matrix;
            matrix.rotate(180);
            m1Group1=m1Group1.transformed(matrix);
            m1Group1=m1Group1.scaled(88,104);
        }
        if(test11==1)
        {
            m1Group1.load(":/back/images/mm2.png");
            QMatrix matrix;
            matrix.rotate(180);
            m1Group1=m1Group1.transformed(matrix);
            m1Group1=m1Group1.scaled(88,98);
        }
        isInitMm11=false;
    }

    if(gameStatus==RUNNING)
        mmX1-=0.8;
    //下面的棉花糖
    //控制下面的函数只执行一次
    if(isInitMm12)
    {
        //哪种触须
        test12=GlobalUtils::getRandomNum(2);

        isMouth12=GlobalUtils::getRandomNum(2);
        isEyes12=GlobalUtils::getRandomNum(2);
        if(!isEyes12)
            isMouth12=false;
        //嘴巴
        if(isMouth12)
        {
            //四种嘴巴
            mouthType12=GlobalUtils::getRandomNum(4);
        }
        if(isEyes12)
        {
            //两种眼睛
            eyesType12=GlobalUtils::getRandomNum(2);
        }

        //设计触须
        if(test12==0)
        {
            m2Group1.load(":/back/images/mm1.png");
            m2Group1=m2Group1.scaled(88,104);
        }
        if(test12==1)
        {
            m2Group1.load(":/back/images/mm2.png");
            m2Group1=m2Group1.scaled(88,98);
        }
        isInitMm12=false;
    }
    if(gameStatus==RUNNING)
        mmX1-=0.8;
    //draw函数绘制杆子
    //设置透明边框
    painter.setPen(Qt::transparent);
    //构造线性渐变填充杆子，先画的在下面
    QLinearGradient linearGradient(QPointF((88-8)/2+mmX1, 0), QPointF((88+8)/2+mmX1, 0));
    linearGradient.setColorAt(0, QColor(179,166,175));
    linearGradient.setColorAt(1, QColor(158,135,127));
    QBrush brush(linearGradient);
    painter.setBrush(brush);
    painter.drawRect((88-8)/2+mmX1,0,8,h1);
    painter.drawRect((88-8)/2+mmX1,h1+250+88,8,780-(h1+250-88));
    //棉花糖
    painter.drawPixmap(mmX1,h1,m1Group1);
    painter.drawPixmap(mmX1,h1+250,m2Group1);

    //杆子阴影
    QPainterPath path;
    QPolygon polygon = QPolygon();
    //梯形
    if(test12==0)
        polygon << QPoint((88-8)/2+mmX1,h1+250+104)<<QPoint((88-8)/2+mmX1+8,h1+250+104)<<QPoint((88-8)/2+mmX1+8,h1+250+104+19)<<QPoint((88-8)/2+mmX1,h1+250+104+8);
    else
        polygon << QPoint((88-8)/2+mmX1,h1+250+98)<<QPoint((88-8)/2+mmX1+8,h1+250+98)<<QPoint((88-8)/2+mmX1+8,h1+250+98+19)<<QPoint((88-8)/2+mmX1,h1+250+98+8);
    path.addPolygon(polygon);
    path.closeSubpath();

    QLinearGradient linear(QPointF((88-8)/2+mmX1, 0), QPointF((88+8)/2+mmX1, 0));
    linear.setColorAt(1, QColor(139,119,110));
    linear.setColorAt(0, QColor(136,120,120));
    QBrush bru(linear);
    painter.setBrush(bru);
    painter.fillPath(path, bru);

    //后画眼睛嘴巴，防止被遮挡
    if(isEyes11)
    {
        //第一种眼睛
        if(eyesType11==0)
        {
            eyes11.load(":/back/images/mm_eyes.PNG");
            eyes11=eyes11.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            eyes11=eyes11.transformed(m);
            //参数
            painter.drawPixmap(mmX1,h1,eyes11);
        }
        //第二种
        if(eyesType11==1)
        {
            eyes11.load(":/back/images/mm_eyes2.PNG");
            eyes11=eyes11.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            eyes11=eyes11.transformed(m);
            //参数
            painter.drawPixmap(mmX1,h1,eyes11);
        }
    }
    if(isMouth11)
    {
        if(mouthType11==0)
        {
            mouth11.load(":/back/images/mm_mouth1.PNG");
            mouth11=mouth11.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            mouth11=mouth11.transformed(m);
            //参数
            painter.drawPixmap(mmX1,h1,mouth11);
        }
        if(mouthType11==1)
        {
            mouth11.load(":/back/images/mm_mouth2.PNG");
            mouth11=mouth11.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            mouth11=mouth11.transformed(m);
            //参数
            painter.drawPixmap(mmX1,h1,mouth11);
        }
        if(mouthType11==2)
        {
            mouth11.load(":/back/images/mm_mouth3.png");
            mouth11=mouth11.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            mouth11=mouth11.transformed(m);
            //参数
            painter.drawPixmap(mmX1,h1,mouth11);
        }
        if(mouthType11==3)
        {
            mouth11.load(":/back/images/mm_mouth4.png");
            mouth11=mouth11.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            mouth11=mouth11.transformed(m);
            //参数
            painter.drawPixmap(mmX1,h1,mouth11);
        }
    }
    //下面的
    if(isEyes12)
    {
        //第一种眼睛
        if(eyesType12==0)
        {
            eyes12.load(":/back/images/mm_eyes.PNG");
            eyes12=eyes12.scaled(88,88);
            //参数
            if(test12==0)
                painter.drawPixmap(mmX1,h1+250+16,eyes12);
            else
                painter.drawPixmap(mmX1,h1+250+10,eyes12);
        }
        //第二种
        if(eyesType12==1)
        {
            eyes12.load(":/back/images/mm_eyes2.PNG");
            eyes12=eyes12.scaled(88,88);
            //参数
            if(test12==0)
                painter.drawPixmap(mmX1,h1+250+16,eyes12);
            else
                painter.drawPixmap(mmX1,h1+250+10,eyes12);
        }
    }
    if(isMouth12)
    {
        if(mouthType12==0)
        {
            mouth12.load(":/back/images/mm_mouth1.PNG");
            mouth12=mouth12.scaled(88,88);
            //参数
            if(test12==0)
                painter.drawPixmap(mmX1,h1+250+16,mouth12);
            else
                painter.drawPixmap(mmX1,h1+250+10,mouth12);
        }
        if(mouthType12==1)
        {
            mouth12.load(":/back/images/mm_mouth2.PNG");
            mouth12=mouth12.scaled(88,88);
            //参数
            if(test12==0)
                painter.drawPixmap(mmX1,h1+250+16,mouth12);
            else
                painter.drawPixmap(mmX1,h1+250+10,mouth12);
        }
        if(mouthType12==2)
        {
            mouth12.load(":/back/images/mm_mouth3.PNG");
            mouth12=mouth12.scaled(88,88);
            //参数
            if(test12==0)
                painter.drawPixmap(mmX1,h1+250+16,mouth12);
            else
                painter.drawPixmap(mmX1,h1+250+10,mouth12);
        }
        if(mouthType12==3)
        {
            mouth12.load(":/back/images/mm_mouth4.PNG");
            mouth12=mouth12.scaled(88,88);
            //参数
            if(test12==0)
                painter.drawPixmap(mmX1,h1+250+16,mouth12);
            else
                painter.drawPixmap(mmX1,h1+250+10,mouth12);
        }
    }

    if(mmX2<-88)
    {
        h2=GlobalUtils::getRandomNum(780-200-250); //获取随机数
        mmX2=mmX1+301;
        isScoreMm2=false;
        isInitMm21=isInitMm22=true;
    }

    if(mmX1+88<androidX+42 && !isScoreMm1)
    {
        score++;
        isScoreMm1=true;
    }
    painter.setViewport(0, 0, 360, 780);
}
//第二个棉花糖
void MainWindow::drawMm2()
{
    QPainter painter(this);
    if(isFlipped)
    {
        painter.setViewport(360, 0, -360, 780);
    }
    if(isInitMm21)
    {
        test21=GlobalUtils::getRandomNum(2);

        isMouth21=GlobalUtils::getRandomNum(2);
        isEyes21=GlobalUtils::getRandomNum(2);
        if(!isEyes21)
            isMouth21=false;
        //嘴巴
        if(isMouth21)
        {
            //四种嘴巴
            mouthType21=GlobalUtils::getRandomNum(4);
        }
        if(isEyes21)
        {
            //两种眼睛
            eyesType21=GlobalUtils::getRandomNum(2);
        }

        if(test21==0)
        {
            m1Group2.load(":/back/images/mm1.png");
            QMatrix matrix;
            matrix.rotate(180);
            m1Group2=m1Group2.transformed(matrix);
            m1Group2=m1Group2.scaled(88,104);
        }
        if(test21==1)
        {
            m1Group2.load(":/back/images/mm2.png");
            QMatrix matrix;
            matrix.rotate(180);
            m1Group2=m1Group2.transformed(matrix);
            m1Group2=m1Group2.scaled(88,98);
        }
        isInitMm21=false;
    }
    if(gameStatus==RUNNING)
        mmX2-=0.8;

    if(isInitMm22)
    {
        test22=GlobalUtils::getRandomNum(2);

        isMouth22=GlobalUtils::getRandomNum(2);
        isEyes22=GlobalUtils::getRandomNum(2);
        if(!isEyes22)
            isMouth22=false;
        //嘴巴
        if(isMouth22)
        {
            //四种嘴巴
            mouthType22=GlobalUtils::getRandomNum(4);
        }
        if(isEyes22)
        {
            //两种眼睛
            eyesType22=GlobalUtils::getRandomNum(2);
        }

        if(test22==0)
        {
            m2Group2.load(":/back/images/mm1.png");
            m2Group2=m2Group2.scaled(88,104);
        }
        if(test22==1)
        {
            m2Group2.load(":/back/images/mm2.png");
            m2Group2=m2Group2.scaled(88,98);
        }
        isInitMm22=false;
    }
    if(gameStatus==RUNNING)
        mmX2-=0.8;
    //出屏之后重置
    if(mmX1<-88)
    {
        h1=GlobalUtils::getRandomNum(780-200-250); //获取随机数
        mmX1=mmX2+301;
        isScoreMm1=false;
        isInitMm11=isInitMm12=true;
    }

    //draw函数绘制杆子
    //设置透明边框
    painter.setPen(Qt::transparent);
    //构造线性渐变填充杆子
    QLinearGradient linearGradient(QPointF((88-8)/2+mmX2, 0), QPointF((88+8)/2+mmX2, 0));
    linearGradient.setColorAt(0, QColor(179,166,175));
    linearGradient.setColorAt(1, QColor(158,135,127));
    QBrush brush(linearGradient);
    painter.setBrush(brush);

    painter.drawRect((88-8)/2+mmX2,0,8,h2);
    painter.drawRect((88-8)/2+mmX2,h2+250+88,8,780-(h2+250-88));
    //绘制棉花糖
    painter.drawPixmap(mmX2,h2,m1Group2);
    painter.drawPixmap(mmX2,h2+250,m2Group2);

    //杆子阴影
    QPainterPath path;
    QPolygon polygon = QPolygon();
    //梯形
    if(test22==0)
        polygon << QPoint((88-8)/2+mmX2,h2+250+104)<<QPoint((88-8)/2+mmX2+8,h2+250+104)<<QPoint((88-8)/2+mmX2+8,h2+250+104+19)<<QPoint((88-8)/2+mmX2,h2+250+104+8);
    else
        polygon << QPoint((88-8)/2+mmX2,h2+250+98)<<QPoint((88-8)/2+mmX2+8,h2+250+98)<<QPoint((88-8)/2+mmX2+8,h2+250+98+19)<<QPoint((88-8)/2+mmX2,h2+250+98+8);
    path.addPolygon(polygon);
    path.closeSubpath();

    QLinearGradient linear(QPointF((88-8)/2+mmX2, 0), QPointF((88+8)/2+mmX2, 0));
    linear.setColorAt(1, QColor(139,119,110));
    linear.setColorAt(0, QColor(136,120,120));
    QBrush bru(linear);
    painter.setBrush(bru);
    painter.fillPath(path, bru);

    //后画眼睛嘴巴，防止被遮挡
    if(isEyes21)
    {
        //第一种眼睛
        if(eyesType21==0)
        {
            eyes21.load(":/back/images/mm_eyes.PNG");
            eyes21=eyes21.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            eyes21=eyes21.transformed(m);
            //参数
            painter.drawPixmap(mmX2,h2,eyes21);
        }
        //第二种
        if(eyesType21==1)
        {
            eyes21.load(":/back/images/mm_eyes2.PNG");
            eyes21=eyes21.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            eyes21=eyes21.transformed(m);
            //参数
            painter.drawPixmap(mmX2,h2,eyes21);
        }
    }
    if(isMouth21)
    {
        if(mouthType21==0)
        {
            mouth21.load(":/back/images/mm_mouth1.PNG");
            mouth21=mouth21.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            mouth21=mouth21.transformed(m);
            //参数
            painter.drawPixmap(mmX2,h2,mouth21);
        }
        if(mouthType21==1)
        {
            mouth21.load(":/back/images/mm_mouth2.PNG");
            mouth21=mouth21.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            mouth21=mouth21.transformed(m);
            //参数
            painter.drawPixmap(mmX2,h2,mouth21);
        }
        if(mouthType21==2)
        {
            mouth21.load(":/back/images/mm_mouth3.png");
            mouth21=mouth21.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            mouth21=mouth21.transformed(m);
            //参数
            painter.drawPixmap(mmX2,h2,mouth21);
        }
        if(mouthType21==3)
        {
            mouth21.load(":/back/images/mm_mouth4.png");
            mouth21=mouth21.scaled(88,88);
            //翻转
            QMatrix m;
            m.rotate(180);
            mouth21=mouth21.transformed(m);
            //参数
            painter.drawPixmap(mmX2,h2,mouth21);
        }
    }
    //下面的
    if(isEyes22)
    {
        //第一种眼睛
        if(eyesType22==0)
        {
            eyes22.load(":/back/images/mm_eyes.PNG");
            eyes22=eyes22.scaled(88,88);
            //参数
            if(test22==0)
                painter.drawPixmap(mmX2,h2+250+16,eyes22);
            else
                painter.drawPixmap(mmX2,h2+250+10,eyes22);
        }
        //第二种
        if(eyesType22==1)
        {
            eyes22.load(":/back/images/mm_eyes2.PNG");
            eyes22=eyes22.scaled(88,88);
            //参数
            if(test22==0)
                painter.drawPixmap(mmX2,h2+250+16,eyes22);
            else
                painter.drawPixmap(mmX2,h2+250+10,eyes22);
        }
    }
    if(isMouth22)
    {
        if(mouthType22==0)
        {
            mouth22.load(":/back/images/mm_mouth1.PNG");
            mouth22=mouth22.scaled(88,88);
            //参数
            if(test22==0)
                painter.drawPixmap(mmX2,h2+250+16,mouth22);
            else
                painter.drawPixmap(mmX2,h2+250+10,mouth22);
        }
        if(mouthType22==1)
        {
            mouth22.load(":/back/images/mm_mouth2.PNG");
            mouth22=mouth22.scaled(88,88);
            //参数
            if(test22==0)
                painter.drawPixmap(mmX2,h2+250+16,mouth22);
            else
                painter.drawPixmap(mmX2,h2+250+10,mouth22);
        }
        if(mouthType22==2)
        {
            mouth22.load(":/back/images/mm_mouth3.PNG");
            mouth22=mouth22.scaled(88,88);
            //参数
            if(test22==0)
                painter.drawPixmap(mmX2,h2+250+16,mouth22);
            else
                painter.drawPixmap(mmX2,h2+250+10,mouth22);
        }
        if(mouthType22==3)
        {
            mouth22.load(":/back/images/mm_mouth4.PNG");
            mouth22=mouth22.scaled(88,88);
            //参数
            if(test22==0)
                painter.drawPixmap(mmX2,h2+250+16,mouth22);
            else
                painter.drawPixmap(mmX2,h2+250+10,mouth22);
        }
    }

    //增加分数
    if(mmX2+88<androidX+42 && !isScoreMm2)
    {
        score++;
        isScoreMm2=true;
    }
    painter.setViewport(0, 0, 360, 780);
}
//开始游戏
void MainWindow::startGame()
{
    gameStatus=RUNNING;
    androidStatus=DOWN;
    androidUpSpeed=0;
    timer->start(16);
}
//终止游戏
void MainWindow::stopGame()
{
    //一旦游戏结束，时间停止
    timer->stop();
    gameStatus=STOPING;
}
//记分板
void MainWindow::drawScore()
{
    QColor color;
    if(themeColor==Theme_Blue)
        color.setRgb(58,120,231);
    if(themeColor==Theme_Yellow)
        color.setRgb(245,180,0);
    if(themeColor==Theme_Green)
        color.setRgb(16,157,88);
    if(themeColor==Theme_Purple)
        color.setRgb(122,24,127);
    if(themeColor==Theme_Grey)
        color.setRgb(158,158,158);
    if(themeColor==Theme_Red)
        color.setRgb(220,68,55);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    //防止记分板装不下数字
    int x=getDigit(score);
    //绝对大小
    path.addRoundedRect(QRectF((360-38-12*(x-1))/2, 13, 38+12*(x-1), 41), 3, 3);
    //填充内部，不包括边界
    painter.fillPath(path, color);

    QFont font( "Microsoft YaHei", 13, 70);
    painter.setFont(font);

    if(themeColor==Theme_Yellow)
        painter.setPen(Qt::black);
    else
        painter.setPen(Qt::white);
    painter.drawText(QRectF((360-38-12*(x-1))/2, 13, 38+12*(x-1), 41),Qt::AlignCenter,QString::number(score));
}
//保持持续的paint事件
void MainWindow::loopPaint()
{
    update();
}
//开始绘画安卓
void MainWindow::initAndroid()
{
    startAndroid=true;
    //开始游戏之后山云仙人掌就不重置了
    isReset=false;
}
//开始绘画棉花糖
void MainWindow::initMm()
{
    startMm=true;
}
//绘画倒计时
void MainWindow::drawCountNumber()
{
    QPainter painter(this);
    if(isStartCount)
    {
        QFont font( "Microsoft JhengHei", 20, 70);
        painter.setFont(font);

        painter.setPen(Qt::black);
        painter.drawText(QRectF((360-72)/2,(780-72)/2,72,72),Qt::AlignCenter,QString::number(countNumber));
    }
}
//隐藏开始键
void MainWindow::hideButton()
{
    play->setVisible(false);
    play->setEnabled(false);
}
//开始倒计时
void MainWindow::startCount()
{
    countNumber=3;
    startAndroid=startMm=false;
    isStartCount=true;
    if(!isFirstInit)
        init();
    //优化版定时器，省去槽函数
    QTimer::singleShot(400,this,[=]{
        countNumber=2;
    });
    QTimer::singleShot(800,this,[=]{
        countNumber=1;
    });
    QTimer::singleShot(1200,this,[=]{
        countNumber=0;
    });
    QTimer::singleShot(1300,this,[=]{
        countNumber=0;
        initAndroid();
        initMm();
        isStartCount=false;
    });
    isFirstInit=false;
}
//向上的初速度
void MainWindow::initSpeed()
 {
     androidUpSpeed=6.3;
 }
//自己写碰撞检测
bool MainWindow::isCrush()
{
    //掉到地板下面了
    if(androidY==780-42)
        return true;
    //与第一对循环棉花糖碰撞
    if(androidX+42>mmX1+10 && androidX<mmX1+78 && androidY+42>h1+20 && androidY<h1+70)
        return true;
    if(androidX+42>mmX1+10 && androidX<mmX1+78 && androidY+42>h1+250+30 && androidY<h1+250+70)
        return true;
    //第二对
    if(androidX+42>mmX2+10 && androidX<mmX2+78 && androidY+42>h2+20 && androidY<h2+70)
        return true;
    if(androidX+42>mmX2+10 && androidX<mmX2+78 && androidY+42>h2+250+30 && androidY<h2+250+70)
        return true;
    //第一对的杆子相碰
    if(androidX+42>mmX1+(88-6)/2 && androidX<mmX1+(88+6)/2 && androidY<h1)
        return true;
    if(androidX+42>mmX1+(88-6)/2 && androidX<mmX1+(88+6)/2 && androidY+42>h1+250+88)
        return true;
    //第二对
    if(androidX+42>mmX2+(88-6)/2 && androidX<mmX2+(88+6)/2 && androidY<h2)
        return true;
    if(androidX+42>mmX2+(88-6)/2 && androidX<mmX2+(88+6)/2 && androidY+42>h2+250+88)
        return true;
    return false;
}
