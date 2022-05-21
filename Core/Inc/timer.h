/*
 * timer.h
 *
 *  Created on: May 21, 2022
 *      Author: marci
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32f4xx_hal.h"

class Timer
{
public:
    static void start(TIM_HandleTypeDef* pHtim);
private:
    static TIM_HandleTypeDef* _pHtim;
};



#endif /* INC_TIMER_H_ */
