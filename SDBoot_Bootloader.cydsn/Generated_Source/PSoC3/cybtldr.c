/*******************************************************************************
* File Name: cybtldr.c  
* Version 2.40
*
*  Description:
*   Provides an API for the Bootloader component. The API includes functions
*   for starting boot loading operations, validating the application and
*   jumping to the application.
*
*  Note: 
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/


#include <cybtldr.h>
#include <cybtldr_common.h>
#include <device.h>
#include <string.h>

/*******************************************************************************
* Defines
********************************************************************************/

#define CYBTLDR_VERSION        {40, 2, 0x01} 

/* Packet framing constants. */
#define SOP                 0x01    /* Start of Packet */
#define EOP                 0x17    /* End of Packet */

/* Bootloader command responces */
#define CYRET_ERR_KEY       0x01  /* The provided key does not match the expected value */
#define CYRET_ERR_VERIFY    0x02  /* The verification of flash failed */
#define CYRET_ERR_LENGTH    0x03  /* The amount of data available is outside the expected range */
#define CYRET_ERR_DATA      0x04  /* The data is not of the proper form */
#define CYRET_ERR_CMD       0x05  /* The command is not recognized */
#define CYRET_ERR_DEVICE    0x06  /* The expected device does not match the detected device */
#define CYRET_ERR_VERSION   0x07  /* The bootloader version detected is not supported */
#define CYRET_ERR_CHECKSUM  0x08  /* The checksum does not match the expected value */
#define CYRET_ERR_ARRAY     0x09  /* The flash array is not valid */
#define CYRET_ERR_ROW       0x0A  /* The flash row is not valid */
#define CYRET_ERR_PROTECT   0x0B  /* The flash row is protected and can not be programmed */
#define CYRET_ERR_APP       0x0C  /* The application is not valid and cannot be set as active */
#define CYRET_ERR_ACTIVE    0x0D  /* The application is currently marked as active */
#define CYRET_ERR_UNK       0x0F  /* An unknown error occurred */

/* Bootloader command definitions. */
#define CYBTLDR_COMMAND_CHECKSUM     0x31    /* Verify the checksum for the bootloadable project. */
#define CYBTLDR_COMMAND_REPORT_SIZE  0x32    /* Report the programmable portions of flash */
#define CYBTLDR_COMMAND_APP_STATUS   0x33    /* Gets status info about the provided app status. */
#define CYBTLDR_COMMAND_ERASE        0x34    /* Erase the specified flash row */
#define CYBTLDR_COMMAND_SYNC         0x35    /* Sync the bootloader and host application */
#define CYBTLDR_COMMAND_APP_ACTIVE   0x36    /* Sets the active application. */
#define CYBTLDR_COMMAND_DATA         0x37    /* Queue up a block of data for programming */
#define CYBTLDR_COMMAND_ENTER        0x38    /* Enter the bootloader */
#define CYBTLDR_COMMAND_PROGRAM      0x39    /* Program the specified row */
#define CYBTLDR_COMMAND_VERIFY       0x3A    /* Compute flash row checksum for verification */
#define CYBTLDR_COMMAND_EXIT         0x3B    /* Exits the bootloader & resets the chip */

/* Bootloader packet byte addresses */
/*******************************************************************************
* [1-byte] [1-byte ] [2-byte] [n-byte] [ 2-byte ] [1-byte]
* [ SOP  ] [Command] [ Size ] [ Data ] [Checksum] [ EOP  ]
*******************************************************************************/
#define CYBTLDR_SOP_ADDR             0x00         /* Start of packet offset from beginning */
#define CYBTLDR_CMD_ADDR             0x01         /* Command offset from beginning */
#define CYBTLDR_SIZE_ADDR            0x02         /* Packet size offset from beginning */
#define CYBTLDR_DATA_ADDR            0x04         /* Packet data offset from beginning */
#define CYBTLDR_CHK_ADDR(x)          (0x04 + (x)) /* Packet checksum offset from end */
#define CYBTLDR_EOP_ADDR(x)          (0x06 + (x)) /* End of packet offset from end */
#define MIN_PKT_SIZE                 7            /* The minimum number of bytes in a packet */


#if (CYDEV_BOOTLOADER_APPLICATIONS > 1)
    uint8 activeApp = CYDEV_BOOTLOADER_APPLICATIONS;
#endif

/* Our definition of a row size. */
#if ((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0))
    #define CYBTLDR_FROW_SIZE           ((CYDEV_FLS_ROW_SIZE) + (CYDEV_ECC_ROW_SIZE))
