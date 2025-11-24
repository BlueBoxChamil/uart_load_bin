/*
  ******************************************************************************
  * @file    driver_can.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   SD controller HAL module driver.
  *          This file provides firmware functions to manage the 
  *          Controller Area Network (CAN) peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/************************************************************************************
 * @fn      can_init
 *
 * @brief   Initializes the CAN peripheral according to the specified 
 *          parameters in the struct_CANInit_t
 *
 * @param   hcan: CAN handle.
 */
void can_init(CAN_HandleTypeDef *hcan)
{
    /* Initialization start */
    __CAN_INIT_START(hcan->CANx);
    __CAN_CHANGE_ENABLE(hcan->CANx);
    
    /* Timestamp enable */
    __CAN_SET_TIMESTAMP_SELECT_INTERNAL(hcan->CANx);

    /* Bit Rate Prescaler */
    __CAN_SET_NOMINAL_RATE_PRESCALER(hcan->CANx, hcan->Init.Prescaler);
    /* Data time segment1/segment2 */
    __CAN_SET_NOMINAL_TIME_SEG_1(hcan->CANx, hcan->Init.TimeSeg1);
    __CAN_SET_NOMINAL_TIME_SEG_2(hcan->CANx, hcan->Init.TimeSeg2);
    /* Synchronization Jump Width */
    __CAN_SET_NOMINAL_SYNC_WIDTH(hcan->CANx, hcan->Init.SyncJumpWidth);

    /* Bus Monitor Mode */
    hcan->CANx->CCCtrl.MON = hcan->Init.BusMonitorMode ? 1 : 0;
    /* Auto Retransmission mode */
    hcan->CANx->CCCtrl.DAR = hcan->Init.AutoRetransmission ? 0 : 1;
    /* default transmit Pause enable */
    hcan->CANx->CCCtrl.TXP = 1;

    hcan->CANx->CCCtrl.BRSE = CAN_FUNC_DISABLE;

    /* Global Filter Config */
    __CAN_STANDARD_REJECT_UNMATCHED_FRAME(hcan->CANx);
    __CAN_EXTENDED_REJECT_UNMATCHED_FRAME(hcan->CANx);
    __CAN_STANDARD_FILTER_REMOTE_FRAME(hcan->CANx);
    __CAN_EXTENDED_FILTER_REMOTE_FRAME(hcan->CANx);

    /* Tx Buffer Transmission Occurred enable */
    __CAN_Tx_OCCURRED_INT_ENABLE(hcan->CANx, 0xFFFFFFFF);

    /* Int status enable */
    __CAN_INT_LINE_ENABLE(hcan->CANx);
}

/************************************************************************************
 * @fn      can_message_ram_init
 *
 * @brief   initialize  standard ID filter / extended ID filter /
 *                      Tx queue Buffer    /
 *                      Rx FIFO 0/1        / Rx Buffer
 *                      element size, Nums and start address.
 *
 * @param   hcan: CAN handle.
 */
