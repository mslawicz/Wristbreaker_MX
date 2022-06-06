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

AS5600::AS5600(I2C_HandleTypeDef* pI2c, bool reversed) :
    PositionSensor(reversed),
    _pI2c(pI2c)
    //_csPort(csPort),
    //_csPin(csPin)
{

}

float AS5600::getPosition()
{
    static constexpr uint32_t I2cTimeout = 1;  //in ms
    uint8_t rdReg = 0x0E;
    auto result = HAL_I2C_Master_Transmit(_pI2c, _DevAddr, &rdReg, 1, I2cTimeout);
    if(result == HAL_OK)
    {
        uint16_t angle;
        result = HAL_I2C_Master_Receive(_pI2c, _DevAddr, reinterpret_cast<uint8_t*>(&angle), 2, I2cTimeout);
        if(result == HAL_OK)
        {
            //convert from 12-bit big endian
            constexpr uint8_t _8bit = 8;
            angle = (angle << _8bit) | (angle >> _8bit);
            if(_reversed)
            {
                angle = Max12Bit - angle;
            }
            _lastValidValue = scale<uint16_t, float>(0, Max12Bit + 1, angle, 0, 1.0F);
        }
        else
        {
            LOG_ERROR_ONCE("AS5600 I2C read error " << result);
        }
    }
    else
    {
        LOG_ERROR_ONCE("AS5600 I2C write error " << result);
    }

    return _lastValidValue;
}
