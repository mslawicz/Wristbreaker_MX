/*
 * pc_link.h
 *
 *  Created on: May 21, 2022
 *      Author: marci
 */

#ifndef INC_PC_LINK_H_
#define INC_PC_LINK_H_

#include "stm32f4xx_hal.h"

constexpr size_t SimDataSize = 63;

struct GameControllerDataType
{
    uint8_t reportID;
    int16_t X;
    int16_t Y;
    int16_t Z;
    int16_t Rz;
    uint16_t Rx;
    uint16_t Ry;
    uint16_t slider;
    uint16_t dial;
    uint8_t hat;
    uint32_t buttons;
};

struct SimDataType
{
    uint8_t reportID;
    uint8_t data[SimDataSize];
};



#endif /* INC_PC_LINK_H_ */
