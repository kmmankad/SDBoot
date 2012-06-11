/*******************************************************************************
* File Name: USB_Bootloader_vnd.c
* Version 2.12
*
* Description:
*  USB vendor request handler.
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

#if(USB_Bootloader_EXTERN_VND == USB_Bootloader_FALSE)


/***************************************
* Vendor Specific Declarations
***************************************/

/* `#START VENDOR_SPECIFIC_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* External References
***************************************/

uint8 USB_Bootloader_InitControlRead(void) ;
uint8 USB_Bootloader_InitControlWrite(void) ;


extern uint8 CYCODE USB_Bootloader_MSOS_CONFIGURATION_DESCR[];

extern volatile T_USB_Bootloader_TD USB_Bootloader_currentTD;


/*******************************************************************************
* Function Name: USB_Bootloader_HandleVendorRqst
********************************************************************************
*
* Summary:
*  This routine provide users with a method to implement vendor specifc
*  requests.
*
*  To implement vendor specific requests, add your code in this function to
*  decode and disposition the request.  If the request is handled, your code
*  must set the variable "requestHandled" to TRUE, indicating that the
*  request has been handled.
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
uint8 USB_Bootloader_HandleVendorRqst(void) 
{
    uint8 requestHandled = USB_Bootloader_FALSE;

    if ((CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_DIR_MASK) == USB_Bootloader_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USB_Bootloader_bRequest))
        {
            case USB_Bootloader_GET_EXTENDED_CONFIG_DESCRIPTOR:
                #if defined(USB_Bootloader_ENABLE_MSOS_STRING)
                    USB_Bootloader_currentTD.pData = &USB_Bootloader_MSOS_CONFIGURATION_DESCR[0u];
                    USB_Bootloader_currentTD.count = USB_Bootloader_MSOS_CONFIGURATION_DESCR[0u];
                    requestHandled  = USB_Bootloader_InitControlRead();
                #endif /* End USB_Bootloader_ENABLE_MSOS_STRING */
                break;
            default:
                break;
        }
    }

    /* `#START VENDOR_SPECIFIC_CODE` Place your vendor specific request here */

    /* `#END` */

    return(requestHandled);
}


/*******************************************************************************
* Additional user functions supporting Vendor Specific Requests
********************************************************************************/

/* `#START VENDOR_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif /* USB_Bootloader_EXTERN_VND */


/* [] END OF FILE */
