/*******************************************************************************
* File Name: USB_Bootloader_audio.c
* Version 2.12
*
* Description:
*  USB AUDIO Class request handler.
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

#if defined(USB_Bootloader_ENABLE_AUDIO_CLASS)

#include "USB_Bootloader_audio.h"
#include "USB_Bootloader_midi.h"


/***************************************
*    AUDIO Variables
***************************************/

#if defined(USB_Bootloader_ENABLE_AUDIO_STREAMING)
    volatile uint8 USB_Bootloader_currentSampleFrequency[USB_Bootloader_MAX_EP][USB_Bootloader_SAMPLE_FREQ_LEN];
    volatile uint8 USB_Bootloader_frequencyChanged;
    volatile uint8 USB_Bootloader_currentMute;
    volatile uint8 USB_Bootloader_currentVolume[USB_Bootloader_VOLUME_LEN];
    volatile uint8 USB_Bootloader_minimumVolume[] = {0x01, 0x80};
    volatile uint8 USB_Bootloader_maximumVolume[] = {0xFF, 0x7F};
    volatile uint8 USB_Bootloader_resolutionVolume[] = {0x01, 0x00};
#endif /* End USB_Bootloader_ENABLE_AUDIO_STREAMING */


/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
*    External references
***************************************/

uint8 USB_Bootloader_InitControlRead(void) ;
uint8 USB_Bootloader_InitControlWrite(void) ;
uint8 USB_Bootloader_InitNoDataControlTransfer(void) ;
uint8 USB_Bootloader_InitZeroLengthControlTransfer(void)
                                                ;

extern volatile T_USB_Bootloader_EP_CTL_BLOCK USB_Bootloader_EP[];
extern volatile T_USB_Bootloader_TD USB_Bootloader_currentTD;


/***************************************
*    Private function prototypes
***************************************/


