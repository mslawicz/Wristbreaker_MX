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

ADC_HandleTypeDef* pHadc;    //pointer to ADC object
uint16_t adcConvBuffer[16]; //buffer for ADC conversion results

SPI_HandleTypeDef* pPosSensSpi;  //pointer to position sensor SPI bus XXX here?

bool spiFree = true; //XXX test
uint16_t angle; //XXX test

void mainLoop()
{
    Timer statusLedTimer;
    Timer gameCtrlTimer;
    GameController gameController;  //USB link-to-PC object (class custom HID - game controller)
    HapticDevice aileronCtrl(nullptr);   //aileron control haptic device
    std::cout << "\r\nWristbreaker v1.0\r\n";

    Timer::start(pTimerHtim);

    /* main forever loop */
    while(true)
    {
        /* aileron control */
        aileronCtrl.handler();
        gameController.data.X = scale<uint16_t, int16_t>(0, MAX_16_BIT, aileronCtrl.getPosition(), -MAX_15_BIT, MAX_15_BIT);

        /* elevator control */
        //elevatorCtrl.handler();
        gameController.data.Y = scale<uint16_t, int16_t>(0, MAX_16_BIT, 0 /*elevatorCtrl.getPosition()*/, -MAX_15_BIT, MAX_15_BIT);

        /* rudder control */
        //rudderCtrl.handler();
        gameController.data.Rz = scale<uint16_t, int16_t>(0, MAX_16_BIT, 0 /*elevatorCtrl.getPosition()*/, -MAX_15_BIT, MAX_15_BIT);

        /* throttle control */
        gameController.data.slider = scale<uint16_t, uint16_t>(0, MAX_12_BIT, adcConvBuffer[0], 0, MAX_15_BIT);

        /* propeller control */
        gameController.data.dial = scale<uint16_t, uint16_t>(0, MAX_12_BIT, adcConvBuffer[1], 0, MAX_15_BIT);

        /* mixture control */
        gameController.data.Z = scale<uint16_t, int16_t>(0, MAX_12_BIT, adcConvBuffer[2], -MAX_15_BIT, MAX_15_BIT);

        /* left brake control */
        gameController.data.Rx = scale<uint16_t, uint16_t>(0, MAX_12_BIT, adcConvBuffer[3], 0, MAX_15_BIT);

        /* right brake control */
        gameController.data.Ry = scale<uint16_t, uint16_t>(0, MAX_12_BIT, adcConvBuffer[4], 0, MAX_15_BIT);

        /* request next conversions of analog channels */
        HAL_ADC_Start_DMA(pHadc, (uint32_t*)adcConvBuffer, pHadc->Init.NbrOfConversion);

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

        //XXX test of SPI
        if(spiFree)
        {
            uint16_t wrBuf = 0xFFFF;
            uint16_t rdBuf;
            HAL_GPIO_WritePin(SPI4_CS0_GPIO_Port, SPI4_CS0_Pin, GPIO_PinState::GPIO_PIN_RESET);
            HAL_SPI_TransmitReceive(pPosSensSpi, (uint8_t*)&wrBuf, (uint8_t*)&rdBuf, 1, 10);
            HAL_GPIO_WritePin(SPI4_CS0_GPIO_Port, SPI4_CS0_Pin, GPIO_PinState::GPIO_PIN_SET);
            angle = rdBuf & 0x3FFF;
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PinState::GPIO_PIN_RESET);
            HAL_SPI_TransmitReceive(pPosSensSpi, (uint8_t*)&wrBuf, (uint8_t*)&rdBuf, 1, 10);
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PinState::GPIO_PIN_SET);
            spiFree = true;
        }
    }
}

