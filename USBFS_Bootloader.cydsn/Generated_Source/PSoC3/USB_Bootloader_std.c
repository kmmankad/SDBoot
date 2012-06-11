/*******************************************************************************
* File Name: USB_Bootloader_std.c
* Version 2.12
*
* Description:
*  USB Standard request handler.
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
* External references
***************************************/

extern const uint8 CYCODE USB_Bootloader_DEVICE0_DESCR[];
extern const uint8 CYCODE USB_Bootloader_DEVICE0_CONFIGURATION0_DESCR[];
extern const uint8 CYCODE USB_Bootloader_STRING_DESCRIPTORS[];
extern const uint8 CYCODE USB_Bootloader_MSOS_DESCRIPTOR[];
extern const uint8 CYCODE USB_Bootloader_SN_STRING_DESCRIPTOR[];

extern volatile uint8 USB_Bootloader_device;
extern volatile uint8 USB_Bootloader_configuration;
extern volatile uint8 USB_Bootloader_configurationChanged;
extern volatile uint8 USB_Bootloader_interfaceSetting[];
extern volatile uint8 USB_Bootloader_interfaceSetting_last[];
extern volatile uint8 USB_Bootloader_deviceAddress;
extern volatile uint8 USB_Bootloader_deviceStatus;
extern volatile uint8 USB_Bootloader_interfaceStatus[];
extern uint8 CYCODE *USB_Bootloader_interfaceClass;
extern const T_USB_Bootloader_LUT CYCODE USB_Bootloader_TABLE[];
extern volatile T_USB_Bootloader_EP_CTL_BLOCK USB_Bootloader_EP[];
extern volatile T_USB_Bootloader_TD USB_Bootloader_currentTD;
#if defined(USB_Bootloader_ENABLE_CDC_CLASS)
    extern volatile uint8 USB_Bootloader_cdc_data_in_ep;
    extern volatile uint8 USB_Bootloader_cdc_data_out_ep;
#endif  /* End USB_Bootloader_ENABLE_CDC_CLASS*/
#if defined(USB_Bootloader_ENABLE_MIDI_STREAMING)
    extern volatile uint8 USB_Bootloader_midi_in_ep;
    extern volatile uint8 USB_Bootloader_midi_out_ep;
#endif /* End USB_Bootloader_ENABLE_MIDI_STREAMING */


/***************************************
*         Forward references
***************************************/

uint8 USB_Bootloader_InitControlRead(void) ;
uint8 USB_Bootloader_InitControlWrite(void) ;
uint8 USB_Bootloader_InitNoDataControlTransfer(void) ;
uint8 USB_Bootloader_DispatchClassRqst(void) ;

void USB_Bootloader_Config(uint8 clearAltSetting) ;
void USB_Bootloader_ConfigAltChanged(void) ;
T_USB_Bootloader_LUT *USB_Bootloader_GetConfigTablePtr(uint8 c)
                                                            ;
T_USB_Bootloader_LUT *USB_Bootloader_GetDeviceTablePtr(void)
                                                            ;
uint8 USB_Bootloader_ClearEndpointHalt(void) ;
uint8 USB_Bootloader_SetEndpointHalt(void) ;
uint8 USB_Bootloader_ValidateAlternateSetting(void) ;

/*DIE ID string descriptor for 8 bytes ID*/
#if defined(USB_Bootloader_ENABLE_IDSN_STRING)
    void USB_Bootloader_ReadDieID(uint8 *descr) ;
    uint8 USB_Bootloader_idSerialNumberStringDescriptor[0x22u]={0x22u, USB_Bootloader_DESCR_STRING};
#endif /* USB_Bootloader_ENABLE_IDSN_STRING */

#if(USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL)
    void USB_Bootloader_Stop_DMA(uint8 epNumber) ;
#endif   /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL */
uint8 CYCODE *USB_Bootloader_GetInterfaceClassTablePtr(void)
                                                    ;


/***************************************
* Global data allocation
***************************************/

volatile uint8 USB_Bootloader_tBuffer[USB_Bootloader_EP_STATUS_LENGTH > USB_Bootloader_DEVICE_STATUS_LENGTH ? \
                               USB_Bootloader_EP_STATUS_LENGTH : USB_Bootloader_DEVICE_STATUS_LENGTH];
volatile uint8 *USB_Bootloader_fwSerialNumberStringDescriptor;
volatile uint8 USB_Bootloader_snStringConfirm = USB_Bootloader_FALSE;


/*******************************************************************************
* Function Name: USB_Bootloader_SerialNumString
********************************************************************************
*
* Summary:
*  Application firmware may supply the source of the USB device descriptors
*  serial number string during runtime.
*
* Parameters:
*  snString:  pointer to string.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void  USB_Bootloader_SerialNumString(uint8 *snString) 
{
    #if defined(USB_Bootloader_ENABLE_FWSN_STRING)
        USB_Bootloader_snStringConfirm = USB_Bootloader_FALSE;
        if(snString != NULL)
        {
            USB_Bootloader_fwSerialNumberStringDescriptor = snString;
            /* check descriptor validation */
            if( (USB_Bootloader_fwSerialNumberStringDescriptor[0u] > 1u ) &&  \
                (USB_Bootloader_fwSerialNumberStringDescriptor[1u] == USB_Bootloader_DESCR_STRING) )
            {
                USB_Bootloader_snStringConfirm = USB_Bootloader_TRUE;
            }
        }
    #else
        snString = snString;
    #endif  /* USB_Bootloader_ENABLE_FWSN_STRING */
}


