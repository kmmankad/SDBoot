/*******************************************************************************
* File Name: USB_Bootloader.h
* Version 2.12
*
* Description:
*  Header File for the USFS component. Contains prototypes and constant values.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_USBFS_USB_Bootloader_H)
#define CY_USBFS_USB_Bootloader_H

#include "cytypes.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC3 and CY_PSOC5 are available */
/* They are defined starting with cy_boot v2.30 */
#ifndef CY_PSOC3
    #error Component USBFS_v2_12 requires cy_boot v2.30 or later
#endif /* End CY_PSOC3 */

#if(CY_PSOC3_ES2)
    #include <intrins.h>
    #define USB_Bootloader_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
#endif  /* End PSOC3_ES2 */


/***************************************
*  Memory Type Definitions
***************************************/

/* Renamed Type Definitions for backward compatibility.
*  Should not be used in new designs.
*/
#define USB_Bootloader_CODE CYCODE
#define USB_Bootloader_FAR CYFAR
#if defined(__C51__) || defined(__CX51__)
    #define USB_Bootloader_DATA data
    #define USB_Bootloader_XDATA xdata
#else
    #define USB_Bootloader_DATA
    #define USB_Bootloader_XDATA
#endif /* End __C51__ */
#define USB_Bootloader_NULL       NULL


/***************************************
* Enumerated Types and Parameters
***************************************/

#define USB_Bootloader__EP_MANUAL 0
#define USB_Bootloader__EP_DMAMANUAL 1
#define USB_Bootloader__EP_DMAAUTO 2

#define USB_Bootloader__MA_STATIC 0
#define USB_Bootloader__MA_DYNAMIC 1



/***************************************
*    Initial Parameter Constants
***************************************/

#define USB_Bootloader_NUM_DEVICES   1
#define USB_Bootloader_ENABLE_DESCRIPTOR_STRINGS   
#define USB_Bootloader_ENABLE_MSOS_STRING   
#define USB_Bootloader_ENABLE_SN_STRING   
#define USB_Bootloader_ENABLE_STRINGS   
#define USB_Bootloader_DEVICE0_CONFIGURATION0_INTERFACE0_ALTERNATE0_HID_IN_BUF_SIZE   65
#define USB_Bootloader_DEVICE0_CONFIGURATION0_INTERFACE0_ALTERNATE0_HID_NUM_IN_RPTS   1
#define USB_Bootloader_DEVICE0_CONFIGURATION0_INTERFACE0_ALTERNATE0_HID_OUT_BUF_SIZE   65
#define USB_Bootloader_DEVICE0_CONFIGURATION0_INTERFACE0_ALTERNATE0_HID_NUM_OUT_RPTS   1
#define USB_Bootloader_DEVICE0_CONFIGURATION0_INTERFACE0_ALTERNATE0_HID_COUNT   1
#define USB_Bootloader_ENABLE_HID_CLASS   
#define USB_Bootloader_HID_RPT_1_SIZE_LSB   0x24u
#define USB_Bootloader_HID_RPT_1_SIZE_MSB   0x00u
#define USB_Bootloader_MAX_REPORTID_NUMBER   0

#define USB_Bootloader_MON_VBUS                       (0u)
#define USB_Bootloader_EXTERN_VND                     (0u)
#define USB_Bootloader_EXTERN_CLS                     (0u)
#define USB_Bootloader_MAX_INTERFACES_NUMBER          (1u)
#define USB_Bootloader_EP0_ISR_REMOVE                 (0u)
#define USB_Bootloader_EP1_ISR_REMOVE                 (0u)
#define USB_Bootloader_EP2_ISR_REMOVE                 (0u)
#define USB_Bootloader_EP3_ISR_REMOVE                 (1u)
#define USB_Bootloader_EP4_ISR_REMOVE                 (1u)
#define USB_Bootloader_EP5_ISR_REMOVE                 (1u)
#define USB_Bootloader_EP6_ISR_REMOVE                 (1u)
#define USB_Bootloader_EP7_ISR_REMOVE                 (1u)
#define USB_Bootloader_EP8_ISR_REMOVE                 (1u)
#define USB_Bootloader_EP_MM                          (0u)
#define USB_Bootloader_EP_MA                          (0u)
#define USB_Bootloader_DMA1_REMOVE                    (1u)
#define USB_Bootloader_DMA2_REMOVE                    (1u)
#define USB_Bootloader_DMA3_REMOVE                    (1u)
#define USB_Bootloader_DMA4_REMOVE                    (1u)
#define USB_Bootloader_DMA5_REMOVE                    (1u)
#define USB_Bootloader_DMA6_REMOVE                    (1u)
#define USB_Bootloader_DMA7_REMOVE                    (1u)
#define USB_Bootloader_DMA8_REMOVE                    (0u)
#define USB_Bootloader_SOF_ISR_REMOVE                 (0u)
#define USB_Bootloader_ARB_ISR_REMOVE                 (0u)
#define USB_Bootloader_DP_ISR_REMOVE                  (0u)
#define USB_Bootloader_ENABLE_CDC_CLASS_API           (1u)
#define USB_Bootloader_ENABLE_MIDI_API                (1u)
#define USB_Bootloader_MIDI_EXT_MODE                  (0u)


/***************************************
*    Data Struct Definition
***************************************/

typedef struct _USB_Bootloader_EpCtlBlock
{
    uint8  attrib;
    uint8  apiEpState;
    uint8  hwEpState;
    uint8  epToggle;
    uint8  addr;
    uint8  epMode;
    uint16 buffOffset;
    uint16 bufferSize;
    uint8  interface;
} T_USB_Bootloader_EP_CTL_BLOCK;

typedef struct _USB_Bootloader_EpSettingsBlock
{
    uint8  interface;
    uint8  altSetting;
    uint8  addr;
    uint8  attributes;
    uint16 bufferSize;
    uint8  bMisc;
} T_USB_Bootloader_EP_SETTINGS_BLOCK;

typedef struct _USB_Bootloader_XferStatusBlock
{
    uint8  status;
    uint16 length;
} T_USB_Bootloader_XFER_STATUS_BLOCK;

typedef struct _USB_Bootloader_Td
{
    uint16  count;
    volatile uint8 *pData;
    T_USB_Bootloader_XFER_STATUS_BLOCK *pStatusBlock;
} T_USB_Bootloader_TD;

typedef struct _USB_Bootloader_Lut
{
    uint8   c;
    void    *p_list;
} T_USB_Bootloader_LUT;

/* Resume/Suspend API Support */
typedef struct _USB_Bootloader_BackupStruct
{
    uint8 enableState;
    uint8 mode;
} USB_Bootloader_BACKUP_STRUCT;


/* Renamed struct fields for backward compatibility.
*  Should not be used in new designs.
*/
#define wBuffOffset         buffOffset
#define wBufferSize         bufferSize
#define bStatus             status
#define wLength             length
#define wCount              count

/* Renamed global variable for backward compatibility.
*  Should not be used in new designs.
*/
#define CurrentTD           USB_Bootloader_currentTD


/***************************************
*       Function Prototypes
***************************************/

void   USB_Bootloader_Start(uint8 device, uint8 mode) ;
void   USB_Bootloader_Init(void) ;
void   USB_Bootloader_InitComponent(uint8 device, uint8 mode) ;
void   USB_Bootloader_Stop(void) ;
uint8  USB_Bootloader_CheckActivity(void) ;
uint8  USB_Bootloader_GetConfiguration(void) ;
uint8 USB_Bootloader_IsConfigurationChanged(void) ;
uint8  USB_Bootloader_GetInterfaceSetting(uint8 interfaceNumber)
                                                        ;
uint8  USB_Bootloader_GetEPState(uint8 epNumber) ;
uint16 USB_Bootloader_GetEPCount(uint8 epNumber) ;
void   USB_Bootloader_LoadInEP(uint8 epNumber, uint8 *pData, uint16 length)
                                                                    ;
uint16 USB_Bootloader_ReadOutEP(uint8 epNumber, uint8 *pData, uint16 length)
                                                                    ;
void   USB_Bootloader_EnableOutEP(uint8 epNumber) ;
void   USB_Bootloader_DisableOutEP(uint8 epNumber) ;
void   USB_Bootloader_Force(uint8 bState) ;
uint8  USB_Bootloader_GetEPAckState(uint8 epNumber) ;
void   USB_Bootloader_SetPowerStatus(uint8 powerStatus) ;
uint8  USB_Bootloader_RWUEnabled(void) ;
void   USB_Bootloader_SerialNumString(uint8 *snString) ;
void   USB_Bootloader_TerminateEP(uint8 ep) ;

void   USB_Bootloader_Suspend(void) ;
void   USB_Bootloader_Resume(void) ;
#if(CY_PSOC5_ES1)
    uint8 USB_Bootloader_Resume_Condition(void);
#endif /* CY_PSOC5_ES1 */    

#if (USB_Bootloader_MON_VBUS == 1u)
    uint8  USB_Bootloader_VBusPresent(void) ;
#endif /* End USB_Bootloader_MON_VBUS */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USB_Bootloader) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

    void USB_Bootloader_CyBtldrCommStart(void) ;
    void USB_Bootloader_CyBtldrCommStop(void) ;
    void USB_Bootloader_CyBtldrCommReset(void) ;
    cystatus USB_Bootloader_CyBtldrCommWrite(uint8 *pData, uint16 size, uint16 *count, uint8 timeOut) CYSMALL
                                                        ;
    cystatus USB_Bootloader_CyBtldrCommRead( uint8 *pData, uint16 size, uint16 *count, uint8 timeOut) CYSMALL
                                                        ;

    #define USB_Bootloader_BTLDR_SIZEOF_WRITE_BUFFER      (64u)    /* EP 1 OUT */
    #define USB_Bootloader_BTLDR_SIZEOF_READ_BUFFER       (64u)    /* EP 2 IN */
    #define USB_Bootloader_BTLDR_MAX_PACKET_SIZE          USB_Bootloader_BTLDR_SIZEOF_WRITE_BUFFER

    /* These defines active if used USBFS interface as an
    *  IO Component for bootloading. When Custom_Interface selected
    *  in Bootloder configuration as the IO Component, user must
    *  provide these functions
    */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USB_Bootloader)
        #define CyBtldrCommStart        USB_Bootloader_CyBtldrCommStart
        #define CyBtldrCommStop         USB_Bootloader_CyBtldrCommStop
        #define CyBtldrCommReset        USB_Bootloader_CyBtldrCommReset
        #define CyBtldrCommWrite        USB_Bootloader_CyBtldrCommWrite
        #define CyBtldrCommRead         USB_Bootloader_CyBtldrCommRead
    #endif  /*End   CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USB_Bootloader */

