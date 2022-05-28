/*
 * actuator.h
 *
 *  Created on: May 25, 2022
 *      Author: marci
 */

#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_


class Actuator
{
public:
    virtual void initialize();
    virtual bool calibrate();
};


#endif /* INC_ACTUATOR_H_ */