/*******************************************************************************
* Function Name: USB_Bootloader_HandleStandardRqst
********************************************************************************
*
* Summary:
*  This Routine dispatches standard requests
*
* Parameters:
*  None.
*
* Return:
*  TRUE if request handled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_HandleStandardRqst(void) 
{
    uint8 requestHandled = USB_Bootloader_FALSE;
    #if defined(USB_Bootloader_ENABLE_STRINGS)
        volatile uint8 *pStr = 0u;
        #if defined(USB_Bootloader_ENABLE_DESCRIPTOR_STRINGS)
            uint8 nStr;
        #endif /* USB_Bootloader_ENABLE_DESCRIPTOR_STRINGS */
    #endif /* USB_Bootloader_ENABLE_STRINGS */
    uint16 count;

    T_USB_Bootloader_LUT *pTmp;
    USB_Bootloader_currentTD.count = 0u;

    if ((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_DIR_MASK) == USB_Bootloader_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USB_Bootloader_bRequest))
        {
            case USB_Bootloader_GET_DESCRIPTOR:
                if (CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_DESCR_DEVICE)
                {
                    pTmp = USB_Bootloader_GetDeviceTablePtr();
                    USB_Bootloader_currentTD.pData = pTmp->p_list;
                    USB_Bootloader_currentTD.count = USB_Bootloader_DEVICE_DESCR_LENGTH;
                    requestHandled  = USB_Bootloader_InitControlRead();
                }
                else if (CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_DESCR_CONFIG)
                {
                    pTmp = USB_Bootloader_GetConfigTablePtr(CY_GET_REG8(USB_Bootloader_wValueLo));
                    USB_Bootloader_currentTD.pData = pTmp->p_list;
                    count = ((uint16)(USB_Bootloader_currentTD.pData)[ \
                                      USB_Bootloader_CONFIG_DESCR_TOTAL_LENGTH_HI] << 8u) | \
                                     (USB_Bootloader_currentTD.pData)[USB_Bootloader_CONFIG_DESCR_TOTAL_LENGTH_LOW];
                    USB_Bootloader_currentTD.count = count;
                    requestHandled  = USB_Bootloader_InitControlRead();
                }
                #if defined(USB_Bootloader_ENABLE_STRINGS)
                else if (CY_GET_REG8(USB_Bootloader_wValueHi) == USB_Bootloader_DESCR_STRING)
                {
                    /* Descriptor Strings*/
                    #if defined(USB_Bootloader_ENABLE_DESCRIPTOR_STRINGS)
                        nStr = 0u;
                        pStr = (volatile uint8 *)&USB_Bootloader_STRING_DESCRIPTORS[0u];
                        while ( (CY_GET_REG8(USB_Bootloader_wValueLo) > nStr) && (*pStr != 0u ))
                        {
                            pStr += *pStr;
                            nStr++;
                        };
                    #endif /* End USB_Bootloader_ENABLE_DESCRIPTOR_STRINGS */
                    /* Microsoft OS String*/
                    #if defined(USB_Bootloader_ENABLE_MSOS_STRING)
                        if( CY_GET_REG8(USB_Bootloader_wValueLo) == USB_Bootloader_STRING_MSOS )
                        {
                            pStr = (volatile uint8 *)&USB_Bootloader_MSOS_DESCRIPTOR[0u];
                        }
                    #endif /* End USB_Bootloader_ENABLE_MSOS_STRING*/
                    /* SN string*/
                    #if defined(USB_Bootloader_ENABLE_SN_STRING)
                        if( (CY_GET_REG8(USB_Bootloader_wValueLo) != 0) && 
                            (CY_GET_REG8(USB_Bootloader_wValueLo) == 
                            USB_Bootloader_DEVICE0_DESCR[USB_Bootloader_DEVICE_DESCR_SN_SHIFT]) )
                        {
                            pStr = (volatile uint8 *)&USB_Bootloader_SN_STRING_DESCRIPTOR[0u];
                            if(USB_Bootloader_snStringConfirm != USB_Bootloader_FALSE)
                            {
                                pStr = USB_Bootloader_fwSerialNumberStringDescriptor;
                            }
                            #if defined(USB_Bootloader_ENABLE_IDSN_STRING)
                                /* Read DIE ID and genarete string descriptor in RAM*/
                                USB_Bootloader_ReadDieID(USB_Bootloader_idSerialNumberStringDescriptor);
                                pStr = USB_Bootloader_idSerialNumberStringDescriptor;
                            #endif    /* End USB_Bootloader_ENABLE_IDSN_STRING */
                        }
                    #endif    /* End USB_Bootloader_ENABLE_SN_STRING */
                    if (*pStr != 0u)
                    {
                        USB_Bootloader_currentTD.count = *pStr;
                        USB_Bootloader_currentTD.pData = pStr;
                        requestHandled  = USB_Bootloader_InitControlRead();
                    }
                }
                #endif /* End USB_Bootloader_ENABLE_STRINGS */
                else
                {
                    requestHandled = USB_Bootloader_DispatchClassRqst();
                }
                break;
            case USB_Bootloader_GET_STATUS:
                switch ((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_RCPT_MASK))
                {
                    case USB_Bootloader_RQST_RCPT_EP:
                        USB_Bootloader_currentTD.count = USB_Bootloader_EP_STATUS_LENGTH;
                        USB_Bootloader_tBuffer[0] = USB_Bootloader_EP[ \
                                        CY_GET_REG8(USB_Bootloader_wIndexLo) & USB_Bootloader_DIR_UNUSED].hwEpState;
                        USB_Bootloader_tBuffer[1] = 0u;
                        USB_Bootloader_currentTD.pData = &USB_Bootloader_tBuffer[0u];
                        requestHandled  = USB_Bootloader_InitControlRead();
                        break;
                    case USB_Bootloader_RQST_RCPT_DEV:
                        USB_Bootloader_currentTD.count = USB_Bootloader_DEVICE_STATUS_LENGTH;
                        USB_Bootloader_tBuffer[0u] = USB_Bootloader_deviceStatus;
                        USB_Bootloader_tBuffer[1u] = 0u;
                        USB_Bootloader_currentTD.pData = &USB_Bootloader_tBuffer[0u];
                        requestHandled  = USB_Bootloader_InitControlRead();
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            case USB_Bootloader_GET_CONFIGURATION:
                USB_Bootloader_currentTD.count = 1u;
                USB_Bootloader_currentTD.pData = (uint8 *)&USB_Bootloader_configuration;
                requestHandled  = USB_Bootloader_InitControlRead();
                break;
            case USB_Bootloader_GET_INTERFACE:
                USB_Bootloader_currentTD.count = 1u;
                USB_Bootloader_currentTD.pData = (uint8 *)&USB_Bootloader_interfaceSetting[ \
                                                                            CY_GET_REG8(USB_Bootloader_wIndexLo)];
                requestHandled  = USB_Bootloader_InitControlRead();
                break;
            default: /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    else {
        /* Control Write */
        switch (CY_GET_REG8(USB_Bootloader_bRequest))
        {
            case USB_Bootloader_SET_ADDRESS:
                USB_Bootloader_deviceAddress = CY_GET_REG8(USB_Bootloader_wValueLo);
                requestHandled = USB_Bootloader_InitNoDataControlTransfer();
                break;
            case USB_Bootloader_SET_CONFIGURATION:
                USB_Bootloader_configuration = CY_GET_REG8(USB_Bootloader_wValueLo);
                USB_Bootloader_configurationChanged = USB_Bootloader_TRUE;
                USB_Bootloader_Config(USB_Bootloader_TRUE);
                requestHandled = USB_Bootloader_InitNoDataControlTransfer();
                break;
            case USB_Bootloader_SET_INTERFACE:
                if (USB_Bootloader_ValidateAlternateSetting())
                {
                    USB_Bootloader_configurationChanged = USB_Bootloader_TRUE;
                    #if ((USB_Bootloader_EP_MA == USB_Bootloader__MA_DYNAMIC) && \
                         (USB_Bootloader_EP_MM == USB_Bootloader__EP_MANUAL) )
                        USB_Bootloader_Config(USB_Bootloader_FALSE);
                    #else
                        USB_Bootloader_ConfigAltChanged();
                    #endif /* End (USB_Bootloader_EP_MA == USB_Bootloader__MA_DYNAMIC) */

                    requestHandled = USB_Bootloader_InitNoDataControlTransfer();
                }
                break;
            case USB_Bootloader_CLEAR_FEATURE:
                switch (CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_RCPT_MASK)
                {
                    case USB_Bootloader_RQST_RCPT_EP:
                        if (CY_GET_REG8(USB_Bootloader_wValueLo) == USB_Bootloader_ENDPOINT_HALT)
                        {
                            requestHandled = USB_Bootloader_ClearEndpointHalt();
                        }
                        break;
                    case USB_Bootloader_RQST_RCPT_DEV:
                        /* Clear device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USB_Bootloader_wValueLo) == USB_Bootloader_DEVICE_REMOTE_WAKEUP)
                        {
                            USB_Bootloader_deviceStatus &= ~USB_Bootloader_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USB_Bootloader_InitNoDataControlTransfer();
                        }
                        break;
                    case USB_Bootloader_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (CY_GET_REG8(USB_Bootloader_wIndexLo) < USB_Bootloader_MAX_INTERFACES_NUMBER)
                        {
                            USB_Bootloader_interfaceStatus[CY_GET_REG8(USB_Bootloader_wIndexLo)] &= \
                                                                ~(CY_GET_REG8(USB_Bootloader_wValueLo)) ;
                            requestHandled = USB_Bootloader_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            case USB_Bootloader_SET_FEATURE:
                switch (CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_RCPT_MASK)
                {
                    case USB_Bootloader_RQST_RCPT_EP:
                        if (CY_GET_REG8(USB_Bootloader_wValueLo) == USB_Bootloader_ENDPOINT_HALT)
                        {
                            requestHandled = USB_Bootloader_SetEndpointHalt();
                        }
                        break;
                    case USB_Bootloader_RQST_RCPT_DEV:
                        /* Set device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USB_Bootloader_wValueLo) == USB_Bootloader_DEVICE_REMOTE_WAKEUP)
                        {
                            USB_Bootloader_deviceStatus |= USB_Bootloader_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USB_Bootloader_InitNoDataControlTransfer();
                        }
                        break;
                    case USB_Bootloader_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (CY_GET_REG8(USB_Bootloader_wIndexLo) < USB_Bootloader_MAX_INTERFACES_NUMBER)
                        {
                            USB_Bootloader_interfaceStatus[CY_GET_REG8(USB_Bootloader_wIndexLo)] &= \
                                                                ~(CY_GET_REG8(USB_Bootloader_wValueLo)) ;
                            requestHandled = USB_Bootloader_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            default:    /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    return(requestHandled);
}


#if defined(USB_Bootloader_ENABLE_IDSN_STRING)

    /***************************************************************************
    * Function Name: USB_Bootloader_ReadDieID
    ****************************************************************************
    *
    * Summary:
    *  This routine read Die ID and genarete Serian Number string descriptor.
    *
    * Parameters:
    *  descr:  pointer on string descriptor.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    ***************************************************************************/
    void USB_Bootloader_ReadDieID(uint8 *descr) 
    {
        uint8 i,j;
        uint8 value;
        static char8 const hex[16u] = "0123456789ABCDEF";

        /* check descriptor validation */
        if( (descr[0u] > 1u ) && (descr[1u] == USB_Bootloader_DESCR_STRING) )
        {
            /* fill descriptor */
            for(j = 0u, i = 2u; i < descr[0u]; i += 2u)
            {
                value = CY_GET_XTND_REG8((void CYFAR *)(USB_Bootloader_DIE_ID + j++));
                descr[i] = (uint8)hex[value >> 4u];
                i += 2u;
                descr[i] = (uint8)hex[value & 0x0Fu];
            }
        }
    }

#endif /* End $INSTANCE_NAME`_ENABLE_IDSN_STRING*/


/*******************************************************************************
* Function Name: USB_Bootloader_Config
********************************************************************************
*
* Summary:
*  This routine configures endpoints for the entire configuration by scanning
*  the configuration descriptor.
*
* Parameters:
*  clearAltSetting: It configures the bAlternateSetting 0 for each interface.
*
* Return:
*  None.
*
* USB_Bootloader_interfaceClass - Initialized class array for each interface.
*   It is used for hangling Class specific requests depend on interface class.
*   Different classes in multiple Alternate settings does not supported.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_Config(uint8 clearAltSetting) 
{
    uint8 ep,cur_ep,i;
    uint8 iso;
    uint16 count;
    uint8 *pDescr;
    T_USB_Bootloader_LUT *pTmp;
    T_USB_Bootloader_EP_SETTINGS_BLOCK *pEP;

    /* Clear all of the endpoints */
    for (ep = 0u; ep < USB_Bootloader_MAX_EP; ep++)
    {
        USB_Bootloader_EP[ep].attrib = 0u;
        USB_Bootloader_EP[ep].hwEpState = 0u;
        USB_Bootloader_EP[ep].apiEpState = USB_Bootloader_NO_EVENT_PENDING;
        USB_Bootloader_EP[ep].epToggle = 0u;
        USB_Bootloader_EP[ep].epMode = USB_Bootloader_MODE_DISABLE;
        USB_Bootloader_EP[ep].bufferSize = 0u;
        USB_Bootloader_EP[ep].interface = 0u;

    }

    /* Clear Alternate settings for all interfaces */
    if(clearAltSetting != 0u)
    {
        for (i = 0u; i < USB_Bootloader_MAX_INTERFACES_NUMBER; i++)
        {
            USB_Bootloader_interfaceSetting[i] = 0x00u;
            USB_Bootloader_interfaceSetting_last[i] = 0x00u;
        }
    }

    /* Init Endpoints and Device Status if configured */
    if(USB_Bootloader_configuration > 0u)
    {
        pTmp = USB_Bootloader_GetConfigTablePtr(USB_Bootloader_configuration - 1u);
        /* Set Power status for current configuration */
        pDescr = (uint8 *)pTmp->p_list;
        if((pDescr[USB_Bootloader_CONFIG_DESCR_ATTRIB] & USB_Bootloader_CONFIG_DESCR_ATTRIB_SELF_POWERED) != 0u)
        {
            USB_Bootloader_deviceStatus |=  USB_Bootloader_DEVICE_STATUS_SELF_POWERED;
        }
        else
        {
            USB_Bootloader_deviceStatus &=  ~USB_Bootloader_DEVICE_STATUS_SELF_POWERED;
        }
        pTmp++;
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        #if ((USB_Bootloader_EP_MA == USB_Bootloader__MA_DYNAMIC) && \
             (USB_Bootloader_EP_MM == USB_Bootloader__EP_MANUAL) )
            /* Config for dynamic EP memory allocation */
            /* p_list points the endpoint setting table. */
            pEP = (T_USB_Bootloader_EP_SETTINGS_BLOCK *) pTmp->p_list;
            for (i = 0u; i < ep; i++, pEP++)
            {
                /* compate current Alternate setting with EP Alt*/
                if(USB_Bootloader_interfaceSetting[pEP->interface] == pEP->altSetting)
                {
                    cur_ep = pEP->addr & USB_Bootloader_DIR_UNUSED;
                    iso  = ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) == USB_Bootloader_EP_TYPE_ISOC);
                    if (pEP->addr & USB_Bootloader_DIR_IN)
                    {
                        /* IN Endpoint */
                        USB_Bootloader_EP[cur_ep].apiEpState = USB_Bootloader_EVENT_PENDING;
                        USB_Bootloader_EP[cur_ep].epMode = \
                                                (iso ? USB_Bootloader_MODE_ISO_IN : USB_Bootloader_MODE_ACK_IN);
                        #if defined(USB_Bootloader_ENABLE_CDC_CLASS)
                            if(((pEP->bMisc == USB_Bootloader_CLASS_CDC_DATA) ||
                                (pEP->bMisc == USB_Bootloader_CLASS_CDC)) &&
                                ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) != USB_Bootloader_EP_TYPE_INT))
                            {
                                USB_Bootloader_cdc_data_in_ep = cur_ep;
                            }
                        #endif  /* End USB_Bootloader_ENABLE_CDC_CLASS*/
                        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING)
                            if((pEP->bMisc == USB_Bootloader_CLASS_AUDIO) &&
                               ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) == USB_Bootloader_EP_TYPE_BULK))
                            {
                                USB_Bootloader_midi_in_ep = cur_ep;
                            }
                        #endif  /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/
                    }
                    else
                    {
                        /* OUT Endpoint */
                        USB_Bootloader_EP[cur_ep].apiEpState = USB_Bootloader_NO_EVENT_PENDING;
                        USB_Bootloader_EP[cur_ep].epMode = \
                                                (iso ? USB_Bootloader_MODE_ISO_OUT : USB_Bootloader_MODE_ACK_OUT);
                        #if defined(USB_Bootloader_ENABLE_CDC_CLASS)
                            if(((pEP->bMisc == USB_Bootloader_CLASS_CDC_DATA) ||
                                (pEP->bMisc == USB_Bootloader_CLASS_CDC)) &&
                                ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) != USB_Bootloader_EP_TYPE_INT))
                            {
                                USB_Bootloader_cdc_data_out_ep = cur_ep;
                            }
                        #endif  /* End USB_Bootloader_ENABLE_CDC_CLASS*/
                        #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING)
                            if((pEP->bMisc == USB_Bootloader_CLASS_AUDIO) &&
                               ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) == USB_Bootloader_EP_TYPE_BULK))
                            {
                                USB_Bootloader_midi_out_ep = cur_ep;
                            }
                        #endif  /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/
                    }
                    USB_Bootloader_EP[cur_ep].bufferSize = pEP->bufferSize;
                    USB_Bootloader_EP[cur_ep].addr = pEP->addr;
                    USB_Bootloader_EP[cur_ep].attrib = pEP->attributes;
                }
            }
        #else /* Config for static EP memory allocation  */
            for (i = USB_Bootloader_EP1; i < USB_Bootloader_MAX_EP; i++)
            {
                /* and p_list points the endpoint setting table. */
                pEP = (T_USB_Bootloader_EP_SETTINGS_BLOCK *) pTmp->p_list;
                /* find max length for each EP and select it (length could be different in different Alt settings)*/
                /* but other settings should be correct with regards to Interface alt Setting */
                for (cur_ep = 0u; cur_ep < ep; cur_ep++, pEP++)
                {
                    /* EP count is equal to EP # in table and we found larger EP length than have before*/
                    if(i == (pEP->addr & USB_Bootloader_DIR_UNUSED))
                    {
                        if(USB_Bootloader_EP[i].bufferSize < pEP->bufferSize)
                        {
                            USB_Bootloader_EP[i].bufferSize = pEP->bufferSize;
                        }
                        /* compate current Alternate setting with EP Alt*/
                        if(USB_Bootloader_interfaceSetting[pEP->interface] == pEP->altSetting)
                        {
                            iso  = ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) == USB_Bootloader_EP_TYPE_ISOC);
                            if (pEP->addr & USB_Bootloader_DIR_IN)
                            {
                                /* IN Endpoint */
                                USB_Bootloader_EP[i].apiEpState = USB_Bootloader_EVENT_PENDING;
                                USB_Bootloader_EP[i].epMode =
                                                    (iso ? USB_Bootloader_MODE_ISO_IN : USB_Bootloader_MODE_ACK_IN);
                                /* Find and init CDC IN endpoint number */
                                #if defined(USB_Bootloader_ENABLE_CDC_CLASS)
                                    if(((pEP->bMisc == USB_Bootloader_CLASS_CDC_DATA) ||
                                        (pEP->bMisc == USB_Bootloader_CLASS_CDC)) &&
                                        ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                        USB_Bootloader_EP_TYPE_INT))
                                    {
                                        USB_Bootloader_cdc_data_in_ep = i;
                                    }
                                #endif  /* End USB_Bootloader_ENABLE_CDC_CLASS*/
                                #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING)
                                    if((pEP->bMisc == USB_Bootloader_CLASS_AUDIO) &&
                                       ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) ==
                                                                                        USB_Bootloader_EP_TYPE_BULK))
                                    {
                                        USB_Bootloader_midi_in_ep = i;
                                    }
                                #endif  /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/
                            }
                            else
                            {
                                /* OUT Endpoint */
                                USB_Bootloader_EP[i].apiEpState = USB_Bootloader_NO_EVENT_PENDING;
                                USB_Bootloader_EP[i].epMode =
                                                (iso ? USB_Bootloader_MODE_ISO_OUT : USB_Bootloader_MODE_ACK_OUT);
                                /* Find and init CDC IN endpoint number */
                                #if defined(USB_Bootloader_ENABLE_CDC_CLASS)
                                    if(((pEP->bMisc == USB_Bootloader_CLASS_CDC_DATA) ||
                                        (pEP->bMisc == USB_Bootloader_CLASS_CDC)) &&
                                        ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) !=
                                                                                    USB_Bootloader_EP_TYPE_INT))
                                    {
                                        USB_Bootloader_cdc_data_out_ep = i;
                                    }
                                #endif  /* End USB_Bootloader_ENABLE_CDC_CLASS*/
                                #if defined(USB_Bootloader_ENABLE_MIDI_STREAMING)
                                    if((pEP->bMisc == USB_Bootloader_CLASS_AUDIO) &&
                                       ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) ==
                                                                                        USB_Bootloader_EP_TYPE_BULK))
                                    {
                                        USB_Bootloader_midi_out_ep = i;
                                    }
                                #endif  /* End USB_Bootloader_ENABLE_MIDI_STREAMING*/
                            }
                            USB_Bootloader_EP[i].addr = pEP->addr;
                            USB_Bootloader_EP[i].attrib = pEP->attributes;

                            #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO)
                                break;      /* use first EP setting in Auto memory managment */
                            #endif /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO */
                        }
                    }
                }
            }
        #endif /* End (USB_Bootloader_EP_MA == USB_Bootloader__MA_DYNAMIC) */

        /* Init class array for each interface and interface number for each EP.
        *  It is used for hangling Class specific requests directed to either an
        *  interface or the endpoint.
        */
        /* p_list points the endpoint setting table. */
        pEP = (T_USB_Bootloader_EP_SETTINGS_BLOCK *) pTmp->p_list;
        for (i = 0u; i < ep; i++, pEP++)
        {
            /* config interface number for each EP*/
            USB_Bootloader_EP[pEP->addr & USB_Bootloader_DIR_UNUSED].interface = pEP->interface;
        }
        /* init pointer on interface class table*/
        USB_Bootloader_interfaceClass = USB_Bootloader_GetInterfaceClassTablePtr();
        /* Set the endpoint buffer addresses */
        count = 0u;
        ep = USB_Bootloader_EP1;
        cur_ep = 0u;
        for (i = 0u; i < 0x80u; i+= 0x10u)
        {
            CY_SET_REG8(&USB_Bootloader_ARB_EP1_CFG_PTR[i], USB_Bootloader_ARB_EPX_CFG_CRC_BYPASS |
                                                              USB_Bootloader_ARB_EPX_CFG_RESET);

            #if(USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL)
                /* Enable all Arbiter EP Interrupts : err, buf under, buf over, dma gnt, in buf full */
                USB_Bootloader_ARB_EP1_INT_EN_PTR[i] = USB_Bootloader_ARB_EPX_INT_MASK;
            #endif   /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL */

            if(USB_Bootloader_EP[ep].epMode != USB_Bootloader_MODE_DISABLE)
            {
                if((USB_Bootloader_EP[ep].addr & USB_Bootloader_DIR_IN) != 0u )
                {
                    CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[i], USB_Bootloader_MODE_NAK_IN);
                }
                else
                {
                    CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[i], USB_Bootloader_MODE_NAK_OUT);
                    /* Prepare EP type mask for automatic memory allocation */
                    #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO)
                        cur_ep |= 0x01u << (ep - USB_Bootloader_EP1);
                    #endif /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO */

                }
            }
            else
            {
                CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[i], USB_Bootloader_MODE_STALL_DATA_EP);
            }

            #if(USB_Bootloader_EP_MM != USB_Bootloader__EP_DMAAUTO)
                USB_Bootloader_EP[ep].buffOffset = count;
                count += USB_Bootloader_EP[ep].bufferSize;
                CY_SET_REG8(&USB_Bootloader_SIE_EP1_CNT0_PTR[i],   USB_Bootloader_EP[ep].bufferSize >> 8u);
                CY_SET_REG8(&USB_Bootloader_SIE_EP1_CNT1_PTR[i],   USB_Bootloader_EP[ep].bufferSize & 0xFFu);

                CY_SET_REG8(&USB_Bootloader_ARB_RW1_RA_PTR[i],     USB_Bootloader_EP[ep].buffOffset & 0xFFu);
                CY_SET_REG8(&USB_Bootloader_ARB_RW1_RA_MSB_PTR[i], USB_Bootloader_EP[ep].buffOffset >> 8u);
                CY_SET_REG8(&USB_Bootloader_ARB_RW1_WA_PTR[i],     USB_Bootloader_EP[ep].buffOffset & 0xFFu);
                CY_SET_REG8(&USB_Bootloader_ARB_RW1_WA_MSB_PTR[i], USB_Bootloader_EP[ep].buffOffset >> 8u);
            #endif /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_DMAAUTO */

            ep++;
        }

        #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO)
             /* BUF_SIZE depend on DMA_THRESS value: 55-32 bytes  44-16 bytes 33-8 bytes 22-4 bytes 11-2 bytes */
            USB_Bootloader_BUF_SIZE_REG = USB_Bootloader_DMA_BUF_SIZE;
            USB_Bootloader_DMA_THRES_REG = USB_Bootloader_DMA_BYTES_PER_BURST;   /* DMA burst threshold */
            USB_Bootloader_DMA_THRES_MSB_REG = 0u;
            USB_Bootloader_EP_ACTIVE_REG = USB_Bootloader_ARB_INT_MASK;
            USB_Bootloader_EP_TYPE_REG = cur_ep;
            /* Cfg_cmp bit set to 1 once configuration is complete. */
            USB_Bootloader_ARB_CFG_REG = USB_Bootloader_ARB_CFG_AUTO_DMA | USB_Bootloader_ARB_CFG_AUTO_MEM |
                                           USB_Bootloader_ARB_CFG_CFG_CPM;
            /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
            USB_Bootloader_ARB_CFG_REG = USB_Bootloader_ARB_CFG_AUTO_DMA | USB_Bootloader_ARB_CFG_AUTO_MEM;
        #endif /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO */

    } /* USB_Bootloader_configuration > 0 */
    CY_SET_REG8(USB_Bootloader_SIE_EP_INT_EN_PTR, 0xFFu);
}


