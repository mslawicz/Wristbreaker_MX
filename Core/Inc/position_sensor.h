/*
 * position_sensor.h
 *
 *  Created on: May 22, 2022
 *      Author: marci
 */

#ifndef INC_POSITION_SENSOR_H_
#define INC_POSITION_SENSOR_H_

#include "stm32f4xx_hal.h"


class PositionSensor
{
public:
    PositionSensor(bool reversed) : _reversed(reversed) {}
    virtual ~PositionSensor() {}
    virtual float getPosition() { return 0; }
    virtual void requestNewValue() {}
protected:
    bool _reversed;
};


#endif /* INC_POSITION_SENSOR_H_ */
