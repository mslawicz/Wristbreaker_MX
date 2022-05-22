/*
 * haptic.h
 *
 *  Created on: 22 maj 2022
 *      Author: marci
 */

#ifndef INC_HAPTIC_H_
#define INC_HAPTIC_H_


/*
 * class of a haptic device
 * this device consists of an actuator and position encoder
 * the object handler() function should be called periodically (as frequent as possible)
 */
class HapticDevice
{
public:
    void handler();
};


#endif /* INC_HAPTIC_H_ */
