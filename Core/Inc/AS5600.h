/*
 * AS5600.h
 *
 *  Created on: 5 cze 2022
 *      Author: marci
 */

#ifndef INC_AS5600_H_
#define INC_AS5600_H_

#include "position_sensor.h"
#include "i2c_supervisor.h"

class AS5600 : public PositionSensor
{
public:
    AS5600(I2cSupervisor& i2cSupervisor, bool reversed = false);
    float getPosition() override;
private:
    I2cSupervisor& _i2cSupervisor;
    static constexpr uint8_t _DevAddr = 0x36 << 1;
    float _lastValidValue{0};
    uint8_t _regAddr = 0x0E;
    uint16_t _angle{0};
    volatile bool _newData{true};
};



#endif /* INC_AS5600_H_ */
