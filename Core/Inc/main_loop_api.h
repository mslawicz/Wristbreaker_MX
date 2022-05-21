/*
 * main_loop_api.h
 *
 *  Created on: May 19, 2022
 *      Author: marci
 */

#ifndef INC_MAIN_LOOP_API_H_
#define INC_MAIN_LOOP_API_H_

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef* pTimerHtim;  //pointer to htim object instance used by Timer class

#ifdef __cplusplus
extern "C"
{
#endif

void mainLoop();

#ifdef __cplusplus
}
#endif

#endif /* INC_MAIN_LOOP_API_H_ */
