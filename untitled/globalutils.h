#ifndef GLOBALUTILS_H
#define GLOBALUTILS_H

#include <QObject>

class GlobalUtils
{
public:
    GlobalUtils();
    //获取随机数
    static int getRandomNum(uint seed, int limit);
};

#endif // GLOBALUTILS_H
