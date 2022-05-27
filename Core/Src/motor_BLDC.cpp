/*
 * motor_BLDC.cpp
 *
 *  Created on: 27 maj 2022
 *      Author: marci
 */

#include "motor_BLDC.h"
#include "SVM_lut.h"

/*
 * set magnetic field vector of a BLDC motor
 * angle in degrees * 100
 * magnitude <0,0xFFFF>
 */
void MotorBLDC::setFieldVector(int32_t angle, uint16_t magnitude)
{
    int32_t pwmA = magnitude * getSVMvalue(angle);
    int32_t pwmB = magnitude * getSVMvalue(angle + 12000);
    int32_t pwmC = magnitude * getSVMvalue(angle + 24000);

    //set PWM waves
}
