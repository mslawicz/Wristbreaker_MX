/*
 * spi_supervisor.cpp
 *
 *  Created on: Jun 8, 2022
 *      Author: marci
 */


#include "spi_supervisor.h"
#include "main_loop_api.h"
#include "main.h"       //XXX is it necessary?
#include "logger.h"

SpiSupervisor::SpiSupervisor(SPI_HandleTypeDef* pSpi) :
    _pSpi(pSpi)
{
    spiSupervisorMap[pSpi] = this;
}

void SpiSupervisor::transactionRequest(SpiTransParams& spiTransParams)
{
    disableIrq();
    _spiRequestQueue.push(spiTransParams);
    if(!_isBusy)
    {
        //CS is not active - start transaction now
        startTransaction();
    }
    enableIrq();
}

void SpiSupervisor::startTransaction()
{
    SpiTransParams& spiTransParams = _spiRequestQueue.front();
    HAL_GPIO_WritePin(spiTransParams.csPort, spiTransParams.csPin, GPIO_PinState::GPIO_PIN_RESET);  //CS active
    if(HAL_SPI_TransmitReceive_IT(_pSpi, spiTransParams.pWrBuf, spiTransParams.pRdBuf, spiTransParams.size) == HAL_OK)   //start transaction
    {
        _isBusy = true;
    }
    else
    {
        _isBusy = false;
        HAL_GPIO_WritePin(spiTransParams.csPort, spiTransParams.csPin, GPIO_PinState::GPIO_PIN_SET);  //CS not active
    }
}

void SpiSupervisor::disableIrq()
{
    if(SPI1 == _pSpi->Instance)
    {
        HAL_NVIC_DisableIRQ(SPI1_IRQn);
    }
    else     if(SPI2 == _pSpi->Instance)
    {
        HAL_NVIC_DisableIRQ(SPI2_IRQn);
    }
    else     if(SPI3 == _pSpi->Instance)
    {
        HAL_NVIC_DisableIRQ(SPI3_IRQn);
    }
    else     if(SPI4 == _pSpi->Instance)
    {
        HAL_NVIC_DisableIRQ(SPI4_IRQn);
    }
    else     if(SPI5 == _pSpi->Instance)
    {
        HAL_NVIC_DisableIRQ(SPI5_IRQn);
    }
}

void SpiSupervisor::enableIrq()
{
    if(SPI1 == _pSpi->Instance)
    {
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }
    else     if(SPI2 == _pSpi->Instance)
    {
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }
    else     if(SPI3 == _pSpi->Instance)
    {
        HAL_NVIC_EnableIRQ(SPI3_IRQn);
    }
    else     if(SPI4 == _pSpi->Instance)
    {
        HAL_NVIC_EnableIRQ(SPI4_IRQn);
    }
    else     if(SPI5 == _pSpi->Instance)
    {
        HAL_NVIC_EnableIRQ(SPI5_IRQn);
    }
}

/**
  * @brief  Tx and Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    auto it = SpiSupervisor::spiSupervisorMap.find(hspi);
    if(it != SpiSupervisor::spiSupervisorMap.end())
    {
        //SPI device found in the map
        std::queue<SpiTransParams>& spiRequestQueue = it->second->getSpiRequestQueue();
        if(!spiRequestQueue.empty())
        {
            auto request = spiRequestQueue.front();
            HAL_GPIO_WritePin(request.csPort, request.csPin, GPIO_PinState::GPIO_PIN_SET);  //CS not active
            spiRequestQueue.pop();
            //if the queue is not empty - start the next transaction
            if(spiRequestQueue.empty())
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
            LOG_ERROR_ONCE("no SPI transaction request in the queue");
        }
    }
    else
    {
        LOG_ERROR_ONCE("SPI supervisor object not found in the map");
    }
}
