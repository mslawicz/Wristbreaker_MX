/*
 * AS5600.cpp
 *
 *  Created on: 5 cze 2022
 *      Author: marci
 */


#include "AS5600.h"

AS5600::AS5600(I2C_HandleTypeDef* pI2c, /*GPIO_TypeDef* csPort, uint16_t csPin, */bool reversed) :
    PositionSensor(reversed),
    _pI2c(pI2c)
    //_csPort(csPort),
    //_csPin(csPin)
{

}

float AS5600::getPosition()
{
//    uint16_t wrBuf = 0xFFFF;
//    uint16_t rdBuf;
//    HAL_GPIO_WritePin(_csPort, _csPin, GPIO_PinState::GPIO_PIN_RESET);
//    auto result = HAL_SPI_TransmitReceive(_pSpi, (uint8_t*)&wrBuf, (uint8_t*)&rdBuf, 1, 2);
//    HAL_GPIO_WritePin(_csPort, _csPin, GPIO_PinState::GPIO_PIN_SET);
//    if(HAL_OK == result)
//    {
//        //check parity
//        uint16_t parity = rdBuf ^ (rdBuf >> 8);
//        parity ^= (parity >> 4);
//        parity ^= (parity >> 2);
//        parity ^= (parity >> 1);
//        if(0 == (parity & 1))
//        {
//            _lastValidValue = _reversed ? (Max14Bit - (rdBuf & Max14Bit)) / Max14BitF : (rdBuf & Max14Bit) / Max14BitF;
//        }
//    }
    return _lastValidValue;
}
