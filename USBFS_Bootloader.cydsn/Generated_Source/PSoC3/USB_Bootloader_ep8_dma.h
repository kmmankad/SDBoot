/******************************************************************************
* File Name: USB_Bootloader_ep8_dma.h  
* Version 1.50
*
*  Description:
*   Provides the function definitions for the DMA Controller.
*
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#if !defined(__USB_Bootloader_ep8_DMA_H__)
#define __USB_Bootloader_ep8_DMA_H__



#include <CYDMAC.H>
#include <CYFITTER.H>

#define USB_Bootloader_ep8__TD_TERMOUT_EN ((USB_Bootloader_ep8__TERMOUT0_EN ? TD_TERMOUT0_EN : 0) | \
    (USB_Bootloader_ep8__TERMOUT1_EN ? TD_TERMOUT1_EN : 0))

/* Zero based index of USB_Bootloader_ep8 dma channel */
extern uint8 USB_Bootloader_ep8_DmaHandle;


uint8 USB_Bootloader_ep8_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress);
void  USB_Bootloader_ep8_DmaRelease(void) ;


/* __USB_Bootloader_ep8_DMA_H__ */
#endif
