/*
 * haptic_param.h
 *
 *  Created on: 30 maj 2022
 *      Author: marci
 */

#ifndef INC_HAPTIC_PARAM_H_
#define INC_HAPTIC_PARAM_H_

#include "stm32f4xx_hal.h"

enum class HapticType : uint8_t
{
    Spring
};

/*
 * structure of haptic device parameters
 */
struct HapticParam
{
    float midPosition;              //mid position of the actuator <0,1>
    float referencePosition;        //reference position relative to mid position <-0.5,0.5>
    float encoderPosition;          //normalized position read from the encoder <0,1>
    float currentPosition;          //current position relative to mid position <-0.5,0.5>
    float calMagnitude;             //nominal actuator force vector magnitude during calibration phase
    float calSpeed;                 //actuator speed in calibration phase
    float calRange;                 //position range in calibration phase relative to mid position
    uint8_t CalDirChg;              //number of calibration pass direction changes
    HapticType type;                //type of the haptic device
};


#endif /* INC_HAPTIC_PARAM_H_ */
