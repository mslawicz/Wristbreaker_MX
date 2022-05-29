/*
 * motor_BLDC.cpp
 *
 *  Created on: 27 maj 2022
 *      Author: marci
 */

#include "motor_BLDC.h"
#include "convert.h"
#include <math.h>

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
        argument = std::fmod(argument, FullCycle);
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
    callTimer.reset();
}

bool MotorBLDC::calibrate(ActuatorParam& parameters)
{
    float dMag = parameters.calMagnitude * 1e-6 * callTimer.getElapsedTime();     //magnitude being increased during the first second

    setFieldVector(_phase, _magnitude);

    _phase += parameters.calSpeed * callTimer.getElapsedTime();
    _magnitude += dMag;
    if(_magnitude > parameters.calMagnitude)
    {
        _magnitude = parameters.calMagnitude;
    }

    callTimer.reset();
    return false;
}

void MotorBLDC::action()
{

}