void can_message_ram_init(CAN_HandleTypeDef *hcan)
{
    uint16_t lu16_AddrOffset; 
    uint16_t lu16_ElementSize;

    /* Set message ram high 16bit address */
    __CAN_SET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx, hcan->RAMConfig.StartAddress >> 16);

    /* ---------------------------------------------*/
    /* ----- standard ID filter Buffer config ------*/
    /* ---------------------------------------------*/
    /* Calculate Start address */
    lu16_AddrOffset = hcan->RAMConfig.StartAddress;
    /* Set the number of standard message ID filter element */
    __CAN_SET_STANDARD_ID_FILTER_LIST_NUMS(hcan->CANx, hcan->RAMConfig.StandardIDFilterNums);
    /* Set the start address of standard Message ID filter list */
    __CAN_SET_STANDARD_ID_FILTER_LIST_START_ADDRESS(hcan->CANx, lu16_AddrOffset);

    /* ---------------------------------------------*/
    /* ----- extended ID filter Buffer config ------*/
    /* ---------------------------------------------*/
    /* Calculate offset address */
    lu16_AddrOffset += hcan->RAMConfig.StandardIDFilterNums * 4;
    /* Set the number of extended message ID filter element */
    __CAN_SET_EXTENDED_ID_FILTER_LIST_NUMS(hcan->CANx, hcan->RAMConfig.ExtendedIDFilterNums);
    /* Set the start address of extended Message ID filter list */
    __CAN_SET_EXTENDED_ID_FILTER_LIST_START_ADDRESS(hcan->CANx, lu16_AddrOffset);

    /* ---------------------------------------------*/
    /* ---------- Tx queue Buffer congfig ----------*/
    /* ---------------------------------------------*/
    /* Calculate offset address */
    lu16_AddrOffset += hcan->RAMConfig.ExtendedIDFilterNums * 8;
    /* Tx queue operation */
    __CAN_SET_Tx_QUEUE_OPERATION(hcan->CANx);
    /* Set the Number of Tx Queue element */
    __CAN_SET_Tx_FIFO_QUEUE_NUMS(hcan->CANx, hcan->RAMConfig.TxQueueNums);
    __CAN_SET_Tx_BUFFER_NUMS(hcan->CANx, 0);
    /* Set the start address of Tx Queue section in Message RAM */
    __CAN_SET_Tx_BUFFER_START_ADDRESS(hcan->CANx, lu16_AddrOffset);

     /* Tx queue  element size  /
        Rx FIFO0  element size  /
        Rx FIFO1  element size  /
        Rx Buffer element size */
    lu16_ElementSize = 16;
    /* classical CAN fixed 16byte */

    /* Record element size */
    hcan->ElementSize = lu16_ElementSize;
        
    /* ---------------------------------------------*/
    /* ------------- Rx FIFO 0 congfig -------------*/
    /* ---------------------------------------------*/
    /* Calculate offset address */
    lu16_AddrOffset += hcan->RAMConfig.TxQueueNums * lu16_ElementSize;

    /* FIFO 0 blocking mode */
    __CAN_SET_Rx_FIFO0_BLOCKING_MODE(hcan->CANx);
    /* Set the Number of Rx FIFO0 element */
    __CAN_SET_Rx_FIFO0_NUMS(hcan->CANx, hcan->RAMConfig.RxFIFO0Nums);
    /* Set the start address of Rx FIFO 0 section in Message RAM */
    __CAN_SET_Rx_FIFO0_START_ADDRESS(hcan->CANx, lu16_AddrOffset);
    
    /* ---------------------------------------------*/
    /* ------------- Rx FIFO 1 congfig -------------*/
    /* ---------------------------------------------*/
    /* Calculate offset address */
    lu16_AddrOffset += hcan->RAMConfig.RxFIFO0Nums * lu16_ElementSize;

    /* FIFO 1 blocking mode */
    __CAN_SET_Rx_FIFO1_BLOCKING_MODE(hcan->CANx);
    /* Set the Number of Rx FIFO1 element */
    __CAN_SET_Rx_FIFO1_NUMS(hcan->CANx, hcan->RAMConfig.RxFIFO1Nums);
    /* Set the start address of Rx FIFO 1 section in Message RAM */
    __CAN_SET_Rx_FIFO1_START_ADDRESS(hcan->CANx, lu16_AddrOffset);

    /* ---------------------------------------------*/
    /* -------- Rx dedicated buffer congfig --------*/
    /* ---------------------------------------------*/
    /* Calculate offset address */
    lu16_AddrOffset += hcan->RAMConfig.RxFIFO1Nums * lu16_ElementSize;

    /* Set the start address of Rx dedicated buffer section in Message RAM */
    __CAN_SET_Rx_BUFFER_START_ADDRESS(hcan->CANx, lu16_AddrOffset);


    /* Initialization stop */
    __CAN_CHANGE_DISABLE(hcan->CANx);
    __CAN_INIT_STOP(hcan->CANx);
}

