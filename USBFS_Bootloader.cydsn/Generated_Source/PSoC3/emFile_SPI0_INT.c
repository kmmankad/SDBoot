/*******************************************************************************
* File Name: emFile_SPI0_INT.c
* Version 2.20
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "emFile_SPI0.h"

#if (emFile_SPI0_RXBUFFERSIZE > 4u)

    extern volatile uint8 emFile_SPI0_RXBUFFER[];
    extern volatile uint8 emFile_SPI0_rxBufferRead;
    extern volatile uint8 emFile_SPI0_rxBufferWrite;
    extern volatile uint8 emFile_SPI0_rxBufferFull;
    
#endif /* emFile_SPI0_RXBUFFERSIZE > 4u */

#if (emFile_SPI0_TXBUFFERSIZE > 4u)

    extern volatile uint8 emFile_SPI0_TXBUFFER[];
    extern volatile uint8 emFile_SPI0_txBufferRead;
    extern volatile uint8 emFile_SPI0_txBufferWrite;
    extern volatile uint8 emFile_SPI0_txBufferFull;

#endif /* emFile_SPI0_TXBUFFERSIZE > 4u */

volatile uint8 emFile_SPI0_swStatusTx = 0u;
volatile uint8 emFile_SPI0_swStatusRx = 0u;

/* User code required at start of ISR */
/* `#START emFile_SPI0_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: emFile_SPI0_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  emFile_SPI0_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  emFile_SPI0_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to 
*  sending and FIFO Not Full.
*  emFile_SPI0_TXBUFFER[emFile_SPI0_TXBUFFERSIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(emFile_SPI0_TX_ISR)
{     
    /* User code required at start of ISR */
    /* `#START emFile_SPI0_TX_ISR_START` */

    /* `#END` */
    
    #if((emFile_SPI0_InternalTxInterruptEnabled) && (emFile_SPI0_TXBUFFERSIZE > 4u))
                         
        /* See if TX data buffer is not empty and there is space in TX FIFO */
        while(emFile_SPI0_txBufferRead != emFile_SPI0_txBufferWrite)
        {
            emFile_SPI0_swStatusTx = emFile_SPI0_GET_STATUS_TX(emFile_SPI0_swStatusTx);
            
            if ((emFile_SPI0_swStatusTx & emFile_SPI0_STS_TX_FIFO_NOT_FULL) != 0u)
            {            
                if(emFile_SPI0_txBufferFull == 0u)
                {
                   emFile_SPI0_txBufferRead++;

                    if(emFile_SPI0_txBufferRead >= emFile_SPI0_TXBUFFERSIZE)
                    {
                        emFile_SPI0_txBufferRead = 0u;
                    }
                }
                else
                {
                    emFile_SPI0_txBufferFull = 0u;
                }
            
                /* Move data from the Buffer to the FIFO */
                CY_SET_REG8(emFile_SPI0_TXDATA_PTR,
                    emFile_SPI0_TXBUFFER[emFile_SPI0_txBufferRead]);
            }
            else
            {
                break;
            }            
        }
            
        /* Disable Interrupt on TX_fifo_not_empty if BUFFER is empty */
        if(emFile_SPI0_txBufferRead == emFile_SPI0_txBufferWrite)
        {
            emFile_SPI0_TX_STATUS_MASK_REG  &= ~emFile_SPI0_STS_TX_FIFO_NOT_FULL; 
        }                       
        
	#endif /* emFile_SPI0_InternalTxInterruptEnabled && (emFile_SPI0_TXBUFFERSIZE > 4u) */
    
    /* User code required at end of ISR (Optional) */
    /* `#START emFile_SPI0_TX_ISR_END` */

    /* `#END` */
    
    #ifdef emFile_SPI0_TxInternalInterrupt__ES2_PATCH
        #if(emFile_SPI0_PSOC3_ES2 && (emFile_SPI0_TxInternalInterrupt__ES2_PATCH))
            emFile_SPI0_TX_ISR_PATCH();
        #endif /* End PSOC3_ES2 */
    #endif /* emFile_SPI0_TxInternalInterrupt__ES2_PATCH */    
}


/*******************************************************************************
* Function Name: emFile_SPI0_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  emFile_SPI0_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  emFile_SPI0_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  emFile_SPI0_RXBUFFER[emFile_SPI0_RXBUFFERSIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(emFile_SPI0_RX_ISR)
{     
    #if((emFile_SPI0_InternalRxInterruptEnabled) && (emFile_SPI0_RXBUFFERSIZE > 4u))
        uint8 rxData = 0u; 
    #endif /* emFile_SPI0_InternalRxInterruptEnabled */  
    
    /* User code required at start of ISR */
    /* `#START emFile_SPI0_RX_ISR_START` */

    /* `#END` */
    
    #if((emFile_SPI0_InternalRxInterruptEnabled) && (emFile_SPI0_RXBUFFERSIZE > 4u))
         
        emFile_SPI0_swStatusRx = emFile_SPI0_GET_STATUS_RX(emFile_SPI0_swStatusRx);          
        
        /* See if RX data FIFO has some data and if it can be moved to the RX Buffer */
        while((emFile_SPI0_swStatusRx & emFile_SPI0_STS_RX_FIFO_NOT_EMPTY) == 
                                                                                emFile_SPI0_STS_RX_FIFO_NOT_EMPTY)
        {
            rxData = CY_GET_REG8(emFile_SPI0_RXDATA_PTR);
            
            /* Set next pointer. */
            emFile_SPI0_rxBufferWrite++;
            if(emFile_SPI0_rxBufferWrite >= emFile_SPI0_RXBUFFERSIZE)
            {
                emFile_SPI0_rxBufferWrite = 0u;
            }
            
            if(emFile_SPI0_rxBufferWrite == emFile_SPI0_rxBufferRead)
            {
                emFile_SPI0_rxBufferRead++;
                if(emFile_SPI0_rxBufferRead >= emFile_SPI0_RXBUFFERSIZE)
                {
                    emFile_SPI0_rxBufferRead = 0u;
                }
                emFile_SPI0_rxBufferFull = 1u;
            }
            
            /* Move data from the FIFO to the Buffer */
            emFile_SPI0_RXBUFFER[emFile_SPI0_rxBufferWrite] = rxData;
                
            emFile_SPI0_swStatusRx = emFile_SPI0_GET_STATUS_RX(emFile_SPI0_swStatusRx);
        }                    
        
	#endif /* emFile_SPI0_InternalRxInterruptEnabled  && (emFile_SPI0_RXBUFFERSIZE > 4u) */        
    
    /* User code required at end of ISR (Optional) */
    /* `#START emFile_SPI0_RX_ISR_END` */

    /* `#END` */
    
    #ifdef emFile_SPI0_RxInternalInterrupt__ES2_PATCH
        #if(emFile_SPI0_PSOC3_ES2 && (emFile_SPI0_RxInternalInterrupt__ES2_PATCH))
            emFile_SPI0_RX_ISR_PATCH();
        #endif /* End PSOC3_ES2 */  
    #endif /* emFile_SPI0_RxInternalInterrupt__ES2_PATCH */       
}

/* [] END OF FILE */
