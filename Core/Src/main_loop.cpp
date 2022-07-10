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
#include "logger.h"
#include "spi_supervisor.h"
#include "haptic_cfg.h"
#include "monitor.h"
#include <iostream>

ADC_HandleTypeDef* pHadc;    //pointer to ADC object
uint16_t adcConvBuffer[MAX_ADC_CH]; //buffer for ADC conversion results
TIM_HandleTypeDef* pMotor1Htim;  //pointer to htim object instance used by motor #1
SPI_HandleTypeDef* pPosSensSpi;  //pointer to position sensor SPI bus
I2C_HandleTypeDef* pEncI2c;      //pointer to encoder I2C bus

#ifdef MONITOR
int16_t monitor_elevTrim;
int16_t monitor_elevRotAcc;
int16_t monitor_yokeDynY;
int16_t monitor_yokeRefY;
int16_t monitor_elevCurPos;
int16_t monitor_elevCurRef;
int16_t monitor_pilotInpY;
#endif

void mainLoop()
{
    constexpr uint32_t HeartbeatPeriod = 500000;
    Timer statusLedTimer;
    Timer gameCtrlTimer;
    Timer simCtrlTimer;
    Timer simOfflineTimer;

    LOG_ALWAYS("Wristbreaker v1.0");

    //assign system LEDs
    GPIO_TypeDef* heartbeatLedPort = LD1_GPIO_Port; //green LED
    uint16_t heartbeatLedPin = LD1_Pin;
    GPIO_TypeDef* simOnlineLedPort = LD2_GPIO_Port; //blue LED
    uint16_t simOnlineLedPin = LD2_Pin;

    GameController gameController;  //USB link-to-PC object (class custom HID - joystick)
    SimController simController;    //USB link-to-PC object (class custom HID - data buffer)
    SpiSupervisor posSensSpiSupervisor(pPosSensSpi, 4);

    //define aileron haptic device
    HapticDevice aileronCtrl(nullptr, nullptr, "aileron controller", AileronCfg);   //aileron control haptic device

    //define elevator haptic device
    HapticDevice elevatorCtrl(new MotorBLDC(7, pMotor1Htim), new AS5048A(posSensSpiSupervisor, ENC1_CS_GPIO_Port, ENC1_CS_Pin, true), "elevator controller", ElevatorCfg);   //elevator control haptic device

    Timer::start(pTimerHtim);

    /* main forever loop */
    while(true)
    {
        //check new data received from simulator
        if(simController.isNewDataReceived())
        {
            simController.parseSimData();
            if(simController.simOnline)
            {
                HAL_GPIO_WritePin(simOnlineLedPort, simOnlineLedPin, GPIO_PinState::GPIO_PIN_SET);
                simOfflineTimer.reset();
            }
            else
            {
                HAL_GPIO_WritePin(simOnlineLedPort, simOnlineLedPin, GPIO_PinState::GPIO_PIN_RESET);
            }
        }

        elevatorCtrl.hapticParam.gain = scale<uint16_t, float>(0, Max12Bit, adcConvBuffer[AdcCh::throttle], 0, 10.0F);    //XXX test    4.4
        //elevatorCtrl.hapticParam.idleMagnitude = scale<uint16_t, float>(0, Max12Bit, adcConvBuffer[AdcCh::propeller], 0, 0.5F);  //XXX test 0.14
        elevatorCtrl.hapticParam.refPosChangeLimit = scale<uint16_t, float>(0, Max12Bit, adcConvBuffer[AdcCh::propeller], 0, 0.00005F);  //XXX test
        elevatorCtrl.hapticParam.effectGain = scale<uint16_t, float>(0, Max12Bit, adcConvBuffer[AdcCh::mixture], 0, 0.5F);    //XXX test    0.13

        /* aileron control */
        aileronCtrl.handler();
        gameController.data.X = scale<float, int16_t>(-aileronCtrl.hapticParam.operRange, aileronCtrl.hapticParam.operRange, aileronCtrl.hapticParam.currentPosition, -Max15Bit, Max15Bit);

        /* elevator control */
        float yokeDynY = ((HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PinState::GPIO_PIN_SET) ? 1.0F : -1.0F) * elevatorCtrl.hapticParam.effectGain * simController.getSimData().rotAccBodyX;
        float yokeRefY = simController.getSimData().elevatorTrim;
        if(simController.getSimData().engineType == 0)
        {
            //this is a piston engine - simulate elevator weight
            //elevator goes to trim position between 50 and 80 percent of takeoff speed
            yokeRefY = scale<uint8_t, float>(128, 204, simController.getSimData().takeoffSpeedPct, -1.0F, yokeRefY);
        }
        yokeRefY = simController.simOnline ? (yokeRefY + yokeDynY) : 0.0F; //<-1,1>
        elevatorCtrl.hapticParam.referencePosition = scale<float, float>(-1.0F, 1.0F, yokeRefY, -elevatorCtrl.hapticParam.operRange, elevatorCtrl.hapticParam.operRange);    //<-oper,oper>
        elevatorCtrl.handler();
        float pilotInpY = scale<float, float>(-elevatorCtrl.hapticParam.operRange, elevatorCtrl.hapticParam.operRange, elevatorCtrl.hapticParam.currentPosition - elevatorCtrl.hapticParam.currentRefPos, -1.0F, 1.0F);  //<-1,1>
        gameController.data.Y = scale<float, int16_t>(-1.0F, 1.0F, pilotInpY, -Max15Bit, Max15Bit); //<-32767,32767>

        /* rudder control */
        //rudderCtrl.handler();
        gameController.data.Rz = scale<uint16_t, int16_t>(0, Max16Bit, 0 /*rudderCtrl.param.currentPosition*/, -Max15Bit, Max15Bit);

#ifdef MONITOR
        monitor_elevTrim = scale<float, int16_t>(-1.0F, 1.0F, simController.getSimData().elevatorTrim, -1000, 1000);
        monitor_elevRotAcc = scale<float, int16_t>(-10.0F, 10.0F, simController.getSimData().rotAccBodyX, -10000, 10000);
        monitor_yokeDynY = scale<float, int16_t>(-10.0F, 10.0F, yokeDynY, -10000, 10000);
        monitor_yokeRefY = scale<float, int16_t>(-1.0F, 1.0F, yokeRefY, -1000, 1000);
        monitor_elevCurPos  = scale<float, int16_t>(-elevatorCtrl.hapticParam.operRange, elevatorCtrl.hapticParam.operRange, elevatorCtrl.hapticParam.currentPosition, -1000, 1000);
        monitor_elevCurRef = scale<float, int16_t>(-elevatorCtrl.hapticParam.operRange, elevatorCtrl.hapticParam.operRange, elevatorCtrl.hapticParam.currentRefPos, -1000, 1000);
        monitor_pilotInpY = scale<float, int16_t>(-1.0F, 1.0F, pilotInpY, -1000, 1000);
#endif

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
            HAL_GPIO_TogglePin(heartbeatLedPort, heartbeatLedPin);
            statusLedTimer.reset();
        }

        if(gameCtrlTimer.hasElapsed(GameController::ReportInterval))
        {
            gameController.sendReport();
            gameCtrlTimer.reset();
        }

        if(simCtrlTimer.hasElapsed(SimController::ReportInterval))
        {
            simController.sendReport();
            simCtrlTimer.reset();
        }

        //check time since the last valid data from simulator
        if(simOfflineTimer.hasElapsed(SimController::OfflineTimout))
        {
            simController.simOnline = false;
            HAL_GPIO_WritePin(simOnlineLedPort, simOnlineLedPin, GPIO_PinState::GPIO_PIN_RESET);
        }
    }
}

