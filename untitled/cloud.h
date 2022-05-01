#ifndef CLOUD_H
#define CLOUD_H

#include <QObject>
#include <QPainter>

class cloud : public QObject
{
    Q_OBJECT
public:
    explicit cloud(QObject *parent = nullptr);
    QPixmap clou;
    //横纵初始坐标
    int x,y,width;

    void setX(int n);
    void setY(int n);
    void setWidth(int n);
    void setCloud();
    void draw(QPainter &painter);

signals:

};

#endif // CLOUD_H
