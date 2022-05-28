/*
 * AS5048A.cpp
 *
 *  Created on: May 25, 2022
 *      Author: marci
 */

#include "AS5048A.h"

AS5048A::AS5048A(SPI_HandleTypeDef* pSpi, GPIO_TypeDef* csPort, uint16_t csPin) :
    _pSpi(pSpi),
    _csPort(csPort),
    _csPin(csPin)
{

}

float AS5048A::getPosition()
{
    constexpr float Max14Bit = 0x3FFF;
    uint16_t wrBuf = 0xFFFF;
    uint16_t rdBuf;
    HAL_GPIO_WritePin(_csPort, _csPin, GPIO_PinState::GPIO_PIN_RESET);
    auto result = HAL_SPI_TransmitReceive(_pSpi, (uint8_t*)&wrBuf, (uint8_t*)&rdBuf, 1, 2);
    HAL_GPIO_WritePin(_csPort, _csPin, GPIO_PinState::GPIO_PIN_SET);
    if(HAL_OK == result)
    {
        //check parity
        uint16_t parity = rdBuf ^ (rdBuf >> 8);
        parity ^= (parity >> 4);
        parity ^= (parity >> 2);
        parity ^= (parity >> 1);
        if(0 == (parity & 1))
        {
            _lastValidValue = (rdBuf & 0x3FFF) / Max14Bit;
        }
    }
    return _lastValidValue;
}
