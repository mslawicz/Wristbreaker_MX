/*
 * haptic.cpp
 *
 *  Created on: 22 maj 2022
 *      Author: marci
 */

#include "haptic.h"

HapticDevice::HapticDevice(PositionSensor* positionSensor) :
    _pPositionSensor(positionSensor)
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
