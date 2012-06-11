/*******************************************************************************
* File Name: USB_Bootloader_episr.c
* Version 2.12
*
* Description:
*  Data endpoint Interrupt Service Routines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USB_Bootloader.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* External function references
***************************************/

void USB_Bootloader_InitComponent(uint8 device, uint8 mode) ;
void USB_Bootloader_ReInitComponent(void) ;
#if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u)
    void USB_Bootloader_MIDI_OUT_EP_Service(void) ;
#endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/


/***************************************
* External references
***************************************/

extern volatile T_USB_Bootloader_EP_CTL_BLOCK USB_Bootloader_EP[];
extern volatile uint8 USB_Bootloader_device;
#if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u)
    extern volatile uint8 USB_Bootloader_midi_out_ep;
    extern volatile uint8 USB_Bootloader_midi_in_ep;
    #if USB_Bootloader_MIDI_IN_BUFF_SIZE >= 256
        extern volatile uint16 USB_Bootloader_midiInPointer;     /* Input endpoint buffer pointer */
    #else
        extern volatile uint8 USB_Bootloader_midiInPointer;      /* Input endpoint buffer pointer */
    #endif /* End USB_Bootloader_MIDI_IN_BUFF_SIZE >=256 */    
#endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

#if(USB_Bootloader_EP1_ISR_REMOVE == 0u)


    /******************************************************************************
    * Function Name: USB_Bootloader_EP_1_ISR
    *******************************************************************************
    *
    * Summary:
    *  Endpoint 1 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    ******************************************************************************/
    CY_ISR(USB_Bootloader_EP_1_ISR)
    {
        /* `#START EP1_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USB_Bootloader_SIE_EP1_CR0_PTR); /* Must read the mode reg */
        /* Do not toggle ISOC endpoint */
        if((USB_Bootloader_EP[USB_Bootloader_EP1].attrib & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_ISOC)
        {
            USB_Bootloader_EP[USB_Bootloader_EP1].epToggle ^= USB_Bootloader_EPX_CNT_DATA_TOGGLE;
        }
        USB_Bootloader_EP[USB_Bootloader_EP1].apiEpState = USB_Bootloader_EVENT_PENDING;
        CY_SET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR, CY_GET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR)
                                                                            & ~USB_Bootloader_SIE_EP_INT_EP1_MASK);

        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u) && \
            (USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO) 
            if(USB_Bootloader_midi_out_ep == USB_Bootloader_EP1)
            {
                USB_Bootloader_MIDI_OUT_EP_Service();
            }   
        #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

        /* `#START EP1_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_1__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif   /* End USB_Bootloader_EP1_ISR_REMOVE */


#if(USB_Bootloader_EP2_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USB_Bootloader_EP_2_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 2 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USB_Bootloader_EP_2_ISR)
    {
        /* `#START EP2_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USB_Bootloader_SIE_EP2_CR0_PTR); /* Must read the mode reg */
        /* Do not toggle ISOC endpoint */
        if((USB_Bootloader_EP[USB_Bootloader_EP2].attrib & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_ISOC)
        {
            USB_Bootloader_EP[USB_Bootloader_EP2].epToggle ^= USB_Bootloader_EPX_CNT_DATA_TOGGLE;
        }
        USB_Bootloader_EP[USB_Bootloader_EP2].apiEpState = USB_Bootloader_EVENT_PENDING;
        CY_SET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR, CY_GET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR)
                                                                        & ~USB_Bootloader_SIE_EP_INT_EP2_MASK);

        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u) && \
            (USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO)
            if(USB_Bootloader_midi_out_ep == USB_Bootloader_EP2)
            {
                USB_Bootloader_MIDI_OUT_EP_Service();
            }   
        #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

        /* `#START EP2_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_2__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif   /* End USB_Bootloader_EP2_ISR_REMOVE */


