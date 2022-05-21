/*
 * timer.cpp
 *
 *  Created on: May 21, 2022
 *      Author: marci
 */

#include "timer.h"
#include "main_loop_api.h"

TIM_HandleTypeDef* pTimerHtim;  //pointer to htim object instance used by Timer class
TIM_HandleTypeDef* Timer::_pHtim;

void Timer::start(TIM_HandleTypeDef* pHtim)
{
    _pHtim = pHtim;
    HAL_TIM_Base_Start(_pHtim);
}