/*******************************************************************************
* Function Name: USB_Bootloader_ConfigAltChanged
********************************************************************************
*
* Summary:
*  This routine undate configuration for the required endpoints only.
*  It is called after SET_INTERFACE request when Static memory allocation used.
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
void USB_Bootloader_ConfigAltChanged() 
{
    uint8 ep,cur_ep,i;
    uint8 iso;
    uint8 ri;

    T_USB_Bootloader_LUT *pTmp;
    T_USB_Bootloader_EP_SETTINGS_BLOCK *pEP;


    /* Init Endpoints and Device Status if configured */
    if(USB_Bootloader_configuration > 0u)
    {
        pTmp = USB_Bootloader_GetConfigTablePtr(USB_Bootloader_configuration - 1u);
        pTmp++;
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        /* Do not touch EP wich doesn't need reconfiguration */
        /* When Alt setting chaged, the only required endpoints need to be reconfigured */
        /* p_list points the endpoint setting table. */
        pEP = (T_USB_Bootloader_EP_SETTINGS_BLOCK *) pTmp->p_list;
        for (i = 0u; i < ep; i++, pEP++)
        {
            /*If Alt setting changed and new is same with EP Alt */
            if((USB_Bootloader_interfaceSetting[pEP->interface] !=
                USB_Bootloader_interfaceSetting_last[pEP->interface] ) &&
               (USB_Bootloader_interfaceSetting[pEP->interface] == pEP->altSetting) )
            {
                cur_ep = pEP->addr & USB_Bootloader_DIR_UNUSED;
                ri = ((cur_ep - USB_Bootloader_EP1) << USB_Bootloader_EPX_CNTX_ADDR_SHIFT);
                iso  = ((pEP->attributes & USB_Bootloader_EP_TYPE_MASK) == USB_Bootloader_EP_TYPE_ISOC);
                if (pEP->addr & USB_Bootloader_DIR_IN)
                {
                    /* IN Endpoint */
                    USB_Bootloader_EP[cur_ep].apiEpState = USB_Bootloader_EVENT_PENDING;
                    USB_Bootloader_EP[cur_ep].epMode =
                                            (iso ? USB_Bootloader_MODE_ISO_IN : USB_Bootloader_MODE_ACK_IN);
                }
                else
                {
                    /* OUT Endpoint */
                    USB_Bootloader_EP[cur_ep].apiEpState = USB_Bootloader_NO_EVENT_PENDING;
                    USB_Bootloader_EP[cur_ep].epMode =
                                            (iso ? USB_Bootloader_MODE_ISO_OUT : USB_Bootloader_MODE_ACK_OUT);
                }
                 /* Change the SIE mode for the selected EP to NAK ALL */
                 CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[ri], USB_Bootloader_MODE_NAK_IN_OUT);
                USB_Bootloader_EP[cur_ep].bufferSize = pEP->bufferSize;
                USB_Bootloader_EP[cur_ep].addr = pEP->addr;
                USB_Bootloader_EP[cur_ep].attrib = pEP->attributes;

                /* Clear the data toggle */
                USB_Bootloader_EP[cur_ep].epToggle = 0u;

                /* Dynamic reconfiguration for mode 3 transfer */
                #if((USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO) && (CY_PSOC3_ES3 || CY_PSOC5_ES2) )
                    /* In_data_rdy for selected EP should be set to 0 */
                    USB_Bootloader_ARB_EP1_CFG_PTR[ri] &= ~USB_Bootloader_ARB_EPX_CFG_IN_DATA_RDY;

                    /* write the EP number for which reconfiguration is required */
                    USB_Bootloader_DYN_RECONFIG_REG = (cur_ep << USB_Bootloader_DYN_RECONFIG_EP_SHIFT);
                    /* Set the dyn_config_en bit in dynamic reconfiguration register */
                    USB_Bootloader_DYN_RECONFIG_REG |= USB_Bootloader_DYN_RECONFIG_ENABLE;
                    /* wait for the dyn_config_rdy bit to set by the block,
                    *  this bit will be set to 1 when block is ready for reconfiguration.
                    */
                    while((USB_Bootloader_DYN_RECONFIG_REG & USB_Bootloader_DYN_RECONFIG_RDY_STS) == 0u);
                    /* Once dyn_config_rdy bit is set, FW can change the EP configuration. */
                    /* Change EP Type with new direction */
                    if((pEP->addr & USB_Bootloader_DIR_IN) == 0)
                    {
                        USB_Bootloader_EP_TYPE_REG |= 0x01u << (cur_ep - USB_Bootloader_EP1);
                    }
                    else
                    {
                        USB_Bootloader_EP_TYPE_REG &= ~(0x01u << (cur_ep - USB_Bootloader_EP1));
                    }
                    /* dynamic reconfiguration enable bit cleared, pointers and control/status
                    *  signals for the selected EP is cleared/re-initialized on negedge
                    *  of dynamic reconfiguration enable bit).
                    */
                    USB_Bootloader_DYN_RECONFIG_REG &= ~USB_Bootloader_DYN_RECONFIG_ENABLE;

                    /* Cfg_cmp bit set to 1 once configuration is complete. */
                    USB_Bootloader_ARB_CFG_REG = USB_Bootloader_ARB_CFG_AUTO_DMA |
                                                USB_Bootloader_ARB_CFG_AUTO_MEM | USB_Bootloader_ARB_CFG_CFG_CPM;
                    /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
                    USB_Bootloader_ARB_CFG_REG = USB_Bootloader_ARB_CFG_AUTO_DMA |
                                                   USB_Bootloader_ARB_CFG_AUTO_MEM;
                    /* The main loop will reenable DMA and OUT endpoint*/
                #else
                CY_SET_REG8(&USB_Bootloader_SIE_EP1_CNT0_PTR[ri],   USB_Bootloader_EP[cur_ep].bufferSize >> 8u);
                CY_SET_REG8(&USB_Bootloader_SIE_EP1_CNT1_PTR[ri],   USB_Bootloader_EP[cur_ep].bufferSize & 0xFFu);

                CY_SET_REG8(&USB_Bootloader_ARB_RW1_RA_PTR[ri],     USB_Bootloader_EP[cur_ep].buffOffset & 0xFFu);
                CY_SET_REG8(&USB_Bootloader_ARB_RW1_RA_MSB_PTR[ri], USB_Bootloader_EP[cur_ep].buffOffset >> 8u);
                CY_SET_REG8(&USB_Bootloader_ARB_RW1_WA_PTR[ri],     USB_Bootloader_EP[cur_ep].buffOffset & 0xFFu);
                CY_SET_REG8(&USB_Bootloader_ARB_RW1_WA_MSB_PTR[ri], USB_Bootloader_EP[cur_ep].buffOffset >> 8u);
                #endif /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO */
            }
        }
    } /* USB_Bootloader_configuration > 0 */
    CY_SET_REG8(USB_Bootloader_SIE_EP_INT_EN_PTR, 0xFFu);
}


