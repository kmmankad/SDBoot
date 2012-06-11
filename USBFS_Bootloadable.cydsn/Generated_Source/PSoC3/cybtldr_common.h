/*******************************************************************************
* File Name: cybldr_common.h  
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

#if !defined(__CYBTLDR_COMMON_H__)
#define __CYBTLDR_COMMON_H__

#include <device.h>

#define CYBTLDR_START_APP                   0x80  /* Mask used to indicate starting application */
#define CYBTLDR_START_BTLDR                 0x40  /* Mask used to indicate starting bootloader */

#ifndef CYDEV_FLASH_BASE
    #define CYDEV_FLASH_BASE                CYDEV_FLS_BASE
    #define CYDEV_FLASH_SIZE                CYDEV_FLS_SIZE
#endif /* CYDEV_FLASH_BASE */

#define CY_META_DATA_SIZE                   64
#define CY_META_APP_CHECKSUM_OFFSET         0

#if (defined(__C51__))
    #define CYAPPADDRESS                    uint16
    /* offset by 2 from 32 bit start because only need 16 bits */
    #define CY_META_APP_ADDR_OFFSET         3   /* 2 bytes */
    #define CY_META_APP_BL_LAST_ROW_OFFSET  7   /* 4 bytes */
    #define CY_META_APP_BYTE_LEN_OFFSET     11  /* 4 bytes */
    #define CY_META_APP_RUN_TYPE_OFFSET     15  /* 1 byte  */
    #define CY_GETCODEDATA(idx)             *((uint8 CYCODE *)idx)
    #define CY_GETCODEWORD(idx)             *((uint32 CYCODE *)idx)
#elif (defined(__GNUC__) || defined(__ARMCC_VERSION))
    #define CYAPPADDRESS                    uint32
    #define CY_META_APP_ADDR_OFFSET         1   /* 4 bytes */
    #define CY_META_APP_BL_LAST_ROW_OFFSET  5   /* 4 bytes */
    #define CY_META_APP_BYTE_LEN_OFFSET     9   /* 4 bytes */
    #define CY_META_APP_RUN_TYPE_OFFSET     13  /* 1 byte  */
    #define CY_GETCODEDATA(idx)             *((uint8 *)(CYDEV_FLASH_BASE + (idx)))
    #define CY_GETCODEWORD(idx)             *((uint32 *)(CYDEV_FLASH_BASE + (idx)))
#endif /* __GNUC__ || __ARMCC_VERSION */

#define CY_META_APP_ACTIVE_OFFSET           16  /* 1 byte  */
#define CY_META_APP_VERIFIED_OFFSET         17  /* 1 byte  */

#define CY_META_APP_BL_BUILD_VER_OFFSET     18  /* 2 bytes */
#define CY_META_APP_ID_OFFSET               20  /* 2 bytes */
#define CY_META_APP_VER_OFFSET              22  /* 2 bytes */
#define CY_META_APP_CUST_ID_OFFSET          24  /* 4 bytes */

#if ((CYDEV_DEBUGGING_XRES) && (CY_PSOC3_ES2 || CY_PSOC5_ES1))
    #define WORKAROUND_OPT_XRES 1
#endif

#if (CY_PSOC4)
    extern uint8 appRunType;

    #define CYBTLDR_SOFTWARE_RESET          CY_SET_REG32(CYREG_CM0_AIRCR, 0x05FA0004)
    #define CYBTLDR_GET_RUN_TYPE            (((CY_GET_REG32(CYREG_RES_CAUSE) & 0x10) > 0) \
                                                ? (CY_GET_REG8(CYDEV_SRAM_BASE + (4 * (CYINT_IRQ_BASE + NUM_INTERRUPTS)))) \
                                                : 0)
    #define CYBTDLR_SET_RUN_TYPE(x)         CY_SET_REG8(CYDEV_SRAM_BASE + (4 * (CYINT_IRQ_BASE + NUM_INTERRUPTS)), (x))
#else
    #define CYBTLDR_SOFTWARE_RESET          CY_SET_REG8(CYREG_RESET_CR2, 0x01)

    #define CYBTLDR_GET_RUN_TYPE            (CY_GET_REG8(CYREG_RESET_SR0) & (CYBTLDR_START_BTLDR | CYBTLDR_START_APP))
    #if defined(WORKAROUND_OPT_XRES)
        #define CYBTDLR_SET_RUN_TYPE(x)     CyBtldr_SetFlashByte(CY_BTLDR_MD_APP_RUN_ADDR(activeApp), (x))
    #else
        #define CYBTDLR_SET_RUN_TYPE(x)     CY_SET_REG8(CYREG_RESET_SR0, (x))
    #endif
#endif

#define CyBtldr_SetFlashRunType(runType)        CyBtldr_SetFlashByte(CY_BTLDR_MD_APP_RUN_ADDR(0), (runType))


void CyBtldr_SetFlashByte(uint32 address, uint8 value) ;

#endif /* __CYBTLDR_COMMON_H__ */

/* [] END OF FILE */
