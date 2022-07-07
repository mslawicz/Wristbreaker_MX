/*
 * monitor.h
 *
 *  Created on: 7 lip 2022
 *      Author: marci
 */

#ifndef INC_MONITOR_H_
#define INC_MONITOR_H_

#define MONITOR

#ifdef MONITOR

#include "stm32f4xx_hal.h"

struct XYZ
{
    int16_t X;
    int16_t Y;
    int16_t Z;
};

#endif //MONITOR

#endif /* INC_MONITOR_H_ */
