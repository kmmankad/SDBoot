/*******************************************************************************
* File Name: USB_Bootloader_pm.c
* Version 2.12
*
* Description:
*  This file provides Suspend/Resume APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <device.h>
#include "USB_Bootloader.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START PM_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* Local data allocation
***************************************/

static USB_Bootloader_BACKUP_STRUCT  USB_Bootloader_backup;


/***************************************
* External function references
***************************************/

#if(USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL)
    extern void USB_Bootloader_Stop_DMA(uint8 epNumber) ;
#endif   /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL */



/*******************************************************************************
* Function Name: USB_Bootloader_DP_Interrupt
********************************************************************************
*
* Summary:
*  This Interrupt Service Routine handles DP pin changes for wake-up from
*  the sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USB_Bootloader_DP_ISR)
{
    /* `#START DP_USER_CODE` Place your code here */

    /* `#END` */

    /* Clears active interrupt */
    CY_GET_REG8(USB_Bootloader_DP_INTSTAT_PTR);

    /* PSoC3 ES1, ES2 RTC ISR PATCH  */
    #if(CY_PSOC3_ES2 && (USB_Bootloader_dp_int__ES2_PATCH))
        USB_Bootloader_ISR_PATCH();
    #endif /* End CY_PSOC3_ES2*/
}


/*******************************************************************************
* Function Name: USB_Bootloader_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_SaveConfig(void) 
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: USB_Bootloader_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_RestoreConfig(void) 
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: USB_Bootloader_Suspend
********************************************************************************
*
* Summary:
*  This function disables the USBFS block and prepares for power donwn mode.
*
* Parameters:
*  None.
*
* Return:
*   None.
*
* Global variables:
*  USB_Bootloader_backup.enable:  modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_Suspend(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();


    if((CY_GET_REG8(USB_Bootloader_CR0_PTR) & USB_Bootloader_CR0_ENABLE) != 0u)
    {   /* USB block is enabled */
        USB_Bootloader_backup.enableState = 1u;

        #if(USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL)
            USB_Bootloader_Stop_DMA(USB_Bootloader_MAX_EP);     /* Stop all DMAs */
        #endif   /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL */

        #if(CY_PSOC3_ES2 || CY_PSOC5_ES1)
            /* Disable USBIO for TO3 */
            USB_Bootloader_PM_AVAIL_CR_REG &= ~USB_Bootloader_PM_AVAIL_EN_FSUSBIO;
        #endif /* End CY_PSOC3_ES2 || CY_PSOC5_ES1 */

        /* Power Down Sequencing for USBIO for TO4*/
        #if(CY_PSOC3_ES3 || CY_PSOC5_ES2)
            /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled */
            USB_Bootloader_USBIO_CR0_REG &= ~USB_Bootloader_USBIO_CR0_TEN;
            CyDelayUs(0);  /*~50ns delay */

            /* Disable the USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted) and pd_pullup_hv(Inverted) high. */
            USB_Bootloader_PM_USB_CR0_REG &= \
                                    ~(USB_Bootloader_PM_USB_CR0_PD_N | USB_Bootloader_PM_USB_CR0_PD_PULLUP_N);
         #endif /* End CY_PSOC3_ES3 || CY_PSOC5_ES2 */

        /* Disable the SIE */
        USB_Bootloader_CR0_REG &= ~USB_Bootloader_CR0_ENABLE;

        #if(CY_PSOC3_ES3 || CY_PSOC5_ES2)
            CyDelayUs(0);  /*~50ns delay */
            /* Store mode and Disable VRegulator*/
            USB_Bootloader_backup.mode = USB_Bootloader_CR1_REG & USB_Bootloader_CR1_REG_ENABLE;
            USB_Bootloader_CR1_REG &= ~USB_Bootloader_CR1_REG_ENABLE;

            CyDelayUs(1);  /* 0.5 us min delay */
            /* Disable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
            USB_Bootloader_PM_USB_CR0_REG &= ~USB_Bootloader_PM_USB_CR0_REF_EN;

            /* Switch DP and DM terminals to GPIO mode and disconnect 1.5k pullup*/
            USB_Bootloader_USBIO_CR1_REG |= USB_Bootloader_USBIO_CR1_IOMODE;

        #endif /* End CY_PSOC3_ES3 || CY_PSOC5_ES2*/

        /* Disable USB in ACT PM */
        USB_Bootloader_PM_ACT_CFG_REG &= ~USB_Bootloader_PM_ACT_EN_FSUSB;
        /* Disable USB block for Standby Power Mode */
        USB_Bootloader_PM_STBY_CFG_REG &= ~USB_Bootloader_PM_STBY_EN_FSUSB;
        CyDelayUs(1); /* min  0.5us delay required */

    }
    else
    {
        USB_Bootloader_backup.enableState = 0u;
    }
    CyExitCriticalSection(enableInterrupts);

    /* Set the DP Interrupt for wake-up from sleep mode. */
    #if(USB_Bootloader_DP_ISR_REMOVE == 0u)
        CyIntSetVector(USB_Bootloader_DP_INTC_VECT_NUM,   USB_Bootloader_DP_ISR);
        CyIntSetPriority(USB_Bootloader_DP_INTC_VECT_NUM, USB_Bootloader_DP_INTC_PRIOR);
        CyIntClearPending(USB_Bootloader_DP_INTC_VECT_NUM);
        CyIntEnable(USB_Bootloader_DP_INTC_VECT_NUM);
    #endif /* End USB_Bootloader_DP_ISR_REMOVE */

}