/*******************************************************************************
* Function Name: USB_Bootloader_DispatchAUDIOClassRqst
********************************************************************************
*
* Summary:
*  This routine dispatches class requests
*
* Parameters:
*  None.
*
* Return:
*  requestHandled
*
* Global variables:
*   USB_Bootloader_currentSampleFrequency: Contains the current audio Sample
*       Frequency. It is set by the Host using SET_CUR request to the endpoint.
*   USB_Bootloader_frequencyChanged: This variable is used as a flag for the
*       user code, to be aware that Host has been sent request for changing
*       Sample Frequency. Sample frequency will be sent on the next OUT
*       transaction. It is contains endpoint address when set. The following
*       code is recommended for detecting new Sample Frequency in main code:
*       if((USB_Bootloader_frequencyChanged != 0) &&
*       (USB_Bootloader_transferState == USB_Bootloader_TRANS_STATE_IDLE))
*       {
*          USB_Bootloader_frequencyChanged = 0;
*       }
*       USB_Bootloader_transferState variable is checked to be sure that
*             transfer completes.
*   USB_Bootloader_currentMute: Contains mute configuration set by Host.
*   USB_Bootloader_currentVolume: Contains volume level set by Host.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_DispatchAUDIOClassRqst() 
{
    uint8 requestHandled = USB_Bootloader_FALSE;
    
    #if defined(USB_Bootloader_ENABLE_AUDIO_STREAMING)
        uint8 epNumber;
        epNumber = CY_GET_REG8(USB_Bootloader_wIndexLo) & USB_Bootloader_DIR_UNUSED;
    #endif /* End USB_Bootloader_ENABLE_AUDIO_STREAMING */

    if ((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_DIR_MASK) == USB_Bootloader_RQST_DIR_D2H)
    {
        /* Control Read */
        if((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_RCPT_MASK) == \
                                                                                    USB_Bootloader_RQST_RCPT_EP)
        {
            /* Endpoint */
            switch (CY_GET_REG8(USB_Bootloader_bRequest))
            {
                case USB_Bootloader_GET_CUR:
                #if defined(USB_Bootloader_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_SAMPLING_FREQ_CONTROL)
                    {
                         /* Endpoint Control Selector is Sampling Frequency */
                        USB_Bootloader_currentTD.wCount = USB_Bootloader_SAMPLE_FREQ_LEN;
                        USB_Bootloader_currentTD.pData  = USB_Bootloader_currentSampleFrequency[epNumber];
                        requestHandled   = USB_Bootloader_InitControlRead();
                    }
                #endif /* End USB_Bootloader_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_READ_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else if((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_RCPT_MASK) == \
                                                                                    USB_Bootloader_RQST_RCPT_IFC)
        {
            /* Interface or Entity ID */
            switch (CY_GET_REG8(USB_Bootloader_bRequest))
            {
                case USB_Bootloader_GET_CUR:
                #if defined(USB_Bootloader_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_MUTE_CONTROL)
                    {
                         /* Entity ID Control Selector is MUTE */
                        USB_Bootloader_currentTD.wCount = 1;
                        USB_Bootloader_currentTD.pData  = &USB_Bootloader_currentMute;
                        requestHandled   = USB_Bootloader_InitControlRead();
                    }
                    else if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USB_Bootloader_currentTD.wCount = USB_Bootloader_VOLUME_LEN;
                        USB_Bootloader_currentTD.pData  = USB_Bootloader_currentVolume;
                        requestHandled   = USB_Bootloader_InitControlRead();
                    }
                    else
                    {
                    }
                    break;
                case USB_Bootloader_GET_MIN:    /* GET_MIN */
                    if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USB_Bootloader_currentTD.wCount = USB_Bootloader_VOLUME_LEN;
                        USB_Bootloader_currentTD.pData  = &USB_Bootloader_minimumVolume[0];
                        requestHandled   = USB_Bootloader_InitControlRead();
                    }
                    break;
                case USB_Bootloader_GET_MAX:    /* GET_MAX */
                    if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_VOLUME_CONTROL)
                    {
                             /* Entity ID Control Selector is VOLUME, */
                        USB_Bootloader_currentTD.wCount = USB_Bootloader_VOLUME_LEN;
                        USB_Bootloader_currentTD.pData  = &USB_Bootloader_maximumVolume[0];
                        requestHandled   = USB_Bootloader_InitControlRead();
                    }
                    break;
                case USB_Bootloader_GET_RES:    /* GET_RES */
                    if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USB_Bootloader_currentTD.wCount = USB_Bootloader_VOLUME_LEN;
                        USB_Bootloader_currentTD.pData  = &USB_Bootloader_resolutionVolume[0];
                        requestHandled   = USB_Bootloader_InitControlRead();
                    }
                    break;
                /* The contents of the status message is reserved for future use.
                *  For the time being, a null packet should be returned in the data stage of the 
                *  control transfer, and the received null packet should be ACKed.
                */
                case USB_Bootloader_GET_STAT:
                        USB_Bootloader_currentTD.wCount = 0;
                        requestHandled   = USB_Bootloader_InitControlWrite();

                #endif /* End USB_Bootloader_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_WRITE_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else
        {   /* USB_Bootloader_RQST_RCPT_OTHER */
        }
    }
    else if ((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_DIR_MASK) == \
                                                                                    USB_Bootloader_RQST_DIR_H2D)
    {
        /* Control Write */
        if((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_RCPT_MASK) == \
                                                                                    USB_Bootloader_RQST_RCPT_EP)
        {
            /* Endpoint */
            switch (CY_GET_REG8(USB_Bootloader_bRequest))
            {
                case USB_Bootloader_SET_CUR:
                #if defined(USB_Bootloader_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_SAMPLING_FREQ_CONTROL)
                    {
                         /* Endpoint Control Selector is Sampling Frequency */
                        USB_Bootloader_currentTD.wCount = USB_Bootloader_SAMPLE_FREQ_LEN;
                        USB_Bootloader_currentTD.pData  = USB_Bootloader_currentSampleFrequency[epNumber];
                        requestHandled   = USB_Bootloader_InitControlWrite();
                        USB_Bootloader_frequencyChanged = epNumber;
                    }
                #endif /* End USB_Bootloader_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_SAMPLING_FREQ_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else if((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_RCPT_MASK) == \
                                                                                    USB_Bootloader_RQST_RCPT_IFC)
        {
            /* Interface or Entity ID */
            switch (CY_GET_REG8(USB_Bootloader_bRequest))
            {
                case USB_Bootloader_SET_CUR:
                #if defined(USB_Bootloader_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_MUTE_CONTROL)
                    {
                         /* Entity ID Control Selector is MUTE */
                        USB_Bootloader_currentTD.wCount = 1;
                        USB_Bootloader_currentTD.pData  = &USB_Bootloader_currentMute;
                        requestHandled   = USB_Bootloader_InitControlWrite();
                    }
                    else if(CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME */
                        USB_Bootloader_currentTD.wCount = USB_Bootloader_VOLUME_LEN;
                        USB_Bootloader_currentTD.pData  = USB_Bootloader_currentVolume;
                        requestHandled   = USB_Bootloader_InitControlWrite();
                    }
                #endif /* End USB_Bootloader_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_CONTROL_SEL_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else
        {   /* USB_Bootloader_RQST_RCPT_OTHER */
        }
    }
    else
    {   /* requestHandled is initialezed as FALSE by default */
    }

    return(requestHandled);
}


/*******************************************************************************
* Additional user functions supporting AUDIO Requests
********************************************************************************/

/* `#START AUDIO_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /* End USB_Bootloader_ENABLE_AUDIO_CLASS*/


/* [] END OF FILE */