#endif /* End CYDEV_BOOTLOADER_IO_COMP  */

#if(USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL)
    void USB_Bootloader_InitEP_DMA(uint8 epNumber, uint8 *pData) ;
    void USB_Bootloader_Stop_DMA(uint8 epNumber) ;
#endif /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL) */

#if defined(USB_Bootloader_ENABLE_MIDI_STREAMING) && (USB_Bootloader_ENABLE_MIDI_API != 0u)
    void USB_Bootloader_MIDI_EP_Init(void) ;
    void USB_Bootloader_MIDI_IN_Service(void) ;
    uint8 USB_Bootloader_PutUsbMidiIn(uint8 ic, uint8* midiMsg, uint8 cable)
                                                            ;
    void USB_Bootloader_MIDI_OUT_EP_Service(void) ;
#endif /* End USB_Bootloader_ENABLE_MIDI_API != 0u */

/* Renamed Functions for backward compatibility.
*  Should not be used in new designs.
*/

#define USB_Bootloader_bCheckActivity             USB_Bootloader_CheckActivity
#define USB_Bootloader_bGetConfiguration          USB_Bootloader_GetConfiguration
#define USB_Bootloader_bGetInterfaceSetting       USB_Bootloader_GetInterfaceSetting
#define USB_Bootloader_bGetEPState                USB_Bootloader_GetEPState
#define USB_Bootloader_wGetEPCount                USB_Bootloader_GetEPCount
#define USB_Bootloader_bGetEPAckState             USB_Bootloader_GetEPAckState
#define USB_Bootloader_bRWUEnabled                USB_Bootloader_RWUEnabled
#define USB_Bootloader_bVBusPresent               USB_Bootloader_VBusPresent

#define USB_Bootloader_bConfiguration             USB_Bootloader_configuration
#define USB_Bootloader_bInterfaceSetting          USB_Bootloader_interfaceSetting
#define USB_Bootloader_bDeviceAddress             USB_Bootloader_deviceAddress
#define USB_Bootloader_bDeviceStatus              USB_Bootloader_deviceStatus
#define USB_Bootloader_bDevice                    USB_Bootloader_device
#define USB_Bootloader_bTransferState             USB_Bootloader_transferState
#define USB_Bootloader_bLastPacketSize            USB_Bootloader_lastPacketSize

#define USB_Bootloader_LoadEP                     USB_Bootloader_LoadInEP
#define USB_Bootloader_LoadInISOCEP               USB_Bootloader_LoadInEP
#define USB_Bootloader_EnableOutISOCEP(e)         USB_Bootloader_EnableOutEP(e)

#define USB_Bootloader_SetVector                  CyIntSetVector
#define USB_Bootloader_SetPriority                CyIntSetPriority
#define USB_Bootloader_EnableInt(a, b)            CyIntEnable(a)


/***************************************
*          API Constants
***************************************/

#define USB_Bootloader_EP0                        (0u)
#define USB_Bootloader_EP1                        (1u)
#define USB_Bootloader_EP2                        (2u)
#define USB_Bootloader_EP3                        (3u)
#define USB_Bootloader_EP4                        (4u)
#define USB_Bootloader_EP5                        (5u)
#define USB_Bootloader_EP6                        (6u)
#define USB_Bootloader_EP7                        (7u)
#define USB_Bootloader_EP8                        (8u)
#define USB_Bootloader_MAX_EP                     (9u)

#define USB_Bootloader_TRUE                       (1u)
#define USB_Bootloader_FALSE                      (0u)

#define USB_Bootloader_NO_EVENT_ALLOWED           (2u)
#define USB_Bootloader_EVENT_PENDING              (1u)
#define USB_Bootloader_NO_EVENT_PENDING           (0u)

#define USB_Bootloader_IN_BUFFER_FULL             USB_Bootloader_NO_EVENT_PENDING
#define USB_Bootloader_IN_BUFFER_EMPTY            USB_Bootloader_EVENT_PENDING
#define USB_Bootloader_OUT_BUFFER_FULL            USB_Bootloader_EVENT_PENDING
#define USB_Bootloader_OUT_BUFFER_EMPTY           USB_Bootloader_NO_EVENT_PENDING

#define USB_Bootloader_FORCE_J                    (0xA0u)
#define USB_Bootloader_FORCE_K                    (0x80u)
#define USB_Bootloader_FORCE_SE0                  (0xC0u)
#define USB_Bootloader_FORCE_NONE                 (0x00u)

#define USB_Bootloader_IDLE_TIMER_RUNNING         (0x02u)
#define USB_Bootloader_IDLE_TIMER_EXPIRED         (0x01u)
#define USB_Bootloader_IDLE_TIMER_INDEFINITE      (0x00u)

#define USB_Bootloader_DEVICE_STATUS_BUS_POWERED  (0x00u)
#define USB_Bootloader_DEVICE_STATUS_SELF_POWERED (0x01u)

#define USB_Bootloader_3V_OPERATION               (0x00u)
#define USB_Bootloader_5V_OPERATION               (0x01u)
#define USB_Bootloader_DWR_VDDD_OPERATION         (0x02u)

#define USB_Bootloader_MODE_DISABLE               (0x00u)
#define USB_Bootloader_MODE_NAK_IN_OUT            (0x01u)
#define USB_Bootloader_MODE_STATUS_OUT_ONLY       (0x02u)
#define USB_Bootloader_MODE_STALL_IN_OUT          (0x03u)
#define USB_Bootloader_MODE_RESERVED_0100         (0x04u)
#define USB_Bootloader_MODE_ISO_OUT               (0x05u)
#define USB_Bootloader_MODE_STATUS_IN_ONLY        (0x06u)
#define USB_Bootloader_MODE_ISO_IN                (0x07u)
#define USB_Bootloader_MODE_NAK_OUT               (0x08u)
#define USB_Bootloader_MODE_ACK_OUT               (0x09u)
#define USB_Bootloader_MODE_RESERVED_1010         (0x0Au)
#define USB_Bootloader_MODE_ACK_OUT_STATUS_IN     (0x0Bu)
#define USB_Bootloader_MODE_NAK_IN                (0x0Cu)
#define USB_Bootloader_MODE_ACK_IN                (0x0Du)
#define USB_Bootloader_MODE_RESERVED_1110         (0x0Eu)
#define USB_Bootloader_MODE_ACK_IN_STATUS_OUT     (0x0Fu)
#define USB_Bootloader_MODE_MASK                  (0x0Fu)
#define USB_Bootloader_MODE_STALL_DATA_EP         (0x80u)

#define USB_Bootloader_MODE_ACKD                  (0x10u)
#define USB_Bootloader_MODE_OUT_RCVD              (0x20u)
#define USB_Bootloader_MODE_IN_RCVD               (0x40u)
#define USB_Bootloader_MODE_SETUP_RCVD            (0x80u)

#define USB_Bootloader_RQST_TYPE_MASK             (0x60u)
#define USB_Bootloader_RQST_TYPE_STD              (0x00u)
#define USB_Bootloader_RQST_TYPE_CLS              (0x20u)
#define USB_Bootloader_RQST_TYPE_VND              (0x40u)
#define USB_Bootloader_RQST_DIR_MASK              (0x80u)
#define USB_Bootloader_RQST_DIR_D2H               (0x80u)
#define USB_Bootloader_RQST_DIR_H2D               (0x00u)
#define USB_Bootloader_RQST_RCPT_MASK             (0x03u)
#define USB_Bootloader_RQST_RCPT_DEV              (0x00u)
#define USB_Bootloader_RQST_RCPT_IFC              (0x01u)
#define USB_Bootloader_RQST_RCPT_EP               (0x02u)
#define USB_Bootloader_RQST_RCPT_OTHER            (0x03u)

/* USB Class Codes */
#define USB_Bootloader_CLASS_DEVICE               (0x00u)     /* Use class code info from Interface Descriptors */
#define USB_Bootloader_CLASS_AUDIO                (0x01u)     /* Audio device */
#define USB_Bootloader_CLASS_CDC                  (0x02u)     /* Communication device class */
#define USB_Bootloader_CLASS_HID                  (0x03u)     /* Human Interface Device */
#define USB_Bootloader_CLASS_PDC                  (0x05u)     /* Physical device class */
#define USB_Bootloader_CLASS_IMAGE                (0x06u)     /* Still Imaging device */
#define USB_Bootloader_CLASS_PRINTER              (0x07u)     /* Printer device  */
#define USB_Bootloader_CLASS_MSD                  (0x08u)     /* Mass Storage device  */
#define USB_Bootloader_CLASS_HUB                  (0x09u)     /* Full/Hi speed Hub */
#define USB_Bootloader_CLASS_CDC_DATA             (0x0Au)     /* CDC data device */
#define USB_Bootloader_CLASS_SMART_CARD           (0x0Bu)     /* Smart Card device */
#define USB_Bootloader_CLASS_CSD                  (0x0Du)     /* Content Security device */
#define USB_Bootloader_CLASS_VIDEO                (0x0Eu)     /* Video device */
#define USB_Bootloader_CLASS_PHD                  (0x0Fu)     /* Personal Healthcare device */
#define USB_Bootloader_CLASS_WIRELESSD            (0xDCu)     /* Wireless Controller */
#define USB_Bootloader_CLASS_MIS                  (0xE0u)     /* Miscellaneous */
#define USB_Bootloader_CLASS_APP                  (0xEFu)     /* Application Specific */
#define USB_Bootloader_CLASS_VENDOR               (0xFFu)     /* Vendor specific */


