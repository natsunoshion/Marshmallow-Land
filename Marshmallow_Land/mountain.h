#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include <QObject>
#include <QPainter>

class mountain : public QObject
{
    Q_OBJECT
public:
    mountain();
    QPixmap moun,mounShadow;

    //水平位置
    double x;
    int width;

    void setMoun(int m);
    void setX(int n);
    void setWidth(int w);
    void draw(QPainter &painter);
    void drawShadow(QPainter &painter,double shadow);

signals:

};

#endif // MOUNTAIN_H
