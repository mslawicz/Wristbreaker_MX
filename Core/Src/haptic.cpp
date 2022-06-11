/*
 * haptic.cpp
 *
 *  Created on: 22 maj 2022
 *      Author: marci
 */

#include "haptic.h"
#include "logger.h"

HapticDevice::HapticDevice(Actuator* pActuator, PositionSensor* pPositionSensor, std::string name) :
    _pActuator(pActuator),
    _pPositionSensor(pPositionSensor),
    _name(name)
{
    LOG_INFO("device \"" << _name << "\" created");
}

void HapticDevice::handler()
{
    static constexpr float HalfRange = 0.5F;
    float currentPosition{0};

    if(nullptr != _pPositionSensor)
    {
        hapticParam.encoderPosition = _pPositionSensor->getPosition();
        currentPosition = hapticParam.encoderPosition - hapticParam.midPosition;
    }
    else
    {
        hapticParam.encoderPosition = HalfRange;
    }

    if(currentPosition > HalfRange)
    {
        currentPosition -= 1.0F;
    }
    else if(currentPosition < -HalfRange)
    {
        currentPosition += 1.0F;
    }
    // currentPosition in the range <-0.5,0.5> relative to midPosition
    hapticParam.currentPosition = currentPosition;

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
            if(_pActuator->calibrate(hapticParam))
            {
                _state = HapticState::Action;
            }
        }
        break;

    case HapticState::Action:
        if(nullptr != _pActuator)
        {
            _pActuator->action(hapticParam);
        }
        break;

    default:
        break;
    }
}

