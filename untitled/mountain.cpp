#include "mountain.h"

mountain::mountain(){

}

void mountain::setMoun(int m)
{
    if(m==0)
    {
        moun.load(":/back/images/mountain1.png");
        moun=moun.scaled(width,width);
        mounShadow.load(":/back/images/mountain1Shadow.png");
        mounShadow=mounShadow.scaled(width,width);
    }
    if(m==1)
    {
        moun.load(":/back/images/mountain2.png");
        moun=moun.scaled(width,width);
        mounShadow.load(":/back/images/mountain1Shadow.png");
        mounShadow=mounShadow.scaled(width,width);
    }
    if(m==2)
    {
        moun.load(":/back/images/mountain3.png");
        moun=moun.scaled(width,width);
        mounShadow.load(":/back/images/mountain3Shadow.png");
        mounShadow=mounShadow.scaled(width,width);
    }
}

void mountain::setX(int n)
{
    x=n;
}

void mountain::setWidth(int w)
{
    width=w;
}

void mountain::draw(QPainter &painter)
{
    painter.drawPixmap(x,780-width,moun);
}

void mountain::drawShadow(QPainter &painter, double shadow)
{
    painter.setOpacity(shadow);
    painter.drawPixmap(x,780-width,mounShadow);
}
