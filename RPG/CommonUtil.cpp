#include "CommonUtil.h"

int CommonUtil::getRand()
{
    return CommonUtil::getRand(1000);
}

int CommonUtil::getRand(const int& _nMax)
{
    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution dist(0, _nMax);

    return dist(mt);
}