#if(USB_Bootloader_EP3_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USB_Bootloader_EP_3_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 3 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USB_Bootloader_EP_3_ISR)
    {
        /* `#START EP3_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USB_Bootloader_SIE_EP3_CR0_PTR); /* Must read the mode reg */
        /* Do not toggle ISOC endpoint */
        if((USB_Bootloader_EP[USB_Bootloader_EP3].attrib & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_ISOC)
        {
            USB_Bootloader_EP[USB_Bootloader_EP3].epToggle ^= USB_Bootloader_EPX_CNT_DATA_TOGGLE;
        }
        USB_Bootloader_EP[USB_Bootloader_EP3].apiEpState = USB_Bootloader_EVENT_PENDING;
        CY_SET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR, CY_GET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR)
                                                                        & ~USB_Bootloader_SIE_EP_INT_EP3_MASK);

        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u) && \
            (USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO) 
            if(USB_Bootloader_midi_out_ep == USB_Bootloader_EP3)
            {
                USB_Bootloader_MIDI_OUT_EP_Service();
            }   
        #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

        /* `#START EP3_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_3__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif   /* End USB_Bootloader_EP3_ISR_REMOVE */


#if(USB_Bootloader_EP4_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USB_Bootloader_EP_4_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 4 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USB_Bootloader_EP_4_ISR)
    {
        /* `#START EP4_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USB_Bootloader_SIE_EP4_CR0_PTR); /* Must read the mode reg */
        /* Do not toggle ISOC endpoint */
        if((USB_Bootloader_EP[USB_Bootloader_EP4].attrib & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_ISOC)
        {
            USB_Bootloader_EP[USB_Bootloader_EP4].epToggle ^= USB_Bootloader_EPX_CNT_DATA_TOGGLE;
        }
        USB_Bootloader_EP[USB_Bootloader_EP4].apiEpState = USB_Bootloader_EVENT_PENDING;
        CY_SET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR, CY_GET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR)
                                                                        & ~USB_Bootloader_SIE_EP_INT_EP4_MASK);

        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u) && \
            (USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO) 
            if(USB_Bootloader_midi_out_ep == USB_Bootloader_EP4)
            {
                USB_Bootloader_MIDI_OUT_EP_Service();
            }   
        #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

        /* `#START EP4_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_4__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif   /* End USB_Bootloader_EP4_ISR_REMOVE */


#if(USB_Bootloader_EP5_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USB_Bootloader_EP_5_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 5 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USB_Bootloader_EP_5_ISR)
    {
        /* `#START EP5_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USB_Bootloader_SIE_EP5_CR0_PTR); /* Must read the mode reg */
        /* Do not toggle ISOC endpoint */
        if((USB_Bootloader_EP[USB_Bootloader_EP5].attrib & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_ISOC)
        {
            USB_Bootloader_EP[USB_Bootloader_EP5].epToggle ^= USB_Bootloader_EPX_CNT_DATA_TOGGLE;
        }
        USB_Bootloader_EP[USB_Bootloader_EP5].apiEpState = USB_Bootloader_EVENT_PENDING;
        CY_SET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR, CY_GET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR)
                                                                        & ~USB_Bootloader_SIE_EP_INT_EP5_MASK);

        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u) && \
            (USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO) 
            if(USB_Bootloader_midi_out_ep == USB_Bootloader_EP5)
            {
                USB_Bootloader_MIDI_OUT_EP_Service();
            }   
        #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

        /* `#START EP5_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_5__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }
#endif   /* End USB_Bootloader_EP5_ISR_REMOVE */


#if(USB_Bootloader_EP6_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USB_Bootloader_EP_6_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 6 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USB_Bootloader_EP_6_ISR)
    {
        /* `#START EP6_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USB_Bootloader_SIE_EP6_CR0_PTR); /* Must read the mode reg */
        /* Do not toggle ISOC endpoint */
        if((USB_Bootloader_EP[USB_Bootloader_EP6].attrib & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_ISOC)
        {
            USB_Bootloader_EP[USB_Bootloader_EP6].epToggle ^= USB_Bootloader_EPX_CNT_DATA_TOGGLE;
        }
        USB_Bootloader_EP[USB_Bootloader_EP6].apiEpState = USB_Bootloader_EVENT_PENDING;
        CY_SET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR, CY_GET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR)
                                                                        & ~USB_Bootloader_SIE_EP_INT_EP6_MASK);

        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u) && \
            (USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO) 
            if(USB_Bootloader_midi_out_ep == USB_Bootloader_EP6)
            {
                USB_Bootloader_MIDI_OUT_EP_Service();
            }   
        #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

        /* `#START EP6_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_6__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif   /* End USB_Bootloader_EP6_ISR_REMOVE */