/************************************************************************************
 * @fn      can_ram_watch_dog_config
 *
 * @brief   ram watch dog config.
 *
 * @param   fu8_InitValue: Start value of the Message RAM Watchdog Counter.
 *                         if the value config '00' the counter is disabled.
 */
void can_ram_watch_dog_config(CAN_HandleTypeDef *hcan, uint8_t fu8_InitValue)
{
    /* Initialization start */
    __CAN_INIT_START(hcan->CANx);
    __CAN_CHANGE_ENABLE(hcan->CANx);
    __CAM_SET_RAM_WATCHDOG_INITIAL_VALUE(hcan->CANx, fu8_InitValue);
    /* Initialization stop */
    __CAN_CHANGE_DISABLE(hcan->CANx);
    __CAN_INIT_STOP(hcan->CANx);
}

/************************************************************************************
 * @fn      can_get_ram_watch_dog_value
 *
 * @brief   get ram watch dog counter value.
 */
uint8_t can_get_ram_watch_dog_value(CAN_HandleTypeDef *hcan)
{
    return __CAM_GET_RAM_WATCHDOG_VALUE(hcan->CANx);
}

/************************************************************************************
 * @fn      can_timestamp_Prescaler_config
 *
 * @brief   Configures the timestamp unit in multiples of CAN bit times 1 ~ 16.
 *
 * @param   fu8_prescaler: prescaler value can be 1 ~ 16.
 */
void can_timestamp_prescaler_config(CAN_HandleTypeDef *hcan, uint8_t fu8_prescaler)
{
    /* Initialization start */
    __CAN_INIT_START(hcan->CANx);
    __CAN_CHANGE_ENABLE(hcan->CANx);
    __CAN_SET_TIMESTAMP_PRESCALER(hcan->CANx, fu8_prescaler);
    /* Initialization stop */
    __CAN_CHANGE_DISABLE(hcan->CANx);
    __CAN_INIT_STOP(hcan->CANx);
}

/************************************************************************************
 * @fn      can_timestamp_counter_reset
 *
 * @brief   timestamp counter reset 0.
 */
void can_timestamp_counter_reset(CAN_HandleTypeDef *hcan)
{
    __CAN_SET_TIMESTAMP(hcan->CANx);
}

/************************************************************************************
 * @fn      can_timestamp_counter_reset
 *
 * @brief   get timestamp counter value.
 */
uint16_t can_get_timestamp_counter(CAN_HandleTypeDef *hcan)
{
    return __CAN_GET_TIMESTAMP(hcan->CANx);
}

/************************************************************************************
 * @fn      can_get_transmit_error_counter
 * @fn      can_get_receive_error_counter
 *
 * @brief   get transmit/receive error  counter value.
 */
uint8_t can_get_transmit_error_counter(CAN_HandleTypeDef *hcan)
{
    return __CAN_GET_TRANSMIT_ERROR_COUNTER(hcan->CANx);
}
uint8_t can_get_receive_error_counter(CAN_HandleTypeDef *hcan)
{
    return __CAN_GET_RECEIVE_ERROR_COUNTER(hcan->CANx);
}

/************************************************************************************
 * @fn      can_add_tx_message
 *
 * @brief   Add a message to the Tx queue.(classics can)
 *
 * @param   hcan: CAN handle.
 *          fstr_TxHeader: Tx message header.
 *          Data: Data buffer pointer.
 * @return  lu32_PutIndex > 0: Put index in Tx queue. 
 *          Error < 0.
 */
