/*******************************************************************************
* File Name: emFile_SPI0.c
* Version 2.20
*
* Description:
*  This file provides all API functionality of the SPI Master component.
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

#include "CyLib.h"
#include "emFile_SPI0.h"

#if(emFile_SPI0_InternalClockUsed)
    #include "emFile_SPI0_IntClock.h"   
#endif /* emFile_SPI0_InternalClockUsed */

#if (emFile_SPI0_TXBUFFERSIZE > 4u)

    volatile uint8 emFile_SPI0_TXBUFFER[emFile_SPI0_TXBUFFERSIZE] = {0u};
    volatile uint8 emFile_SPI0_txBufferRead = 0u;
    volatile uint8 emFile_SPI0_txBufferWrite = 0u;
    volatile uint8 emFile_SPI0_txBufferFull = 0u;
    
#endif /* emFile_SPI0_TXBUFFERSIZE > 4u */

#if (emFile_SPI0_RXBUFFERSIZE > 4u)

    volatile uint8 emFile_SPI0_RXBUFFER[emFile_SPI0_RXBUFFERSIZE] = {0u};
    volatile uint8 emFile_SPI0_rxBufferRead = 0u;
    volatile uint8 emFile_SPI0_rxBufferWrite = 0u;
    volatile uint8 emFile_SPI0_rxBufferFull = 0u;
    
#endif /* emFile_SPI0_RXBUFFERSIZE > 4u */

uint8 emFile_SPI0_initVar = 0u;

extern volatile uint8 emFile_SPI0_swStatusTx;
extern volatile uint8 emFile_SPI0_swStatusRx;


/*******************************************************************************
* Function Name: emFile_SPI0_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the 
*  interrupt service routine, configuring the bit-counter parameters and 
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_Init(void)
{    
    /* Initialize the Bit counter */
    emFile_SPI0_COUNTER_PERIOD_REG = emFile_SPI0_BITCTR_INIT;
    
    /* ISR initialization  */  
    #if(emFile_SPI0_InternalTxInterruptEnabled)
    
        CyIntDisable(emFile_SPI0_TX_ISR_NUMBER);

        /* Set the ISR to point to the emFile_SPI0_isr Interrupt. */
        CyIntSetVector(emFile_SPI0_TX_ISR_NUMBER, emFile_SPI0_TX_ISR);

        /* Set the priority. */
        CyIntSetPriority(emFile_SPI0_TX_ISR_NUMBER, emFile_SPI0_TX_ISR_PRIORITY);
        
    #endif /* emFile_SPI0_InternalTxInterruptEnabled */                                
    
    #if(emFile_SPI0_InternalRxInterruptEnabled)
    
        CyIntDisable(emFile_SPI0_RX_ISR_NUMBER);

        /* Set the ISR to point to the emFile_SPI0_isr Interrupt. */
        CyIntSetVector(emFile_SPI0_RX_ISR_NUMBER, emFile_SPI0_RX_ISR);

        /* Set the priority. */
        CyIntSetPriority(emFile_SPI0_RX_ISR_NUMBER, emFile_SPI0_RX_ISR_PRIORITY);
        
    #endif /* emFile_SPI0_InternalRxInterruptEnabled */
    
    /* Clear any stray data from the RX and TX FIFO */    
	emFile_SPI0_ClearFIFO();
	
	#if(emFile_SPI0_RXBUFFERSIZE > 4u)
    
        emFile_SPI0_rxBufferRead = 0u;
        emFile_SPI0_rxBufferWrite = 0u;

    #endif /* emFile_SPI0_RXBUFFERSIZE > 4u */
	
    #if(emFile_SPI0_TXBUFFERSIZE > 4u)
    
        emFile_SPI0_txBufferRead = 0u;
        emFile_SPI0_txBufferWrite = 0u;

    #endif /* emFile_SPI0_TXBUFFERSIZE > 4u */
    
    (void) emFile_SPI0_ReadTxStatus(); /* Clear any pending status bits */
    (void) emFile_SPI0_ReadRxStatus(); /* Clear any pending status bits */
	
	/* Configure the Initial interrupt mask */
    #if (emFile_SPI0_TXBUFFERSIZE > 4u)
        emFile_SPI0_TX_STATUS_MASK_REG  = emFile_SPI0_TX_INIT_INTERRUPTS_MASK & 
                                                ~emFile_SPI0_STS_TX_FIFO_NOT_FULL;                    
	#else /* emFile_SPI0_TXBUFFERSIZE < 4u */    
        emFile_SPI0_TX_STATUS_MASK_REG  = emFile_SPI0_TX_INIT_INTERRUPTS_MASK;       
	#endif /* emFile_SPI0_TXBUFFERSIZE > 4u */
    
    emFile_SPI0_RX_STATUS_MASK_REG  = emFile_SPI0_RX_INIT_INTERRUPTS_MASK; 
}
  
    
/*******************************************************************************
* Function Name: emFile_SPI0_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void emFile_SPI0_Enable(void) 
{             
    uint8 enableInterrupts = 0u;    
    
    enableInterrupts = CyEnterCriticalSection();
    
    emFile_SPI0_COUNTER_CONTROL_REG |= emFile_SPI0_CNTR_ENABLE;
    emFile_SPI0_TX_STATUS_ACTL_REG |= emFile_SPI0_INT_ENABLE;
    emFile_SPI0_RX_STATUS_ACTL_REG |= emFile_SPI0_INT_ENABLE;
    
    CyExitCriticalSection(enableInterrupts);
    
    #if(emFile_SPI0_InternalClockUsed)    
        emFile_SPI0_IntClock_Enable();        
    #endif /* emFile_SPI0_InternalClockUsed */
    
    #if(emFile_SPI0_InternalTxInterruptEnabled)    
        CyIntEnable(emFile_SPI0_TX_ISR_NUMBER);        
    #endif /* emFile_SPI0_InternalTxInterruptEnabled */
    
    #if(emFile_SPI0_InternalRxInterruptEnabled)    
        CyIntEnable(emFile_SPI0_RX_ISR_NUMBER);        
    #endif /* emFile_SPI0_InternalRxInterruptEnabled */
}


