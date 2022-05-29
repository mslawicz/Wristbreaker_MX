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
 * structure of haptic device parameters
 */
struct HapticParam
{
    float midPosition;      //mid position of the actuator <0,1>
    float referencePosition{0};     //reference position relative to mid position <-0.5,0.5>
    float currentPosition{0};       //current position relative to mid position <-0.5,0.5>
};

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
    HapticDevice(Actuator* pActuator, PositionSensor* pPositionSensor);
    void handler();
    HapticParam param;
private:
    Actuator* _pActuator{nullptr};
    PositionSensor* _pPositionSensor{nullptr};
    HapticState _state{HapticState::Start};
};


#endif /* INC_HAPTIC_H_ */
