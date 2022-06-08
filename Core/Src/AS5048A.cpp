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

//XXX some global buffers
uint16_t g_wrBuf = 0xFFFF;
uint16_t g_rdBuf;

AS5048A::AS5048A(SPI_HandleTypeDef* pSpi, GPIO_TypeDef* csPort, uint16_t csPin, bool reversed) :
    PositionSensor(reversed),
    _pSpi(pSpi),
    _csPort(csPort),
    _csPin(csPin)
{

}

float AS5048A::getPosition()
{
    uint16_t rdBuf = g_rdBuf;
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

    //LOG_ERROR_ONCE("AS5048A SPI error code " << result);

    return _lastValidValue;
}