/*******************************************************************************
* Function Name: emFile_SPI0_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  emFile_SPI0_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_Start(void)
{       
    if(emFile_SPI0_initVar == 0u)
    {               
        emFile_SPI0_Init();
        emFile_SPI0_initVar = 1u; 
    }                       
        
    emFile_SPI0_Enable();        
}


/*******************************************************************************
* Function Name: emFile_SPI0_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void emFile_SPI0_Stop(void) 
{
    uint8 enableInterrupts = 0u;    
    
    enableInterrupts = CyEnterCriticalSection();
    
    emFile_SPI0_TX_STATUS_ACTL_REG &= ~emFile_SPI0_INT_ENABLE;
    emFile_SPI0_RX_STATUS_ACTL_REG &= ~emFile_SPI0_INT_ENABLE;
    
    CyExitCriticalSection(enableInterrupts);
    
    #if(emFile_SPI0_InternalClockUsed)    
        emFile_SPI0_IntClock_Disable();        
    #endif /* emFile_SPI0_InternalClockUsed */
    
    #if(emFile_SPI0_InternalTxInterruptEnabled)    
        CyIntDisable(emFile_SPI0_TX_ISR_NUMBER);        
    #endif /* emFile_SPI0_InternalTxInterruptEnabled */
    
    #if(emFile_SPI0_InternalRxInterruptEnabled)    
        CyIntDisable(emFile_SPI0_RX_ISR_NUMBER);        
    #endif /* emFile_SPI0_InternalRxInterruptEnabled */
}


/*******************************************************************************
* Function Name: emFile_SPI0_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void emFile_SPI0_EnableTxInt(void) 
{       
    #if(emFile_SPI0_InternalTxInterruptEnabled)    
        CyIntEnable(emFile_SPI0_TX_ISR_NUMBER);
    #endif /* emFile_SPI0_InternalTxInterruptEnabled */     
}


