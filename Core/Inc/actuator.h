/*
 * actuator.h
 *
 *  Created on: May 25, 2022
 *      Author: marci
 */

#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#include "timer.h"

struct ActuatorParam
{
    float calMagnitude;     //nominal actuator force vector magnitude during calibration phase
    float calSpeed;         //actuator speed in calibration phase
};

class Actuator
{
public:
    virtual ~Actuator() {}
    virtual void initialize();
    virtual bool calibrate(ActuatorParam& parameters);
    virtual void action();
protected:
    Timer callTimer;
};


#endif /* INC_ACTUATOR_H_ */