int32_t can_add_tx_message(CAN_HandleTypeDef *hcan, struct_CANTxHeaderDef_t fstr_TxHeader, uint8_t *Data)
{
    uint32_t i;

    struct_CanTxElement_t *CanTxElement;
    uint32_t lu32_Address;
    uint32_t lu32_PutIndex;
    
    /* Tx queue full */
    if (__CAN_IS_TxFIFO_QUEUE_FULL(hcan->CANx))
        return CAN_ERR_TXFIFO_FULL;

    /* the start address of Tx Queue section in Message RAM */
    lu32_Address  = __CAN_GET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx) << 16;
    lu32_Address |= __CAN_GET_Tx_BUFFER_START_ADDRESS(hcan->CANx);

    /* Calculate element offset address */
    lu32_PutIndex = __CAN_GET_TxFIFO_QUEUE_PUT_INDEX(hcan->CANx);
    lu32_Address += lu32_PutIndex * hcan->ElementSize;

    /* write param/data to message ram */
    CanTxElement = (struct_CanTxElement_t *)lu32_Address;

    /* Frame type, data or remote */
    CanTxElement->FrameCFG.XTD = fstr_TxHeader.IdType;

    if (fstr_TxHeader.IdType == CAN_ID_STANDARD) 
        CanTxElement->FrameCFG.ID = fstr_TxHeader.Identifier << 18;
    else
        CanTxElement->FrameCFG.ID = fstr_TxHeader.Identifier;

    /* Data frame */
    if (fstr_TxHeader.FrameType == CAN_DATA_FRAME) 
    {
        CanTxElement->FrameCFG.DLC = fstr_TxHeader.DLC;
        CanTxElement->FrameCFG.RTR = CAN_DATA_FRAME;

        for (i = 0; i < fstr_TxHeader.DLC; i++)
        {
            CanTxElement->Data[i] = Data[i];
        }
    }
    /* Remote frame */
    else 
    {
        CanTxElement->FrameCFG.DLC = 0;
        CanTxElement->FrameCFG.RTR = CAN_REMOTE_FRAM;
    }

    /* Add request */
    __CAN_ADD_Tx_REQUEST(hcan->CANx, 1 << lu32_PutIndex);

    return lu32_PutIndex;
}

/************************************************************************************
 * @fn      can_is_tx_message_pending
 *
 * @brief   Check if a transmission request is pending.
 *
 * @param   hcan: CAN handle.
 *          fu32_PutIndex: tx message index in Tx queue. 
 * @return  true:  tx message pending.
 *          false: tx message not pending.
 */
bool can_is_tx_message_pending(CAN_HandleTypeDef *hcan, uint32_t fu32_PutIndex)
{
    bool lb_status;

    return lb_status = (__CAN_GET_Tx_REQUEST_PENDING(hcan->CANx) & (1 << fu32_PutIndex)) ? true : false;
}

/************************************************************************************
 * @fn      can_abort_tx_message
 *
 * @brief   abort a message from the Tx queue.
 *
 * @param   hcan: CAN handle.
 *          fu32_PutIndex: tx message index in Tx queue. 
 */
void can_abort_tx_message(CAN_HandleTypeDef *hcan, uint32_t fu32_PutIndex)
{
    /* Tx Buffer Cancellation Request */
    __CAN_CANCELLATION_Tx_REQUEST(hcan->CANx, 1 << fu32_PutIndex);
}

/************************************************************************************
 * @fn      can_add_standard_filter
 *
 * @brief   Added standard filter config.
 *
 * @param   hcan: CAN handle.
 *          fstr_FilterCfg: standard filter config param.
 *          fu32_Index: filter number index.
 */
