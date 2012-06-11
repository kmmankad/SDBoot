/***************************************************************************
* File Name: USB_Bootloader_ep8_dma.c  
* Version 1.50
*
*  Description:
*   Provides an API for the DMAC component. The API includes functions
*   for the DMA controller, DMA channels and Transfer Descriptors.
*
*
*   Note:
*     This module requires the developer to finish or fill in the auto
*     generated funcions and setup the dma channel and TD's.
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include <CYLIB.H>
#include <CYDMAC.H>
#include <USB_Bootloader_ep8_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* USB_Bootloader_ep8__DRQ_CTL_REG
* 
* 
* USB_Bootloader_ep8__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* USB_Bootloader_ep8__NUMBEROF_TDS
* 
* Priority of this channel.
* USB_Bootloader_ep8__PRIORITY
* 
* True if USB_Bootloader_ep8_TERMIN_SEL is used.
* USB_Bootloader_ep8__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* USB_Bootloader_ep8__TERMIN_SEL
* 
* 
* True if USB_Bootloader_ep8_TERMOUT0_SEL is used.
* USB_Bootloader_ep8__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* USB_Bootloader_ep8__TERMOUT0_SEL
* 
* 
* True if USB_Bootloader_ep8_TERMOUT1_SEL is used.
* USB_Bootloader_ep8__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* USB_Bootloader_ep8__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of USB_Bootloader_ep8 dma channel */
uint8 USB_Bootloader_ep8_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 USB_Bootloader_ep8_DmaInitalize
**********************************************************************
* Summary:
*   Allocates and initialises a channel of the DMAC to be used by the
*   caller.
*
* Parameters:
*   BurstCount.
*       
*       
*   ReqestPerBurst.
*       
*       
*   UpperSrcAddress.
*       
*       
*   UpperDestAddress.
*       
*
* Return:
*   The channel that can be used by the caller for DMA activity.
*   DMA_INVALID_CHANNEL (0xFF) if there are no channels left. 
*
*
*******************************************************************/
uint8 USB_Bootloader_ep8_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress)
{

    /* Allocate a DMA channel. */
    USB_Bootloader_ep8_DmaHandle = USB_Bootloader_ep8__DRQ_NUMBER;

    if(USB_Bootloader_ep8_DmaHandle != DMA_INVALID_CHANNEL)
    {
        /* Configure the channel. */
        CyDmaChSetConfiguration(USB_Bootloader_ep8_DmaHandle,
                                BurstCount,
                                ReqestPerBurst,
                                USB_Bootloader_ep8__TERMOUT0_SEL,
                                USB_Bootloader_ep8__TERMOUT1_SEL,
                                USB_Bootloader_ep8__TERMIN_SEL);

        /* Set the extended address for the transfers */
        CyDmaChSetExtendedAddress(USB_Bootloader_ep8_DmaHandle, UpperSrcAddress, UpperDestAddress);

        /* Set the priority for this channel */
        CyDmaChPriority(USB_Bootloader_ep8_DmaHandle, USB_Bootloader_ep8__PRIORITY);
    }

    return USB_Bootloader_ep8_DmaHandle;
}

/*********************************************************************
* Function Name: void USB_Bootloader_ep8_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with USB_Bootloader_ep8.
*
*
* Parameters:
*   void.
*
*
*
* Return:
*   void.
*
*******************************************************************/
void USB_Bootloader_ep8_DmaRelease(void) 
{
    /* Disable the channel, even if someone just did! */
    CyDmaChDisable(USB_Bootloader_ep8_DmaHandle);


    /* Free Transfer Descriptors. */


}