/*******************************************************************************
* Function Name: USB_Bootloader_Resume
********************************************************************************
*
* Summary:
*  This function enables the USBFS block after power down mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USB_Bootloader_backup - checked.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_Resume(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    if(USB_Bootloader_backup.enableState != 0u)
    {
        #if(USB_Bootloader_DP_ISR_REMOVE == 0u)
            CyIntDisable(USB_Bootloader_DP_INTC_VECT_NUM);
        #endif /* End USB_Bootloader_DP_ISR_REMOVE */

        #if(CY_PSOC3_ES2 || CY_PSOC5_ES1)
            /* Enable USBIO for TO3 */
            USB_Bootloader_PM_AVAIL_CR_REG |= USB_Bootloader_PM_AVAIL_EN_FSUSBIO;

            /* Bus Reset Length, It has correct default value in ES3 */
            USB_Bootloader_BUS_RST_CNT_REG = USB_Bootloader_BUS_RST_COUNT;
        #endif /* End CY_PSOC3_ES2 || CY_PSOC5_ES1 */

        /* Enable USB block */
        USB_Bootloader_PM_ACT_CFG_REG |= USB_Bootloader_PM_ACT_EN_FSUSB;
        /* Enable USB block for Standby Power Mode */
        USB_Bootloader_PM_STBY_CFG_REG |= USB_Bootloader_PM_STBY_EN_FSUSB;
        /* Enable core clock */
        USB_Bootloader_USB_CLK_EN_REG |= USB_Bootloader_USB_CLK_ENABLE;

        /* USBIO Wakeup Sequencing for TO4 */
        #if (CY_PSOC3_ES3 || CY_PSOC5_ES2)
            /* Enable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
            USB_Bootloader_PM_USB_CR0_REG |= USB_Bootloader_PM_USB_CR0_REF_EN;
            /* The reference will be available ~40us after power restored */
            CyDelayUs(40);
            /* Return VRegulator*/
            USB_Bootloader_CR1_REG |= USB_Bootloader_backup.mode;
            CyDelayUs(0);  /*~50ns delay */
            /* Enable USBIO */
            USB_Bootloader_PM_USB_CR0_REG |= USB_Bootloader_PM_USB_CR0_PD_N;
            CyDelayUs(2);
            /* Set the USBIO pull-up enable */
            USB_Bootloader_PM_USB_CR0_REG |= USB_Bootloader_PM_USB_CR0_PD_PULLUP_N;
        #endif /* End CY_PSOC3_ES3 || CY_PSOC5_ES2 */

        /* Reinit Arbiter congiguration for DMA transfers */
        #if(USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL)
            /* usb arb interrupt enable */
            USB_Bootloader_ARB_INT_EN_REG = USB_Bootloader_ARB_INT_MASK;
            #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAMANUAL)
                USB_Bootloader_ARB_CFG_REG = USB_Bootloader_ARB_CFG_MANUAL_DMA;
            #endif   /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAMANUAL */
            #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO)
                /*Set cfg cmplt this rises DMA request when the full configuration is done */
                USB_Bootloader_ARB_CFG_REG = USB_Bootloader_ARB_CFG_AUTO_DMA | USB_Bootloader_ARB_CFG_AUTO_MEM;
            #endif   /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO */
        #endif   /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL */

        /* STALL_IN_OUT */
        CY_SET_REG8(USB_Bootloader_EP0_CR_PTR, USB_Bootloader_MODE_STALL_IN_OUT);
        /* Enable the SIE with a last address */
        USB_Bootloader_CR0_REG |= USB_Bootloader_CR0_ENABLE;
        /* Finally, Enable d+ pullup and select iomode to USB mode*/
        CY_SET_REG8(USB_Bootloader_USBIO_CR1_PTR, USB_Bootloader_USBIO_CR1_USBPUEN);

    }
    CyExitCriticalSection(enableInterrupts);
}

#if(CY_PSOC5_ES1)


/*******************************************************************************
* Function Name: USB_Bootloader_Resume_Condition
********************************************************************************
* Summary:
*  This function enables the USBFS block after power down mode, reads the 
*  current state of the DP pin trough the USBIO block and disables the USBFS 
*  block backward when DP pin is not in low state.
 
*
* Parameters:  
*  void 
*
* Return: 
*  Zero for low level DP pin state and not zero for high level.
*  
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_Resume_Condition(void) 
{
    uint8 pinVal;
    
    CyIMO_SetFreq(CY_IMO_FREQ_USB);
    USB_Bootloader_Resume();
    
    /* Check DP pin for low level */
    pinVal = USB_Bootloader_USBIO_CR1_REG & USB_Bootloader_USBIO_CR1_DP0;
    
    /* disables the USBFS block if */
    if( pinVal != 0u)
    {
    
        USB_Bootloader_Suspend();
        CyIMO_SetFreq(CY_IMO_FREQ_3MHZ);
    }    
    
    return (pinVal);
}

#endif /* End CY_PSOC5_ES1 */


/* [] END OF FILE */