/*******************************************************************************
* Function Name: USB_Bootloader_GetConfigTablePtr
********************************************************************************
*
* Summary:
*  This routine returns a pointer a configuration table entry
*
* Parameters:
*  c:  Configuration Index
*
* Return:
*  Device Descriptor pointer.
*
*******************************************************************************/
T_USB_Bootloader_LUT *USB_Bootloader_GetConfigTablePtr(uint8 c) \
                                                        
{
    /* Device Table */
    T_USB_Bootloader_LUT *pTmp;

    pTmp = USB_Bootloader_GetDeviceTablePtr();

    /* The first entry points to the Device Descriptor,
       the the configuration entries  */
    return(pTmp[c + 1u].p_list);
}


/*******************************************************************************
* Function Name: USB_Bootloader_GetDeviceTablePtr
********************************************************************************
*
* Summary:
*  This routine returns a pointer to the Device table
*
* Parameters:
*  None.
*
* Return:
*  Device Table pointer
*
*******************************************************************************/
T_USB_Bootloader_LUT *USB_Bootloader_GetDeviceTablePtr(void) 
{
    /* Device Table */
    return(USB_Bootloader_TABLE[USB_Bootloader_device].p_list);
}


/*******************************************************************************
* Function Name: USB_GetInterfaceClassTablePtr
********************************************************************************
*
* Summary:
*  This routine returns Interface Class table pointer, which contains
*  the relation between interface number and interface class.
*
* Parameters:
*  None.
*
* Return:
*  Interface Class table pointer.
*
*******************************************************************************/
uint8 CYCODE *USB_Bootloader_GetInterfaceClassTablePtr(void)
                                                        