/* Standard Request Types (Table 9-4) */
#define USB_Bootloader_GET_STATUS                 (0x00u)
#define USB_Bootloader_CLEAR_FEATURE              (0x01u)
#define USB_Bootloader_SET_FEATURE                (0x03u)
#define USB_Bootloader_SET_ADDRESS                (0x05u)
#define USB_Bootloader_GET_DESCRIPTOR             (0x06u)
#define USB_Bootloader_SET_DESCRIPTOR             (0x07u)
#define USB_Bootloader_GET_CONFIGURATION          (0x08u)
#define USB_Bootloader_SET_CONFIGURATION          (0x09u)
#define USB_Bootloader_GET_INTERFACE              (0x0Au)
#define USB_Bootloader_SET_INTERFACE              (0x0Bu)
#define USB_Bootloader_SYNCH_FRAME                (0x0Cu)

/* Vendor Specific Request Types */
/* Request for Microsoft OS String Descriptor*/
#define USB_Bootloader_GET_EXTENDED_CONFIG_DESCRIPTOR (0x01u)

/* Descriptor Types (Table 9-5) */
#define USB_Bootloader_DESCR_DEVICE                   (1u)
#define USB_Bootloader_DESCR_CONFIG                   (2u)
#define USB_Bootloader_DESCR_STRING                   (3u)
#define USB_Bootloader_DESCR_INTERFACE                (4u)
#define USB_Bootloader_DESCR_ENDPOINT                 (5u)
#define USB_Bootloader_DESCR_DEVICE_QUALIFIER         (6u)
#define USB_Bootloader_DESCR_OTHER_SPEED              (7u)
#define USB_Bootloader_DESCR_INTERFACE_POWER          (8u)

/* Device Descriptor Defines */
#define USB_Bootloader_DEVICE_DESCR_LENGTH            (18u)
#define USB_Bootloader_DEVICE_DESCR_SN_SHIFT          (16u)

/* Config Descriptor Shifts and Masks */
#define USB_Bootloader_CONFIG_DESCR_LENGTH                (0u)
#define USB_Bootloader_CONFIG_DESCR_TYPE                  (1u)
#define USB_Bootloader_CONFIG_DESCR_TOTAL_LENGTH_LOW      (2u)
#define USB_Bootloader_CONFIG_DESCR_TOTAL_LENGTH_HI       (3u)
#define USB_Bootloader_CONFIG_DESCR_NUM_INTERFACES        (4u)
#define USB_Bootloader_CONFIG_DESCR_CONFIG_VALUE          (5u)
#define USB_Bootloader_CONFIG_DESCR_CONFIGURATION         (6u)
#define USB_Bootloader_CONFIG_DESCR_ATTRIB                (7u)
#define USB_Bootloader_CONFIG_DESCR_ATTRIB_SELF_POWERED   (0x40u)
#define USB_Bootloader_CONFIG_DESCR_ATTRIB_RWU_EN         (0x20u)

/* Feature Selectors (Table 9-6) */
#define USB_Bootloader_DEVICE_REMOTE_WAKEUP           (0x01u)
#define USB_Bootloader_ENDPOINT_HALT                  (0x00u)
#define USB_Bootloader_TEST_MODE                      (0x02u)

/* USB Device Status (Figure 9-4) */
#define USB_Bootloader_DEVICE_STATUS_BUS_POWERED      (0x00u)
#define USB_Bootloader_DEVICE_STATUS_SELF_POWERED     (0x01u)
#define USB_Bootloader_DEVICE_STATUS_REMOTE_WAKEUP    (0x02u)

/* USB Endpoint Status (Figure 9-4) */
#define USB_Bootloader_ENDPOINT_STATUS_HALT           (0x01u)

/* USB Endpoint Directions */
#define USB_Bootloader_DIR_IN                         (0x80u)
#define USB_Bootloader_DIR_OUT                        (0x00u)
#define USB_Bootloader_DIR_UNUSED                     (0x7Fu)

/* USB Endpoint Attributes */
#define USB_Bootloader_EP_TYPE_CTRL                   (0x00u)
#define USB_Bootloader_EP_TYPE_ISOC                   (0x01u)
#define USB_Bootloader_EP_TYPE_BULK                   (0x02u)
#define USB_Bootloader_EP_TYPE_INT                    (0x03u)
#define USB_Bootloader_EP_TYPE_MASK                   (0x03u)

#define USB_Bootloader_EP_SYNC_TYPE_NO_SYNC           (0x00u)
#define USB_Bootloader_EP_SYNC_TYPE_ASYNC             (0x04u)
#define USB_Bootloader_EP_SYNC_TYPE_ADAPTIVE          (0x08u)
#define USB_Bootloader_EP_SYNC_TYPE_SYNCHRONOUS       (0x0Cu)
#define USB_Bootloader_EP_SYNC_TYPE_MASK              (0x0Cu)

#define USB_Bootloader_EP_USAGE_TYPE_DATA             (0x00u)
#define USB_Bootloader_EP_USAGE_TYPE_FEEDBACK         (0x10u)
#define USB_Bootloader_EP_USAGE_TYPE_IMPLICIT         (0x20u)
#define USB_Bootloader_EP_USAGE_TYPE_RESERVED         (0x30u)
#define USB_Bootloader_EP_USAGE_TYPE_MASK             (0x30u)

/* Endpoint Status defines */
#define USB_Bootloader_EP_STATUS_LENGTH               (0x02u)

/* Endpoint Device defines */
#define USB_Bootloader_DEVICE_STATUS_LENGTH           (0x02u)

/* Transfer Completion Notification */
#define USB_Bootloader_XFER_IDLE                      (0x00u)
#define USB_Bootloader_XFER_STATUS_ACK                (0x01u)
#define USB_Bootloader_XFER_PREMATURE                 (0x02u)
#define USB_Bootloader_XFER_ERROR                     (0x03u)

/* Driver State defines */
#define USB_Bootloader_TRANS_STATE_IDLE               (0x00u)
#define USB_Bootloader_TRANS_STATE_CONTROL_READ       (0x02u)
#define USB_Bootloader_TRANS_STATE_CONTROL_WRITE      (0x04u)
#define USB_Bootloader_TRANS_STATE_NO_DATA_CONTROL    (0x06u)

/* String Descriptor defines */
#define USB_Bootloader_STRING_MSOS                    (0xEEu)

#if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAMANUAL)
    /* DMA manual mode defines */
    #define USB_Bootloader_DMA_BYTES_PER_BURST            (0u)
    #define USB_Bootloader_DMA_REQUEST_PER_BURST          (0u)
#endif /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAMANUAL */
#if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO)
    /* DMA automatic mode defines */
    #define USB_Bootloader_DMA_BYTES_PER_BURST            (32u)
    /* BUF_SIZE-BYTES_PER_BURST examples: 55-32 bytes  44-16 bytes 33-8 bytes 22-4 bytes 11-2 bytes */
    #define USB_Bootloader_DMA_BUF_SIZE                   (0x55u)
    #define USB_Bootloader_DMA_REQUEST_PER_BURST          (1u)
#endif /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO */


/***************************************
*              Registers
***************************************/

#define USB_Bootloader_ARB_CFG_PTR        (  (reg8 *) USB_Bootloader_USB__ARB_CFG)
#define USB_Bootloader_ARB_CFG_REG        (* (reg8 *) USB_Bootloader_USB__ARB_CFG)

#define USB_Bootloader_ARB_EP1_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__ARB_EP1_CFG)
#define USB_Bootloader_ARB_EP1_CFG_REG    (* (reg8 *) USB_Bootloader_USB__ARB_EP1_CFG)
#define USB_Bootloader_ARB_EP1_INT_EN_PTR (  (reg8 *) USB_Bootloader_USB__ARB_EP1_INT_EN)
#define USB_Bootloader_ARB_EP1_INT_EN_REG (* (reg8 *) USB_Bootloader_USB__ARB_EP1_INT_EN)
#define USB_Bootloader_ARB_EP1_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_EP1_SR)
#define USB_Bootloader_ARB_EP1_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_EP1_SR)

#define USB_Bootloader_ARB_EP2_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__ARB_EP2_CFG)
#define USB_Bootloader_ARB_EP2_CFG_REG    (* (reg8 *) USB_Bootloader_USB__ARB_EP2_CFG)
#define USB_Bootloader_ARB_EP2_INT_EN_PTR (  (reg8 *) USB_Bootloader_USB__ARB_EP2_INT_EN)
#define USB_Bootloader_ARB_EP2_INT_EN_REG (* (reg8 *) USB_Bootloader_USB__ARB_EP2_INT_EN)
#define USB_Bootloader_ARB_EP2_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_EP2_SR)
#define USB_Bootloader_ARB_EP2_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_EP2_SR)

#define USB_Bootloader_ARB_EP3_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__ARB_EP3_CFG)
#define USB_Bootloader_ARB_EP3_CFG_REG    (* (reg8 *) USB_Bootloader_USB__ARB_EP3_CFG)
#define USB_Bootloader_ARB_EP3_INT_EN_PTR (  (reg8 *) USB_Bootloader_USB__ARB_EP3_INT_EN)
#define USB_Bootloader_ARB_EP3_INT_EN_REG (* (reg8 *) USB_Bootloader_USB__ARB_EP3_INT_EN)
#define USB_Bootloader_ARB_EP3_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_EP3_SR)
#define USB_Bootloader_ARB_EP3_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_EP3_SR)

#define USB_Bootloader_ARB_EP4_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__ARB_EP4_CFG)
#define USB_Bootloader_ARB_EP4_CFG_REG    (* (reg8 *) USB_Bootloader_USB__ARB_EP4_CFG)
#define USB_Bootloader_ARB_EP4_INT_EN_PTR (  (reg8 *) USB_Bootloader_USB__ARB_EP4_INT_EN)
#define USB_Bootloader_ARB_EP4_INT_EN_REG (* (reg8 *) USB_Bootloader_USB__ARB_EP4_INT_EN)
#define USB_Bootloader_ARB_EP4_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_EP4_SR)
#define USB_Bootloader_ARB_EP4_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_EP4_SR)

