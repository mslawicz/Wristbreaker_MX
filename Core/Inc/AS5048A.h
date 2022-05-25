/*
 * AS5048A.h
 *
 *  Created on: May 25, 2022
 *      Author: marci
 */

#ifndef INC_AS5048A_H_
#define INC_AS5048A_H_

#include "position_sensor.h"

class AS5048A : public PositionSensor
{
public:
    AS5048A(SPI_HandleTypeDef* pSpi, GPIO_TypeDef* csPort, uint16_t csPin);
    uint16_t getPosition() override;
private:
    SPI_HandleTypeDef* _pSpi;
    GPIO_TypeDef* _csPort;
    uint16_t _csPin;
    uint16_t _lastValidValue{0};
};


#endif /* INC_AS5048A_H_ */
