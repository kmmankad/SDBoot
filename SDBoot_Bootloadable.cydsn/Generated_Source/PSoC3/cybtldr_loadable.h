/*******************************************************************************
* File Name: cybldr_loadable.h  
* Version 2.40
*
*  Description:
*   Provides an API for the Bootloadable application. The API includes a
*   single function for starting bootloader.
*
*  Note: 
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
********************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/


#ifndef __CYBYTLDR_LOADABLE_H__
#define __CYBYTLDR_LOADABLE_H__

#include "cydevice_trm.h"

/*******************************************************************************
* Function Name: CyBtldr_Load
********************************************************************************
* Summary:
*   Begins the bootloading algorithm, downloading a new ACD image from the host.
*
* Parameters:
*   void:
*   
* Return:
*   This method will never return. It will load a new application and reset
*   the device.
*
*******************************************************************************/
extern void CyBtldr_Load(void) ;

#endif /* __CYBYTLDR_LOADABLE_H__ */

/* [] END OF FILE */
