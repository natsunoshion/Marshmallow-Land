#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include <QObject>
#include <QPainter>

class mountain : public QObject
{
    Q_OBJECT
public:
    mountain();
    QPixmap moun;
    //水平位置
    int x;
    int width;

    void setMoun(int m);
    void setX(int n);
    void setWidth(int w);
    void draw(QPainter &painter);

signals:

};

#endif // MOUNTAIN_H