{
    T_USB_Bootloader_LUT *pTmp;
    uint8 currentInterfacesNum;

    pTmp = USB_Bootloader_GetConfigTablePtr(USB_Bootloader_configuration - 1u);
    currentInterfacesNum  = ((uint8 *) pTmp->p_list)[USB_Bootloader_CONFIG_DESCR_NUM_INTERFACES];
    /* Third entry in the LUT starts the Interface Table pointers */
    pTmp += 2;
    /* The INTERFACE_CLASS table is located after all interfaces*/
    pTmp += currentInterfacesNum;
    return(pTmp->p_list);
}


/*******************************************************************************
* Function Name: USB_Bootloader_TerminateEP
********************************************************************************
*
* Summary:
*  This routine handles set endpoint halt.
*
* Parameters:
*  Endpoint number.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_TerminateEP(uint8 ep) 
{
    uint8 ri;

    ep &= USB_Bootloader_DIR_UNUSED;
    ri = ((ep - USB_Bootloader_EP1) << USB_Bootloader_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USB_Bootloader_EP0) && (ep < USB_Bootloader_MAX_EP))
    {
        /* Set the endpoint Halt */
        USB_Bootloader_EP[ep].hwEpState |= (USB_Bootloader_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USB_Bootloader_EP[ep].epToggle = 0u;
        USB_Bootloader_EP[ep].apiEpState = USB_Bootloader_NO_EVENT_ALLOWED;

        if (USB_Bootloader_EP[ep].addr & USB_Bootloader_DIR_IN)
        {
            /* IN Endpoint */
            CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[ri], USB_Bootloader_MODE_NAK_IN);
        }
        else
        {
            /* OUT Endpoint */
            CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[ri], USB_Bootloader_MODE_NAK_OUT);
        }
    }
}


