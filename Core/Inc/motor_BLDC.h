/*
 * motor_BLDC.h
 *
 *  Created on: 27 maj 2022
 *      Author: marci
 */

#ifndef INC_MOTOR_BLDC_H_
#define INC_MOTOR_BLDC_H_

#include "actuator.h"
#include "stm32f4xx_hal.h"

class MotorBLDC : public Actuator
{
protected:
    uint8_t _polePairs;
private:
    void setFieldVector(int32_t angle, uint16_t magnitude);
};



#endif /* INC_MOTOR_BLDC_H_ */
