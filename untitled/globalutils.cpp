#include "globalutils.h"

GlobalUtils::GlobalUtils(){

}

//获取随机数 0 ~ 200之间
int GlobalUtils::getRandomNum(uint seed,int limit)
{
    qsrand(seed);
    return qrand()%limit;
}