/*******************************************************************************
* Function Name: emFile_SPI0_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void emFile_SPI0_EnableRxInt(void) 
{       
    #if(emFile_SPI0_InternalRxInterruptEnabled)            
        CyIntEnable(emFile_SPI0_RX_ISR_NUMBER);
    #endif /* emFile_SPI0_InternalRxInterruptEnabled */     
}


/*******************************************************************************
* Function Name: emFile_SPI0_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void emFile_SPI0_DisableTxInt(void) 
{
    #if(emFile_SPI0_InternalTxInterruptEnabled)    
        CyIntDisable(emFile_SPI0_TX_ISR_NUMBER);        
    #endif /* emFile_SPI0_InternalTxInterruptEnabled */
}


/*******************************************************************************
* Function Name: emFile_SPI0_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void emFile_SPI0_DisableRxInt(void) 
{
    #if(emFile_SPI0_InternalRxInterruptEnabled)    
        CyIntDisable(emFile_SPI0_RX_ISR_NUMBER);        
    #endif /* emFile_SPI0_InternalRxInterruptEnabled */
}


/*******************************************************************************
* Function Name: emFile_SPI0_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the 
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void emFile_SPI0_SetTxInterruptMode(uint8 intSrc) 
{
    emFile_SPI0_TX_STATUS_MASK_REG  = intSrc;    
}


/*******************************************************************************
* Function Name: emFile_SPI0_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the 
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void emFile_SPI0_SetRxInterruptMode(uint8 intSrc) 
{
    emFile_SPI0_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: emFile_SPI0_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  emFile_SPI0_swStatusTx - used to store in software status register, 
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 emFile_SPI0_ReadTxStatus(void)
{
    uint8 tmpStatus = 0u;
        
    #if (emFile_SPI0_TXBUFFERSIZE > 4u)
    
        emFile_SPI0_DisableTxInt();
        
        tmpStatus = emFile_SPI0_GET_STATUS_TX(emFile_SPI0_swStatusTx);                    
        
        emFile_SPI0_swStatusTx = 0u;        
        
        /* Enable Interrupts */
        emFile_SPI0_EnableTxInt();
        
    #else /* (emFile_SPI0_TXBUFFERSIZE < 4u) */
    
        tmpStatus = emFile_SPI0_TX_STATUS_REG;
        
    #endif /* (emFile_SPI0_TXBUFFERSIZE > 4u) */
    
    return(tmpStatus);
}


/*******************************************************************************
* Function Name: emFile_SPI0_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  emFile_SPI0_swStatusRx - used to store in software Rx status register, 
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error 
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 emFile_SPI0_ReadRxStatus(void)
{
    uint8 tmpStatus = 0u;
        
    #if (emFile_SPI0_RXBUFFERSIZE > 4u)
    
        emFile_SPI0_DisableRxInt();
        
        tmpStatus = emFile_SPI0_GET_STATUS_RX(emFile_SPI0_swStatusRx);
               
        emFile_SPI0_swStatusRx = 0u;
        
        /* Enable Interrupts */
        emFile_SPI0_EnableRxInt();
        
    #else /* (emFile_SPI0_RXBUFFERSIZE < 4u) */
    
        tmpStatus = emFile_SPI0_RX_STATUS_REG;
        
    #endif /* (emFile_SPI0_RXBUFFERSIZE > 4u) */
    
    return(tmpStatus);
}


