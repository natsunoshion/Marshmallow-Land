#include "globalutils.h"

GlobalUtils::GlobalUtils(){

}

//获取随机数 0 ~ limit之间
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
