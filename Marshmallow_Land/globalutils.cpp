#include "globalutils.h"

GlobalUtils::GlobalUtils(){

}

//高质量随机数获取，通过QRandomGenerator实现
int GlobalUtils::getRandomNum(int limit)
{
    return QRandomGenerator::global()->bounded(limit);
}

int GlobalUtils::getRandomNum(int lowest, int highest)
{
    return QRandomGenerator::global()->bounded(lowest,highest);
}

double GlobalUtils::getRandomNum(double limit)
{
    return QRandomGenerator::global()->bounded(limit);
}
