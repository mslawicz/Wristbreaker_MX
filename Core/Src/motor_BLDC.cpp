/*
 * motor_BLDC.cpp
 *
 *  Created on: 27 maj 2022
 *      Author: marci
 */

#include "motor_BLDC.h"
#include "convert.h"
#include "constant.h"
#include <math.h>

float g_motor[10];  //XXX test

MotorBLDC::MotorBLDC(uint8_t polePairs, TIM_HandleTypeDef* pPwmHtim) :
    _polePairs(polePairs),
    _pPwmHtim(pPwmHtim)
{

}

/*
 * set magnetic field vector of a BLDC motor
 * angle in degrees
 * magnitude <0,1>
 */
void MotorBLDC::setFieldVector(float angle, float magnitude)
{
    float pwmA = magnitude * getSvmValue(angle);
    float pwmB = magnitude * getSvmValue(angle + OneThirdCycle);
    float pwmC = magnitude * getSvmValue(angle - OneThirdCycle);

    //set PWM waves
    _pPwmHtim->Instance->CCR1 = scale<float, uint16_t>(-1.0F, 1.0F, pwmA, 0, _pPwmHtim->Init.Period);
    _pPwmHtim->Instance->CCR2 = scale<float, uint16_t>(-1.0F, 1.0F, pwmB, 0, _pPwmHtim->Init.Period);
    _pPwmHtim->Instance->CCR3 = scale<float, uint16_t>(-1.0F, 1.0F, pwmC, 0, _pPwmHtim->Init.Period);
}

// returns space vector modulation value
// argument in degrees
float MotorBLDC::getSvmValue(float argument)
{
    float sign = 1.0F;

    if (argument < 0)
    {
        argument = -argument;
        sign = -sign;
    }

    if (argument >= FullCycle)
    {
        argument = fmod(argument, FullCycle);
    }

    if (argument >= HalfCycle)
    {
        argument -= HalfCycle;
        sign = -sign;
    }

    if (argument >= QuarterCycle)
    {
        argument = HalfCycle - argument;
    }

    // at this stage the argument is in the range 0..90

    int lowerIndex = static_cast<int>(argument);
    if (lowerIndex == (LutSize - 1))
    {
        return sign * SvmLUT[lowerIndex];   //NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return sign * (SvmLUT[lowerIndex] + (argument - static_cast<float>(lowerIndex))* (SvmLUT[lowerIndex + 1] - SvmLUT[lowerIndex])); //NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
}

void MotorBLDC::initialize()
{
    _phase = 0;
    _magnitude = 0;
    _dPhaseDir = 1.0F;
    _phaseShift = 0;
    _calSteps = 0;
    _runTimer.reset();
}

bool MotorBLDC::calibrate(HapticParam& hapticParam)
{
    //XXX test of data
    g_motor[0] = hapticParam.encoderPosition;
    g_motor[1] = hapticParam.currentPosition;

    if((_magnitude >= hapticParam.calMagnitude) &&
       (isInRange<float>(hapticParam.currentPosition, -hapticParam.calRange, hapticParam.calRange)))
    {
        //conditions for phase shift calculation are met
        float encoderPhase = fmod(static_cast<float>(hapticParam.encoderPosition * _polePairs * FullCycle), FullCycle);
        g_motor[5] = _phase;    //XXX test
        g_motor[6] = encoderPhase;    //XXX test
        float phaseShift = fmod(_phase - encoderPhase, FullCycle);
        if(phaseShift < 0)
        {
            phaseShift += FullCycle;
        }
        g_motor[7] = phaseShift;    //XXX test
        _phaseShift += phaseShift;
        _calSteps++;
        g_motor[8] = _phaseShift / _calSteps;    //XXX test

        if(_calTimer.getElapsedTime() > hapticParam.calTimeout)
        {
            //TODO end calibration here
            __NOP();    //it never hits this point - the timer is being reset
        }
    }
    else
    {
        _calTimer.reset();
    }

    setFieldVector(_phase, _magnitude);

    if(hapticParam.currentPosition > hapticParam.calRange)
    {
        _dPhaseDir = -1.0F;
    }
    if(hapticParam.currentPosition < -hapticParam.calRange)
    {
        _dPhaseDir = 1.0F;
    }

    _phase += _dPhaseDir * hapticParam.calSpeed * _runTimer.getElapsedTime();
    _magnitude += hapticParam.calMagnitude * Micro * _runTimer.getElapsedTime();     //magnitude being increased during the first second
    if(_magnitude > hapticParam.calMagnitude)
    {
        _magnitude = hapticParam.calMagnitude;
    }

    _runTimer.reset();
    return false;
}

void MotorBLDC::action()
{

}
