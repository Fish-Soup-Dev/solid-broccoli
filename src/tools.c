#include "include/tools.h"

int map(float x, float in_min, float in_max, int out_min, int out_max)
{
    int res = (int)((x - in_min) * ((float)out_max - (float)out_min) / (in_max - in_min) + (float)out_min);

    if (res > out_max)
    {
        return out_max;
    }
    else if (res < out_min)
    {
        return out_min;
    }

    return res;
}