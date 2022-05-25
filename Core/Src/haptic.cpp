/*
 * haptic.cpp
 *
 *  Created on: 22 maj 2022
 *      Author: marci
 */

#include "haptic.h"

HapticDevice::HapticDevice(Actuator* pActuator, PositionSensor* pPositionSensor) :
    _pActuator(pActuator),
    _pPositionSensor(pPositionSensor)
{

}

void HapticDevice::handler()
{

}

uint16_t HapticDevice::getPosition()
{
    if(nullptr != _pPositionSensor)
    {
        return _pPositionSensor->getPosition();
    }

    return 0;
}
