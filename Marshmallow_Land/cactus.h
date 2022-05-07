#ifndef CACTUS_H
#define CACTUS_H

#include <QObject>
#include <QPainter>

class cactus : public QObject
{
    Q_OBJECT
public:
    explicit cactus(QObject *parent = nullptr);
    QPixmap cact,cactShadow;

    //横纵初始坐标
    double x;
    int width;

    void setCact(int m);
    void setX(int n);
    void setWidth(int w);
    void draw(QPainter &painter);
    void drawShadow(QPainter &painter,double shadow);

signals:

};

#endif // CACTUS_H
