#ifndef GLOBALUTILS_H
#define GLOBALUTILS_H

#include <QObject>
#include <QRandomGenerator>

class GlobalUtils
{
public:
    GlobalUtils();

    //获取随机数
    static int getRandomNum(int limit);
    static int getRandomNum(int lowest,int highest);
    static double getRandomNum(double limit);
};

#endif // GLOBALUTILS_H
