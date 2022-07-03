/*
 * haptic.h
 *
 *  Created on: 22 maj 2022
 *      Author: marci
 */

#ifndef INC_HAPTIC_H_
#define INC_HAPTIC_H_

#include "stm32f4xx_hal.h"
#include "haptic_param.h"
#include "actuator.h"
#include "position_sensor.h"
#include <string>

enum class HapticState : uint8_t
{
    Start,
    Calibration,
    Action
};

/*
 * class of a haptic device
 * this device consists of an actuator and position sensor
 * the object handler() function should be called periodically (as frequent as possible)
 */
class HapticDevice
{
public:
    HapticDevice(Actuator* pActuator, PositionSensor* pPositionSensor, std::string name, HapticParam configuration);
    void handler();
    HapticParam hapticParam;
private:
    Actuator* _pActuator{nullptr};
    PositionSensor* _pPositionSensor{nullptr};
    HapticState _state{HapticState::Start};
    std::string _name;
};


#endif /* INC_HAPTIC_H_ */
