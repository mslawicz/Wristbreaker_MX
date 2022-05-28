/*
 * convert.h
 *
 *  Created on: May 21, 2022
 *      Author: marci
 */

#ifndef INC_CONVERT_H_
#define INC_CONVERT_H_

constexpr float FullCycle = 360.0F;
constexpr float HalfCycle = 180.0F;
constexpr float OneThirdCycle = 120.0F;
constexpr float QuarterCycle = 90.0F;
constexpr float PI = 3.14159265359F;

template<typename iType, typename oType> oType scale(iType iMin, iType iMax, iType input, oType oMin, oType oMax, bool limit = true)
{
    auto result = static_cast<oType>(1.0F * (input-iMin) / (iMax-iMin) * (oMax-oMin) + oMin);
    if(limit)
    {
        if(result < oMin)
        {
            result = oMin;
        }
        else if(result > oMax)
        {
            result = oMax;
        }
    }
    return result;
}

template<typename T> T limit(T value, T min, T max)
{
    if(value > max)
    {
        value = max;
    }
    else if(value < min)
    {
        value = min;
    }
    return value;
}

#endif /* INC_CONVERT_H_ */
