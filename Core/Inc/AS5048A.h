/*
 * AS5048A.h
 *
 *  Created on: May 25, 2022
 *      Author: marci
 */

#ifndef INC_AS5048A_H_
#define INC_AS5048A_H_

#include "position_sensor.h"
#include "spi_supervisor.h"

class AS5048A : public PositionSensor
{
public:
    AS5048A(SpiSupervisor& spiSupervisor, GPIO_TypeDef* csPort, uint16_t csPin, bool reversed = false);
    float getPosition() override;
private:
    SpiSupervisor& _spiSupervisor;
    GPIO_TypeDef* _csPort;
    uint16_t _csPin;
    float _lastValidValue{0};
    uint16_t _wrBuf{0xFFFF};
    uint16_t _rdBuf{0};
    volatile bool _newData{true};
};


#endif /* INC_AS5048A_H_ */