/*******************************************************************************
* Function Name: USB_Bootloader_SetEndpointHalt
********************************************************************************
*
* Summary:
*  This routine handles set endpoint halt.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_SetEndpointHalt(void) 
{
    uint8 ep, ri;
    uint8 requestHandled = USB_Bootloader_FALSE;

    /* Clear endpoint halt */
    ep = CY_GET_REG8(USB_Bootloader_wIndexLo) & USB_Bootloader_DIR_UNUSED;
    ri = ((ep - USB_Bootloader_EP1) << USB_Bootloader_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USB_Bootloader_EP0) && (ep < USB_Bootloader_MAX_EP))
    {
        /* Set the endpoint Halt */
        USB_Bootloader_EP[ep].hwEpState |= (USB_Bootloader_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USB_Bootloader_EP[ep].epToggle = 0u;
        USB_Bootloader_EP[ep].apiEpState = USB_Bootloader_NO_EVENT_ALLOWED;

        if (USB_Bootloader_EP[ep].addr & USB_Bootloader_DIR_IN)
        {
            /* IN Endpoint */
            CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[ri], USB_Bootloader_MODE_STALL_DATA_EP |
                                                               USB_Bootloader_MODE_ACK_IN);
        }
        else
        {
            /* OUT Endpoint */
            CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[ri], USB_Bootloader_MODE_STALL_DATA_EP |
                                                               USB_Bootloader_MODE_ACK_OUT);
        }
        requestHandled = USB_Bootloader_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USB_Bootloader_ClearEndpointHalt