#define USB_Bootloader_ARB_EP5_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__ARB_EP5_CFG)
#define USB_Bootloader_ARB_EP5_CFG_REG    (* (reg8 *) USB_Bootloader_USB__ARB_EP5_CFG)
#define USB_Bootloader_ARB_EP5_INT_EN_PTR (  (reg8 *) USB_Bootloader_USB__ARB_EP5_INT_EN)
#define USB_Bootloader_ARB_EP5_INT_EN_REG (* (reg8 *) USB_Bootloader_USB__ARB_EP5_INT_EN)
#define USB_Bootloader_ARB_EP5_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_EP5_SR)
#define USB_Bootloader_ARB_EP5_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_EP5_SR)

#define USB_Bootloader_ARB_EP6_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__ARB_EP6_CFG)
#define USB_Bootloader_ARB_EP6_CFG_REG    (* (reg8 *) USB_Bootloader_USB__ARB_EP6_CFG)
#define USB_Bootloader_ARB_EP6_INT_EN_PTR (  (reg8 *) USB_Bootloader_USB__ARB_EP6_INT_EN)
#define USB_Bootloader_ARB_EP6_INT_EN_REG (* (reg8 *) USB_Bootloader_USB__ARB_EP6_INT_EN)
#define USB_Bootloader_ARB_EP6_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_EP6_SR)
#define USB_Bootloader_ARB_EP6_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_EP6_SR)

#define USB_Bootloader_ARB_EP7_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__ARB_EP7_CFG)
#define USB_Bootloader_ARB_EP7_CFG_REG    (* (reg8 *) USB_Bootloader_USB__ARB_EP7_CFG)
#define USB_Bootloader_ARB_EP7_INT_EN_PTR (  (reg8 *) USB_Bootloader_USB__ARB_EP7_INT_EN)
#define USB_Bootloader_ARB_EP7_INT_EN_REG (* (reg8 *) USB_Bootloader_USB__ARB_EP7_INT_EN)
#define USB_Bootloader_ARB_EP7_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_EP7_SR)
#define USB_Bootloader_ARB_EP7_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_EP7_SR)

#define USB_Bootloader_ARB_EP8_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__ARB_EP8_CFG)
#define USB_Bootloader_ARB_EP8_CFG_REG    (* (reg8 *) USB_Bootloader_USB__ARB_EP8_CFG)
#define USB_Bootloader_ARB_EP8_INT_EN_PTR (  (reg8 *) USB_Bootloader_USB__ARB_EP8_INT_EN)
#define USB_Bootloader_ARB_EP8_INT_EN_REG (* (reg8 *) USB_Bootloader_USB__ARB_EP8_INT_EN)
#define USB_Bootloader_ARB_EP8_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_EP8_SR)
#define USB_Bootloader_ARB_EP8_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_EP8_SR)

#define USB_Bootloader_ARB_INT_EN_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_INT_EN)
#define USB_Bootloader_ARB_INT_EN_REG     (* (reg8 *) USB_Bootloader_USB__ARB_INT_EN)
#define USB_Bootloader_ARB_INT_SR_PTR     (  (reg8 *) USB_Bootloader_USB__ARB_INT_SR)
#define USB_Bootloader_ARB_INT_SR_REG     (* (reg8 *) USB_Bootloader_USB__ARB_INT_SR)

#define USB_Bootloader_ARB_RW1_DR_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW1_DR)
#define USB_Bootloader_ARB_RW1_RA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW1_RA)
#define USB_Bootloader_ARB_RW1_RA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW1_RA_MSB)
#define USB_Bootloader_ARB_RW1_WA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW1_WA)
#define USB_Bootloader_ARB_RW1_WA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW1_WA_MSB)

#define USB_Bootloader_ARB_RW2_DR_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW2_DR)
#define USB_Bootloader_ARB_RW2_RA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW2_RA)
#define USB_Bootloader_ARB_RW2_RA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW2_RA_MSB)
#define USB_Bootloader_ARB_RW2_WA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW2_WA)
#define USB_Bootloader_ARB_RW2_WA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW2_WA_MSB)

#define USB_Bootloader_ARB_RW3_DR_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW3_DR)
#define USB_Bootloader_ARB_RW3_RA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW3_RA)
#define USB_Bootloader_ARB_RW3_RA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW3_RA_MSB)
#define USB_Bootloader_ARB_RW3_WA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW3_WA)
#define USB_Bootloader_ARB_RW3_WA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW3_WA_MSB)

#define USB_Bootloader_ARB_RW4_DR_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW4_DR)
#define USB_Bootloader_ARB_RW4_RA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW4_RA)
#define USB_Bootloader_ARB_RW4_RA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW4_RA_MSB)
#define USB_Bootloader_ARB_RW4_WA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW4_WA)
#define USB_Bootloader_ARB_RW4_WA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW4_WA_MSB)

#define USB_Bootloader_ARB_RW5_DR_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW5_DR)
#define USB_Bootloader_ARB_RW5_RA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW5_RA)
#define USB_Bootloader_ARB_RW5_RA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW5_RA_MSB)
#define USB_Bootloader_ARB_RW5_WA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW5_WA)
#define USB_Bootloader_ARB_RW5_WA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW5_WA_MSB)

#define USB_Bootloader_ARB_RW6_DR_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW6_DR)
#define USB_Bootloader_ARB_RW6_RA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW6_RA)
#define USB_Bootloader_ARB_RW6_RA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW6_RA_MSB)
#define USB_Bootloader_ARB_RW6_WA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW6_WA)
#define USB_Bootloader_ARB_RW6_WA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW6_WA_MSB)

#define USB_Bootloader_ARB_RW7_DR_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW7_DR)
#define USB_Bootloader_ARB_RW7_RA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW7_RA)
#define USB_Bootloader_ARB_RW7_RA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW7_RA_MSB)
#define USB_Bootloader_ARB_RW7_WA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW7_WA)
#define USB_Bootloader_ARB_RW7_WA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW7_WA_MSB)

#define USB_Bootloader_ARB_RW8_DR_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW8_DR)
#define USB_Bootloader_ARB_RW8_RA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW8_RA)
#define USB_Bootloader_ARB_RW8_RA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW8_RA_MSB)
#define USB_Bootloader_ARB_RW8_WA_PTR     ((reg8 *) USB_Bootloader_USB__ARB_RW8_WA)
#define USB_Bootloader_ARB_RW8_WA_MSB_PTR ((reg8 *) USB_Bootloader_USB__ARB_RW8_WA_MSB)

#define USB_Bootloader_BUF_SIZE_PTR       (  (reg8 *) USB_Bootloader_USB__BUF_SIZE)
#define USB_Bootloader_BUF_SIZE_REG       (* (reg8 *) USB_Bootloader_USB__BUF_SIZE)
#define USB_Bootloader_BUS_RST_CNT_PTR    (  (reg8 *) USB_Bootloader_USB__BUS_RST_CNT)
#define USB_Bootloader_BUS_RST_CNT_REG    (* (reg8 *) USB_Bootloader_USB__BUS_RST_CNT)
#define USB_Bootloader_CWA_PTR            (  (reg8 *) USB_Bootloader_USB__CWA)
#define USB_Bootloader_CWA_REG            (* (reg8 *) USB_Bootloader_USB__CWA)
#define USB_Bootloader_CWA_MSB_PTR        (  (reg8 *) USB_Bootloader_USB__CWA_MSB)
#define USB_Bootloader_CWA_MSB_REG        (* (reg8 *) USB_Bootloader_USB__CWA_MSB)
#define USB_Bootloader_CR0_PTR            (  (reg8 *) USB_Bootloader_USB__CR0)
#define USB_Bootloader_CR0_REG            (* (reg8 *) USB_Bootloader_USB__CR0)
#define USB_Bootloader_CR1_PTR            (  (reg8 *) USB_Bootloader_USB__CR1)
#define USB_Bootloader_CR1_REG            (* (reg8 *) USB_Bootloader_USB__CR1)

#define USB_Bootloader_DMA_THRES_PTR      (  (reg8 *) USB_Bootloader_USB__DMA_THRES)
#define USB_Bootloader_DMA_THRES_REG      (* (reg8 *) USB_Bootloader_USB__DMA_THRES)
#define USB_Bootloader_DMA_THRES_MSB_PTR  (  (reg8 *) USB_Bootloader_USB__DMA_THRES_MSB)
#define USB_Bootloader_DMA_THRES_MSB_REG  (* (reg8 *) USB_Bootloader_USB__DMA_THRES_MSB)

#define USB_Bootloader_EP_ACTIVE_PTR      (  (reg8 *) USB_Bootloader_USB__EP_ACTIVE)
#define USB_Bootloader_EP_ACTIVE_REG      (* (reg8 *) USB_Bootloader_USB__EP_ACTIVE)
#define USB_Bootloader_EP_TYPE_PTR        (  (reg8 *) USB_Bootloader_USB__EP_TYPE)
#define USB_Bootloader_EP_TYPE_REG        (* (reg8 *) USB_Bootloader_USB__EP_TYPE)

#define USB_Bootloader_EP0_CNT_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_CNT)
#define USB_Bootloader_EP0_CNT_REG        (* (reg8 *) USB_Bootloader_USB__EP0_CNT)
#define USB_Bootloader_EP0_CR_PTR         (  (reg8 *) USB_Bootloader_USB__EP0_CR)
#define USB_Bootloader_EP0_CR_REG         (* (reg8 *) USB_Bootloader_USB__EP0_CR)
#define USB_Bootloader_EP0_DR0_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_DR0)
#define USB_Bootloader_EP0_DR0_REG        (* (reg8 *) USB_Bootloader_USB__EP0_DR0)
#define USB_Bootloader_EP0_DR1_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_DR1)
#define USB_Bootloader_EP0_DR1_REG        (* (reg8 *) USB_Bootloader_USB__EP0_DR1)
#define USB_Bootloader_EP0_DR2_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_DR2)
#define USB_Bootloader_EP0_DR2_REG        (* (reg8 *) USB_Bootloader_USB__EP0_DR2)
#define USB_Bootloader_EP0_DR3_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_DR3)
#define USB_Bootloader_EP0_DR3_REG        (* (reg8 *) USB_Bootloader_USB__EP0_DR3)
#define USB_Bootloader_EP0_DR4_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_DR4)
#define USB_Bootloader_EP0_DR4_REG        (* (reg8 *) USB_Bootloader_USB__EP0_DR4)
#define USB_Bootloader_EP0_DR5_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_DR5)
#define USB_Bootloader_EP0_DR5_REG        (* (reg8 *) USB_Bootloader_USB__EP0_DR5)
#define USB_Bootloader_EP0_DR6_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_DR6)
#define USB_Bootloader_EP0_DR6_REG        (* (reg8 *) USB_Bootloader_USB__EP0_DR6)
#define USB_Bootloader_EP0_DR7_PTR        (  (reg8 *) USB_Bootloader_USB__EP0_DR7)
#define USB_Bootloader_EP0_DR7_REG        (* (reg8 *) USB_Bootloader_USB__EP0_DR7)