void can_add_standard_filter(CAN_HandleTypeDef *hcan, struct_FilterCfg_t fstr_FilterCfg, uint32_t fu32_Index)
{
    uint32_t lu32_Address;

    struct_StdFilterElement_t *StdFilterElement;

    /* Set the start address of standard Message ID filter list */
    lu32_Address  = __CAN_GET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx) << 16;
    lu32_Address |= __CAN_GET_STANDARD_ID_FILTER_LIST_START_ADDRESS(hcan->CANx);
    /* Calculate element offset address */
    lu32_Address += fu32_Index * 4;

    /* write param/data to message ram */
    StdFilterElement = (struct_StdFilterElement_t *)lu32_Address;

    StdFilterElement->SFT   = fstr_FilterCfg.FilterType;
    StdFilterElement->SFEC  = fstr_FilterCfg.ProcessMode;
    StdFilterElement->SFID1 = fstr_FilterCfg.StdFilterID_1;
    StdFilterElement->SFID2 = fstr_FilterCfg.StdFilterID_2;

    if (fstr_FilterCfg.ProcessMode == FILTER_PROCESS_STORE_IN_RxBUFFER) 
    {
        StdFilterElement->SFID2 = fstr_FilterCfg.RxBufferIndex;

        if (fstr_FilterCfg.RxBufferIndex < 32)
            hcan->RxBufferUsed_L |= 1 << fstr_FilterCfg.RxBufferIndex;
        else
            hcan->RxBufferUsed_H |= 1 << (fstr_FilterCfg.RxBufferIndex - 32);
    }
}

/************************************************************************************
 * @fn      can_add_extended_filter
 *
 * @brief   Added extended filter config.
 *
 * @param   hcan: CAN handle.
 *          fstr_FilterCfg: extended filter config param.
 *          fu32_Index: filter number index.
 */
void can_add_extended_filter(CAN_HandleTypeDef *hcan, struct_FilterCfg_t fstr_FilterCfg, uint32_t fu32_Index)
{
    uint32_t lu32_Address;
    
    struct_ExtFilterElement_t *ExtFilterElement;
    
    /* Set the start address of extended Message ID filter list */
    lu32_Address =  __CAN_GET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx) << 16;
    lu32_Address |= __CAN_GET_EXTENDED_ID_FILTER_LIST_START_ADDRESS(hcan->CANx);
    /* Calculate element offset address */
    lu32_Address += fu32_Index * 8;
    
    /* write param/data to message ram */
    ExtFilterElement = (struct_ExtFilterElement_t *)lu32_Address;

    ExtFilterElement->EFT   = fstr_FilterCfg.FilterType;
    ExtFilterElement->EFEC  = fstr_FilterCfg.ProcessMode;
    ExtFilterElement->EFID1 = fstr_FilterCfg.StdFilterID_1;
    ExtFilterElement->EFID2 = fstr_FilterCfg.StdFilterID_2;

    if (fstr_FilterCfg.ProcessMode == FILTER_PROCESS_STORE_IN_RxBUFFER) 
    {
        ExtFilterElement->EFID2 = fstr_FilterCfg.RxBufferIndex;

        if (fstr_FilterCfg.RxBufferIndex < 32)
            hcan->RxBufferUsed_L |= 1 << fstr_FilterCfg.RxBufferIndex;
        else
            hcan->RxBufferUsed_H |= 1 << (fstr_FilterCfg.RxBufferIndex - 32);
    }
}

/************************************************************************************
 * @fn      can_remove_standard_filter
 *
 * @brief   Remove standard filter config.
 *
 * @param   hcan: CAN handle.
 *          fu32_Index: filter number index.
 */
void can_remove_standard_filter(CAN_HandleTypeDef *hcan, uint32_t fu32_Index)
{
    uint32_t lu32_Address;

    struct_StdFilterElement_t *StdFilterElement;

    /* Set the start address of standard Message ID filter list */
    lu32_Address =  __CAN_GET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx) << 16;
    lu32_Address |= __CAN_GET_STANDARD_ID_FILTER_LIST_START_ADDRESS(hcan->CANx);
    /* Calculate element offset address */
    lu32_Address += fu32_Index * 4;

    /* write param/data to message ram */
    StdFilterElement = (struct_StdFilterElement_t *)lu32_Address;

    /* Disable selected element */
    if (StdFilterElement->SFEC == FILTER_PROCESS_STORE_IN_RxBUFFER) 
    {
        if (StdFilterElement->SFID2 < 32)
            hcan->RxBufferUsed_L &= ~(1 << StdFilterElement->SFID2);
        else
            hcan->RxBufferUsed_H &= ~(1 << (StdFilterElement->SFID2 - 32));
    }

    StdFilterElement->SFT  = CAN_FILTER_DISABLE;
    StdFilterElement->SFEC = FILTER_PROCESS_DISABLE;
}