#else
    #define CYBTLDR_FROW_SIZE           CYDEV_FLS_ROW_SIZE
#endif

#if (CY_PSOC4)
    #define BL_LAST_ROW         (CY_GET_XTND_REG16(CY_BTLDR_MD_LAST_BLDR_ROW_ADDR))
    #define BL_APP_RUN_ADDR     (CY_GET_XTND_REG8(CY_BTLDR_MD_APP_RUN_ADDR))
    #define BL_APP_ENTRY_ADDR   (CY_GET_XTND_REG32(CY_BTLDR_MD_APP_ENTRY_POINT_ADDR))
    #define BL_APP_BYTE_LEN     (CY_GET_XTND_REG32(CY_BTLDR_MD_APP_BYTE_LEN))
    #define BL_APP_VERIFIED     (CY_GET_XTND_REG8(CY_BTLDR_MD_APP_VERIFIED_ADDR))
    #define BL_APP_CHECKSUM     (CY_GET_XTND_REG8(CY_BTLDR_MD_CHECKSUM_ADDR))
    
    #define INIT_FLASH_WRITE        (1)
    #define FLASH_NUMBER_SECTORS    (1)
    #define CYBTDLR_CHECKSUM_START   0xC0    /* Exclude the vector table as these get remapped to SRAM */
#else
    #define BL_LAST_ROW         (*CY_BTLDR_P_LAST_BLDR_ROW)
    #define BL_APP_RUN_ADDR     (*CY_BTLDR_P_APP_RUN_ADDR)
    #define BL_APP_ENTRY_ADDR   (*CY_BTLDR_P_APP_ENTRY_POINT)
    #define BL_APP_BYTE_LEN     (*CY_BTLDR_P_APP_BYTE_LEN)
    #define BL_APP_VERIFIED     (*CY_BTLDR_P_APP_VERIFIED)
    #define BL_APP_CHECKSUM     (*CY_BTLDR_P_CHECKSUM)
    
    #define INIT_FLASH_WRITE        (CYRET_SUCCESS == CySetTemp() && CYRET_SUCCESS == CySetFlashEEBuffer(flashBuffer))
    #define CYBTDLR_CHECKSUM_START   0    /* The bootloader always starts at 0 in flash */
    uint8 flashBuffer[CYBTLDR_FROW_SIZE];
#endif

#define FIRST_APP_BYTE   (CYDEV_FLS_ROW_SIZE * (BL_LAST_ROW + 1))

#define SIZEOF_COMMAND_BUFFER       300 /* Maximum number of bytes accepted in a packet plus some */

typedef struct _CYBTLDR_ENTER
{
    uint32 SiliconId;
    uint8 Revision;
    uint8 BootLoaderVersion[3];

} CYBTLDR_ENTER;

/* CyBtldr_Checksum and CyBtldr_SizeBytes are forcefully set in code. We then post process
 * the hex file from the linker and inject their values then. When the hex file is loaded
 * onto the device these two variables should have valid values. Because the compiler can
 * do optimizations remove the constant accesses, these should not be accessed directly.
 * Insted, the variables CyBtldr_ChecksumAccess & CyBtldr_SizeBytesAccess should be used
 * to get the proper values at runtime.
*/
#if (defined(__C51__))
    const uint8 CYCODE CyBtldr_Checksum = 0;
    const uint32 CYCODE CyBtldr_SizeBytes = 0xFFFFFFFF;
#else
    const uint8 CYCODE __attribute__((section (".bootloader"))) CyBtldr_Checksum = 0;
    const uint32 CYCODE __attribute__((section (".bootloader"))) CyBtldr_SizeBytes = 0xFFFFFFFF;
#endif
uint8 *CyBtldr_ChecksumAccess = (uint8*)(&CyBtldr_Checksum);
uint32 *CyBtldr_SizeBytesAccess = (uint32*)(&CyBtldr_SizeBytes);

/* Function prototypes */
int WritePacket(uint8 command, uint8 CYXDATA* buffer, uint16 size) CYSMALL;

/*******************************************************************************
* Function Name: CyBtldr_BadBootloader
********************************************************************************
* Summary:
*   This is an infinite loop that should be run iff there is an error with the
*   bootloader.
*
* Parameters:  
*   None
*
* Returns:
*   None
*
*******************************************************************************/
void CyBtldr_BadBootloader(void)
{
    while (1); /* the bootloader is invalid so all we can do is hang the MCU */
}