#define USB_Bootloader_OSCLK_DR0_PTR      (  (reg8 *) USB_Bootloader_USB__OSCLK_DR0)
#define USB_Bootloader_OSCLK_DR0_REG      (* (reg8 *) USB_Bootloader_USB__OSCLK_DR0)
#define USB_Bootloader_OSCLK_DR1_PTR      (  (reg8 *) USB_Bootloader_USB__OSCLK_DR1)
#define USB_Bootloader_OSCLK_DR1_REG      (* (reg8 *) USB_Bootloader_USB__OSCLK_DR1)

#define USB_Bootloader_PM_ACT_CFG_PTR     (  (reg8 *) USB_Bootloader_USB__PM_ACT_CFG)
#define USB_Bootloader_PM_ACT_CFG_REG     (* (reg8 *) USB_Bootloader_USB__PM_ACT_CFG)
#define USB_Bootloader_PM_STBY_CFG_PTR    (  (reg8 *) USB_Bootloader_USB__PM_STBY_CFG)
#define USB_Bootloader_PM_STBY_CFG_REG    (* (reg8 *) USB_Bootloader_USB__PM_STBY_CFG)

#define USB_Bootloader_SIE_EP_INT_EN_PTR  (  (reg8 *) USB_Bootloader_USB__SIE_EP_INT_EN)
#define USB_Bootloader_SIE_EP_INT_EN_REG  (* (reg8 *) USB_Bootloader_USB__SIE_EP_INT_EN)
#define USB_Bootloader_SIE_EP_INT_SR_PTR  (  (reg8 *) USB_Bootloader_USB__SIE_EP_INT_SR)
#define USB_Bootloader_SIE_EP_INT_SR_REG  (* (reg8 *) USB_Bootloader_USB__SIE_EP_INT_SR)

#define USB_Bootloader_SIE_EP1_CNT0_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP1_CNT0)
#define USB_Bootloader_SIE_EP1_CNT0_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP1_CNT0)
#define USB_Bootloader_SIE_EP1_CNT1_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP1_CNT1)
#define USB_Bootloader_SIE_EP1_CNT1_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP1_CNT1)
#define USB_Bootloader_SIE_EP1_CR0_PTR    (  (reg8 *) USB_Bootloader_USB__SIE_EP1_CR0)
#define USB_Bootloader_SIE_EP1_CR0_REG    (* (reg8 *) USB_Bootloader_USB__SIE_EP1_CR0)

#define USB_Bootloader_SIE_EP2_CNT0_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP2_CNT0)
#define USB_Bootloader_SIE_EP2_CNT0_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP2_CNT0)
#define USB_Bootloader_SIE_EP2_CNT1_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP2_CNT1)
#define USB_Bootloader_SIE_EP2_CNT1_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP2_CNT1)
#define USB_Bootloader_SIE_EP2_CR0_PTR    (  (reg8 *) USB_Bootloader_USB__SIE_EP2_CR0)
#define USB_Bootloader_SIE_EP2_CR0_REG    (* (reg8 *) USB_Bootloader_USB__SIE_EP2_CR0)

#define USB_Bootloader_SIE_EP3_CNT0_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP3_CNT0)
#define USB_Bootloader_SIE_EP3_CNT0_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP3_CNT0)
#define USB_Bootloader_SIE_EP3_CNT1_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP3_CNT1)
#define USB_Bootloader_SIE_EP3_CNT1_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP3_CNT1)
#define USB_Bootloader_SIE_EP3_CR0_PTR    (  (reg8 *) USB_Bootloader_USB__SIE_EP3_CR0)
#define USB_Bootloader_SIE_EP3_CR0_REG    (* (reg8 *) USB_Bootloader_USB__SIE_EP3_CR0)

#define USB_Bootloader_SIE_EP4_CNT0_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP4_CNT0)
#define USB_Bootloader_SIE_EP4_CNT0_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP4_CNT0)
#define USB_Bootloader_SIE_EP4_CNT1_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP4_CNT1)
#define USB_Bootloader_SIE_EP4_CNT1_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP4_CNT1)
#define USB_Bootloader_SIE_EP4_CR0_PTR    (  (reg8 *) USB_Bootloader_USB__SIE_EP4_CR0)
#define USB_Bootloader_SIE_EP4_CR0_REG    (* (reg8 *) USB_Bootloader_USB__SIE_EP4_CR0)

#define USB_Bootloader_SIE_EP5_CNT0_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP5_CNT0)
#define USB_Bootloader_SIE_EP5_CNT0_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP5_CNT0)
#define USB_Bootloader_SIE_EP5_CNT1_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP5_CNT1)
#define USB_Bootloader_SIE_EP5_CNT1_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP5_CNT1)
#define USB_Bootloader_SIE_EP5_CR0_PTR    (  (reg8 *) USB_Bootloader_USB__SIE_EP5_CR0)
#define USB_Bootloader_SIE_EP5_CR0_REG    (* (reg8 *) USB_Bootloader_USB__SIE_EP5_CR0)

#define USB_Bootloader_SIE_EP6_CNT0_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP6_CNT0)
#define USB_Bootloader_SIE_EP6_CNT0_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP6_CNT0)
#define USB_Bootloader_SIE_EP6_CNT1_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP6_CNT1)
#define USB_Bootloader_SIE_EP6_CNT1_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP6_CNT1)
#define USB_Bootloader_SIE_EP6_CR0_PTR    (  (reg8 *) USB_Bootloader_USB__SIE_EP6_CR0)
#define USB_Bootloader_SIE_EP6_CR0_REG    (* (reg8 *) USB_Bootloader_USB__SIE_EP6_CR0)

#define USB_Bootloader_SIE_EP7_CNT0_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP7_CNT0)
#define USB_Bootloader_SIE_EP7_CNT0_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP7_CNT0)
#define USB_Bootloader_SIE_EP7_CNT1_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP7_CNT1)
#define USB_Bootloader_SIE_EP7_CNT1_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP7_CNT1)
#define USB_Bootloader_SIE_EP7_CR0_PTR    (  (reg8 *) USB_Bootloader_USB__SIE_EP7_CR0)
#define USB_Bootloader_SIE_EP7_CR0_REG    (* (reg8 *) USB_Bootloader_USB__SIE_EP7_CR0)

#define USB_Bootloader_SIE_EP8_CNT0_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP8_CNT0)
#define USB_Bootloader_SIE_EP8_CNT0_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP8_CNT0)
#define USB_Bootloader_SIE_EP8_CNT1_PTR   (  (reg8 *) USB_Bootloader_USB__SIE_EP8_CNT1)
#define USB_Bootloader_SIE_EP8_CNT1_REG   (* (reg8 *) USB_Bootloader_USB__SIE_EP8_CNT1)
#define USB_Bootloader_SIE_EP8_CR0_PTR    (  (reg8 *) USB_Bootloader_USB__SIE_EP8_CR0)
#define USB_Bootloader_SIE_EP8_CR0_REG    (* (reg8 *) USB_Bootloader_USB__SIE_EP8_CR0)

#define USB_Bootloader_SOF0_PTR           (  (reg8 *) USB_Bootloader_USB__SOF0)
#define USB_Bootloader_SOF0_REG           (* (reg8 *) USB_Bootloader_USB__SOF0)
#define USB_Bootloader_SOF1_PTR           (  (reg8 *) USB_Bootloader_USB__SOF1)
#define USB_Bootloader_SOF1_REG           (* (reg8 *) USB_Bootloader_USB__SOF1)

#define USB_Bootloader_USB_CLK_EN_PTR     (  (reg8 *) USB_Bootloader_USB__USB_CLK_EN)
#define USB_Bootloader_USB_CLK_EN_REG     (* (reg8 *) USB_Bootloader_USB__USB_CLK_EN)

#define USB_Bootloader_USBIO_CR0_PTR      (  (reg8 *) USB_Bootloader_USB__USBIO_CR0)
#define USB_Bootloader_USBIO_CR0_REG      (* (reg8 *) USB_Bootloader_USB__USBIO_CR0)
#define USB_Bootloader_USBIO_CR1_PTR      (  (reg8 *) USB_Bootloader_USB__USBIO_CR1)
#define USB_Bootloader_USBIO_CR1_REG      (* (reg8 *) USB_Bootloader_USB__USBIO_CR1)
#define USB_Bootloader_USBIO_CR2_PTR      (  (reg8 *) USB_Bootloader_USB__USBIO_CR2)
#define USB_Bootloader_USBIO_CR2_REG      (* (reg8 *) USB_Bootloader_USB__USBIO_CR2)

#define USB_Bootloader_DIE_ID             CYDEV_FLSHID_CUST_TABLES_BASE

#if(CY_PSOC3_ES2 || CY_PSOC5_ES1)
    #define USB_Bootloader_PM_AVAIL_CR_PTR    (  (reg8 *) CYREG_PM_AVAIL_CR6)
    #define USB_Bootloader_PM_AVAIL_CR_REG    (* (reg8 *) CYREG_PM_AVAIL_CR6)
#else  /* CY_PSOC3_ES3 || CY_PSOC5_ES2 */
    #define USB_Bootloader_PM_USB_CR0_PTR     (  (reg8 *) CYREG_PM_USB_CR0)
    #define USB_Bootloader_PM_USB_CR0_REG     (* (reg8 *) CYREG_PM_USB_CR0)
    #define USB_Bootloader_DYN_RECONFIG_PTR   (  (reg8 *) USB_Bootloader_USB__DYN_RECONFIG)
    #define USB_Bootloader_DYN_RECONFIG_REG   (* (reg8 *) USB_Bootloader_USB__DYN_RECONFIG)
