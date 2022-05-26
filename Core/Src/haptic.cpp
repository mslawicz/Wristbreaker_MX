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
    int32_t currentPosition{0};
    if(nullptr != _pPositionSensor)
    {
        currentPosition = _pPositionSensor->getPosition() - _param.midPosition;
    }

    if(currentPosition > 0x7FFF)
    {
        currentPosition -= 0x10000;
    }
    else if(currentPosition < -0x8000)
    {
        currentPosition += 0x10000;
    }
    else if(currentPosition == -0x8000)
    {
        currentPosition = -0x7FFF;
    }

    _param.currentPosition = static_cast<int16_t>(currentPosition);
}

