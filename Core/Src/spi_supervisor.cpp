/*
 * spi_supervisor.cpp
 *
 *  Created on: Jun 8, 2022
 *      Author: marci
 */


#include "spi_supervisor.h"
#include "main_loop_api.h"
#include "main.h"       //XXX is it necessary?

SpiSupervisor::SpiSupervisor(SPI_HandleTypeDef* pSpi) :
    _pSpi(pSpi)
{

}

void SpiSupervisor::TransactionRequest(SpiTransParams& spiTransParams)
{
    disableIrq();
    _spiRequestQueue.push(spiTransParams);
    if(!_isBusy)
    {
        //CS is not active - start transaction now
        startTransaction(spiTransParams);
    }
    enableIrq();
}

void SpiSupervisor::startTransaction(SpiTransParams& spiTransParams)
{
    HAL_GPIO_WritePin(spiTransParams.csPort, spiTransParams.csPin, GPIO_PinState::GPIO_PIN_RESET);  //CS active
    if(HAL_SPI_TransmitReceive_IT(_pSpi, (uint8_t*)&spiTransParams.wrBuf, (uint8_t*)&spiTransParams.rdBuf, spiTransParams.size) == HAL_OK)   //start transaction
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
    if(hspi == pPosSensSpi)
    {
        /*position sensor SPI transmission complete*/
        //how to get data from a certain SpiSupervisor object?
    }
}