/*******************************************************************************
* Function Name: CalcPacketChecksum
********************************************************************************
* Summary:
*   This computes the 16 bit checksum for the provided number of bytes contained
*   in the provided buffer
*
* Parameters:  
*   buffer:
*      The buffer containing the data to compute the checksum for
*   size:
*      The number of bytes in buffer to compute the checksum for
*
* Returns:
*   16 bit checksum for the provided data
*
*******************************************************************************/
uint16 CalcPacketChecksum(uint8* buffer, uint16 size) CYSMALL
{
#if (CYDEV_BOOTLOADER_CHECKSUM == CYDEV_BOOTLOADER_CHECKSUM_CRC)
    uint16 CYDATA crc = 0xffff;
    uint16 CYDATA tmp;
    uint8  CYDATA i;
       
    if (size == 0)
        return (~crc);

    do
    {
        for (i = 0, tmp = 0x00ff & *buffer++; i < 8; i++, tmp >>= 1)
        {
            if ((crc & 0x0001) ^ (tmp & 0x0001))
            {
                crc = (crc >> 1) ^ 0x8408;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    while (--size);

    crc = ~crc;
    tmp = crc;
    crc = (crc << 8) | (tmp >> 8 & 0xFF);
       
    return crc;

#else /* CYDEV_BOOTLOADER_CHECKSUM == CYDEV_BOOTLOADER_CHECKSUM_BASIC */
    uint16 CYDATA sum = 0;
    while (size-- > 0)
    {
        sum += *buffer++;
    }
    
    return (1 + ~sum);
#endif /* CYDEV_BOOTLOADER_CHECKSUM == CYDEV_BOOTLOADER_CHECKSUM_BASIC */
}


/*******************************************************************************
* Function Name: Calc8BitFlashSum
********************************************************************************
* Summary:
*   This computes the 8 bit sum for the provided number of bytes contained
*   in flash.
*
* Parameters:  
*   start:
*      The starting address to start summing data for
*   size:
*      The number of bytes to read and compute the sum for
*
* Returns:
*   8 bit sum for the provided data
*
*******************************************************************************/
uint8 Calc8BitFlashSum(uint32 start, uint32 size) CYSMALL
{
    uint8 CYDATA sum = 0;
    while (size-- > 0)
    {
        sum += CY_GETCODEDATA(start + size);
    }
    
    return sum;
}


/*******************************************************************************
* Function Name: CyBtldr_Start
********************************************************************************
* Summary:
*   Runs the bootloading algorithm, determining if a bootload is necessary and
*    launching the application if it is not.
*
* Parameters:
*   void:
*   
* Return:
*   This method will never return. It will either load a new application and reset
*    the device or it will jump directly to the existing application.
*
* Remark:
*    If this method determines that the bootloader appliation itself is corrupt,
*    this method will not return, instead it will simply hang the application.
*
*******************************************************************************/
void CyBtldr_Start(void) CYSMALL
{
    uint8 CYDATA calcedChecksum = 
        Calc8BitFlashSum(CYBTDLR_CHECKSUM_START, *CyBtldr_SizeBytesAccess - CYBTDLR_CHECKSUM_START);

#if (CYDEV_BOOTLOADER_APPLICATIONS > 1)
    uint8 i;
    for (i = 0; i < CYDEV_BOOTLOADER_APPLICATIONS; i++)
    {
        if (*CYBTLDR_P_APP_ACTIVE(i) == 1)
        {
            activeApp = i;
            break;
        }
    }
#endif

    calcedChecksum -= *CyBtldr_ChecksumAccess; /* we actually included the checksum, so remove it */
    calcedChecksum = 1 + ~calcedChecksum;
    if ((calcedChecksum != *CyBtldr_ChecksumAccess) || !(*CyBtldr_SizeBytesAccess) || !(INIT_FLASH_WRITE))
    {
        CyBtldr_BadBootloader();
    }
    
    if (CyBtldr_ValidateApp(activeApp) ||
#if defined (WORKAROUND_OPT_XRES)
    (BL_APP_RUN_ADDR) == CYBTLDR_START_BTLDR)
#else
    CYBTLDR_GET_RUN_TYPE == CYBTLDR_START_BTLDR)
#endif
    {
        CYBTDLR_SET_RUN_TYPE(0);
        CyBtldr_HostLink(0);
    }

#if (CYDEV_BOOTLOADER_WAIT_COMMAND == 1)
    CyBtldr_HostLink(CYDEV_BOOTLOADER_WAIT_TIME); /* Timeout is in 10s of miliseconds */
#endif
    CyBtldr_LaunchApplication();
}


/*******************************************************************************
* Function Name: CyBtldr_LaunchApplication
********************************************************************************
* Summary:
*   Jumps the PC to the start address of the user application in flash.
*
* Parameters:  
*   None
*
* Returns:
*   This method will never return if it succesfully goes to the user application.
*
*******************************************************************************/
void CyBtldr_LaunchApplication(void) CYSMALL
{
    CYBTDLR_SET_RUN_TYPE(CYBTLDR_START_APP);
    CYBTLDR_SOFTWARE_RESET;
}


/*******************************************************************************
* Function Name: CyBtldr_CheckLaunch
********************************************************************************
* Summary:
*   This routine checks to see if the bootloader or the bootloadable application
*   should be run.  If the application is to be run, it will start executing.
*   If the bootloader is to be run, it will return so the bootloader can 
*   continue starting up.
*
* Parameters:  
*   None
*
* Returns:
*   None
*
*******************************************************************************/
void CyBtldr_CheckLaunch(void) CYSMALL
{
#if defined (WORKAROUND_OPT_XRES)
    if ((BL_APP_RUN_ADDR) == CYBTLDR_START_APP)
    {
        if (CYRET_SUCCESS != CySetTemp() || CYRET_SUCCESS != CySetFlashEEBuffer(flashBuffer))
        {
            CyBtldr_BadBootloader();
        }
#else
    if (CYBTLDR_GET_RUN_TYPE == CYBTLDR_START_APP)
    {
#endif
        CYBTDLR_SET_RUN_TYPE(0);
        /* indicates that we have told ourselves to jump to the application since we
         * have already told ourselves to jump, we do not do any expensive verification
         * of the application. We just check to make sure that the value at 
         * CY_APP_ADDR_ADDRESS is something other than 0
         */
        if (BL_APP_ENTRY_ADDR)
        {
            LaunchApp(BL_APP_ENTRY_ADDR); /* we never return from this method */
        }
    }
}


/* Moves the arguement appAddr (RO) into PC, moving execution to the appAddr */
#if defined (__ARMCC_VERSION)
__asm void LaunchApp(uint32 appAddr)
{
#if (CY_PSOC5)
    EXTERN CyResetStatus
    LDR R2, =CyResetStatus
    LDR R2, [R2]
    NOP
#endif
    BX  R0

	ALIGN
}
#elif defined(__GNUC__)
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void LaunchApp(uint32 appAddr)
{
#if (CY_PSOC5)
    __asm volatile(
        "    MOV R2, %0\n"
        "    LDR  R2, [R2]\n"
        "    BX  %1\n"
        : : "r" (CyResetStatus), "r" (appAddr)
        : "r0", "r2");
#else
    __asm volatile("    BX  R0\n");
#endif
}
#endif


/*******************************************************************************
* Function Name: CyBtldr_ValidateApplication
********************************************************************************
* Summary:
*   This routine computes the checksum, zero check, 0xFF check of the
*   application area to determine whether a valid application is loaded.
*
* Parameters:  
*   appId:
*       The application number to verify
*
* Returns:
*   CYRET_SUCCESS  - if successful
*   CYRET_BAD_DATA - if the bootloadable is corrupt
*
*******************************************************************************/
#if (CYDEV_BOOTLOADER_APPLICATIONS > 1)
    cystatus CyBtldr_ValidateApp(uint8 appId) CYSMALL
{
#else
    cystatus CyBtldr_ValidateApplication() CYSMALL
{
#endif
    uint32 CYDATA idx;
    uint32 CYDATA end = FIRST_APP_BYTE + BL_APP_BYTE_LEN;
    CYBIT         valid = 0; /* Assume bad flash image */
    uint8  CYDATA calcedChecksum = 0;
    
#if (CYDEV_BOOTLOADER_APPLICATIONS > 1)
    if (appId >= CYDEV_BOOTLOADER_APPLICATIONS)
    {
        return CYRET_BAD_DATA;
    }
#endif
#if (CYDEV_BOOTLOADER_FAST_VERIFY)
    if (BL_APP_VERIFIED)
    {
        return CYRET_SUCCESS;
    }
#endif

    for (idx = FIRST_APP_BYTE; idx < end; ++idx)
    {
        uint8 CYDATA curByte = CY_GETCODEDATA(idx);
        if (curByte != 0 && curByte != 0xFF)
        {
            valid = 1;
        }

        calcedChecksum += curByte;
    }

    /* We do not compute checksum over the meta data section, so no need to subtract off
     * App Verified or App Active information here like we do when verifying a row.
     */


#if ((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0))
    /* Add ECC data to checksum */
    idx = ((FIRST_APP_BYTE) >> 3);
    /* Flash may run into meta data, ECC does not so use full row */
    end = (end == (FLASH_SIZE - CY_META_DATA_SIZE))
        ? FLASH_SIZE >> 3
        : end >> 3;
    for (; idx < end; ++idx)
    {
        calcedChecksum += CY_GET_XTND_REG8((void CYFAR *)(CYDEV_ECC_BASE + idx));
    }
#endif

    calcedChecksum = 1 + ~calcedChecksum;
    if (calcedChecksum != BL_APP_CHECKSUM || !valid)
    {
        return CYRET_BAD_DATA;
    }

#if (CYDEV_BOOTLOADER_FAST_VERIFY)
    CyBtldr_SetFlashByte(CY_BTLDR_MD_APP_VERIFIED_ADDR, 1);
#endif
    
    return CYRET_SUCCESS;
}


/*******************************************************************************
* Function Name: CyBtldr_HostLink
********************************************************************************
* Summary:
*   Causes the bootloader to attempt to read data being transmitted by the
*   host application.  If data is sent from the host, this establishes the
*   communication interface to process all requests.
*
* Parameters:
*   timeOut:
*       The amount of time to listen for data before giving up. Timeout is 
*       measured in 10s of ms.  Use 0 for infinite wait.
*
* Return:
*   None
*
*******************************************************************************/
void CyBtldr_HostLink(uint8 timeOut)
{
    uint16 CYDATA numberRead;
    uint16 CYDATA rspSize;
    uint8 CYDATA ackCode;
    uint16 CYDATA pktChecksum;
    cystatus CYDATA readStat;
    uint16 CYDATA pktSize = 0;
    uint16 CYDATA dataOffset = 0;
#if (CYDEV_BOOTLOADER_APPLICATIONS <= 1)
    uint8 CYDATA clearedMetaData = 0;
#endif
    CYBIT communicationActive = (timeOut == 0);
    uint8 packetBuffer[SIZEOF_COMMAND_BUFFER];
    uint8 dataBuffer[SIZEOF_COMMAND_BUFFER];

    if (timeOut == 0)
    {
        timeOut = 0xFF;
    }

    /* Initialize communications channel. */
    CyBtldrCommStart();

    /* Make sure global interrupts are enabled. */
    CyGlobalIntEnable;

    do
    {
        ackCode = CYRET_SUCCESS;
        readStat = CyBtldrCommRead(packetBuffer, SIZEOF_COMMAND_BUFFER, &numberRead, timeOut);
        if (readStat == CYRET_TIMEOUT || readStat == CYRET_EMPTY)
        {
            continue;
        }

        if (numberRead < MIN_PKT_SIZE || packetBuffer[CYBTLDR_SOP_ADDR] != SOP)
        {
            ackCode = CYRET_ERR_DATA;
        }
        else
        {
            pktSize = CY_GET_REG16(&packetBuffer[CYBTLDR_SIZE_ADDR]);
            pktChecksum = CY_GET_REG16(&packetBuffer[CYBTLDR_CHK_ADDR(pktSize)]);

            if ((pktSize + MIN_PKT_SIZE) > numberRead)
            {
                ackCode = CYRET_ERR_LENGTH;
            }
            else if (packetBuffer[CYBTLDR_EOP_ADDR(pktSize)] != EOP)
            {
                ackCode = CYRET_ERR_DATA;
            }
            else if (pktChecksum != CalcPacketChecksum(packetBuffer, pktSize + CYBTLDR_DATA_ADDR))
            {
                ackCode = CYRET_ERR_CHECKSUM;
            }
        }

        rspSize = 0;
        if (ackCode == CYRET_SUCCESS)
        {
            uint8 CYDATA btldrData = packetBuffer[CYBTLDR_DATA_ADDR];

            ackCode = CYRET_ERR_DATA;
            switch (packetBuffer[CYBTLDR_CMD_ADDR])
            {
                case CYBTLDR_COMMAND_CHECKSUM:
                    if (communicationActive && pktSize == 0)
                    {
                        rspSize = 1;
                        packetBuffer[CYBTLDR_DATA_ADDR] = (CyBtldr_ValidateApp(activeApp) == CYRET_SUCCESS);
                        ackCode = CYRET_SUCCESS;
                    }
                    break;

                case CYBTLDR_COMMAND_REPORT_SIZE:
                    if (communicationActive && pktSize == 1)
                    {
                        if (btldrData < FLASH_NUMBER_SECTORS)
                        {
                            uint16 CYDATA startRow = (btldrData == 0)
                                ? (*CyBtldr_SizeBytesAccess / CYDEV_FLS_ROW_SIZE)
                                : 0;

                            packetBuffer[CYBTLDR_DATA_ADDR] = LO8(startRow);
                            packetBuffer[CYBTLDR_DATA_ADDR + 1] = HI8(startRow);
                            packetBuffer[CYBTLDR_DATA_ADDR + 2] = LO8(FLASH_NUMBER_ROWS - 1);
                            packetBuffer[CYBTLDR_DATA_ADDR + 3] = HI8(FLASH_NUMBER_ROWS - 1);
                        }
#if defined(FIRST_EE_ARRAYID)
                        else if (btldrData == FIRST_EE_ARRAYID)
                        {
                            packetBuffer[CYBTLDR_DATA_ADDR] = 0;
                            packetBuffer[CYBTLDR_DATA_ADDR + 1] = 0;
                            packetBuffer[CYBTLDR_DATA_ADDR + 2] = LO8(CYDEV_EE_SIZE / CYDEV_EEPROM_ROW_SIZE);
                            packetBuffer[CYBTLDR_DATA_ADDR + 3] = HI8(CYDEV_EE_SIZE / CYDEV_EEPROM_ROW_SIZE);
                        }
#endif

                        rspSize = 4;
                        ackCode = CYRET_SUCCESS;
                    }
                    break;

#if (CYDEV_BOOTLOADER_APPLICATIONS > 1)
                case CYBTLDR_COMMAND_APP_STATUS:
                    if (communicationActive && pktSize == 1)
                    {
                        rspSize = 2;
                        packetBuffer[CYBTLDR_DATA_ADDR] = CyBtldr_ValidateApp(btldrData);
                        packetBuffer[CYBTLDR_DATA_ADDR + 1] = (uint8)(*CYBTLDR_P_APP_ACTIVE(btldrData));
                        ackCode = CYRET_SUCCESS;
                    }
                    break;
#endif

                case CYBTLDR_COMMAND_ERASE:
                    if (communicationActive && pktSize != 3)
                    {
                        break;
                    }
                    dataOffset = CYBTLDR_FROW_SIZE;
                    memset(dataBuffer, 0, CYBTLDR_FROW_SIZE);
                    /* Fall through to write the row */

                case CYBTLDR_COMMAND_PROGRAM:
                    if (communicationActive && pktSize >= 3)
                    {
                        memcpy(&dataBuffer[dataOffset], &packetBuffer[CYBTLDR_DATA_ADDR + 3], pktSize - 3);
                        dataOffset += (pktSize - 3);

#if (CY_PSOC4)
                        pktSize = CYBTLDR_FROW_SIZE;
#else
                        pktSize = (btldrData > LAST_FLASH_ARRAYID) ? CYDEV_EEPROM_ROW_SIZE : CYBTLDR_FROW_SIZE;
#endif

                        if (dataOffset == pktSize)
                        {
                            dataOffset = CY_GET_REG16(&(packetBuffer[CYBTLDR_DATA_ADDR + 1])); //Row number

#if (CYDEV_BOOTLOADER_APPLICATIONS <= 1)
                            if (!clearedMetaData)
                            {
                                uint8 erase[CYBTLDR_FROW_SIZE];
                                memset(erase, 0, CYBTLDR_FROW_SIZE);
                                #if (CY_PSOC4)
                                    CySysFlashWriteRow(CY_BTLDR_MD_ROW, erase);
                                #else
                                    CyWriteRowFull(CY_BTLDR_MD_ARRAY, CY_BTLDR_MD_ROW, erase, CYBTLDR_FROW_SIZE);
                                #endif
                                clearedMetaData = 1;
                            }
#else
                            if (activeApp < CYDEV_BOOTLOADER_APPLICATIONS)
                            {
#if (CY_PSOC4)
                                uint16 row = dataOffset;
                                uint16 firstRow = CY_GET_REG32(CYBTLDR_MD_LAST_BLDR_ROW_ADDR(activeApp)) + 1;
#else
                                uint16 row = (btldrData * (FLASH_SIZEOF_SECTOR / CYDEV_FLS_ROW_SIZE)) + dataOffset;
                                uint16 firstRow = (*((uint16 CYCODE *)(CYBTLDR_MD_LAST_BLDR_ROW_ADDR(activeApp)))) + 1;
#endif
                                /* Last row is equal to the first row plus the number of rows available for each app.
                                 * To compute this, we first subtract the number of appliaction images from the total
                                 * flash rows: (FLASH_NUMBER_ROWS - CYDEV_BOOTLOADER_APPLICATIONS).
                                 * Then subtract off the first row:
                                 * App Rows = (FLASH_NUMBER_ROWS - CYDEV_BOOTLOADER_APPLICATIONS - firstRow)
                                 * Then divide that number by the number of application that must fit within the space,
                                 * if we are app1 then that number is 2, if app2 then 1.  Our divisor is then:
                                 * (CYDEV_BOOTLOADER_APPLICATIONS - activeApp).
                                 * Adding this number to firstRow gives the address right beyond our valid range so
                                 * we subtract 1.
                                 */
                                uint16 lastRow = (firstRow - 1) + ((FLASH_NUMBER_ROWS - CYDEV_BOOTLOADER_APPLICATIONS - firstRow) / (CYDEV_BOOTLOADER_APPLICATIONS - activeApp));

                                /* Check to see if the row to program is within the range of the active application,
                                 * or if it maches the active application's meta data row.  If so, refuse to program
                                 * as it would corrupt the active app.
                                 */
                                if ((row >= firstRow && row <= lastRow) || (btldrData == CY_BTLDR_MD_ARRAY && dataOffset == CY_BTLDR_MD_ROW(activeApp)))
                                {
                                    ackCode = CYRET_ERR_ACTIVE;
                                    dataOffset = 0;
                                    break;
                                }
                            }
#endif

                            #if (CY_PSOC4)
                                ackCode = CySysFlashWriteRow(dataOffset, dataBuffer)
                            #else
                                ackCode = CyWriteRowFull(btldrData, dataOffset, dataBuffer, pktSize)
                            #endif
                                    ? CYRET_ERR_ROW : CYRET_SUCCESS;
                        }
                        else
                        {
                            ackCode = CYRET_ERR_LENGTH;
                        }
                        dataOffset = 0;
                    }
                    break;

                case CYBTLDR_COMMAND_SYNC:
                    /* If something failed the host would send this command to reset the bootloader. */
                    dataOffset = 0;
                    continue; /* Don't ack the packet, just get ready to accept the next one */

#if (CYDEV_BOOTLOADER_APPLICATIONS > 1)
                case CYBTLDR_COMMAND_APP_ACTIVE:
                    if (communicationActive && pktSize == 1)
                    {
                        if (!CyBtldr_ValidateApp(btldrData))
                        {
                            uint8 CYDATA idx;
                            for (idx = 0; idx < CYDEV_BOOTLOADER_APPLICATIONS; idx++)
                            {
                                CyBtldr_SetFlashByte(CYBTLDR_MD_APP_ACTIVE_ADDR(idx), idx == btldrData);
                            }
                            activeApp = btldrData;
                            ackCode = CYRET_SUCCESS;
                        }
                        else
                        {
                            ackCode = CYRET_ERR_APP;
                        }
                    }
                    break;
#endif

                case CYBTLDR_COMMAND_DATA:
                    /* We have part of a block of data. */
                    ackCode = CYRET_SUCCESS;
                    memcpy(&dataBuffer[dataOffset], &packetBuffer[CYBTLDR_DATA_ADDR], pktSize);
                    dataOffset += pktSize;
                    break;

                case CYBTLDR_COMMAND_ENTER:
                    if (pktSize == 0)
                    {
                        #if (defined(__C51__))
                            CYBTLDR_ENTER CYDATA BtldrVersion = 
                                {CYSWAP_ENDIAN32(CYDEV_CHIP_JTAG_ID), CYDEV_CHIP_REV_EXPECT, CYBTLDR_VERSION};
                        #else
                            CYBTLDR_ENTER CYDATA BtldrVersion = 
                                {CYDEV_CHIP_JTAG_ID, CYDEV_CHIP_REV_EXPECT, CYBTLDR_VERSION};
                        #endif
                        communicationActive = 1;
                        rspSize = sizeof(CYBTLDR_ENTER);
                        memcpy(&packetBuffer[CYBTLDR_DATA_ADDR], &BtldrVersion, rspSize);
                        ackCode = CYRET_SUCCESS;
                    }
                    break;

                case CYBTLDR_COMMAND_VERIFY:
                    if (communicationActive && pktSize == 3)
                    {
                        /* Read the existing flash data. */
                        uint16 CYDATA rowNum = CY_GET_REG16(&(packetBuffer[CYBTLDR_DATA_ADDR + 1]));
                        uint32 CYDATA rowAddr = ((uint32)btldrData * CYDEV_FLS_SECTOR_SIZE) 
                            + ((uint32)rowNum * CYDEV_FLS_ROW_SIZE);
                        uint8 CYDATA checksum = Calc8BitFlashSum(rowAddr, CYDEV_FLS_ROW_SIZE);

#if (!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0)
                        /* Save the ECC area. */
                        uint16 CYDATA index;
                        rowAddr = CYDEV_ECC_BASE + ((uint32)btldrData * CYDEV_FLS_SECTOR_SIZE / 8) 
                            + ((uint32)rowNum * CYDEV_ECC_ROW_SIZE);
                        for (index = 0; index < CYDEV_ECC_ROW_SIZE; index++)
                        {
                            checksum += CY_GET_XTND_REG8((uint8 CYFAR *)(rowAddr + index));
                        }
#endif

                        /* App Verified & App Active are information that is updated in flash at runtime
                         * remove these items from the checksum to allow the host to verify everything is
                         * correct.
                         */
#if (CYDEV_BOOTLOADER_APPLICATIONS > 1)
                        if (CY_BTLDR_MD_ARRAY == btldrData && (CY_BTLDR_MD_ROW(0) == rowNum || CY_BTLDR_MD_ROW(1) == rowNum))
#else
                        if (CY_BTLDR_MD_ARRAY == btldrData && CY_BTLDR_MD_ROW == rowNum)
#endif
                        {
                            uint8 appId = ((rowNum & 1) == 1) ? 0 : 1;
                            checksum -= *CYBTLDR_P_APP_ACTIVE(appId);
                            checksum -= *CY_BTLDR_P_APP_VERIFIED;
                        }

                        packetBuffer[CYBTLDR_DATA_ADDR] = 1 + ~checksum;
                        ackCode = CYRET_SUCCESS;
                        rspSize = 1;
                    }
                    break;

                case CYBTLDR_COMMAND_EXIT:
                    if (!CyBtldr_ValidateApp(activeApp))
                    {
                        CYBTDLR_SET_RUN_TYPE(CYBTLDR_START_APP);
                    }

                    CYBTLDR_SOFTWARE_RESET;
                    continue; /* Will never get here */

                default:
                    ackCode = CYRET_ERR_CMD;
                    break;
            }
        }

        /* ?CK the packet and function. */
        WritePacket(ackCode, packetBuffer, rspSize);
    }
    while (communicationActive);
}


/*******************************************************************************
* Function Name: WritePacket
********************************************************************************
* Summary:
*   Creates a bootloader responce packet and transmits it back to the bootloader
*   host application over the already established communications protocol.
*
* Parameters:
*   status:
*       The status code to pass back as the second byte of the packet
*   buffer:
*       The buffer containing the data portion of the packet
*   size:
*       The number of bytes contained within the buffer to pass back
*   
* Return:
*   CYRET_SUCCESS if successful.
*   CYRET_UNKNOWN if there was an error tranmitting the packet.
*
*******************************************************************************/
int WritePacket(uint8 status, uint8 CYXDATA* buffer, uint16 size) CYSMALL
{
    uint16 CYDATA checksum;

    /* Start of the packet. */
    buffer[CYBTLDR_SOP_ADDR] = SOP;
    buffer[CYBTLDR_CMD_ADDR] = status;
    buffer[CYBTLDR_SIZE_ADDR] = LO8(size);
    buffer[CYBTLDR_SIZE_ADDR + 1] = HI8(size);

    /* Compute the checksum. */
    checksum = CalcPacketChecksum(buffer, size + CYBTLDR_DATA_ADDR);

    buffer[CYBTLDR_CHK_ADDR(size)] = LO8(checksum);
    buffer[CYBTLDR_CHK_ADDR(1 + size)] = HI8(checksum);
    buffer[CYBTLDR_EOP_ADDR(size)] = EOP;

    /* Start the packet transmit. */
    return CyBtldrCommWrite(buffer, size + MIN_PKT_SIZE, &size, 150);
}
