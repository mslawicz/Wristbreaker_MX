/*
 * main_loop.h
 *
 *  Created on: May 19, 2022
 *      Author: marci
 */

#ifndef INC_MAIN_LOOP_H_
#define INC_MAIN_LOOP_H_

#include "stm32f4xx_hal.h"

#define MAX_12_BIT  0xFFF
#define MAX_15_BIT  0x7FFF
#define MAX_16_BIT  0xFFFF

enum AdcCh
{
    throttle,
    propeller,
    mixture,
    leftBrake,
    rightBrake
};

#endif /* INC_MAIN_LOOP_H_ */
