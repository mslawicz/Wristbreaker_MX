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
    auto result = static_cast<oType>((input-iMin) * (oMax-oMin) / (iMax-iMin) + oMin);
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

#endif /* INC_CONVERT_H_ */
