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
#include "pc_link.h"
#include "haptic.h"
#include "convert.h"
#include <iostream>


void mainLoop()
{
    Timer statusLedTimer;
    Timer gameCtrlTimer;
    GameController gameController;  //USB link-to-PC object (class custom HID - game controller)
    HapticDevice aileronCtrl(nullptr);   //aileron control haptic device
    HapticDevice throttleCtrl(nullptr);  //throttle control haptic device
    std::cout << "\r\nWristbreaker v1.0\r\n";

    Timer::start(pTimerHtim);

    /* main forever loop */
    while(true)
    {
        aileronCtrl.handler();
        gameController.data.X = scale<uint16_t, int16_t>(0, 0xFFFF, aileronCtrl.getPosition(), -MAX_15_BIT, MAX_15_BIT);

        throttleCtrl.handler();
        gameController.data.slider = scale<uint16_t, uint16_t>(0, 0xFFFF, throttleCtrl.getPosition(), 0, MAX_15_BIT);

        if(statusLedTimer.hasElapsed(500000))
        {
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            statusLedTimer.reset();
        }

        if(gameCtrlTimer.hasElapsed(GameController::ReportInterval))
        {
            gameController.sendReport();
            gameCtrlTimer.reset();
        }
    }
}