#endif /* End CY_PSOC3_ES2 || CY_PSOC5_ES1 */

#define USB_Bootloader_DM_INP_DIS_PTR     (  (reg8 *) USB_Bootloader_Dm__INP_DIS)
#define USB_Bootloader_DM_INP_DIS_REG     (* (reg8 *) USB_Bootloader_Dm__INP_DIS)
#define USB_Bootloader_DP_INP_DIS_PTR     (  (reg8 *) USB_Bootloader_Dp__INP_DIS)
#define USB_Bootloader_DP_INP_DIS_REG     (* (reg8 *) USB_Bootloader_Dp__INP_DIS)
#define USB_Bootloader_DP_INTSTAT_PTR     (  (reg8 *) USB_Bootloader_Dp__INTSTAT)
#define USB_Bootloader_DP_INTSTAT_REG     (* (reg8 *) USB_Bootloader_Dp__INTSTAT)

#if (USB_Bootloader_MON_VBUS == 1u)
    #define USB_Bootloader_VBUS_DR_PTR        (  (reg8 *) USB_Bootloader_VBUS__DR)
    #define USB_Bootloader_VBUS_DR_REG        (* (reg8 *) USB_Bootloader_VBUS__DR)
    #define USB_Bootloader_VBUS_PS_PTR        (  (reg8 *) USB_Bootloader_VBUS__PS)
    #define USB_Bootloader_VBUS_PS_REG        (* (reg8 *) USB_Bootloader_VBUS__PS)
    #define USB_Bootloader_VBUS_MASK          USB_Bootloader_VBUS__MASK
#endif /* End USB_Bootloader_MON_VBUS */

/* Renamed Registers for backward compatibility.
*  Should not be used in new designs.
*/
#define USB_Bootloader_ARB_CFG        USB_Bootloader_ARB_CFG_PTR

#define USB_Bootloader_ARB_EP1_CFG    USB_Bootloader_ARB_EP1_CFG_PTR
#define USB_Bootloader_ARB_EP1_INT_EN USB_Bootloader_ARB_EP1_INT_EN_PTR
#define USB_Bootloader_ARB_EP1_SR     USB_Bootloader_ARB_EP1_SR_PTR

#define USB_Bootloader_ARB_EP2_CFG    USB_Bootloader_ARB_EP2_CFG_PTR
#define USB_Bootloader_ARB_EP2_INT_EN USB_Bootloader_ARB_EP2_INT_EN_PTR
#define USB_Bootloader_ARB_EP2_SR     USB_Bootloader_ARB_EP2_SR_PTR

#define USB_Bootloader_ARB_EP3_CFG    USB_Bootloader_ARB_EP3_CFG_PTR
#define USB_Bootloader_ARB_EP3_INT_EN USB_Bootloader_ARB_EP3_INT_EN_PTR
#define USB_Bootloader_ARB_EP3_SR     USB_Bootloader_ARB_EP3_SR_PTR

#define USB_Bootloader_ARB_EP4_CFG    USB_Bootloader_ARB_EP4_CFG_PTR
#define USB_Bootloader_ARB_EP4_INT_EN USB_Bootloader_ARB_EP4_INT_EN_PTR
#define USB_Bootloader_ARB_EP4_SR     USB_Bootloader_ARB_EP4_SR_PTR

#define USB_Bootloader_ARB_EP5_CFG    USB_Bootloader_ARB_EP5_CFG_PTR
#define USB_Bootloader_ARB_EP5_INT_EN USB_Bootloader_ARB_EP5_INT_EN_PTR
#define USB_Bootloader_ARB_EP5_SR     USB_Bootloader_ARB_EP5_SR_PTR

#define USB_Bootloader_ARB_EP6_CFG    USB_Bootloader_ARB_EP6_CFG_PTR
#define USB_Bootloader_ARB_EP6_INT_EN USB_Bootloader_ARB_EP6_INT_EN_PTR
#define USB_Bootloader_ARB_EP6_SR     USB_Bootloader_ARB_EP6_SR_PTR

#define USB_Bootloader_ARB_EP7_CFG    USB_Bootloader_ARB_EP7_CFG_PTR
#define USB_Bootloader_ARB_EP7_INT_EN USB_Bootloader_ARB_EP7_INT_EN_PTR
#define USB_Bootloader_ARB_EP7_SR     USB_Bootloader_ARB_EP7_SR_PTR

#define USB_Bootloader_ARB_EP8_CFG    USB_Bootloader_ARB_EP8_CFG_PTR
#define USB_Bootloader_ARB_EP8_INT_EN USB_Bootloader_ARB_EP8_INT_EN_PTR
#define USB_Bootloader_ARB_EP8_SR     USB_Bootloader_ARB_EP8_SR_PTR

#define USB_Bootloader_ARB_INT_EN     USB_Bootloader_ARB_INT_EN_PTR
#define USB_Bootloader_ARB_INT_SR     USB_Bootloader_ARB_INT_SR_PTR

#define USB_Bootloader_ARB_RW1_DR     USB_Bootloader_ARB_RW1_DR_PTR
#define USB_Bootloader_ARB_RW1_RA     USB_Bootloader_ARB_RW1_RA_PTR
#define USB_Bootloader_ARB_RW1_RA_MSB USB_Bootloader_ARB_RW1_RA_MSB_PTR
#define USB_Bootloader_ARB_RW1_WA     USB_Bootloader_ARB_RW1_WA_PTR
#define USB_Bootloader_ARB_RW1_WA_MSB USB_Bootloader_ARB_RW1_WA_MSB_PTR

#define USB_Bootloader_ARB_RW2_DR     USB_Bootloader_ARB_RW2_DR_PTR
#define USB_Bootloader_ARB_RW2_RA     USB_Bootloader_ARB_RW2_RA_PTR
#define USB_Bootloader_ARB_RW2_RA_MSB USB_Bootloader_ARB_RW2_RA_MSB_PTR
#define USB_Bootloader_ARB_RW2_WA     USB_Bootloader_ARB_RW2_WA_PTR
#define USB_Bootloader_ARB_RW2_WA_MSB USB_Bootloader_ARB_RW2_WA_MSB_PTR

#define USB_Bootloader_ARB_RW3_DR     USB_Bootloader_ARB_RW3_DR_PTR
#define USB_Bootloader_ARB_RW3_RA     USB_Bootloader_ARB_RW3_RA_PTR
#define USB_Bootloader_ARB_RW3_RA_MSB USB_Bootloader_ARB_RW3_RA_MSB_PTR
#define USB_Bootloader_ARB_RW3_WA     USB_Bootloader_ARB_RW3_WA_PTR
#define USB_Bootloader_ARB_RW3_WA_MSB USB_Bootloader_ARB_RW3_WA_MSB_PTR

#define USB_Bootloader_ARB_RW4_DR     USB_Bootloader_ARB_RW4_DR_PTR
#define USB_Bootloader_ARB_RW4_RA     USB_Bootloader_ARB_RW4_RA_PTR
#define USB_Bootloader_ARB_RW4_RA_MSB USB_Bootloader_ARB_RW4_RA_MSB_PTR
#define USB_Bootloader_ARB_RW4_WA     USB_Bootloader_ARB_RW4_WA_PTR
#define USB_Bootloader_ARB_RW4_WA_MSB USB_Bootloader_ARB_RW4_WA_MSB_PTR

#define USB_Bootloader_ARB_RW5_DR     USB_Bootloader_ARB_RW5_DR_PTR
#define USB_Bootloader_ARB_RW5_RA     USB_Bootloader_ARB_RW5_RA_PTR
#define USB_Bootloader_ARB_RW5_RA_MSB USB_Bootloader_ARB_RW5_RA_MSB_PTR
#define USB_Bootloader_ARB_RW5_WA     USB_Bootloader_ARB_RW5_WA_PTR
#define USB_Bootloader_ARB_RW5_WA_MSB USB_Bootloader_ARB_RW5_WA_MSB_PTR

#define USB_Bootloader_ARB_RW6_DR     USB_Bootloader_ARB_RW6_DR_PTR
#define USB_Bootloader_ARB_RW6_RA     USB_Bootloader_ARB_RW6_RA_PTR
#define USB_Bootloader_ARB_RW6_RA_MSB USB_Bootloader_ARB_RW6_RA_MSB_PTR
#define USB_Bootloader_ARB_RW6_WA     USB_Bootloader_ARB_RW6_WA_PTR
#define USB_Bootloader_ARB_RW6_WA_MSB USB_Bootloader_ARB_RW6_WA_MSB_PTR

#define USB_Bootloader_ARB_RW7_DR     USB_Bootloader_ARB_RW7_DR_PTR
#define USB_Bootloader_ARB_RW7_RA     USB_Bootloader_ARB_RW7_RA_PTR
#define USB_Bootloader_ARB_RW7_RA_MSB USB_Bootloader_ARB_RW7_RA_MSB_PTR
#define USB_Bootloader_ARB_RW7_WA     USB_Bootloader_ARB_RW7_WA_PTR
#define USB_Bootloader_ARB_RW7_WA_MSB USB_Bootloader_ARB_RW7_WA_MSB_PTR

#define USB_Bootloader_ARB_RW8_DR     USB_Bootloader_ARB_RW8_DR_PTR
#define USB_Bootloader_ARB_RW8_RA     USB_Bootloader_ARB_RW8_RA_PTR
#define USB_Bootloader_ARB_RW8_RA_MSB USB_Bootloader_ARB_RW8_RA_MSB_PTR
#define USB_Bootloader_ARB_RW8_WA     USB_Bootloader_ARB_RW8_WA_PTR
#define USB_Bootloader_ARB_RW8_WA_MSB USB_Bootloader_ARB_RW8_WA_MSB_PTR

#define USB_Bootloader_BUF_SIZE       USB_Bootloader_BUF_SIZE_PTR
#define USB_Bootloader_BUS_RST_CNT    USB_Bootloader_BUS_RST_CNT_PTR
#define USB_Bootloader_CR0            USB_Bootloader_CR0_PTR
#define USB_Bootloader_CR1            USB_Bootloader_CR1_PTR
#define USB_Bootloader_CWA            USB_Bootloader_CWA_PTR
#define USB_Bootloader_CWA_MSB        USB_Bootloader_CWA_MSB_PTR

