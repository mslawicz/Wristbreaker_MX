/*
 * spi_supervisor.h
 *
 *  Created on: Jun 8, 2022
 *      Author: marci
 */

#ifndef INC_SPI_SUPERVISOR_H_
#define INC_SPI_SUPERVISOR_H_

#include "stm32f4xx_hal.h"
#include <queue>

enum class SpiTransType : uint8_t
{
    Transmit,
    Receive,
    TransmitReceive
};

struct SpiTransParams
{
    GPIO_TypeDef* csPort;
    uint16_t csPin;
    SpiTransType type;
    uint8_t* wrBuf;
    uint8_t* rdBuf;
    uint16_t size;
};

class SpiSupervisor
{
public:
    SpiSupervisor(SPI_HandleTypeDef* pSpi);
    void TransactionRequest(SpiTransParams& spiTransParams);
    void startTransaction(SpiTransParams& spiTransParams);
private:
    SPI_HandleTypeDef* _pSpi;
    std::queue<SpiTransParams> _spiRequestQueue;
    void disableIrq();
    void enableIrq();
    bool _isBusy{false};
};

#endif /* INC_SPI_SUPERVISOR_H_ */
