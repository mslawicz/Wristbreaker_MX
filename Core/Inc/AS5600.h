/*
 * AS5600.h
 *
 *  Created on: 5 cze 2022
 *      Author: marci
 */

#ifndef INC_AS5600_H_
#define INC_AS5600_H_

#include "position_sensor.h"

class AS5600 : public PositionSensor
{
public:
    AS5600(I2C_HandleTypeDef* pI2c, bool reversed = false);
    float getPosition() override;
private:
    I2C_HandleTypeDef* _pI2c;
    static constexpr uint8_t _DevAddr = 0x36;
    //GPIO_TypeDef* _csPort;
    //uint16_t _csPin;
    float _lastValidValue{0};
};



#endif /* INC_AS5600_H_ */
