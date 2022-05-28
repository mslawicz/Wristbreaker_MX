/*
 * motor_BLDC.cpp
 *
 *  Created on: 27 maj 2022
 *      Author: marci
 */

#include "motor_BLDC.h"
#include <math.h>

MotorBLDC::MotorBLDC(uint8_t polePairs) :
    _polePairs(polePairs)
{

}

/*
 * set magnetic field vector of a BLDC motor
 * angle in degrees
 * magnitude <0,1>
 */
void MotorBLDC::setFieldVector(float angle, float magnitude)
{
    //int32_t pwmA = magnitude * getSVMvalue(angle);
    //int32_t pwmB = magnitude * getSVMvalue(angle + 12000);
    //int32_t pwmC = magnitude * getSVMvalue(angle + 24000);

    //set PWM waves
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
