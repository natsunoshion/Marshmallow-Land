#include "building.h"

building::building(QObject *parent) : QObject(parent)
{
    buil.load((":/back/images/building.png"));
    builShadow.load((":/back/images/buildingShadow.png"));
}

void building::draw(QPainter &painter)
{
    buil=buil.scaled(width,height);
    painter.drawPixmap(x,780-height,buil);
}

void building::drawShadow(QPainter &painter, double shadow)
{
    builShadow=builShadow.scaled(width,height);
    painter.setOpacity(shadow);
    painter.drawPixmap(x,780-height,builShadow);
    //重置
    painter.setOpacity(1.0);
}

void building::setX(double n)
{
    x=n;
}

void building::setWidth(int w)
{
    width=w;
}

void building::setHeight(int h)
{
    height=h;
}
