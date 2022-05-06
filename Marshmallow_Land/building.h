#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <QPainter>
#include "globalutils.h"

class building : public QObject
{
    Q_OBJECT
public:
    explicit building(QObject *parent = nullptr);
    int width,height;
    double x;
    QPixmap buil,builShadow;

    void draw(QPainter &painter);
    void drawShadow(QPainter &painter,double shadow);
    void setX(double n);
    void setWidth(int w);
    void setHeight(int h);

signals:

};

#endif // BUILDING_H
