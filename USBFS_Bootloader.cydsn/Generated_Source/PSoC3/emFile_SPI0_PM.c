/*******************************************************************************
* File Name: emFile_SPI0_PM.c
* Version 2.20
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "emFile_SPI0.h"

static emFile_SPI0_BACKUP_STRUCT emFile_SPI0_backup = {
                                        0u,
                                        emFile_SPI0_BITCTR_INIT,
                                        #if(emFile_SPI0_PSOC3_ES2 || emFile_SPI0_PSOC5_ES1)
                                            emFile_SPI0_TX_INIT_INTERRUPTS_MASK,
                                            emFile_SPI0_RX_INIT_INTERRUPTS_MASK
                                        #endif /* emFile_SPI0_PSOC3_ES2 || emFile_SPI0_PSOC5_ES1 */
                                        };

#if (emFile_SPI0_TXBUFFERSIZE > 4u)

    extern volatile uint8 emFile_SPI0_txBufferRead;
    extern volatile uint8 emFile_SPI0_txBufferWrite;
    
#endif /* emFile_SPI0_TXBUFFERSIZE > 4u */

#if (emFile_SPI0_RXBUFFERSIZE > 4u)

    extern volatile uint8 emFile_SPI0_rxBufferRead;
    extern volatile uint8 emFile_SPI0_rxBufferWrite;
    
#endif /* emFile_SPI0_RXBUFFERSIZE > 4u */


/*******************************************************************************
* Function Name: emFile_SPI0_SaveConfig
********************************************************************************
*
* Summary:
*  Saves SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
* 
* Global Variables:
*  emFile_SPI0_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_SaveConfig(void)
{
    /* Store Status Mask registers */
    #if (emFile_SPI0_PSOC3_ES2 || emFile_SPI0_PSOC5_ES1)     
    
       emFile_SPI0_backup.saveSrTxIntMask = emFile_SPI0_TX_STATUS_MASK_REG;
       emFile_SPI0_backup.saveSrRxIntMask = emFile_SPI0_RX_STATUS_MASK_REG;
       emFile_SPI0_backup.cntrPeriod = emFile_SPI0_COUNTER_PERIOD_REG;
       
    #endif /* (emFile_SPI0_PSOC3_ES2 || emFile_SPI0_PSOC5_ES1) */
}


/*******************************************************************************
* Function Name: emFile_SPI0_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  emFile_SPI0_backup - used when non-retention registers are restored.
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer: 
*  emFile_SPI0_STATUS_MASK_REG and emFile_SPI0_COUNTER_PERIOD_REG.
*
*******************************************************************************/
void emFile_SPI0_RestoreConfig(void) 
{
    /* Restore the data, saved by SaveConfig() function */
    #if (emFile_SPI0_PSOC3_ES2 || emFile_SPI0_PSOC5_ES1)
    
        emFile_SPI0_TX_STATUS_MASK_REG = emFile_SPI0_backup.saveSrTxIntMask;
        emFile_SPI0_RX_STATUS_MASK_REG = emFile_SPI0_backup.saveSrRxIntMask;
        emFile_SPI0_COUNTER_PERIOD_REG = emFile_SPI0_backup.cntrPeriod;
        
    #endif /* (emFile_SPI0_PSOC3_ES2 || emFile_SPI0_PSOC5_ES1) */
}


/*******************************************************************************
* Function Name: emFile_SPI0_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  emFile_SPI0_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_Sleep(void)
{
    /* Save components enable state */
    if ((emFile_SPI0_TX_STATUS_ACTL_REG & emFile_SPI0_INT_ENABLE) == emFile_SPI0_INT_ENABLE)
    {
        emFile_SPI0_backup.enableState = 1u;
    }
    else /* Components block is disabled */
    {
        emFile_SPI0_backup.enableState = 0u;
    }

    emFile_SPI0_Stop();

    emFile_SPI0_SaveConfig();
}


/*******************************************************************************
* Function Name: emFile_SPI0_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  emFile_SPI0_backup - used when non-retention registers are restored.
*  emFile_SPI0_txBufferWrite - modified every function call - resets to 
*  zero.
*  emFile_SPI0_txBufferRead - modified every function call - resets to 
*  zero.
*  emFile_SPI0_rxBufferWrite - modified every function call - resets to
*  zero.
*  emFile_SPI0_rxBufferRead - modified every function call - resets to
*  zero. 
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_Wakeup(void)
{        
    emFile_SPI0_RestoreConfig();
         
    #if (emFile_SPI0_TXBUFFERSIZE > 4u)
    
        emFile_SPI0_txBufferRead = 0u;
        emFile_SPI0_txBufferWrite = 0u;
        
    #endif /* emFile_SPI0_TXBUFFERSIZE > 4u */
    
    #if (emFile_SPI0_RXBUFFERSIZE > 4u)    
    
        emFile_SPI0_rxBufferRead = 0u;
        emFile_SPI0_rxBufferWrite = 0u;
        
    #endif /* emFile_SPI0_RXBUFFERSIZE > 4u */ 
    
    emFile_SPI0_ClearFIFO();
    
    /* Restore components block enable state */
    if (emFile_SPI0_backup.enableState != 0u)
    {
         /* Components block was enabled */
         emFile_SPI0_Enable();
    } /* Do nothing if components block was disabled */
}


/* [] END OF FILE */
