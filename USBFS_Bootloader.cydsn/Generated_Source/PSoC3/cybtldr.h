/*******************************************************************************
* File Name: cybldr.h  
* Version 2.40
*
*  Description:
*   Provides an API for the Bootloader. The API includes functions for starting
*   boot loading operations, validating the application and jumping to the 
*   application.
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


#if !defined(__CYBTLDR_H__)
#define __CYBTLDR_H__
#include <device.h>
#include <cybtldr_common.h>

/*******************************************************************************
* Metadata addresses and pointer defines
********************************************************************************/

#define CYBTLDR_MD_BASE(x)                 (CYDEV_FLASH_BASE + (CYDEV_FLASH_SIZE - ((x) * CYDEV_FLS_ROW_SIZE) - CY_META_DATA_SIZE))
#define CY_BTLDR_MD_ARRAY                  (FLASH_NUMBER_SECTORS - 1)

#define CYBTLDR_MD_CHECKSUM_ADDR(x)        (CYBTLDR_MD_BASE(x) + CY_META_APP_CHECKSUM_OFFSET)
#define CYBTLDR_MD_APP_ENTRY_POINT_ADDR(x) (CYBTLDR_MD_BASE(x) + CY_META_APP_ADDR_OFFSET)
#define CYBTLDR_MD_LAST_BLDR_ROW_ADDR(x)   (CYBTLDR_MD_BASE(x) + CY_META_APP_BL_LAST_ROW_OFFSET)
#define CYBTLDR_MD_APP_BYTE_LEN(x)         (CYBTLDR_MD_BASE(x) + CY_META_APP_BYTE_LEN_OFFSET)
#define CYBTLDR_MD_APP_RUN_ADDR(x)         (CYBTLDR_MD_BASE(x) + CY_META_APP_RUN_TYPE_OFFSET)
#define CYBTLDR_MD_APP_ACTIVE_ADDR(x)      (CYBTLDR_MD_BASE(x) + CY_META_APP_ACTIVE_OFFSET)
#define CYBTLDR_MD_APP_VERIFIED_ADDR(x)    (CYBTLDR_MD_BASE(x) + CY_META_APP_VERIFIED_OFFSET)
#define CYBTLDR_MD_APP_BLDBL_VER_ADDR(x)   (CYBTLDR_MD_BASE(x) + CY_META_APP_BL_BUILD_VER_OFFSET)
#define CYBTLDR_MD_APP_VER_ADDR(x)         (CYBTLDR_MD_BASE(x) + CY_META_APP_VER_OFFSET)
#define CYBTLDR_MD_APP_ID_ADDR(x)          (CYBTLDR_MD_BASE(x) + CY_META_APP_ID_OFFSET)
#define CYBTLDR_MD_APP_CUST_ID_ADDR(x)     (CYBTLDR_MD_BASE(x) + CY_META_APP_CUST_ID_OFFSET)

#if (CYDEV_BOOTLOADER_APPLICATIONS == 1)
    #define CY_BTLDR_MD_BASE                    CYBTLDR_MD_BASE(0)
    #define CY_BTLDR_MD_ROW                     ((FLASH_NUMBER_ROWS / FLASH_NUMBER_SECTORS) - 1)

    #define CY_BTLDR_MD_CHECKSUM_ADDR           CYBTLDR_MD_CHECKSUM_ADDR(0)
    #define CY_BTLDR_MD_LAST_BLDR_ROW_ADDR      CYBTLDR_MD_LAST_BLDR_ROW_ADDR(0)
    #define CY_BTLDR_MD_APP_BYTE_LEN            CYBTLDR_MD_APP_BYTE_LEN(0)
    #define CY_BTLDR_MD_APP_VERIFIED_ADDR       CYBTLDR_MD_APP_VERIFIED_ADDR(0)
    #define CY_BTLDR_MD_APP_ENTRY_POINT_ADDR    CYBTLDR_MD_APP_ENTRY_POINT_ADDR(0)
    #define CY_BTLDR_MD_APP_RUN_ADDR            CYBTLDR_MD_APP_RUN_ADDR(0)
