/*
 * motor_BLDC.cpp
 *
 *  Created on: 27 maj 2022
 *      Author: marci
 */

#include "motor_BLDC.h"
#include "convert.h"
#include "constant.h"
#include "logger.h"
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
    _dirChanges = 0;
    _runTimer.reset();
}

bool MotorBLDC::calibrate(HapticParam& hapticParam)
{
    //test of the motor spinning in open loop
    _magnitude = hapticParam.gain;
    _phase += 0.1F * hapticParam.referencePosition;
    setFieldVector(_phase, _magnitude);
    return false;
}

void MotorBLDC::action(HapticParam& hapticParam)
{
    switch(hapticParam.type)
    {
    case HapticType::Spring:
    {
        g_motor[0] = hapticParam.encoderPosition;   //XXX test
        g_motor[1] = hapticParam.currentPosition;   //XXX test
        float positionError = hapticParam.referencePosition - hapticParam.currentPosition;
        g_motor[2] = positionError; //XXX test

        float encoderPhase = fmod(static_cast<float>(hapticParam.encoderPosition * _polePairs * FullCycle), FullCycle);
        float motorPhase = encoderPhase + _phaseShift;
        float targetPhase = motorPhase + ((positionError > 0) ? QuarterCycle : -QuarterCycle);

        g_motor[5] = encoderPhase;   //XXX test
        g_motor[6] = motorPhase;   //XXX test
        g_motor[7] = targetPhase;   //XXX test

        float magnitude = limit<float>(hapticParam.idleMagnitude + fabs(positionError) * hapticParam.gain, 0, 1.0F);
        g_motor[3] = magnitude;   //XXX test

        setFieldVector(targetPhase, magnitude);

        break;
    }

    default:
        LOG_ERROR_ONCE("unknown type of a haptic device");
        break;
    }
}
