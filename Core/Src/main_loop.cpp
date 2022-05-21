/*
 * main_loop.cpp
 *
 *  Created on: May 19, 2022
 *      Author: marci
 */

#include "main.h"
#include "main_loop_api.h"
#include "main_loop.h"
#include "timer.h"
#include <iostream>

void mainLoop()
{
    std::cout << "\r\nWristbreaker v1.0\r\n";

    Timer::start(pTimerHtim);

    while(true)
    {
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        HAL_Delay(250);
    }
}
