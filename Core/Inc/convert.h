/*
 * convert.h
 *
 *  Created on: May 21, 2022
 *      Author: marci
 */

#ifndef INC_CONVERT_H_
#define INC_CONVERT_H_

template<typename iType, typename oType> oType scale(iType iMin, iType iMax, iType input, oType oMin, oType oMax, bool limit = true)
{
    if(limit)
    {
        if(input < iMin)
        {
            input = iMin;
        }
        else if(input > iMax)
        {
            input = iMax;
        }
    }
    auto result = static_cast<oType>(1.0F * (input-iMin) / (iMax-iMin) * (oMax-oMin) + oMin);
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

//check if variable is in the required range
template<typename T> bool isInRange(T value, T min, T max)
{
    return ((value >= min) && (value <= max));
}

template<typename T> void placeData(T data, uint8_t*& pBuffer)
{
    //memcpy(pBuffer, &variable, sizeof(T));
    *reinterpret_cast<T*>(pBuffer) = data;
    pBuffer += sizeof(T);
}

template<typename T> T parseData(uint8_t*& pBuffer)
{
    T data = *reinterpret_cast<T*>(pBuffer);
    pBuffer += sizeof(T);
    return data;
}

#endif /* INC_CONVERT_H_ */
