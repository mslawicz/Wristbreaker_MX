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
#include "AS5048A.h"
#include "motor_BLDC.h"
#include "constant.h"
#include <iostream>

ADC_HandleTypeDef* pHadc;    //pointer to ADC object
uint16_t adcConvBuffer[MAX_ADC_CH]; //buffer for ADC conversion results
TIM_HandleTypeDef* pMotor1Htim;  //pointer to htim object instance used by motor #1
SPI_HandleTypeDef* pPosSensSpi;  //pointer to position sensor SPI bus


void mainLoop()
{
    constexpr uint32_t HeartbeatPeriod = 500000;
    Timer statusLedTimer;
    Timer gameCtrlTimer;
    GameController gameController;  //USB link-to-PC object (class custom HID - game controller)
    HapticDevice aileronCtrl(new MotorBLDC(14, pMotor1Htim), new AS5048A(pPosSensSpi, ENC1_CS_GPIO_Port, ENC1_CS_Pin, true));   //aileron control haptic device
    aileronCtrl.hapticParam.midPosition = 0.2F;
    aileronCtrl.hapticParam.calMagnitude = 0.6F;
    aileronCtrl.hapticParam.calSpeed = 0.001F;
    aileronCtrl.hapticParam.calRange = 0.2F;
    aileronCtrl.hapticParam.operRange = 0.4F;
    aileronCtrl.hapticParam.CalDirChg = 3;

    std::cout << "\r\nWristbreaker v1.0\r\n";

    Timer::start(pTimerHtim);

    /* main forever loop */
    while(true)
    {
        aileronCtrl.hapticParam.gain = scale<uint16_t, float>(0, Max12Bit, adcConvBuffer[AdcCh::throttle], 0, 5.0F);    //XXX test
        aileronCtrl.hapticParam.idleMagnitude = scale<uint16_t, float>(0, Max12Bit, adcConvBuffer[AdcCh::propeller], 0, 0.5F);  //XXX test

        /* aileron control */
        aileronCtrl.handler();
        gameController.data.X = scale<float, int16_t>(-aileronCtrl.hapticParam.operRange, aileronCtrl.hapticParam.operRange, aileronCtrl.hapticParam.currentPosition, -Max15Bit, Max15Bit);

        /* elevator control */
        //elevatorCtrl.handler();
        gameController.data.Y = scale<uint16_t, int16_t>(0, Max16Bit, 0 /*elevatorCtrl.param.currentPosition*/, -Max15Bit, Max15Bit);

        /* rudder control */
        //rudderCtrl.handler();
        gameController.data.Rz = scale<uint16_t, int16_t>(0, Max16Bit, 0 /*rudderCtrl.param.currentPosition*/, -Max15Bit, Max15Bit);

        /* throttle control */
        gameController.data.slider = scale<uint16_t, uint16_t>(0, Max12Bit, adcConvBuffer[AdcCh::throttle], 0, Max15Bit);

        /* propeller control */
        gameController.data.dial = scale<uint16_t, uint16_t>(0, Max12Bit, adcConvBuffer[AdcCh::propeller], 0, Max15Bit);

        /* mixture control */
        gameController.data.Z = scale<uint16_t, int16_t>(0, Max12Bit, adcConvBuffer[AdcCh::mixture], -Max15Bit, Max15Bit);

        /* left brake control */
        gameController.data.Rx = scale<uint16_t, uint16_t>(0, Max12Bit, adcConvBuffer[AdcCh::leftBrake], 0, Max15Bit);

        /* right brake control */
        gameController.data.Ry = scale<uint16_t, uint16_t>(0, Max12Bit, adcConvBuffer[AdcCh::rightBrake], 0, Max15Bit);

        /* request next conversions of analog channels */
        HAL_ADC_Start_DMA(pHadc, (uint32_t*)adcConvBuffer, pHadc->Init.NbrOfConversion);

        if(statusLedTimer.hasElapsed(HeartbeatPeriod))
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