/*******************************************************************************
* Function Name: emFile_SPI0_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  emFile_SPI0_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  emFile_SPI0_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  emFile_SPI0_TXBUFFER[emFile_SPI0_TXBUFFERSIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_WriteTxData(uint8 txData)
{    
    #if(emFile_SPI0_TXBUFFERSIZE > 4u)

        int16 tmpTxBufferRead = 0u;
                
        /* Block if buffer is full, so we don't overwrite. */
        do
        {
            tmpTxBufferRead = emFile_SPI0_txBufferRead - 1u;
            if (tmpTxBufferRead < 0u)
            {
                tmpTxBufferRead = emFile_SPI0_TXBUFFERSIZE - 1u;        
            }    
        } while(tmpTxBufferRead == emFile_SPI0_txBufferWrite);               
                   
        /* Disable Interrupt to protect variables that could change on interrupt. */
        emFile_SPI0_DisableTxInt();
        
        emFile_SPI0_swStatusTx = emFile_SPI0_GET_STATUS_TX(emFile_SPI0_swStatusTx);
        
        if((emFile_SPI0_txBufferRead == emFile_SPI0_txBufferWrite) &&
            ((emFile_SPI0_swStatusTx & emFile_SPI0_STS_TX_FIFO_NOT_FULL) != 0u))
        {
            /* Add directly to the FIFO. */
            CY_SET_REG8(emFile_SPI0_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the software buffer. */
            emFile_SPI0_txBufferWrite++;
            if(emFile_SPI0_txBufferWrite >= emFile_SPI0_TXBUFFERSIZE)
            {
                emFile_SPI0_txBufferWrite = 0u;
            }   
                      
            if(emFile_SPI0_txBufferWrite == emFile_SPI0_txBufferRead)
            {
                emFile_SPI0_txBufferRead++;
                if(emFile_SPI0_txBufferRead >= emFile_SPI0_RXBUFFERSIZE)
                {
                    emFile_SPI0_txBufferRead = 0u;
                }
                emFile_SPI0_txBufferFull = 1u;
            }
            
            emFile_SPI0_TXBUFFER[emFile_SPI0_txBufferWrite] = txData;
            
            emFile_SPI0_TX_STATUS_MASK_REG |= emFile_SPI0_STS_TX_FIFO_NOT_FULL;            
        }                         
        
        /* Enable Interrupt. */
        emFile_SPI0_EnableTxInt();                        

    #else /* emFile_SPI0_TXBUFFERSIZE <= 4u */

        /* Block while FIFO is full */
        while((emFile_SPI0_TX_STATUS_REG & emFile_SPI0_STS_TX_FIFO_NOT_FULL) == 0u);
        
        /* Then write the byte */
        CY_SET_REG8(emFile_SPI0_TXDATA_PTR, txData);

    #endif /* emFile_SPI0_TXBUFFERSIZE > 4u */
}


/*******************************************************************************
* Function Name: emFile_SPI0_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  emFile_SPI0_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  emFile_SPI0_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  emFile_SPI0_RXBUFFER[emFile_SPI0_RXBUFFERSIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call 
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call 
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 emFile_SPI0_ReadRxData(void)
{
    uint8 rxData = 0u;

    #if(emFile_SPI0_RXBUFFERSIZE > 4u)
    
        /* Disable Interrupt to protect variables that could change on interrupt. */
        emFile_SPI0_DisableRxInt();
        
        if(emFile_SPI0_rxBufferRead != emFile_SPI0_rxBufferWrite)
        {      
            if(emFile_SPI0_rxBufferFull == 0u)
            {
                emFile_SPI0_rxBufferRead++;
                if(emFile_SPI0_rxBufferRead >= emFile_SPI0_RXBUFFERSIZE)
                {
                    emFile_SPI0_rxBufferRead = 0u;
                }
            }
            else
            {
                emFile_SPI0_rxBufferFull = 0u;
            }
        }    
        
        rxData = emFile_SPI0_RXBUFFER[emFile_SPI0_rxBufferRead];
                           
        /* Enable Interrupt. */
        emFile_SPI0_EnableRxInt();
    
    #else /* emFile_SPI0_RXBUFFERSIZE <= 4u */
    
        rxData = CY_GET_REG8(emFile_SPI0_RXDATA_PTR);
    
    #endif /* emFile_SPI0_RXBUFFERSIZE > 4u */

	return (rxData);
    
}


