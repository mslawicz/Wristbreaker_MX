/*
 * i2c_supervisor.cpp
 *
 *  Created on: Jun 10, 2022
 *      Author: marci
 */

#include "i2c_supervisor.h"
#include "main_loop_api.h"
#include "logger.h"

I2cSupervisor::I2cSupervisor(I2C_HandleTypeDef* pI2c, uint8_t maxSize) :
    _pI2c(pI2c),
    _maxSize(maxSize)
{
    i2cSupervisorMap[pI2c] = this;
}

void I2cSupervisor::transactionRequest(I2cTransParams& i2cTransParams)
{
    disableIrq();
    if(_i2cRequestQueue.size() < _maxSize)
    {
        _i2cRequestQueue.push(i2cTransParams);
    }
    else
    {
        LOG_WARNING_ONCE("I2C supervisor queue size limit reached");
    }
    if(!_isBusy)
    {
        startTransaction();
    }
    enableIrq();
}

void I2cSupervisor::startTransaction()
{
    HAL_StatusTypeDef result;
    I2cTransParams& i2cTransParams = _i2cRequestQueue.front();

    if(i2cTransParams.type == I2cTransType::Transmit)
    {
        result = HAL_I2C_Master_Transmit_IT(_pI2c, i2cTransParams.slaveAddr, i2cTransParams.pData, i2cTransParams.size);
    }
    else
    {
        result = HAL_I2C_Master_Receive_IT(_pI2c, i2cTransParams.slaveAddr, i2cTransParams.pData, i2cTransParams.size);
    }

    if(result == HAL_OK)   //start transaction
    {
        _isBusy = true;
    }
    else
    {
        _isBusy = false;
        LOG_ERROR_ONCE("I2C transaction error " << result);
    }
}

void I2cSupervisor::disableIrq()
{
    if(I2C1 == _pI2c->Instance)
    {
        HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    }
    else     if(I2C2 == _pI2c->Instance)
    {
        HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
    }
    else     if(I2C3 == _pI2c->Instance)
    {
        HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
    }
}

void I2cSupervisor::enableIrq()
{
    if(I2C1 == _pI2c->Instance)
    {
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    }
    else     if(I2C2 == _pI2c->Instance)
    {
        HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    }
    else     if(I2C3 == _pI2c->Instance)
    {
        HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
    }
}


void processI2CMasterCpltCallback(I2C_HandleTypeDef *hi2c)
{
    auto it = I2cSupervisor::i2cSupervisorMap.find(hi2c);
    if(it != I2cSupervisor::i2cSupervisorMap.end())
    {
        //I2C device found in the map
        std::queue<I2cTransParams>& i2cRequestQueue = it->second->getI2cRequestQueue();
        if(!i2cRequestQueue.empty())
        {
            auto request = i2cRequestQueue.front();
            //mark new data
            if(request.pNewData != nullptr)
            {
                *request.pNewData = true;
            }
            i2cRequestQueue.pop();
            //if the queue is not empty - start the next transaction
            if(i2cRequestQueue.empty())
            {
                it->second->markNotBusy();
            }
            else
            {
                it->second->startTransaction();
            }
        }
        else
        {
            LOG_ERROR_ONCE("no I2C transaction request in the queue");
        }
    }
    else
    {
        LOG_ERROR_ONCE("I2C supervisor object not found in the map");
    }
}

/**
  * @brief  Master Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    processI2CMasterCpltCallback(hi2c);
}

/**
  * @brief  Master Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    processI2CMasterCpltCallback(hi2c);
}

/**
  * @brief  I2C error callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    LOG_ERROR_ONCE("I2C error callback");
}