#else
    #define CY_BTLDR_MD_ROW(x)                  ((FLASH_NUMBER_ROWS / FLASH_NUMBER_SECTORS) - 1 - (x))

    #define CY_BTLDR_MD_CHECKSUM_ADDR           CYBTLDR_MD_CHECKSUM_ADDR(appId)
    #define CY_BTLDR_MD_LAST_BLDR_ROW_ADDR      CYBTLDR_MD_LAST_BLDR_ROW_ADDR(appId)
    #define CY_BTLDR_MD_APP_BYTE_LEN            CYBTLDR_MD_APP_BYTE_LEN(appId)
    #define CY_BTLDR_MD_APP_VERIFIED_ADDR       CYBTLDR_MD_APP_VERIFIED_ADDR(appId)
    #define CY_BTLDR_MD_APP_ENTRY_POINT_ADDR    CYBTLDR_MD_APP_ENTRY_POINT_ADDR(activeApp)
    #define CY_BTLDR_MD_APP_RUN_ADDR            CYBTLDR_MD_APP_RUN_ADDR(activeApp)
#endif

/* Pointers to the Bootloader metadata items */
#define CYBTLDR_P_APP_ACTIVE(x)             ((uint8 CYCODE *)        CYBTLDR_MD_APP_ACTIVE_ADDR(x))

#define CY_BTLDR_P_CHECKSUM                 ((uint8  CYCODE *)       CY_BTLDR_MD_CHECKSUM_ADDR)
#define CY_BTLDR_P_APP_ENTRY_POINT          ((CYAPPADDRESS CYCODE *) CY_BTLDR_MD_APP_ENTRY_POINT_ADDR)
#define CY_BTLDR_P_LAST_BLDR_ROW            ((uint16 CYCODE *)       CY_BTLDR_MD_LAST_BLDR_ROW_ADDR)
#define CY_BTLDR_P_APP_BYTE_LEN             ((CYAPPADDRESS CYCODE *) CY_BTLDR_MD_APP_BYTE_LEN)
#define CY_BTLDR_P_APP_RUN_ADDR             ((uint8 CYCODE *)        CY_BTLDR_MD_APP_RUN_ADDR)
#define CY_BTLDR_P_APP_VERIFIED             ((uint8 CYCODE *)        CY_BTLDR_MD_APP_VERIFIED_ADDR)
#define CY_BTLDR_P_APP_BLD_BL_VER           ((uint16 CYCODE *)       CY_BTLDR_MD_APP_BLDBL_VER_ADDR)
#define CY_BTLDR_P_APP_VER                  ((uint16 CYCODE *)       CY_BTLDR_MD_APP_VER_ADDR)
#define CY_BTLDR_P_APP_ID                   ((uint16 CYCODE *)       CY_BTLDR_MD_APP_ID_ADDR)
#define CY_BTLDR_P_APP_CUST_ID              ((uint32 CYCODE *)       CY_BTLDR_MD_APP_CUST_ID_ADDR)


/*******************************************************************************
* External References
********************************************************************************/
extern void LaunchApp(uint32 addr);
extern void CyBtldr_Start(void) CYSMALL;
extern void CyBtldr_LaunchApplication(void) CYSMALL;
extern void CyBtldr_HostLink(uint8 TimeOut);

#if (CYDEV_BOOTLOADER_APPLICATIONS > 1)
    extern cystatus CyBtldr_ValidateApp(uint8 appId) CYSMALL;
    #define CyBtldr_ValidateApplication()   CyBtldr_ValidateApp(0)
#else
    extern cystatus CyBtldr_ValidateApplication(void) CYSMALL;
	#define CyBtldr_ValidateApp(x) CyBtldr_ValidateApplication()
#endif


/* If using custom interface as the IO Component, user must provide these functions */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface)
    extern void CyBtldrCommStart(void);
    extern void CyBtldrCommStop (void);
    extern void CyBtldrCommReset(void);
    extern cystatus CyBtldrCommWrite(uint8* buffer, uint16 size, uint16* count, uint8 timeOut);
    extern cystatus CyBtldrCommRead (uint8* buffer, uint16 size, uint16* count, uint8 timeOut);
#endif

#endif /* __CYBTLDR_H__ */