/************************************************************************************
 * @fn      can_remove_extended_filter
 *
 * @brief   Remove extended filter config.
 *
 * @param   hcan: CAN handle.
 *          fu32_Index: filter number index.
 */
void can_remove_extended_filter(CAN_HandleTypeDef *hcan, uint32_t fu32_Index)
{
    uint32_t lu32_Address;

    struct_ExtFilterElement_t *ExtFilterElement;

    /* Set the start address of extended Message ID filter list */
    lu32_Address =  __CAN_GET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx) << 16;
    lu32_Address |= __CAN_GET_EXTENDED_ID_FILTER_LIST_START_ADDRESS(hcan->CANx);
    /* Calculate element offset address */
    lu32_Address += fu32_Index * 8;
    
    /* write param/data to message ram */
    ExtFilterElement = (struct_ExtFilterElement_t *)lu32_Address;

    /* Disable selected element */
    if (ExtFilterElement->EFEC == FILTER_PROCESS_STORE_IN_RxBUFFER) 
    {
        if (ExtFilterElement->EFID2 < 32)
            hcan->RxBufferUsed_L &= ~(1 << ExtFilterElement->EFID2);
        else
            hcan->RxBufferUsed_H &= ~(1 << (ExtFilterElement->EFID2 - 32));
    }

    ExtFilterElement->EFEC = FILTER_PROCESS_DISABLE;
}

/************************************************************************************
 * @fn      can_get_rxbuffer_message
 *
 * @brief   Get an CAN frame from the Rx buffer.
 *
 * @param   hcan: CAN handle.
 *          fu32_RxBufferIndex: The message index in the RxBuffer.
 */
void can_get_rxbuffer_message(CAN_HandleTypeDef *hcan, uint32_t fu32_RxBufferIndex, struct_CANRxHeaderDef_t *RxHeader, uint8_t *Data)
{
    uint32_t lu32_Address;

    struct_CanRxElement_t *CanRxElement;

    /* Get the start address of Rx dedicated buffer section in Message RAM */
    lu32_Address =  __CAN_GET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx) << 16;
    lu32_Address |= __CAN_GET_Rx_BUFFER_START_ADDRESS(hcan->CANx);
    /* Calculate element offset address */
    lu32_Address += fu32_RxBufferIndex * hcan->ElementSize;
    
    /* Read message param/data from RxBuffer ram */
    CanRxElement = (struct_CanRxElement_t *)lu32_Address;

    RxHeader->IdType     = CanRxElement->FrameCFG.XTD ? CAN_ID_EXTENDED : CAN_ID_STANDARD;
    RxHeader->Identifier = CanRxElement->FrameCFG.ID;
    RxHeader->FrameType  = CanRxElement->FrameCFG.RTR ? CAN_REMOTE_FRAM : CAN_DATA_FRAME;
    
    if (RxHeader->FrameType == CAN_DATA_FRAME) 
    {
        RxHeader->DLC = CanRxElement->FrameCFG.DLC;

        for (int i = 0; i < RxHeader->DLC; i++)
        {
            Data[i] = CanRxElement->Data[i];
        }
    }

    RxHeader->Timestamp = CanRxElement->FrameCFG.RXTS;

    if (CanRxElement->FrameCFG.ANMF == 0) 
    {
        RxHeader->FilterMatchIndex = CanRxElement->FrameCFG.FIDX;
    }
}

