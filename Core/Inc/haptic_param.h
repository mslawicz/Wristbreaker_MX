/*
 * haptic_param.h
 *
 *  Created on: 30 maj 2022
 *      Author: marci
 */

#ifndef INC_HAPTIC_PARAM_H_
#define INC_HAPTIC_PARAM_H_

#include "stm32f4xx_hal.h"

enum HapticType : uint8_t
{
    Spring
};

/*
 * structure of haptic device parameters
 */
struct HapticParam
{
    HapticType type;                //type of the haptic device
    float midPosition;              //mid position of the actuator <0,1>
    float referencePosition;        //reference position relative to mid position <-0.5,0.5>
    float encoderPosition;          //normalized position read from the encoder <0,1>
    float currentPosition;          //current position relative to mid position <-0.5,0.5>
    float calMagnitude;             //nominal actuator force vector magnitude during calibration phase
    float calSpeed;                 //actuator speed in calibration phase
    float calRange;                 //position range in calibration phase relative to mid position
    float operRange;                //operation range relative to mid position
    float idleMagnitude;            //field vector magnitude in reference position
    float gain;                     //torque calculation gain
    uint8_t CalDirChg;              //number of calibration pass direction changes
    float refPosChangeLimit;        //maximum change of reference position at a time
    float effectGain;               //gain of force feedback dynamic effects
    float currentRefPos;            //current reference position used for calculations
};

#endif /* INC_HAPTIC_PARAM_H_ */