#define USB_Bootloader_DMA_THRES      USB_Bootloader_DMA_THRES_PTR
#define USB_Bootloader_DMA_THRES_MSB  USB_Bootloader_DMA_THRES_MSB_PTR

#define USB_Bootloader_EP_ACTIVE      USB_Bootloader_EP_ACTIVE_PTR
#define USB_Bootloader_EP_TYPE        USB_Bootloader_EP_TYPE_PTR

#define USB_Bootloader_EP0_CNT        USB_Bootloader_EP0_CNT_PTR
#define USB_Bootloader_EP0_CR         USB_Bootloader_EP0_CR_PTR
#define USB_Bootloader_EP0_DR0        USB_Bootloader_EP0_DR0_PTR
#define USB_Bootloader_EP0_DR1        USB_Bootloader_EP0_DR1_PTR
#define USB_Bootloader_EP0_DR2        USB_Bootloader_EP0_DR2_PTR
#define USB_Bootloader_EP0_DR3        USB_Bootloader_EP0_DR3_PTR
#define USB_Bootloader_EP0_DR4        USB_Bootloader_EP0_DR4_PTR
#define USB_Bootloader_EP0_DR5        USB_Bootloader_EP0_DR5_PTR
#define USB_Bootloader_EP0_DR6        USB_Bootloader_EP0_DR6_PTR
#define USB_Bootloader_EP0_DR7        USB_Bootloader_EP0_DR7_PTR

#define USB_Bootloader_OSCLK_DR0      USB_Bootloader_OSCLK_DR0_PTR
#define USB_Bootloader_OSCLK_DR1      USB_Bootloader_OSCLK_DR1_PTR

#define USB_Bootloader_PM_ACT_CFG     USB_Bootloader_PM_ACT_CFG_PTR
#define USB_Bootloader_PM_STBY_CFG    USB_Bootloader_PM_STBY_CFG_PTR
#if(CY_PSOC3_ES2 || CY_PSOC5_ES1)
    #define USB_Bootloader_PM_AVAIL_CR    USB_Bootloader_PM_AVAIL_CR_PTR
#endif /* End CY_PSOC3_ES2 || CY_PSOC5_ES1 */
#define USB_Bootloader_SIE_EP_INT_EN  USB_Bootloader_SIE_EP_INT_EN_PTR
#define USB_Bootloader_SIE_EP_INT_SR  USB_Bootloader_SIE_EP_INT_SR_PTR

#define USB_Bootloader_SIE_EP1_CNT0   USB_Bootloader_SIE_EP1_CNT0_PTR
#define USB_Bootloader_SIE_EP1_CNT1   USB_Bootloader_SIE_EP1_CNT1_PTR
#define USB_Bootloader_SIE_EP1_CR0    USB_Bootloader_SIE_EP1_CR0_PTR

#define USB_Bootloader_SIE_EP2_CNT0   USB_Bootloader_SIE_EP2_CNT0_PTR
#define USB_Bootloader_SIE_EP2_CNT1   USB_Bootloader_SIE_EP2_CNT1_PTR
#define USB_Bootloader_SIE_EP2_CR0    USB_Bootloader_SIE_EP2_CR0_PTR

#define USB_Bootloader_SIE_EP3_CNT0   USB_Bootloader_SIE_EP3_CNT0_PTR
#define USB_Bootloader_SIE_EP3_CNT1   USB_Bootloader_SIE_EP3_CNT1_PTR
#define USB_Bootloader_SIE_EP3_CR0    USB_Bootloader_SIE_EP3_CR0_PTR

#define USB_Bootloader_SIE_EP4_CNT0   USB_Bootloader_SIE_EP4_CNT0_PTR
#define USB_Bootloader_SIE_EP4_CNT1   USB_Bootloader_SIE_EP4_CNT1_PTR
#define USB_Bootloader_SIE_EP4_CR0    USB_Bootloader_SIE_EP4_CR0_PTR

#define USB_Bootloader_SIE_EP5_CNT0   USB_Bootloader_SIE_EP5_CNT0_PTR
#define USB_Bootloader_SIE_EP5_CNT1   USB_Bootloader_SIE_EP5_CNT1_PTR
#define USB_Bootloader_SIE_EP5_CR0    USB_Bootloader_SIE_EP5_CR0_PTR

#define USB_Bootloader_SIE_EP6_CNT0   USB_Bootloader_SIE_EP6_CNT0_PTR
#define USB_Bootloader_SIE_EP6_CNT1   USB_Bootloader_SIE_EP6_CNT1_PTR
#define USB_Bootloader_SIE_EP6_CR0    USB_Bootloader_SIE_EP6_CR0_PTR

#define USB_Bootloader_SIE_EP7_CNT0   USB_Bootloader_SIE_EP7_CNT0_PTR
#define USB_Bootloader_SIE_EP7_CNT1   USB_Bootloader_SIE_EP7_CNT1_PTR
#define USB_Bootloader_SIE_EP7_CR0    USB_Bootloader_SIE_EP7_CR0_PTR

#define USB_Bootloader_SIE_EP8_CNT0   USB_Bootloader_SIE_EP8_CNT0_PTR
#define USB_Bootloader_SIE_EP8_CNT1   USB_Bootloader_SIE_EP8_CNT1_PTR
#define USB_Bootloader_SIE_EP8_CR0    USB_Bootloader_SIE_EP8_CR0_PTR

#define USB_Bootloader_SOF0           USB_Bootloader_SOF0_PTR
#define USB_Bootloader_SOF1           USB_Bootloader_SOF1_PTR

#define USB_Bootloader_USB_CLK_EN     USB_Bootloader_USB_CLK_EN_PTR

#define USB_Bootloader_USBIO_CR0      USB_Bootloader_USBIO_CR0_PTR
#define USB_Bootloader_USBIO_CR1      USB_Bootloader_USBIO_CR1_PTR
#define USB_Bootloader_USBIO_CR2      USB_Bootloader_USBIO_CR2_PTR

#define USB_Bootloader_USB_MEM        ((reg8 *) CYDEV_USB_MEM_BASE)

#if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
    /* PSoC3 interrupt registers*/
    #define USB_Bootloader_USB_ISR_PRIOR  ((reg8 *) CYDEV_INTC_PRIOR0)
    #define USB_Bootloader_USB_ISR_SET_EN ((reg8 *) CYDEV_INTC_SET_EN0)
    #define USB_Bootloader_USB_ISR_CLR_EN ((reg8 *) CYDEV_INTC_CLR_EN0)
    #define USB_Bootloader_USB_ISR_VECT   ((cyisraddress *) CYDEV_INTC_VECT_MBASE)
#elif(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_PANTHER)
    /* PSoC5 interrupt registers*/
    #define USB_Bootloader_USB_ISR_PRIOR  ((reg8 *) CYDEV_NVIC_PRI_0)
    #define USB_Bootloader_USB_ISR_SET_EN ((reg8 *) CYDEV_NVIC_SETENA0)
    #define USB_Bootloader_USB_ISR_CLR_EN ((reg8 *) CYDEV_NVIC_CLRENA0)
    #define USB_Bootloader_USB_ISR_VECT   ((cyisraddress *) CYDEV_NVIC_VECT_OFFSET)
#endif /* End CYDEV_CHIP_DIE_EXPECT */


/***************************************
* Interrupt vectors, masks and priorities
***************************************/

#define USB_Bootloader_BUS_RESET_PRIOR    USB_Bootloader_bus_reset__INTC_PRIOR_NUM
#define USB_Bootloader_BUS_RESET_MASK     USB_Bootloader_bus_reset__INTC_MASK
#define USB_Bootloader_BUS_RESET_VECT_NUM USB_Bootloader_bus_reset__INTC_NUMBER

#define USB_Bootloader_SOF_PRIOR          USB_Bootloader_sof_int__INTC_PRIOR_NUM
#define USB_Bootloader_SOF_MASK           USB_Bootloader_sof_int__INTC_MASK
#define USB_Bootloader_SOF_VECT_NUM       USB_Bootloader_sof_int__INTC_NUMBER

#define USB_Bootloader_EP_0_PRIOR         USB_Bootloader_ep_0__INTC_PRIOR_NUM
#define USB_Bootloader_EP_0_MASK          USB_Bootloader_ep_0__INTC_MASK
#define USB_Bootloader_EP_0_VECT_NUM      USB_Bootloader_ep_0__INTC_NUMBER

#define USB_Bootloader_EP_1_PRIOR         USB_Bootloader_ep_1__INTC_PRIOR_NUM
#define USB_Bootloader_EP_1_MASK          USB_Bootloader_ep_1__INTC_MASK
#define USB_Bootloader_EP_1_VECT_NUM      USB_Bootloader_ep_1__INTC_NUMBER

#define USB_Bootloader_EP_2_PRIOR         USB_Bootloader_ep_2__INTC_PRIOR_NUM
#define USB_Bootloader_EP_2_MASK          USB_Bootloader_ep_2__INTC_MASK
#define USB_Bootloader_EP_2_VECT_NUM      USB_Bootloader_ep_2__INTC_NUMBER

#define USB_Bootloader_EP_3_PRIOR         USB_Bootloader_ep_3__INTC_PRIOR_NUM
#define USB_Bootloader_EP_3_MASK          USB_Bootloader_ep_3__INTC_MASK
#define USB_Bootloader_EP_3_VECT_NUM      USB_Bootloader_ep_3__INTC_NUMBER

#define USB_Bootloader_EP_4_PRIOR         USB_Bootloader_ep_4__INTC_PRIOR_NUM
#define USB_Bootloader_EP_4_MASK          USB_Bootloader_ep_4__INTC_MASK
#define USB_Bootloader_EP_4_VECT_NUM      USB_Bootloader_ep_4__INTC_NUMBER

#define USB_Bootloader_EP_5_PRIOR         USB_Bootloader_ep_5__INTC_PRIOR_NUM
#define USB_Bootloader_EP_5_MASK          USB_Bootloader_ep_5__INTC_MASK
#define USB_Bootloader_EP_5_VECT_NUM      USB_Bootloader_ep_5__INTC_NUMBER

