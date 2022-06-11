/*
 * AS5048A.cpp
 *
 *  Created on: May 25, 2022
 *      Author: marci
 */

#include "AS5048A.h"
#include "constant.h"
#include "logger.h"
#include "convert.h"

AS5048A::AS5048A(SpiSupervisor& spiSupervisor, GPIO_TypeDef* csPort, uint16_t csPin, bool reversed) :
    PositionSensor(reversed),
    _spiSupervisor(spiSupervisor),
    _csPort(csPort),
    _csPin(csPin)
{
    HAL_GPIO_WritePin(_csPort, _csPin, GPIO_PinState::GPIO_PIN_SET);  //CS not active
}

float AS5048A::getPosition()
{
    if(_newData)
    {
        _newData = false;
        uint16_t rdBuf = _rdBuf;
        //check parity
        uint16_t parity = rdBuf ^ (rdBuf >> 8);
        parity ^= (parity >> 4);
        parity ^= (parity >> 2);
        parity ^= (parity >> 1);
        if(0 == (parity & 1))
        {
            rdBuf &= Max14Bit;
            if(_reversed)
            {
                rdBuf = Max14Bit - rdBuf;
            }
            _lastValidValue = scale<uint16_t, float>(0, Max14Bit + 1, rdBuf, 0, 1.0F);
        }
        else
        {
            LOG_ERROR_ONCE("AS5048A value parity error");
        }
        //request new value from the sensor
        SpiTransParams spiTransParams{_csPort, _csPin, SpiTransType::TransmitReceive, reinterpret_cast<uint8_t*>(&_wrBuf), reinterpret_cast<uint8_t*>(&_rdBuf), 1, &_newData};
        _spiSupervisor.transactionRequest(spiTransParams);
    }
    return _lastValidValue;
}
