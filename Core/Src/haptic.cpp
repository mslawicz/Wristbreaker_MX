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
    float currentPosition{0};
    if(nullptr != _pPositionSensor)
    {
        currentPosition = _pPositionSensor->getPosition() - param.midPosition;
    }

    if(currentPosition > 0.5F)
    {
        currentPosition -= 1.0F;
    }
    else if(currentPosition < -0.5F)
    {
        currentPosition += 1.0F;
    }
    // currentPosition in the range <-0.5,0.5> relative to midPosition
    param.currentPosition = currentPosition;

    //haptic handler state machine
    switch(_state)
    {
    case HapticState::Start:
        if(nullptr != _pActuator)
        {
            _pActuator->initialize();
        }
        _state = HapticState::Calibration;
        break;

    case HapticState::Calibration:
        if(nullptr != _pActuator)
        {
            if(_pActuator->calibrate())
            {
                _state = HapticState::Action;
            }
        }
        break;

    case HapticState::Action:

        break;

    default:
        break;
    }
}

