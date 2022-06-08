/*
 * spi_supervisor.cpp
 *
 *  Created on: Jun 8, 2022
 *      Author: marci
 */


#include "spi_supervisor.h"
#include "main_loop_api.h"
#include "main.h"       //XXX is it necessary?

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
        HAL_GPIO_WritePin(ENC1_CS_GPIO_Port, ENC1_CS_Pin, GPIO_PIN_SET);    /*XXX this should be handled in a SPI supervisor class*/
    }
}
