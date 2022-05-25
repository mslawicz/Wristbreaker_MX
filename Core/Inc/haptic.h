/*
 * haptic.h
 *
 *  Created on: 22 maj 2022
 *      Author: marci
 */

#ifndef INC_HAPTIC_H_
#define INC_HAPTIC_H_

#include "stm32f4xx_hal.h"
#include "actuator.h"
#include "position_sensor.h"

/*
 * class of a haptic device
 * this device consists of an actuator and position sensor
 * the object handler() function should be called periodically (as frequent as possible)
 */
class HapticDevice
{
public:
    HapticDevice(Actuator* pActuator, PositionSensor* pPositionSensor);
    void handler();
    uint16_t getPosition();
private:
    Actuator* _pActuator{nullptr};
    PositionSensor* _pPositionSensor{nullptr};
};


#endif /* INC_HAPTIC_H_ */