/************************************************************************************
 * @fn      can_get_rxfifo0_message
 *
 * @brief   Get an CAN frame from the Rx FIFO 0.
 *
 * @param   hcan: CAN handle.
 */
uint32_t can_get_rxfifo0_message(CAN_HandleTypeDef *hcan, struct_CANRxHeaderDef_t *RxHeader, uint8_t *Data)
{
    uint32_t lu32_Address, lu32_GetIndex;

    struct_CanRxElement_t *CanRxElement;

    /* Rx FIFO0 get index */
    lu32_GetIndex = __CAN_GET_Rx_FIFO0_GET_INDEX(hcan->CANx);

    /* Get the start address of Rx FIFO 0 section in Message RAM */
    lu32_Address =  __CAN_GET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx) << 16;
    lu32_Address |= __CAN_GET_Rx_FIFO0_START_ADDRESS(hcan->CANx);
    /* Calculate element offset address */
    lu32_Address += lu32_GetIndex * hcan->ElementSize;

    /* Read message param/data from RxBuffer ram */
    CanRxElement = (struct_CanRxElement_t *)lu32_Address;

    RxHeader->IdType     = CanRxElement->FrameCFG.XTD ? CAN_ID_EXTENDED : CAN_ID_STANDARD;
    RxHeader->FrameType  = CanRxElement->FrameCFG.RTR ? CAN_REMOTE_FRAM : CAN_DATA_FRAME;

    if (RxHeader->IdType == CAN_ID_STANDARD)
        RxHeader->Identifier = CanRxElement->FrameCFG.ID >> 18;
    else
        RxHeader->Identifier = CanRxElement->FrameCFG.ID;

    if (RxHeader->FrameType == CAN_DATA_FRAME) 
    {
        RxHeader->DLC = CanRxElement->FrameCFG.DLC;

        for (int i = 0; i < RxHeader->DLC; i++)
        {
            Data[i] = CanRxElement->Data[i];
        }
    }

    RxHeader->Timestamp = CanRxElement->FrameCFG.RXTS;

    if (CanRxElement->FrameCFG.ANMF == 0) 
    {
        RxHeader->FilterMatchIndex = CanRxElement->FrameCFG.FIDX;
    }

    /* Read message ack */
    __CAN_SET_Rx_FIFO0_ACKNOWLEDGE(hcan->CANx, lu32_GetIndex);
    
    return lu32_GetIndex;
}

/************************************************************************************
 * @fn      can_get_rxfifo1_message
 *
 * @brief   Get an CAN frame from the Rx FIFO 1.
 *
 * @param   hcan: CAN handle.
 */
uint32_t can_get_rxfifo1_message(CAN_HandleTypeDef *hcan, struct_CANRxHeaderDef_t *RxHeader, uint8_t *Data)
{
    uint32_t lu32_Address, lu32_GetIndex;

    struct_CanRxElement_t *CanRxElement;

    /* Rx FIFO1 get index */
    lu32_GetIndex = __CAN_GET_Rx_FIFO1_GET_INDEX(hcan->CANx);

    /* Get the start address of Rx FIFO 1 section in Message RAM */
    lu32_Address =  __CAN_GET_MESSAGE_RAM_HIGHH_16BIT_ADDR(hcan->CANx) << 16;
    lu32_Address |= __CAN_GET_Rx_FIFO1_START_ADDRESS(hcan->CANx);
    /* Calculate element offset address */
    lu32_Address += lu32_GetIndex * hcan->ElementSize;

    /* Read message param/data from RxBuffer ram */
    CanRxElement = (struct_CanRxElement_t *)lu32_Address;

    RxHeader->IdType     = CanRxElement->FrameCFG.XTD ? CAN_ID_EXTENDED : CAN_ID_STANDARD;
    RxHeader->FrameType  = CanRxElement->FrameCFG.RTR ? CAN_REMOTE_FRAM : CAN_DATA_FRAME;

    if (RxHeader->IdType == CAN_ID_STANDARD)
        RxHeader->Identifier = CanRxElement->FrameCFG.ID >> 18;
    else
        RxHeader->Identifier = CanRxElement->FrameCFG.ID;

    if (RxHeader->FrameType == CAN_DATA_FRAME) 
    {
        RxHeader->DLC = CanRxElement->FrameCFG.DLC;

        for (int i = 0; i < RxHeader->DLC; i++)
        {
            Data[i] = CanRxElement->Data[i];
        }
    }

    RxHeader->Timestamp = CanRxElement->FrameCFG.RXTS;

    if (CanRxElement->FrameCFG.ANMF == 0) 
    {
        RxHeader->FilterMatchIndex = CanRxElement->FrameCFG.FIDX;
    }
    
    /* Read message ack */
    __CAN_SET_Rx_FIFO1_ACKNOWLEDGE(hcan->CANx, lu32_GetIndex);

    return lu32_GetIndex;
}