/*******************************************************************************
* Function Name: emFile_SPI0_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if 
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  emFile_SPI0_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  emFile_SPI0_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 emFile_SPI0_GetRxBufferSize(void) 
{
    uint8 size = 0u;

    #if(emFile_SPI0_RXBUFFERSIZE > 4u)
    
        /* Disable Interrupt to protect variables that could change on interrupt. */
        emFile_SPI0_DisableRxInt();
    
        if(emFile_SPI0_rxBufferRead == emFile_SPI0_rxBufferWrite)
        {
            size = 0u; /* No data in RX buffer */
        }
        else if(emFile_SPI0_rxBufferRead < emFile_SPI0_rxBufferWrite)
        {
            size = (emFile_SPI0_rxBufferWrite - emFile_SPI0_rxBufferRead);
        }
        else
        {
            size = (emFile_SPI0_RXBUFFERSIZE - emFile_SPI0_rxBufferRead) + emFile_SPI0_rxBufferWrite;
        }
    
        /* Enable interrupt. */
        emFile_SPI0_EnableRxInt();
    
    #else /* emFile_SPI0_RXBUFFERSIZE <= 4u */
    
        /* We can only know if there is data in the fifo. */
        size = ((emFile_SPI0_RX_STATUS_REG & emFile_SPI0_STS_RX_FIFO_NOT_EMPTY) == 
                 emFile_SPI0_STS_RX_FIFO_NOT_EMPTY) ? 1u : 0u;
    
    #endif /* emFile_SPI0_RXBUFFERSIZE < 4u */

    return (size);
}


/*******************************************************************************
* Function Name: emFile_SPI0_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if 
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  emFile_SPI0_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  emFile_SPI0_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  emFile_SPI0_GetTxBufferSize(void) 
{
    uint8 size = 0u;

    #if(emFile_SPI0_TXBUFFERSIZE > 4u)
    
        /* Disable Interrupt to protect variables that could change on interrupt. */
        emFile_SPI0_DisableTxInt();
    
        if(emFile_SPI0_txBufferRead == emFile_SPI0_txBufferWrite)
        {
            size = 0u;
        }
        else if(emFile_SPI0_txBufferRead < emFile_SPI0_txBufferWrite)
        {
            size = (emFile_SPI0_txBufferWrite - emFile_SPI0_txBufferRead);
        }
        else
        {
            size = (emFile_SPI0_TXBUFFERSIZE - emFile_SPI0_txBufferRead) + emFile_SPI0_txBufferWrite;
        }
    
        /* Enable Interrupt. */
        emFile_SPI0_EnableTxInt();
    
    #else /* emFile_SPI0_TXBUFFERSIZE <= 4u */
    
        size = emFile_SPI0_TX_STATUS_REG;
    
        /* Is the fifo is full. */
        if((size & emFile_SPI0_STS_TX_FIFO_EMPTY) == emFile_SPI0_STS_TX_FIFO_EMPTY)
        {
            size = 0u;
        }
        else if((size & emFile_SPI0_STS_TX_FIFO_NOT_FULL) == emFile_SPI0_STS_TX_FIFO_NOT_FULL)
        {
            size = 1u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 4u;
        }
    
    #endif /* emFile_SPI0_TXBUFFERSIZE > 4u */

    return (size);
}


/*******************************************************************************
* Function Name: emFile_SPI0_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  emFile_SPI0_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function 
*  call - resets to zero.
*  emFile_SPI0_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to 
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_ClearRxBuffer(void)
{
	/* Clear Hardware RX FIFO */
    while((!(emFile_SPI0_RX_STATUS_REG & emFile_SPI0_STS_RX_FIFO_NOT_EMPTY)) == 0u)
    {
        CY_GET_REG8(emFile_SPI0_RXDATA_PTR);
    }
	
    #if(emFile_SPI0_RXBUFFERSIZE > 4u)
    
        /* Disable interrupt to protect variables that could change on interrupt. */        
        emFile_SPI0_DisableRxInt();
    
        emFile_SPI0_rxBufferRead = 0u;
        emFile_SPI0_rxBufferWrite = 0u;
    
        /* Enable Rx interrupt. */
        emFile_SPI0_EnableRxInt();
        
    #endif /* emFile_SPI0_RXBUFFERSIZE > 4u */
}