********************************************************************************
*
* Summary:
*  This routine handles clear endpoint halt.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_ClearEndpointHalt(void) 
{
    uint8 ep, ri;
    uint8 requestHandled = USB_Bootloader_FALSE;

    /* Clear endpoint halt */
    ep = CY_GET_REG8(USB_Bootloader_wIndexLo) & USB_Bootloader_DIR_UNUSED;
    ri = ((ep - USB_Bootloader_EP1) << USB_Bootloader_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USB_Bootloader_EP0) && (ep < USB_Bootloader_MAX_EP))
    {
        /* Set the endpoint Halt */
        USB_Bootloader_EP[ep].hwEpState &= ~(USB_Bootloader_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USB_Bootloader_EP[ep].epToggle = 0u;

        if (USB_Bootloader_EP[ep].addr & USB_Bootloader_DIR_IN)
        {
            /* IN Endpoint */
            USB_Bootloader_EP[ep].apiEpState = USB_Bootloader_NO_EVENT_PENDING;
            CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[ri], USB_Bootloader_MODE_NAK_IN);
        }
        else
        {
            /* OUT Endpoint */
            USB_Bootloader_EP[ep].apiEpState = USB_Bootloader_EVENT_PENDING;
            CY_SET_REG8(&USB_Bootloader_SIE_EP1_CR0_PTR[ri], USB_Bootloader_MODE_ACK_OUT);
        }
        requestHandled = USB_Bootloader_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USB_Bootloader_ValidateAlternateSetting
********************************************************************************
*
* Summary:
*  Validates (and records) a SET INTERFACE request.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_ValidateAlternateSetting(void) 
{
    uint8 requestHandled = USB_Bootloader_TRUE;
    uint8 interfaceNum;
    T_USB_Bootloader_LUT *pTmp;
    uint8 currentInterfacesNum;

    interfaceNum = CY_GET_REG8(USB_Bootloader_wIndexLo);
    /* Validate interface setting, stall if invalid. */
    pTmp = USB_Bootloader_GetConfigTablePtr(USB_Bootloader_configuration - 1u);
    currentInterfacesNum  = ((uint8 *) pTmp->p_list)[USB_Bootloader_CONFIG_DESCR_NUM_INTERFACES];

    if((interfaceNum >= currentInterfacesNum) || (interfaceNum >= USB_Bootloader_MAX_INTERFACES_NUMBER))
    {   /* wrong interface number */
        requestHandled = USB_Bootloader_FALSE;
    }
    else
    {
        /* save current Alt setting to find out the difference in Config() function */
        USB_Bootloader_interfaceSetting_last[interfaceNum] = USB_Bootloader_interfaceSetting[interfaceNum];
        USB_Bootloader_interfaceSetting[interfaceNum] = CY_GET_REG8(USB_Bootloader_wValueLo);
    }

    return (requestHandled);
}


/* [] END OF FILE */
