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
    .CalDirChg = 3,
    .refPosChangeLimit = 0.001F,
    .effectGain = 0.13F
};


//configuration of elevator haptic device
//this device uses GM5208-12 motor
constexpr HapticParam ElevatorCfg =
{
    .type = HapticType::Spring,
    .midPosition = 0.2F,
    .calMagnitude = 0.5F,
    .calSpeed = 0.0004F,
    .calRange = 0.2F,
    .operRange = 0.4F,
    .idleMagnitude = 0.14F,
    .gain = 4.5F,
    .CalDirChg = 3,
    .refPosChangeLimit = 0.001F,
    .effectGain = 0.13F
};


#endif /* INC_HAPTIC_CFG_H_ */