#define USB_Bootloader_EP_6_PRIOR         USB_Bootloader_ep_6__INTC_PRIOR_NUM
#define USB_Bootloader_EP_6_MASK          USB_Bootloader_ep_6__INTC_MASK
#define USB_Bootloader_EP_6_VECT_NUM      USB_Bootloader_ep_6__INTC_NUMBER

#define USB_Bootloader_EP_7_PRIOR         USB_Bootloader_ep_7__INTC_PRIOR_NUM
#define USB_Bootloader_EP_7_MASK          USB_Bootloader_ep_7__INTC_MASK
#define USB_Bootloader_EP_7_VECT_NUM      USB_Bootloader_ep_7__INTC_NUMBER

#define USB_Bootloader_EP_8_PRIOR         USB_Bootloader_ep_8__INTC_PRIOR_NUM
#define USB_Bootloader_EP_8_MASK          USB_Bootloader_ep_8__INTC_MASK
#define USB_Bootloader_EP_8_VECT_NUM      USB_Bootloader_ep_8__INTC_NUMBER

#define USB_Bootloader_DP_INTC_PRIOR      USB_Bootloader_dp_int__INTC_PRIOR_NUM
#define USB_Bootloader_DP_INTC_MASK       USB_Bootloader_dp_int__INTC_MASK
#define USB_Bootloader_DP_INTC_VECT_NUM   USB_Bootloader_dp_int__INTC_NUMBER

/* ARB ISR should have higher priority from EP_X ISR, therefore it is defined to higest (0) */
#define USB_Bootloader_ARB_PRIOR          (0u)
#define USB_Bootloader_ARB_MASK           USB_Bootloader_arb_int__INTC_MASK
#define USB_Bootloader_ARB_VECT_NUM       USB_Bootloader_arb_int__INTC_NUMBER

/***************************************
 *  Endpoint 0 offsets (Table 9-2)
 **************************************/

#define USB_Bootloader_bmRequestType      USB_Bootloader_EP0_DR0_PTR
#define USB_Bootloader_bRequest           USB_Bootloader_EP0_DR1_PTR
#define USB_Bootloader_wValue             USB_Bootloader_EP0_DR2_PTR
#define USB_Bootloader_wValueHi           USB_Bootloader_EP0_DR3_PTR
#define USB_Bootloader_wValueLo           USB_Bootloader_EP0_DR2_PTR
#define USB_Bootloader_wIndex             USB_Bootloader_EP0_DR4_PTR
#define USB_Bootloader_wIndexHi           USB_Bootloader_EP0_DR5_PTR
#define USB_Bootloader_wIndexLo           USB_Bootloader_EP0_DR4_PTR
#define USB_Bootloader_length             USB_Bootloader_EP0_DR6_PTR
#define USB_Bootloader_lengthHi           USB_Bootloader_EP0_DR7_PTR
#define USB_Bootloader_lengthLo           USB_Bootloader_EP0_DR6_PTR


/***************************************
*       Register Constants
***************************************/
#define USB_Bootloader_VDDD_MV                    CYDEV_VDDD_MV
#define USB_Bootloader_3500MV                     (3500u)

#define USB_Bootloader_CR1_REG_ENABLE             (0x01u)
#define USB_Bootloader_CR1_ENABLE_LOCK            (0x02u)
#define USB_Bootloader_CR1_BUS_ACTIVITY_SHIFT     (0x02u)
#define USB_Bootloader_CR1_BUS_ACTIVITY           (0x01u << USB_Bootloader_CR1_BUS_ACTIVITY_SHIFT)
#if(CY_PSOC3_ES3 || CY_PSOC5_ES2)
    #define USB_Bootloader_CR1_TRIM_MSB_EN            (0x08u)
#endif /* End CY_PSOC3_ES3 || CY_PSOC5_ES2 */

#define USB_Bootloader_EP0_CNT_DATA_TOGGLE        (0x80u)
#define USB_Bootloader_EPX_CNT_DATA_TOGGLE        (0x80u)
#define USB_Bootloader_EPX_CNT0_MASK              (0x07u)
#define USB_Bootloader_EPX_CNTX_ADDR_SHIFT        (0x04u)
#define USB_Bootloader_EPX_CNTX_ADDR_OFFSET       (0x10u)
#define USB_Bootloader_EPX_CNTX_CRC_COUNT         (0x02u)
#define USB_Bootloader_EPX_DATA_BUF_MAX           (512u)

#define USB_Bootloader_CR0_ENABLE                 (0x80u)

#if(CY_PSOC3_ES2 || CY_PSOC5_ES1)
    /* USB_BUS_RST_CNT register 2 bits wide only in ES2, therefore we use max length */
    #define USB_Bootloader_BUS_RST_COUNT              (0x03u)
    #define USB_Bootloader_USBIO_CR1_IOMODE           (0x80u)
    #define USB_Bootloader_USBIO_CR1_DRIVE_MODE       (0x40u)
    #define USB_Bootloader_USBIO_CR1_DPI              (0x20u)
    #define USB_Bootloader_USBIO_CR1_DMI              (0x10u)
    #define USB_Bootloader_USBIO_CR1_P2PUEN           (0x08u)
#else  /* CY_PSOC3_ES3 || CY_PSOC5_ES2 */
    /* In leopard, a 100 KHz clock is used. Recommended is to count 10 pulses */
    #define USB_Bootloader_BUS_RST_COUNT              (0x0au)
    #define USB_Bootloader_USBIO_CR1_IOMODE           (0x20u)
#endif /* End CY_PSOC3_ES2 || CY_PSOC5_ES1 */
#define USB_Bootloader_USBIO_CR1_USBPUEN          (0x04u)
#define USB_Bootloader_USBIO_CR1_DP0              (0x02u)
#define USB_Bootloader_USBIO_CR1_DM0              (0x01u)

#define USB_Bootloader_USBIO_CR0_TEN              (0x80u)
#define USB_Bootloader_USBIO_CR0_TSE0             (0x40u)
#define USB_Bootloader_USBIO_CR0_TD               (0x20u)
#define USB_Bootloader_USBIO_CR0_RD               (0x01u)

#define USB_Bootloader_FASTCLK_IMO_CR_USBCLK_ON   (0x40u)
#define USB_Bootloader_FASTCLK_IMO_CR_XCLKEN      (0x20u)
#define USB_Bootloader_FASTCLK_IMO_CR_FX2ON       (0x10u)

#define USB_Bootloader_ARB_EPX_CFG_RESET          (0x08u)
#define USB_Bootloader_ARB_EPX_CFG_CRC_BYPASS     (0x04u)
#define USB_Bootloader_ARB_EPX_CFG_DMA_REQ        (0x02u)
#define USB_Bootloader_ARB_EPX_CFG_IN_DATA_RDY    (0x01u)

#define USB_Bootloader_ARB_EPX_SR_IN_BUF_FULL     (0x01u)
#define USB_Bootloader_ARB_EPX_SR_DMA_GNT         (0x02u)
#define USB_Bootloader_ARB_EPX_SR_BUF_OVER        (0x04u)
#define USB_Bootloader_ARB_EPX_SR_BUF_UNDER       (0x08u)

#define USB_Bootloader_ARB_CFG_AUTO_MEM           (0x10u)
#define USB_Bootloader_ARB_CFG_MANUAL_DMA         (0x20u)
#define USB_Bootloader_ARB_CFG_AUTO_DMA           (0x40u)
#define USB_Bootloader_ARB_CFG_CFG_CPM            (0x80u)

#define USB_Bootloader_ARB_EPX_INT_MASK           (0x1Fu)
#define USB_Bootloader_ARB_INT_MASK              ((USB_Bootloader_DMA1_REMOVE ^ 1) | \
                                                    (USB_Bootloader_DMA2_REMOVE ^ 1) << 1 | \
                                                    (USB_Bootloader_DMA3_REMOVE ^ 1) << 2 | \
                                                    (USB_Bootloader_DMA4_REMOVE ^ 1) << 3 | \
                                                    (USB_Bootloader_DMA5_REMOVE ^ 1) << 4 | \
                                                    (USB_Bootloader_DMA6_REMOVE ^ 1) << 5 | \
                                                    (USB_Bootloader_DMA7_REMOVE ^ 1) << 6 | \
                                                    (USB_Bootloader_DMA8_REMOVE ^ 1) << 7)

#define USB_Bootloader_SIE_EP_INT_EP1_MASK        (0x01u)
#define USB_Bootloader_SIE_EP_INT_EP2_MASK        (0x02u)
#define USB_Bootloader_SIE_EP_INT_EP3_MASK        (0x04u)
#define USB_Bootloader_SIE_EP_INT_EP4_MASK        (0x08u)
#define USB_Bootloader_SIE_EP_INT_EP5_MASK        (0x10u)
#define USB_Bootloader_SIE_EP_INT_EP6_MASK        (0x20u)
#define USB_Bootloader_SIE_EP_INT_EP7_MASK        (0x40u)
#define USB_Bootloader_SIE_EP_INT_EP8_MASK        (0x80u)

#define USB_Bootloader_PM_ACT_EN_FSUSB            USB_Bootloader_USB__PM_ACT_MSK
#define USB_Bootloader_PM_STBY_EN_FSUSB           USB_Bootloader_USB__PM_STBY_MSK
#define USB_Bootloader_PM_AVAIL_EN_FSUSBIO        (0x10u)

#define USB_Bootloader_PM_USB_CR0_REF_EN          (0x01u)
#define USB_Bootloader_PM_USB_CR0_PD_N            (0x02u)
#define USB_Bootloader_PM_USB_CR0_PD_PULLUP_N     (0x04u)

#define USB_Bootloader_USB_CLK_ENABLE             (0x01u)

#define USB_Bootloader_DM_MASK                    USB_Bootloader_Dm__0__MASK
#define USB_Bootloader_DP_MASK                    USB_Bootloader_Dp__0__MASK

#define USB_Bootloader_DYN_RECONFIG_ENABLE        (0x01u)
#define USB_Bootloader_DYN_RECONFIG_EP_SHIFT      (0x01u)
#define USB_Bootloader_DYN_RECONFIG_RDY_STS       (0x10u)


#endif /* End CY_USBFS_USB_Bootloader_H */


/* [] END OF FILE */
