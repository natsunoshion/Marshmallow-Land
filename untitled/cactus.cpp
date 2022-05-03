#include "cactus.h"

cactus::cactus(QObject *parent) : QObject(parent)
{

}

void cactus::setCact(int m)
{
    if(m==0)
    {
        cact.load(":/back/images/cactus1.png");
        cact=cact.scaled(width,width);
    }
    if(m==1)
    {
        cact.load(":/back/images/cactus2.png");
        cact=cact.scaled(width,width);
    }
    if(m==2)
    {
        cact.load(":/back/images/cactus3.png");
        cact=cact.scaled(width,width);
    }
}

void cactus::setX(int n)
{
    x=n;
}

void cactus::setWidth(int w)
{
    width=w;
}

void cactus::draw(QPainter &painter)
{
    painter.drawPixmap(x,780-width,cact);
}