#if(USB_Bootloader_EP7_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USB_Bootloader_EP_7_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 7 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USB_Bootloader_EP_7_ISR)
    {
        /* `#START EP7_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USB_Bootloader_SIE_EP7_CR0_PTR); /* Must read the mode reg */
        /* Do not toggle ISOC endpoint */
        if((USB_Bootloader_EP[USB_Bootloader_EP7].attrib & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_ISOC)
        {
            USB_Bootloader_EP[USB_Bootloader_EP7].epToggle ^= USB_Bootloader_EPX_CNT_DATA_TOGGLE;
        }
        USB_Bootloader_EP[USB_Bootloader_EP7].apiEpState = USB_Bootloader_EVENT_PENDING;
        CY_SET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR, CY_GET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR)
                                                                        & ~USB_Bootloader_SIE_EP_INT_EP7_MASK);

        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u) && \
            (USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO) 
            if(USB_Bootloader_midi_out_ep == USB_Bootloader_EP7)
            {
                USB_Bootloader_MIDI_OUT_EP_Service();
            }   
        #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

        /* `#START EP7_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_7__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif   /* End USB_Bootloader_EP7_ISR_REMOVE */


#if(USB_Bootloader_EP8_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USB_Bootloader_EP_8_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 8 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USB_Bootloader_EP_8_ISR)
    {
        /* `#START EP8_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USB_Bootloader_SIE_EP8_CR0_PTR); /* Must read the mode reg */
        /* Do not toggle ISOC endpoint */
        if((USB_Bootloader_EP[USB_Bootloader_EP8].attrib & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_ISOC)
        {
            USB_Bootloader_EP[USB_Bootloader_EP8].epToggle ^= USB_Bootloader_EPX_CNT_DATA_TOGGLE;
        }
        USB_Bootloader_EP[USB_Bootloader_EP8].apiEpState = USB_Bootloader_EVENT_PENDING;
        CY_SET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR, CY_GET_REG8(USB_Bootloader_SIE_EP_INT_SR_PTR)
                                                                        & ~USB_Bootloader_SIE_EP_INT_EP8_MASK);

        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u) && \
            (USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO) 
            if(USB_Bootloader_midi_out_ep == USB_Bootloader_EP8)
            {
                USB_Bootloader_MIDI_OUT_EP_Service();
            }   
        #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/

        /* `#START EP8_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_8__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif   /* End USB_Bootloader_EP8_ISR_REMOVE */


/*******************************************************************************
* Function Name: USB_Bootloader_SOF_ISR
********************************************************************************
*
* Summary:
*  Start of Frame Interrupt Service Routine
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USB_Bootloader_SOF_ISR)
{
    /* `#START SOF_USER_CODE` Place your code here */

    /* `#END` */

    /* PSoC3 ES1, ES2 RTC ISR PATCH  */
    #if(CY_PSOC3_ES2 && (USB_Bootloader_sof_int__ES2_PATCH))
        USB_Bootloader_ISR_PATCH();
    #endif /* End CY_PSOC3_ES2*/
}


/*******************************************************************************
* Function Name: USB_Bootloader_BUS_RESET_ISR
********************************************************************************
*
* Summary:
*  USB Bus Reset Interrupt Service Routine.  Calls _Start with the same
*  parameters as the last USER call to _Start
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USB_Bootloader_BUS_RESET_ISR)
{
    /* `#START BUS_RESET_USER_CODE` Place your code here */

    /* `#END` */

    USB_Bootloader_ReInitComponent();

    /* PSoC3 ES1, ES2 RTC ISR PATCH  */
    #if(CY_PSOC3_ES2 && (USB_Bootloader_bus_reset__ES2_PATCH))
        USB_Bootloader_ISR_PATCH();
    #endif /* End CY_PSOC3_ES2*/
}


