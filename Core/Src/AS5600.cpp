/*
 * AS5600.cpp
 *
 *  Created on: 5 cze 2022
 *      Author: marci
 */


#include "AS5600.h"
#include "convert.h"
#include "constant.h"
#include "logger.h"

AS5600::AS5600(I2cSupervisor& i2cSupervisor, bool reversed) :
    PositionSensor(reversed),
    _i2cSupervisor(i2cSupervisor)
{

}

float AS5600::getPosition()
{
    if(_newData)
    {
        _newData = false;
        //convert from 12-bit big endian
        constexpr uint8_t _8bit = 8;
        uint16_t angle = (_angle << _8bit) | (_angle >> _8bit);
        if(_reversed)
        {
            angle = Max12Bit - angle;
        }
        _lastValidValue = scale<uint16_t, float>(0, Max12Bit + 1, angle, 0, 1.0F);
        //request new data from the sensor
        I2cTransParams i2cTransParams{_DevAddr, I2cTransType::Transmit, &_regAddr, 1, nullptr};
        _i2cSupervisor.transactionRequest(i2cTransParams);
        i2cTransParams = {_DevAddr, I2cTransType::Receive, reinterpret_cast<uint8_t*>(&_angle), 2, &_newData};
        _i2cSupervisor.transactionRequest(i2cTransParams);
    }
    return _lastValidValue;
}