/*******************************************************************************
* Function Name: emFile_SPI0_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  emFile_SPI0_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  emFile_SPI0_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to 
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when 
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_ClearTxBuffer(void)
{
    uint8 enableInterrupts = 0u;
    
    /* Clear Hardware TX FIFO */       
    enableInterrupts = CyEnterCriticalSection();
    
    #if(emFile_SPI0_DataWidth <= 8u)
    
        /* Clear TX FIFO */
        emFile_SPI0_AUX_CONTROL_DP0_REG |= emFile_SPI0_FIFO_CLR;
        emFile_SPI0_AUX_CONTROL_DP0_REG &= ~emFile_SPI0_FIFO_CLR;
    
    #else
    
        /* Clear TX FIFO */
        emFile_SPI0_AUX_CONTROL_DP0_REG |= emFile_SPI0_FIFO_CLR;
        emFile_SPI0_AUX_CONTROL_DP0_REG &= ~emFile_SPI0_FIFO_CLR;
        emFile_SPI0_AUX_CONTROL_DP1_REG |= emFile_SPI0_FIFO_CLR;
        emFile_SPI0_AUX_CONTROL_DP1_REG &= ~emFile_SPI0_FIFO_CLR;
        
    #endif /* emFile_SPI0_DataWidth > 8u */
    
    CyExitCriticalSection(enableInterrupts);
	
    #if(emFile_SPI0_TXBUFFERSIZE > 4u)
    
        /* Disable Interrupt to protect variables that could change on interrupt. */
        emFile_SPI0_DisableTxInt();
    
        emFile_SPI0_txBufferRead = 0u;
        emFile_SPI0_txBufferWrite = 0u;
    
        /* If Buffer is empty then disable TX FIFO status interrupt */
        emFile_SPI0_TX_STATUS_MASK_REG &= ~emFile_SPI0_STS_TX_FIFO_NOT_FULL;

        /* Enable Interrupt. */
        emFile_SPI0_EnableTxInt();
    
    #endif /* emFile_SPI0_TXBUFFERSIZE > 4u */
}


#if (emFile_SPI0_BidirectionalMode == 1u)

    /*******************************************************************************
    * Function Name: emFile_SPI0_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void emFile_SPI0_TxEnable(void) 
    {
        emFile_SPI0_CONTROL_REG |= emFile_SPI0_CTRL_TX_SIGNAL_EN;
    }
    
    
    /*******************************************************************************
    * Function Name: emFile_SPI0_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void emFile_SPI0_TxDisable(void) 
    {
        emFile_SPI0_CONTROL_REG &= ~emFile_SPI0_CTRL_TX_SIGNAL_EN;
    }
    
#endif /* emFile_SPI0_BidirectionalMode == 1u */