#if((USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL) && (USB_Bootloader_ARB_ISR_REMOVE == 0u))


    /*******************************************************************************
    * Function Name: USB_Bootloader_ARB_ISR
    ********************************************************************************
    *
    * Summary:
    *  Arbiter Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side effect:
    *  Search for EP8 int_status will be much slower than search for EP1 int_status.
    *
    *******************************************************************************/
    CY_ISR(USB_Bootloader_ARB_ISR)
    {
        uint8 int_status;
        uint8 ep_status;
        uint8 ep = USB_Bootloader_EP1;
        uint8 ptr = 0u;

        /* `#START ARB_BEGIN_USER_CODE` Place your code here */

        /* `#END` */

        int_status = USB_Bootloader_ARB_INT_SR_REG;                   /* read Arbiter Status Register */
        USB_Bootloader_ARB_INT_SR_REG = int_status;                   /* Clear Serviced Interrupts */

        while(int_status != 0u)
        {
            if(int_status & 1u)  /* If EpX interrupt present */
            {
                ep_status  = USB_Bootloader_ARB_EP1_SR_PTR[ptr];       /* read Endpoint Status Register */
                /* If In Buffer Full */
                if(ep_status & USB_Bootloader_ARB_EPX_SR_IN_BUF_FULL)
                {
                    if(USB_Bootloader_EP[ep].addr & USB_Bootloader_DIR_IN)
                    {
                        /* Write the Mode register */
                        USB_Bootloader_SIE_EP1_CR0_PTR[ptr] = USB_Bootloader_EP[ep].epMode;
                    }
                }
                /* If DMA Grant */
                if(ep_status & USB_Bootloader_ARB_EPX_SR_DMA_GNT)
                {
                    if(USB_Bootloader_EP[ep].addr & USB_Bootloader_DIR_IN)
                    {
//                        if((USB_Bootloader_SIE_EP1_CR0_PTR[ptr] & USB_Bootloader_MODE_MASK)
//                                                                 != USB_Bootloader_EP[ep].epMode)
//                        { /* Error condition, IN_BUF_FULL ISR is missed */
//                          /* Set Data ready status, This will generate new DMA request */
//                            USB_Bootloader_ARB_EP1_CFG_PTR[ptr] &= ~USB_Bootloader_ARB_EPX_CFG_IN_DATA_RDY;
//                            USB_Bootloader_ARB_EP1_CFG_PTR[ptr] |= USB_Bootloader_ARB_EPX_CFG_IN_DATA_RDY;
//                        }
//                        else
//                        {
                            if(USB_Bootloader_ARB_EP1_CFG_PTR[ptr] & USB_Bootloader_ARB_EPX_CFG_IN_DATA_RDY)
                            {
                                USB_Bootloader_ARB_EP1_CFG_PTR[ptr] &= ~USB_Bootloader_ARB_EPX_CFG_IN_DATA_RDY;
                            }
                            #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && \
                                       (USB_Bootloader_ENABLE_MIDI_API != 0u)
                                if(ep == USB_Bootloader_midi_in_ep)
                                {   /* Clear MIDI input pointer*/
                                    USB_Bootloader_midiInPointer = 0u;
                                }
                            #endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/
//                        }
                    }
                    else
                    {
                        /* (re)arm Out EP only for mode2 */
                        USB_Bootloader_EP[ep].apiEpState = USB_Bootloader_NO_EVENT_PENDING;
                        #if(USB_Bootloader_EP_MM != USB_Bootloader__EP_DMAAUTO)
                            /* Write the Mode register */
                            USB_Bootloader_SIE_EP1_CR0_PTR[ptr] = USB_Bootloader_EP[ep].epMode;
                        #endif /* End USB_Bootloader_EP_MM */
                    }
                }

                /* `#START ARB_USER_CODE` Place your code here for handle Buffer Underflow/Overflow */

                /* `#END` */

                USB_Bootloader_ARB_EP1_SR_PTR[ptr] = ep_status;       /* Clear Serviced events */
            }
            ptr += USB_Bootloader_EPX_CNTX_ADDR_OFFSET;               /* prepare pointer for next EP */
            ep++;
            int_status >>= 1;
        }

        /* `#START ARB_END_USER_CODE` Place your code here */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (USB_Bootloader_arb_int__ES2_PATCH))
            USB_Bootloader_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif /* End USB_Bootloader_EP_MM */


/* [] END OF FILE */
