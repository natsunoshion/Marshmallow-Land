#include "cloud.h"

cloud::cloud(QObject *parent) : QObject(parent)
{

}

void cloud::setX(int n)
{
    x=n;
}

void cloud::setY(int n)
{
    y=n;
}

void cloud::setWidth(int n)
{
    width=n;
}

void cloud::setCloud()
{
    int random=GlobalUtils::getRandomNum(50);
    if(random==0)
        clou.load(":/back/images/cloud_off.png");
    else
        clou.load(":/back/images/cloud.png");
    clou=clou.scaled(width,width);
}

void cloud::draw(QPainter &painter)
{
    painter.drawPixmap(x,y,clou);
}
