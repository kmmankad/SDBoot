/*******************************************************************************
* File Name: cybldr_common.c
* Version 2.40
*
*  Description:
*   Provides an API for the common functionality shared by both Bootloader and
*   Boot Loadable applications.
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

#include <cybtldr_common.h>

void CyBtldr_SetFlashByte(uint32 address, uint8 runType) 
{
    uint32 flsAddr = address - CYDEV_FLASH_BASE;
    uint8 rowData[CYDEV_FLS_ROW_SIZE];
#if !(CY_PSOC4)
    uint8 arrayId = (flsAddr / CYDEV_FLS_SECTOR_SIZE);
#endif
    uint16 rowNum = ((flsAddr % CYDEV_FLS_SECTOR_SIZE) / CYDEV_FLS_ROW_SIZE);
    uint32 baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
    uint16 idx;
    for (idx = 0; idx < CYDEV_FLS_ROW_SIZE; idx++)
    {
        rowData[idx] = CY_GETCODEDATA(baseAddr + idx);
    }
    rowData[address % CYDEV_FLS_ROW_SIZE] = runType;

#if (CY_PSOC4)
    CySysFlashWriteRow(rowNum, rowData);
#else
    CyWriteRowData(arrayId, rowNum, rowData);
#endif
}

/* [] END OF FILE */
