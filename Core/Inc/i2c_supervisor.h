/*
 * i2c_supervisor.h
 *
 *  Created on: Jun 10, 2022
 *      Author: marci
 */

#ifndef INC_I2C_SUPERVISOR_H_
#define INC_I2C_SUPERVISOR_H_


#include "stm32f4xx_hal.h"
#include <queue>
#include <map>

enum class I2cTransType : uint8_t
{
    Transmit,
    Receive
};

struct I2cTransParams
{
    uint8_t slaveAddr;
    I2cTransType type;
    uint8_t* pData;
    uint16_t size;
    volatile bool* pNewData;
};

class I2cSupervisor
{
public:
    I2cSupervisor(I2C_HandleTypeDef* pI2c);
    void transactionRequest(I2cTransParams& i2cTransParams);
    void startTransaction();
    void markNotBusy() { _isBusy = false; }
    static inline std::map<I2C_HandleTypeDef*, I2cSupervisor*> i2cSupervisorMap;
    std::queue<I2cTransParams>& getI2cRequestQueue() { return _i2cRequestQueue; }
private:
    I2C_HandleTypeDef* _pI2c;
    std::queue<I2cTransParams> _i2cRequestQueue;
    void disableIrq();
    void enableIrq();
    bool _isBusy{false};
};


#endif /* INC_I2C_SUPERVISOR_H_ */