/*******************************************************************************
* Function Name: emFile_SPI0_PutArray
********************************************************************************                       
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available 
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void emFile_SPI0_PutArray(uint8 *buffer, uint8 byteCount)
{
    while(byteCount > 0u)
    {
        emFile_SPI0_WriteTxData(*buffer++);
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: emFile_SPI0_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void emFile_SPI0_ClearFIFO(void) 
{
    uint8 enableInterrupts = 0u;
    
    while((!(emFile_SPI0_RX_STATUS_REG & emFile_SPI0_STS_RX_FIFO_NOT_EMPTY)) == 0u)
    {
        CY_GET_REG8(emFile_SPI0_RXDATA_PTR);
    }
    
    enableInterrupts = CyEnterCriticalSection();
    
    #if(emFile_SPI0_DataWidth <= 8u)
    
        /* Clear TX FIFO */
        emFile_SPI0_AUX_CONTROL_DP0_REG |= emFile_SPI0_FIFO_CLR;
        emFile_SPI0_AUX_CONTROL_DP0_REG &= ~emFile_SPI0_FIFO_CLR;
    
    #else
    
        /* Clear TX FIFO */
        emFile_SPI0_AUX_CONTROL_DP0_REG |= emFile_SPI0_FIFO_CLR;
        emFile_SPI0_AUX_CONTROL_DP0_REG &= ~emFile_SPI0_FIFO_CLR;
        emFile_SPI0_AUX_CONTROL_DP1_REG |= emFile_SPI0_FIFO_CLR;
        emFile_SPI0_AUX_CONTROL_DP1_REG &= ~emFile_SPI0_FIFO_CLR;
        
    #endif /* emFile_SPI0_DataWidth > 8u */
    
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/

/*******************************************************************************
* Function Name: emFile_SPI0_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void emFile_SPI0_EnableInt(void) 
{       
    #if(emFile_SPI0_InternalTxInterruptEnabled)    
        CyIntEnable(emFile_SPI0_TX_ISR_NUMBER);
    #endif /* emFile_SPI0_InternalTxInterruptEnabled */                                
    
    #if(emFile_SPI0_InternalRxInterruptEnabled)           
        CyIntEnable(emFile_SPI0_RX_ISR_NUMBER);
    #endif /* emFile_SPI0_InternalRxInterruptEnabled */     
}


/*******************************************************************************
* Function Name: emFile_SPI0_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void emFile_SPI0_DisableInt(void) 
{
    #if(emFile_SPI0_InternalTxInterruptEnabled)    
        CyIntDisable(emFile_SPI0_TX_ISR_NUMBER);
    #endif /* emFile_SPI0_InternalTxInterruptEnabled */
    
    #if(emFile_SPI0_InternalRxInterruptEnabled)           
        CyIntDisable(emFile_SPI0_RX_ISR_NUMBER);
    #endif /* emFile_SPI0_InternalRxInterruptEnabled */
}


/*******************************************************************************
* Function Name: emFile_SPI0_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the 
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void emFile_SPI0_SetInterruptMode(uint8 intSrc) 
{
    emFile_SPI0_TX_STATUS_MASK_REG  = intSrc & ~(1u << emFile_SPI0_STS_SPI_IDLE_SHIFT);
    emFile_SPI0_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: emFile_SPI0_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  emFile_SPI0_swStatus - used to store in software status register, 
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 emFile_SPI0_ReadStatus(void)
{
    uint8 tmpStatus;
        
    #if ((emFile_SPI0_TXBUFFERSIZE > 4u) || (emFile_SPI0_RXBUFFERSIZE > 4u))
    
        emFile_SPI0_DisableInt();
        
        tmpStatus = (emFile_SPI0_GET_STATUS_TX(emFile_SPI0_swStatusTx) & 
                      ~(1u << emFile_SPI0_STS_SPI_IDLE_SHIFT)) | 
                      emFile_SPI0_GET_STATUS_RX(emFile_SPI0_swStatusRx);
        
        emFile_SPI0_swStatusTx = 0u;
        emFile_SPI0_swStatusRx = 0u;
        
        /* Enable Interrupts */
        emFile_SPI0_EnableInt();
        
    #else /* (emFile_SPI0_TXBUFFERSIZE < 4u) && (emFile_SPI0_RXBUFFERSIZE < 4u) */
    
        tmpStatus = (emFile_SPI0_TX_STATUS_REG & ~(1u << emFile_SPI0_STS_SPI_IDLE_SHIFT)) |
                     emFile_SPI0_RX_STATUS_REG;
        
    #endif /* (emFile_SPI0_TXBUFFERSIZE > 4u) || (emFile_SPI0_RXBUFFERSIZE > 4u) */
    
    return(tmpStatus);
}


/* [] END OF FILE */
