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
#include <map>

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
    uint8_t* pWrBuf;
    uint8_t* pRdBuf;
    uint16_t size;
    volatile bool* pNewData;
};

class SpiSupervisor
{
public:
    SpiSupervisor(SPI_HandleTypeDef* pSpi);
    void transactionRequest(SpiTransParams& spiTransParams);
    void startTransaction();
    void markNotBusy() { _isBusy = false; }
    static inline std::map<SPI_HandleTypeDef*, SpiSupervisor*> spiSupervisorMap;
    std::queue<SpiTransParams>& getSpiRequestQueue() { return _spiRequestQueue; }
private:
    SPI_HandleTypeDef* _pSpi;
    std::queue<SpiTransParams> _spiRequestQueue;
    void disableIrq();
    void enableIrq();
    bool _isBusy{false};
};

#endif /* INC_SPI_SUPERVISOR_H_ */