/************************************************************************************
 * @fn      can_get_rxfifo0_fill_level
 *
 * @brief   Get number of elements stored in Rx FIFO 0, range 0 to 64.
 *
 * @param   hcan: CAN handle.
 */
uint32_t can_get_rxfifo0_fill_level(CAN_HandleTypeDef *hcan)
{
    return __CAN_GET_Rx_FIFO0_FILL_LEVEL(hcan->CANx);
}

/************************************************************************************
 * @fn      can_get_rxfifo1_fill_level
 *
 * @brief   Get number of elements stored in Rx FIFO 0, range 0 to 64.
 *
 * @param   hcan: CAN handle.
 */
uint32_t can_get_rxfifo1_fill_level(CAN_HandleTypeDef *hcan)
{
    return __CAN_GET_Rx_FIFO1_FILL_LEVEL(hcan->CANx);
}

/************************************************************************************
 * @fn      can_get_rxbuffer_0_31_status
 *
 * @brief   get rx buffer new data 0~31 status.
 *
 * @param   hcan: CAN handle.
 */
uint32_t can_get_rxbuffer_0_31_status(CAN_HandleTypeDef *hcan)
{
    return hcan->CANx->NewData1;
}

/************************************************************************************
 * @fn      can_get_rxbuffer_32_63_status
 *
 * @brief   get rx buffer new data 32~63 status.
 *
 * @param   hcan: CAN handle.
 */
uint32_t can_get_rxbuffer_32_63_status(CAN_HandleTypeDef *hcan)
{
    return hcan->CANx->NewData2;
}

/************************************************************************************
 * @fn      can_int_enable
 *
 * @brief   can interrupt enable.
 */
void can_int_enable(CAN_HandleTypeDef *hcan, enum_CAN_INT_Status_t fe_INT_Index)
{
    __CAN_INT_ENABLE(hcan->CANx, fe_INT_Index);
}

/************************************************************************************
 * @fn      can_int_disable
 *
 * @brief   can interrupt disable.
 */
void can_int_disable(CAN_HandleTypeDef *hcan, enum_CAN_INT_Status_t fe_INT_Index)
{
    __CAN_INT_DISABLE(hcan->CANx, fe_INT_Index);
}

/************************************************************************************
 * @fn      can_get_int_status
 *
 * @brief   get interrupt status.
 */
bool can_get_int_status(CAN_HandleTypeDef *hcan, enum_CAN_INT_Status_t fe_INT_Index)
{
    bool lb_Status = (__CAN_INT_GET_STATUS(hcan->CANx) & fe_INT_Index) ? true : false;

    return lb_Status;
}

/************************************************************************************
 * @fn      can_clear_int_status
 *
 * @brief   clear interrupt status.
 */
void can_clear_int_status(CAN_HandleTypeDef *hcan, enum_CAN_INT_Status_t fe_INT_Index)
{
    __CAN_INT_CLEAR(hcan->CANx, fe_INT_Index);
}
