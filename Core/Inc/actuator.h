/*
 * actuator.h
 *
 *  Created on: May 25, 2022
 *      Author: marci
 */

#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#include "haptic_param.h"
#include "timer.h"

class Actuator
{
public:
    virtual ~Actuator() {}
    virtual void initialize();
    virtual bool calibrate(HapticParam& hapticParam);
    virtual void action();
protected:
    Timer _runTimer;    //run timer
    Timer _calTimer;    //calibration timer
};


#endif /* INC_ACTUATOR_H_ */
