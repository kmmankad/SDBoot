/*******************************************************************************
* File Name: cybldr_loadable.c
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


#include "cybtldr_loadable.h"
#include "cybtldr_common.h"

void CyBtldr_Load(void) 
{
#if defined (WORKAROUND_OPT_XRES)
    uint8 rowBuffer[CYDEV_FLS_ROW_SIZE + CYDEV_ECC_ROW_SIZE];

    /* Don't really care whether the call succeeds or not. Either way we need to reset and run the bootloader */
    CySetTemp(); 
    CySetFlashEEBuffer(rowBuffer);
#endif
    CYBTDLR_SET_RUN_TYPE(CYBTLDR_START_BTLDR);
    CYBTLDR_SOFTWARE_RESET;
}

/* [] END OF FILE */
