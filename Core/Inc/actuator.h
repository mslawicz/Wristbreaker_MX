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
    virtual ~Actuator() {}
    virtual void initialize();
    virtual bool calibrate();
    virtual void action();
};


#endif /* INC_ACTUATOR_H_ */
