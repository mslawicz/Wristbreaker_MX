/*
 * haptic_cfg.h
 *
 *  Created on: 3 lip 2022
 *      Author: marci
 */

#ifndef INC_HAPTIC_CFG_H_
#define INC_HAPTIC_CFG_H_

#include "haptic_param.h"


//configuration of aileron haptic device
//this device uses HT4315 motor
constexpr HapticParam AileronCfg =
{
    .type = HapticType::Spring,
    .midPosition = 0.2F,
    .calMagnitude = 0.6F,
    .calSpeed = 0.001F,
    .calRange = 0.2F,
    .operRange = 0.4F,
    .idleMagnitude = 0.12F,
    .gain = 3.8F,
    .CalDirChg = 3
};


//configuration of elevator haptic device
//this device uses GM5208-12 motor
constexpr HapticParam ElevatorCfg =
{
    .type = HapticType::Spring,
    .midPosition = 0.2F,
    .calMagnitude = 0.4F,
    .calSpeed = 0.0005F,
    .calRange = 0.2F,
    .operRange = 0.4F,
    .idleMagnitude = 0.06F,
    .gain = 4.5F,
    .CalDirChg = 3
};


#endif /* INC_HAPTIC_CFG_H_ */
