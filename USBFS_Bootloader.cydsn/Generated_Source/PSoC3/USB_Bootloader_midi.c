/*******************************************************************************
* File Name: USB_Bootloader_midi.c
* Version 2.12
*
* Description:
*  MIDI Streaming request handler.
*  This file contains routines for sending and receiving MIDI
*  messages, and handles running status in both directions.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USB_Bootloader.h"

#if defined(USB_Bootloader_ENABLE_MIDI_STREAMING)

#include "USB_Bootloader_midi.h"
#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
    #include "MIDI1_UART.h"
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */    
#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
    #include "MIDI2_UART.h"
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */    
#if(USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL)
    #include <CYDMAC.H>
#endif /* End USB_Bootloader_EP_MM */


/***************************************
*    MIDI Constants
***************************************/

/* The Size of the MIDI messages (MIDI Table 4-1) */
const uint8 CYCODE USB_Bootloader_MIDI_SIZE[] = {
/*  Miscellaneous function codes(Reserved)  */ 0x03u,
/*  Cable events (Reserved)                 */ 0x03u,
/*  Two-byte System Common messages         */ 0x02u,
/*  Three-byte System Common messages       */ 0x03u,
/*  SysEx starts or continues               */ 0x03u,
/*  Single-byte System Common Message or
    SysEx ends with following single byte   */ 0x01u,
/*  SysEx ends with following two bytes     */ 0x02u,
/*  SysEx ends with following three bytes   */ 0x03u,
/*  Note-off                                */ 0x03u,
/*  Note-on                                 */ 0x03u,
/*  Poly-KeyPress                           */ 0x03u,
/*  Control Change                          */ 0x03u,
/*  Program Change                          */ 0x02u,
/*  Channel Pressure                        */ 0x02u,
/*  PitchBend Change                        */ 0x03u,
/*  Single Byte                             */ 0x01u
};


/***************************************
*  Global variables
***************************************/

volatile uint8 USB_Bootloader_midiInBuffer[USB_Bootloader_MIDI_IN_BUFF_SIZE];   /* Input endpoint buffer */
volatile uint8 USB_Bootloader_midiOutBuffer[USB_Bootloader_MIDI_OUT_BUFF_SIZE]; /* Output endpoint buffer */
#if USB_Bootloader_MIDI_IN_BUFF_SIZE >= 256
    volatile uint16 USB_Bootloader_midiInPointer;                                 /* Input endpoint buffer pointer */
#else
    volatile uint8 USB_Bootloader_midiInPointer;                                  /* Input endpoint buffer pointer */
#endif /* End USB_Bootloader_MIDI_IN_BUFF_SIZE >=256 */    

volatile uint8 USB_Bootloader_midi_in_ep;                                         /* Input endpoint number */
volatile uint8 USB_Bootloader_midi_out_ep;                                        /* Output endpoint number */

#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
    volatile USB_Bootloader_MIDI_RX_STATUS USB_Bootloader_MIDI1_Event;          /* MIDI RX status structure */
    volatile uint8 USB_Bootloader_MIDI1_TxRunStat;                                /* MIDI Output running status */
    volatile uint8 USB_Bootloader_MIDI1_InqFlags;                                 /* Device inquiry flag */    
    
#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
    volatile USB_Bootloader_MIDI_RX_STATUS USB_Bootloader_MIDI2_Event;          /* MIDI RX status structure */
    volatile uint8 USB_Bootloader_MIDI2_TxRunStat;                                /* MIDI Output running status */
    volatile uint8 USB_Bootloader_MIDI2_InqFlags;                                 /* Device inquiry flag */    
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */


/***************************************
* Custom Declarations
***************************************/

/* `#START MIDI_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* Optional MIDI APIs
***************************************/
#if (USB_Bootloader_ENABLE_MIDI_API != 0u)


/***************************************
* External function references
***************************************/

void USB_Bootloader_callbackLocalMidiEvent(uint8 cable, uint8 *midiMsg) 
                                                     ;


/***************************************
*    External references
***************************************/

extern volatile T_USB_Bootloader_EP_CTL_BLOCK USB_Bootloader_EP[];
#if(USB_Bootloader_EP_MM != USB_Bootloader__EP_MANUAL)
    extern uint8 USB_Bootloader_DmaChan[USB_Bootloader_MAX_EP];
#endif /* End USB_Bootloader_EP_MM */

#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
    #if (MIDI1_UART_RXBUFFERSIZE >= 256)
        extern volatile uint16 MIDI1_UART_rxBufferRead;
        extern volatile uint16 MIDI1_UART_rxBufferWrite;
    #else    
        extern volatile uint8 MIDI1_UART_rxBufferRead;
        extern volatile uint8 MIDI1_UART_rxBufferWrite;
    #endif /* End MIDI1_UART_RXBUFFERSIZE >= 256 */    
    extern volatile uint8 MIDI1_UART_rxBufferLoopDetect;
    extern volatile uint8 MIDI1_UART_rxBuffer[MIDI1_UART_RXBUFFERSIZE];
    
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */    
#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
    #if (MIDI2_UART_RXBUFFERSIZE >= 256)
        extern volatile uint16 MIDI2_UART_rxBufferRead;
        extern volatile uint16 MIDI2_UART_rxBufferWrite;
    #else    
        extern volatile uint8 MIDI2_UART_rxBufferRead;
        extern volatile uint8 MIDI2_UART_rxBufferWrite;
    #endif /* End MIDI2_UART_RXBUFFERSIZE >= 256 */    
    extern volatile uint8 MIDI2_UART_rxBufferLoopDetect;
    extern volatile uint8 MIDI2_UART_rxBuffer[MIDI2_UART_RXBUFFERSIZE];
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */    



/***************************************
*    Private function prototypes
***************************************/

void USB_Bootloader_PrepareInBuffer(uint8 ic, uint8 *srcBuff, uint8 eventLen, uint8 cable)
                                                                 ;
#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
    void USB_Bootloader_MIDI_Init(void) ;

    uint8 USB_Bootloader_MIDI1_GetEvent(void) ;
    void USB_Bootloader_MIDI1_ProcessUsbOut(uint8 *epBuf) ;
    
    #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
        uint8 USB_Bootloader_MIDI2_GetEvent(void) ;
        void USB_Bootloader_MIDI2_ProcessUsbOut(uint8 *epBuf) 
                                                    ;
    #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */


/*******************************************************************************
* Function Name: USB_Bootloader_MIDI_EP_Init
********************************************************************************
*
* Summary:
*  This function initializes the MIDI interface and UART(s) to be ready to 
*   receive data from the PC and MIDI ports.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  USB_Bootloader_midiInBuffer: This buffer is used for saving and combining 
*    the received data from UART(s) and(or) generated internally by 
*    PutUsbMidiIn() function messages. USB_Bootloader_MIDI_IN_EP_Service() 
*    function transfers the data from this buffer to the PC.
*  USB_Bootloader_midiOutBuffer: This buffer is used by the
*    USB_Bootloader_MIDI_OUT_EP_Service() function for saving the received
*    from the PC data, then the data are parsed and transferred to UART(s) 
*    buffer and to the internal processing by the
*    USB_Bootloader_callbackLocalMidiEvent function.
*  USB_Bootloader_midi_out_ep: Used as an OUT endpoint number.
*  USB_Bootloader_midi_in_ep: Used as an IN endpoint number.
*   USB_Bootloader_midiInPointer: Initialized to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void USB_Bootloader_MIDI_EP_Init(void) 
{
    USB_Bootloader_midiInPointer = 0u;
    
    #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO)
        /* Init DMA configurations for IN EP*/
        USB_Bootloader_LoadInEP(USB_Bootloader_midi_in_ep, (uint8 *)USB_Bootloader_midiInBuffer,
                                                                                USB_Bootloader_MIDI_IN_BUFF_SIZE);
        /* Init DMA configurations for OUT EP*/
        USB_Bootloader_ReadOutEP(USB_Bootloader_midi_out_ep, (uint8 *)USB_Bootloader_midiOutBuffer, 
                                                                                USB_Bootloader_MIDI_OUT_BUFF_SIZE);
    #endif  /* End USB_Bootloader__EP_DMAAUTO */  
    
    USB_Bootloader_EnableOutEP(USB_Bootloader_midi_out_ep);
    
    /* Initialize the MIDI port(s) */
    #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
        USB_Bootloader_MIDI_Init();
    #endif /* USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */    
}


/*******************************************************************************
* Function Name: USB_Bootloader_MIDI_OUT_EP_Service
********************************************************************************
*
* Summary: 
*  Services the USB MIDI OUT endpoints.
*  This function is called from OUT EP ISR. It transfers the received from PC
*  data to the external MIDI port(UART TX buffer) and calls the 
*  USB_Bootloader_callbackLocalMidiEvent() function to internal process
*  of the MIDI data.
*  This function is blocked by UART, if not enough space is available in UART 
*  TX buffer. Therefore it is recommended to use large UART TX buffer size.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  USB_Bootloader_midiOutBuffer: Used as temporary buffer between USB internal 
*   memory and UART TX buffer.
*  USB_Bootloader_midi_out_ep: Used as an OUT endpoint number.
*
* Reentrant:
*  No
*
*******************************************************************************/
void USB_Bootloader_MIDI_OUT_EP_Service(void) 
{
    #if USB_Bootloader_MIDI_OUT_BUFF_SIZE >= 256
        uint16 outLength;
        uint16 outPointer;
    #else
        uint8 outLength;
        uint8 outPointer;
    #endif /* End USB_Bootloader_MIDI_OUT_BUFF_SIZE >=256 */    
    
    uint8 dmaState = 0u;

    /* Service the USB MIDI output endpoint */
    if (USB_Bootloader_GetEPState(USB_Bootloader_midi_out_ep) == USB_Bootloader_OUT_BUFFER_FULL)
    {
        outLength = USB_Bootloader_GetEPCount(USB_Bootloader_midi_out_ep);
        #if(USB_Bootloader_EP_MM != USB_Bootloader__EP_DMAAUTO)
            outLength = USB_Bootloader_ReadOutEP(USB_Bootloader_midi_out_ep, 
                                                   (uint8 *)USB_Bootloader_midiOutBuffer, outLength);
            #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAMANUAL)
                do  /* wait for DMA transfer complete */
                {
                    CyDmaChStatus(USB_Bootloader_DmaChan[USB_Bootloader_midi_out_ep], NULL, &dmaState);
                }while((dmaState & (STATUS_TD_ACTIVE | STATUS_CHAIN_ACTIVE)) != 0);
            #endif /* End USBFS_EP_MM == USBFS__EP_DMAMANUAL */
        #endif  /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_DMAAUTO */  
        dmaState = dmaState;    /* remove warning */
        
        if (outLength >= USB_Bootloader_EVENT_LENGTH)
        {
            outPointer = 0u;
            while (outPointer < outLength)
            {
                /* In some OS OUT packet could be appended by nulls which could be skipped */
                if (USB_Bootloader_midiOutBuffer[outPointer] == 0u)
                {
                    break;
                }
                /* Route USB MIDI to the External connection */
                #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
                    if ((USB_Bootloader_midiOutBuffer[outPointer] & USB_Bootloader_CABLE_MASK) == 
                        USB_Bootloader_MIDI_CABLE_00)
                    {
                        USB_Bootloader_MIDI1_ProcessUsbOut((uint8 *)&USB_Bootloader_midiOutBuffer[outPointer]);
                    }
                    else if ((USB_Bootloader_midiOutBuffer[outPointer] & USB_Bootloader_CABLE_MASK) == 
                             USB_Bootloader_MIDI_CABLE_01)
                    {
                        #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
                            USB_Bootloader_MIDI2_ProcessUsbOut((uint8 *)&USB_Bootloader_midiOutBuffer[outPointer]);
                        #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */
                    }
                    else
                    {
                        /* `#START CUSTOM_MIDI_OUT_EP_SERV` Place your code here */

                        /* `#END` */
                    }
                #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */

                /* Process any local MIDI output functions */
                USB_Bootloader_callbackLocalMidiEvent(
                    USB_Bootloader_midiOutBuffer[outPointer] & USB_Bootloader_CABLE_MASK,
                    (uint8 *)&USB_Bootloader_midiOutBuffer[outPointer + USB_Bootloader_EVENT_BYTE1]);
                outPointer += USB_Bootloader_EVENT_LENGTH;
            }
        }
        #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO)
            /* Enable Out EP*/
            USB_Bootloader_EnableOutEP(USB_Bootloader_midi_out_ep);   
        #endif  /* End USB_Bootloader_EP_MM == USB_Bootloader__EP_DMAAUTO */  
    }
}


/*******************************************************************************
* Function Name: USB_Bootloader_MIDI_IN_EP_Service
********************************************************************************
*
* Summary: 
*  Services the USB MIDI IN endpoint. Non-blocking. 
*  Checks that previous packet was processed by HOST, otherwise service the 
*  input endpoint on the subsequent call. It is called from the 
*  USB_Bootloader_MIDI_IN_Service() and from the 
*  USB_Bootloader_PutUsbMidiIn() function.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  USB_Bootloader_midi_in_ep: Used as an IN endpoint number.
*  USB_Bootloader_midiInBuffer: Function loads the data from this buffer to 
*    the USB IN endpoint.
*   USB_Bootloader_midiInPointer: Cleared to zero when data are sent.
*
* Reentrant:
*  No
*
*******************************************************************************/
void USB_Bootloader_MIDI_IN_EP_Service(void) 
{
    /* Service the USB MIDI input endpoint */
    /* Check that previous packet was processed by HOST, otherwise service the USB later */
    if ((USB_Bootloader_midiInPointer != 0u) && 
        (USB_Bootloader_GetEPState(USB_Bootloader_midi_in_ep) == USB_Bootloader_EVENT_PENDING))
    {
        #if(USB_Bootloader_EP_MM != USB_Bootloader__EP_DMAAUTO)
            USB_Bootloader_LoadInEP(USB_Bootloader_midi_in_ep, (uint8 *)USB_Bootloader_midiInBuffer,
                                                                                USB_Bootloader_midiInPointer);
        #else /* USB_Bootloader_EP_MM != USB_Bootloader__EP_DMAAUTO */                  
            /* rearm IN EP */
            USB_Bootloader_LoadInEP(USB_Bootloader_midi_in_ep, NULL, USB_Bootloader_midiInPointer);
        #endif /* End USB_Bootloader_EP_MM != USB_Bootloader__EP_DMAAUTO*/
        
        /* Clear the midiInPointer. For DMA mode, clear this pointer in the ARB ISR when data are moved by DMA */
        #if(USB_Bootloader_EP_MM == USB_Bootloader__EP_MANUAL)
            USB_Bootloader_midiInPointer = 0u;
        #endif /* USB_Bootloader_EP_MM == USB_Bootloader__EP_MANUAL */                  
    }
}


/*******************************************************************************
* Function Name: USB_Bootloader_MIDI_IN_Service
********************************************************************************
*
* Summary: 
*  Services the traffic from the MIDI input ports (RX UART) and prepare data 
*  in USB MIDI IN endpoint buffer. 
*  Calls the USB_Bootloader_MIDI_IN_EP_Service() function to sent the 
*  data from buffer to PC. Non-blocking. Should be called from main foreground 
*  task. 
*  This function is not protected from the reentrant calls. When it is required 
*  to use this function in UART RX ISR to guaranty low latency, care should be
*  taken to protect from reentrant calls.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*   USB_Bootloader_midiInPointer: Cleared to zero when data are sent.
*
* Reentrant:
*  No
*
*******************************************************************************/
void USB_Bootloader_MIDI_IN_Service(void) 
{
    /* Service the MIDI UART inputs until either both receivers have no more
    *   events or until the input endpoint buffer fills up. 
    */
    #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
        uint8 m1 = 0;
        uint8 m2 = 0;
        do 
        {
            if (USB_Bootloader_midiInPointer <= (USB_Bootloader_MIDI_IN_BUFF_SIZE - USB_Bootloader_EVENT_LENGTH))
            {
                /* Check MIDI1 input port for a complete event */
                if ((m1 = USB_Bootloader_MIDI1_GetEvent()) != 0u)
                {
                    USB_Bootloader_PrepareInBuffer(m1, (uint8 *)&USB_Bootloader_MIDI1_Event.msgBuff[0], 
                                                USB_Bootloader_MIDI1_Event.size, USB_Bootloader_MIDI_CABLE_00);
                }
            }

        #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
            if (USB_Bootloader_midiInPointer <= (USB_Bootloader_MIDI_IN_BUFF_SIZE - USB_Bootloader_EVENT_LENGTH))
            {
                /* Check MIDI2 input port for a complete event */
                if ((m2 = USB_Bootloader_MIDI2_GetEvent()) != 0u)
                {
                    USB_Bootloader_PrepareInBuffer(m2, (uint8 *)&USB_Bootloader_MIDI2_Event.msgBuff[0], 
                                                USB_Bootloader_MIDI2_Event.size, USB_Bootloader_MIDI_CABLE_01);
                }
            }
        #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */

        } while (((m1 != 0u) || (m2 != 0u)) && 
             (USB_Bootloader_midiInPointer <= (USB_Bootloader_MIDI_IN_BUFF_SIZE - USB_Bootloader_EVENT_LENGTH)));
    #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */

    /* Service the USB MIDI input endpoint */
    USB_Bootloader_MIDI_IN_EP_Service();
    
}


/*******************************************************************************
* Function Name: USB_Bootloader_PutUsbMidiIn
********************************************************************************
*
* Summary:
*  Puts one MIDI messages into the USB MIDI In endpoint buffer. These are 
*  MIDI input messages to the host. This function is only used if the device 
*  has internal MIDI input functionality. USBMIDI_MIDI_IN_Service() function 
*  should additionally be called to send the message from local buffer to 
*  IN endpoint.  
*
* Parameters:
*  ic:   0 = No message (should never happen)
*        1 - 3 = Complete MIDI message in midiMsg
*        3 - IN EP LENGTH = Complete SySEx message(without EOSEX byte) in 
*            midiMsg. The length is limited by the max BULK EP size(64) 
*        MIDI_SYSEX = Start or continuation of SysEx message
*                     (put event bytes in midiMsg buffer)
*        MIDI_EOSEX = End of SysEx message
*                     (put event bytes in midiMsg buffer)
*        MIDI_TUNEREQ = Tune Request message (single byte system common msg)
*        0xf8 - 0xff = Single byte real-time message
*  midiMsg: pointer to MIDI message.
*  cable:   cable number.
*
* Return:
*  USB_Bootloader_TRUE if error.
*  USB_Bootloader_FALSE if success.
*
* Global variables:
*  USB_Bootloader_midi_in_ep: MIDI IN endpoint number used for sending data.
*  USB_Bootloader_midiInPointer: Checked this variable to see if there is 
*    enough free space in the IN endpoint buffer. If buffer is full, initiate 
*    sending to PC. 
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 USB_Bootloader_PutUsbMidiIn(uint8 ic, uint8* midiMsg, uint8 cable)
                                                            
{
    uint8 retError = USB_Bootloader_FALSE;

    /* Protect PrepareInBuffer() function from concurent calls */
    #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
        MIDI1_UART_DisableRxInt();
        #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
            MIDI2_UART_DisableRxInt();
        #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */
    #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */

    if (USB_Bootloader_midiInPointer > 
                (USB_Bootloader_EP[USB_Bootloader_midi_in_ep].bufferSize - USB_Bootloader_EVENT_LENGTH))
    {
        USB_Bootloader_MIDI_IN_EP_Service(); 
    }
    if (USB_Bootloader_midiInPointer <= 
                (USB_Bootloader_EP[USB_Bootloader_midi_in_ep].bufferSize - USB_Bootloader_EVENT_LENGTH))
    {
        if((ic < USB_Bootloader_EVENT_LENGTH) || (ic >= USB_Bootloader_MIDI_STATUS_MASK))
        {
            USB_Bootloader_PrepareInBuffer(ic, midiMsg, ic, cable);
        }
        else
        {   /* only SysEx message is greater than 4 bytes */
            do
            {
                USB_Bootloader_PrepareInBuffer(USB_Bootloader_MIDI_SYSEX, midiMsg, 
                                                 USB_Bootloader_EVENT_BYTE3, cable);
                ic -= USB_Bootloader_EVENT_BYTE3;
                midiMsg += USB_Bootloader_EVENT_BYTE3;
                if (USB_Bootloader_midiInPointer > 
                    (USB_Bootloader_EP[USB_Bootloader_midi_in_ep].bufferSize - USB_Bootloader_EVENT_LENGTH))
                {
                    USB_Bootloader_MIDI_IN_EP_Service();
                    if (USB_Bootloader_midiInPointer >
                        (USB_Bootloader_EP[USB_Bootloader_midi_in_ep].bufferSize - USB_Bootloader_EVENT_LENGTH))
                    {
                        /* Error condition. HOST is not ready to receive this packet */
                        retError = USB_Bootloader_TRUE;
                        break;
                    }
                }
            }while(ic > USB_Bootloader_EVENT_BYTE3);
            
            if(retError == USB_Bootloader_FALSE)
            {
                USB_Bootloader_PrepareInBuffer(USB_Bootloader_MIDI_EOSEX, midiMsg, ic, cable);
            }    
        }
    }
    else
    {
        /* Error condition. HOST is not ready to receive this packet */
        retError = USB_Bootloader_TRUE;
    }

    #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)
        MIDI1_UART_EnableRxInt();
        #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
            MIDI2_UART_EnableRxInt();
        #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */
    #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */

    return (retError);
}


/*******************************************************************************
* Function Name: USB_Bootloader_PrepareInBuffer
********************************************************************************
*
* Summary:
*  Builds a USB MIDI event in the input endpoint buffer at the current pointer.
*  Puts one MIDI message into the USB MIDI In endpoint buffer.
*
* Parameters:
*  ic:   0 = No message (should never happen)
*        1 - 3 = Complete MIDI message at pMdat[0]
*        MIDI_SYSEX = Start or continuation of SysEx message
*                     (put eventLen bytes in buffer)
*        MIDI_EOSEX = End of SysEx message
*                     (put eventLen bytes in buffer,
*                      and append MIDI_EOSEX)
*        MIDI_TUNEREQ = Tune Request message (single byte system common msg)
*        0xf8 - 0xff = Single byte real-time message
*
*  srcBuff: pointer to MIDI data
*  eventLen: number of bytes in MIDI event
*  cable: MIDI source port number
*
* Return:
*  None
*
* Global variables:
*  USB_Bootloader_midiInBuffer: This buffer is used for saving and combine the
*    received from UART(s) and(or) generated internally by 
*    USB_Bootloader_PutUsbMidiIn() function messages. 
*  USB_Bootloader_midiInPointer: Used as an index for midiInBuffer to 
*     write data.
*
* Reentrant:
*  No
*
*******************************************************************************/
void USB_Bootloader_PrepareInBuffer(uint8 ic, uint8 *srcBuff, uint8 eventLen, uint8 cable)
                                                             
{
    uint8 srcBuffZero;
    uint8 srcBuffOne;

    srcBuffZero = srcBuff[0u];
    srcBuffOne  = srcBuff[1u];
    
    if (ic >= (USB_Bootloader_MIDI_STATUS_MASK | USB_Bootloader_MIDI_SINGLE_BYTE_MASK))
    {
        USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = USB_Bootloader_SINGLE_BYTE | cable;
        USB_Bootloader_midiInPointer++;
        USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = ic;
        USB_Bootloader_midiInPointer++;
        USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = 0u;
        USB_Bootloader_midiInPointer++;
        USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = 0u;
        USB_Bootloader_midiInPointer++;
    }
    else if((ic < USB_Bootloader_EVENT_LENGTH) || (ic == USB_Bootloader_MIDI_SYSEX))
    {
        if(ic == USB_Bootloader_MIDI_SYSEX)
        {
            USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = USB_Bootloader_SYSEX | cable;
            USB_Bootloader_midiInPointer++;
        }
        else if (srcBuffZero < USB_Bootloader_MIDI_SYSEX)
        {
            USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] =
                                                    ((srcBuffZero >> 4u) & USB_Bootloader_CIN_MASK) | cable;
            USB_Bootloader_midiInPointer++;
        }
        else if (srcBuffZero == USB_Bootloader_MIDI_TUNEREQ)
        {
            USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = USB_Bootloader_1BYTE_COMMON | cable;
            USB_Bootloader_midiInPointer++;
        }
        else if ((srcBuffZero == USB_Bootloader_MIDI_QFM) || (srcBuffZero == USB_Bootloader_MIDI_SONGSEL))
        {
            USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = USB_Bootloader_2BYTE_COMMON | cable;
            USB_Bootloader_midiInPointer++;
        }
        else if (srcBuffZero == USB_Bootloader_MIDI_SPP)
        {
            USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = USB_Bootloader_3BYTE_COMMON | cable;
            USB_Bootloader_midiInPointer++;
        }
        else
        {
        }
        
        USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = srcBuffZero;
        USB_Bootloader_midiInPointer++;
        USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = srcBuffOne;
        USB_Bootloader_midiInPointer++;
        USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = srcBuff[2u];
        USB_Bootloader_midiInPointer++;
    }
    else if (ic == USB_Bootloader_MIDI_EOSEX)
    {
        switch (eventLen)
        {
            case 0:
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] =
                                                                    USB_Bootloader_SYSEX_ENDS_WITH1 | cable;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = USB_Bootloader_MIDI_EOSEX;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = 0u;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = 0u;
                USB_Bootloader_midiInPointer++;
                break;
            case 1:
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] =
                                                                    USB_Bootloader_SYSEX_ENDS_WITH2 | cable;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = srcBuffZero;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = USB_Bootloader_MIDI_EOSEX;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = 0u;
                USB_Bootloader_midiInPointer++;
                break;
            case 2:
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] =
                                                                    USB_Bootloader_SYSEX_ENDS_WITH3 | cable;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = srcBuffZero;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = srcBuffOne;
                USB_Bootloader_midiInPointer++;
                USB_Bootloader_midiInBuffer[USB_Bootloader_midiInPointer] = USB_Bootloader_MIDI_EOSEX;
                USB_Bootloader_midiInPointer++;
                break;
            default:
                break;
        }
    }
}

/* The implementation for external serial input and output connections 
*  to route USB MIDI data to and from those connections.
*/   
#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF)


    /*******************************************************************************
    * Function Name: USB_Bootloader_MIDI_Init
    ********************************************************************************
    *
    * Summary:
    *  Initializes MIDI variables and starts the UART(s) hardware block(s).
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  Change the priority of the UART(s) TX interrupts to be higher than the 
    *  default EP ISR priority.
    *
    * Global variables:
    *   USB_Bootloader_MIDI_Event: initialized to zero.
    *   USB_Bootloader_MIDI_TxRunStat: initialized to zero.
    *
    *******************************************************************************/
    void USB_Bootloader_MIDI_Init(void) 
    {
        USB_Bootloader_MIDI1_Event.length = 0u;
        USB_Bootloader_MIDI1_Event.count = 0u;
        USB_Bootloader_MIDI1_Event.size = 0u;
        USB_Bootloader_MIDI1_Event.runstat = 0u;
        USB_Bootloader_MIDI1_TxRunStat = 0u;
        USB_Bootloader_MIDI1_InqFlags = 0u;
        /* Start UART block */
        MIDI1_UART_Start();
        /* Change the priority of the UART TX and RX interrupt */
        CyIntSetPriority(MIDI1_UART_TX_VECT_NUM, USB_Bootloader_CUSTOM_UART_TX_PRIOR_NUM);
        CyIntSetPriority(MIDI1_UART_RX_VECT_NUM, USB_Bootloader_CUSTOM_UART_RX_PRIOR_NUM);

        #if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)
            USB_Bootloader_MIDI2_Event.length = 0u;
            USB_Bootloader_MIDI2_Event.count = 0u;
            USB_Bootloader_MIDI2_Event.size = 0u;
            USB_Bootloader_MIDI2_Event.runstat = 0u;
            USB_Bootloader_MIDI2_TxRunStat = 0u;
            USB_Bootloader_MIDI2_InqFlags = 0u;
            /* Start second UART block */
            MIDI2_UART_Start();
            /* Change the priority of the UART TX interrupt */
            CyIntSetPriority(MIDI2_UART_TX_VECT_NUM, USB_Bootloader_CUSTOM_UART_TX_PRIOR_NUM);
            CyIntSetPriority(MIDI2_UART_RX_VECT_NUM, USB_Bootloader_CUSTOM_UART_RX_PRIOR_NUM);
        #endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF*/
        
        /* `#START MIDI_INIT_CUSTOM` Init other extended UARTs here */

        /* `#END` */    

    }


    /*******************************************************************************
    * Function Name: USB_Bootloader_ProcessMidiIn
    ********************************************************************************
    *
    * Summary:
    *  Processes one byte of incoming MIDI data.
    *
    * Parameters:
    *   mData = current MIDI input data byte
    *   *rxStat = pointer to a MIDI_RX_STATUS structure    
    *
    * Return:
    *   0, if no complete message
    *   1 - 4, if message complete
    *   MIDI_SYSEX, if start or continuation of system exclusive
    *   MIDI_EOSEX, if end of system exclusive
    *   0xf8 - 0xff, if single byte real time message
    *
    *******************************************************************************/
    uint8 USB_Bootloader_ProcessMidiIn(uint8 mData, USB_Bootloader_MIDI_RX_STATUS *rxStat)
                                                                
    {
        uint8 midiReturn = 0u;

        /* Check for a MIDI status byte.  All status bytes, except real time messages,
        *  which are a single byte, force the start of a new buffer cycle. 
        */
        if ((mData & USB_Bootloader_MIDI_STATUS_BYTE_MASK) != 0u)
        {
            if ((mData & USB_Bootloader_MIDI_STATUS_MASK) == USB_Bootloader_MIDI_STATUS_MASK)
            {
                if ((mData & USB_Bootloader_MIDI_SINGLE_BYTE_MASK) != 0u) /* System Real-Time Messages(singl byte) */
                {
                    midiReturn = mData;
                }
                else                              /* System Common Messages */
                {
                    switch (mData)
                    {
                        case USB_Bootloader_MIDI_SYSEX:
                            rxStat->msgBuff[0u] = USB_Bootloader_MIDI_SYSEX;
                            rxStat->runstat = USB_Bootloader_MIDI_SYSEX;
                            rxStat->count = 1u;
                            rxStat->length = 3u;
                            break;
                        case USB_Bootloader_MIDI_EOSEX:
                            rxStat->runstat = 0u;
                            rxStat->size = rxStat->count;
                            rxStat->count = 0u;
                            midiReturn = USB_Bootloader_MIDI_EOSEX;
                            break;
                        case USB_Bootloader_MIDI_SPP:
                            rxStat->msgBuff[0u] = USB_Bootloader_MIDI_SPP;
                            rxStat->runstat = 0u;
                            rxStat->count = 1u;
                            rxStat->length = 3u;
                            break;
                        case USB_Bootloader_MIDI_SONGSEL:
                            rxStat->msgBuff[0u] = USB_Bootloader_MIDI_SONGSEL;
                            rxStat->runstat = 0u;
                            rxStat->count = 1u;
                            rxStat->length = 2u;
                            break;
                        case USB_Bootloader_MIDI_QFM:
                            rxStat->msgBuff[0u] = USB_Bootloader_MIDI_QFM;
                            rxStat->runstat = 0u;
                            rxStat->count = 1u;
                            rxStat->length = 2u;
                            break;
                        case USB_Bootloader_MIDI_TUNEREQ:
                            rxStat->msgBuff[0u] = USB_Bootloader_MIDI_TUNEREQ;
                            rxStat->runstat = 0u;
                            rxStat->size = 1u;
                            rxStat->count = 0u;
                            midiReturn = rxStat->size;
                            break;
                        default:
                            break;
                    }
                }
            }
            else /* Channel Messages */
            {
                rxStat->msgBuff[0u] = mData;
                rxStat->runstat = mData;
                rxStat->count = 1u;
                switch (mData & USB_Bootloader_MIDI_STATUS_MASK)
                {
                    case USB_Bootloader_MIDI_NOTE_OFF:
                    case USB_Bootloader_MIDI_NOTE_ON:
                    case USB_Bootloader_MIDI_POLY_KEY_PRESSURE:
                    case USB_Bootloader_MIDI_CONTROL_CHANGE:
                    case USB_Bootloader_MIDI_PITCH_BEND_CHANGE:
                        rxStat->length = 3u;
                        break;
                    case USB_Bootloader_MIDI_PROGRAM_CHANGE:
                    case USB_Bootloader_MIDI_CHANNEL_PRESSURE:
                        rxStat->length = 2u;
                        break;
                    default:
                        rxStat->runstat = 0u;
                        rxStat->count = 0u;
                        break;
                }
            }
        }

        /* Otherwise, it's a data byte */
        else
        {
            if (rxStat->runstat == USB_Bootloader_MIDI_SYSEX)
            {
                rxStat->msgBuff[rxStat->count] = mData;
                rxStat->count++;
                if (rxStat->count >= rxStat->length)
                {
                    rxStat->size = rxStat->count;
                    rxStat->count = 0u;
                    midiReturn = USB_Bootloader_MIDI_SYSEX;
                }
            }
            else if (rxStat->count > 0u)
            {
                rxStat->msgBuff[rxStat->count] = mData;
                rxStat->count++;
                if (rxStat->count >= rxStat->length)
                {
                    rxStat->size = rxStat->count;
                    rxStat->count = 0u;
                    midiReturn = rxStat->size;
                }
            }
            else if (rxStat->runstat != 0u)
            {
                rxStat->msgBuff[0u] = rxStat->runstat;
                rxStat->msgBuff[1u] = mData;
                rxStat->count = 2u;
                switch (rxStat->runstat & USB_Bootloader_MIDI_STATUS_MASK)
                {
                    case USB_Bootloader_MIDI_NOTE_OFF:
                    case USB_Bootloader_MIDI_NOTE_ON:
                    case USB_Bootloader_MIDI_POLY_KEY_PRESSURE:
                    case USB_Bootloader_MIDI_CONTROL_CHANGE:
                    case USB_Bootloader_MIDI_PITCH_BEND_CHANGE:
                        rxStat->length = 3u;
                        break;
                    case USB_Bootloader_MIDI_PROGRAM_CHANGE:
                    case USB_Bootloader_MIDI_CHANNEL_PRESSURE:
                        rxStat->size =rxStat->count;
                        rxStat->count = 0u;
                        midiReturn = rxStat->size;
                        break;
                    default:
                        rxStat->count = 0u;
                    break;
                }
            }
            else
            {
            }
        }
        return (midiReturn);
    }


    /*******************************************************************************
    * Function Name: USB_Bootloader_MIDI1_GetEvent
    ********************************************************************************
    *
    * Summary:
    *  Checks for incoming MIDI data, calls the MIDI event builder if so. 
    *  Returns either empty or with a complete event.
    *
    * Parameters:
    *  None
    *
    * Return:
    *   0, if no complete message
    *   1 - 4, if message complete
    *   MIDI_SYSEX, if start or continuation of system exclusive
    *   MIDI_EOSEX, if end of system exclusive
    *   0xf8 - 0xff, if single byte real time message
    *    
    * Global variables:
    *  USB_Bootloader_MIDI1_Event: RX status structure used to parse received 
    *    data.
    *
    *******************************************************************************/
    uint8 USB_Bootloader_MIDI1_GetEvent(void) 
    {
        uint8 msgRtn = 0u;
        uint8 rxData;
        #if (MIDI1_UART_RXBUFFERSIZE >= 256u)
            uint16 loc_rxBufferRead;
            #if CY_PSOC3 /* This local variable required only for PSOC3 and large buffer */
                uint16 loc_rxBufferWrite;
            #endif /* end CY_PSOC3 */    
        #else    
            uint8 loc_rxBufferRead;
        #endif /* End MIDI1_UART_RXBUFFERSIZE >= 256 */    

        if ( ((MIDI1_UART_rxBufferRead != MIDI1_UART_rxBufferWrite) || (MIDI1_UART_rxBufferLoopDetect > 0u)) 
             && (msgRtn == 0u) )
        {
            /* Protect variables that could change on interrupt by disabling Rx interrupt.*/        
            #if ((MIDI1_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                CyIntDisable(MIDI1_UART_RX_VECT_NUM);
            #endif /* End MIDI1_UART_RXBUFFERSIZE >= 256 */    
            loc_rxBufferRead = MIDI1_UART_rxBufferRead;
            #if ((MIDI1_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                loc_rxBufferWrite = MIDI1_UART_rxBufferWrite;
                CyIntEnable(MIDI1_UART_RX_VECT_NUM);
            #endif /* End MIDI1_UART_RXBUFFERSIZE >= 256 */  
            
            /* Stay here until either the buffer is empty or we have a complete message
            *  in the message buffer. Note that we must use a temporary output pointer to
            *   since it takes two instructions to increment with a wrap, and we can't
            *   risk doing that with the real pointer and getting an interrupt in between
            *   instructions. 
            */

            #if ((MIDI1_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                while ( ((loc_rxBufferRead != loc_rxBufferWrite) || (MIDI1_UART_rxBufferLoopDetect > 0u))
                    && (msgRtn == 0u) )
            #else        
                while ( ((loc_rxBufferRead != MIDI1_UART_rxBufferWrite) || (MIDI1_UART_rxBufferLoopDetect > 0u))
                    && (msgRtn == 0u) )
            #endif /* End MIDI1_UART_RXBUFFERSIZE >= 256 && CY_PSOC3 */  
                {
                    rxData = MIDI1_UART_rxBuffer[loc_rxBufferRead];
                    loc_rxBufferRead++;
                    if(loc_rxBufferRead >= MIDI1_UART_RXBUFFERSIZE)
                    {
                        loc_rxBufferRead = 0u;
                    }
                    if(MIDI1_UART_rxBufferLoopDetect > 0u )
                    {
                        MIDI1_UART_rxBufferLoopDetect = 0u;
                    }
                        
                    msgRtn = USB_Bootloader_ProcessMidiIn(rxData, 
                                                    (USB_Bootloader_MIDI_RX_STATUS *)&USB_Bootloader_MIDI1_Event);
        		}
            
            /* Finally, update the real output pointer, then return with
            *  an indication as to whether there's a complete message in the buffer. 
            */
            #if ((MIDI1_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                CyIntDisable(MIDI1_UART_RX_VECT_NUM);
            #endif /* End MIDI1_UART_RXBUFFERSIZE >= 256 */    
        	MIDI1_UART_rxBufferRead = loc_rxBufferRead;
            #if ((MIDI1_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                CyIntEnable(MIDI1_UART_RX_VECT_NUM);
            #endif /* End MIDI1_UART_RXBUFFERSIZE >= 256 */   
        }
        
        return (msgRtn);
    }


    /*******************************************************************************
    * Function Name: USB_Bootloader_MIDI1_ProcessUsbOut
    ********************************************************************************
    *
    * Summary:
    *  Process a USB MIDI output event.
    *  Puts data into the MIDI TX output buffer.
    *
    * Parameters:
    *  *epBuf: pointer on MIDI event.
    *
    * Return:
    *   None
    *
    * Global variables:
    *  USB_Bootloader_MIDI1_TxRunStat: This variable used to save the MIDI 
    *    status byte and skip to send the repeated status byte in subsequent event.
    *  USB_Bootloader_MIDI1_InqFlags: The following flags are set when SysEx 
    *    message comes. 
    *    USB_Bootloader_INQ_SYSEX_FLAG: Non-Real Time SySEx message received.
    *    USB_Bootloader_INQ_IDENTITY_REQ_FLAG: Identity Request received. 
    *      This bit should be cleared by user when Identity Reply message generated.
    *    
    *******************************************************************************/
    void USB_Bootloader_MIDI1_ProcessUsbOut(uint8 *epBuf) 
    {
        uint8 cmd;
        uint8 len;
        uint8 i;

        /* User code is required at the beginning of the procedure */
        /* `#START MIDI1_PROCESS_OUT_BEGIN` */

        /* `#END` */

        cmd = epBuf[USB_Bootloader_EVENT_BYTE0] & USB_Bootloader_CIN_MASK;
        if((cmd != USB_Bootloader_RESERVED0) && (cmd != USB_Bootloader_RESERVED1))
        {
            len = USB_Bootloader_MIDI_SIZE[cmd];
            i = USB_Bootloader_EVENT_BYTE1;
            /* Universal System Exclusive message parsing */
            if(cmd == USB_Bootloader_SYSEX)
            {
                if((epBuf[USB_Bootloader_EVENT_BYTE1] == USB_Bootloader_MIDI_SYSEX) &&
                   (epBuf[USB_Bootloader_EVENT_BYTE2] == USB_Bootloader_MIDI_SYSEX_NON_REAL_TIME))
                {   /* Non-Real Time SySEx starts */
                    USB_Bootloader_MIDI1_InqFlags |= USB_Bootloader_INQ_SYSEX_FLAG;
                }
                else
                {
                    USB_Bootloader_MIDI1_InqFlags &= ~USB_Bootloader_INQ_SYSEX_FLAG;
                }
            }
            else if(cmd == USB_Bootloader_SYSEX_ENDS_WITH1)
            {   
                USB_Bootloader_MIDI1_InqFlags &= ~USB_Bootloader_INQ_SYSEX_FLAG;
            }
            else if(cmd == USB_Bootloader_SYSEX_ENDS_WITH2)
            {   
                USB_Bootloader_MIDI1_InqFlags &= ~USB_Bootloader_INQ_SYSEX_FLAG;
            }
            else if(cmd == USB_Bootloader_SYSEX_ENDS_WITH3)
            {
                /* Identify Request support */
		        if((USB_Bootloader_MIDI1_InqFlags & USB_Bootloader_INQ_SYSEX_FLAG) != 0u)
			    {
                    USB_Bootloader_MIDI1_InqFlags &= ~USB_Bootloader_INQ_SYSEX_FLAG;
                    if((epBuf[USB_Bootloader_EVENT_BYTE1] == USB_Bootloader_MIDI_SYSEX_GEN_INFORMATION) &&
                    (epBuf[USB_Bootloader_EVENT_BYTE2] == USB_Bootloader_MIDI_SYSEX_IDENTITY_REQ))
                    {   /* Set the flag about received the Identity Request.
                        *  The Identity Reply message may be send by user code.
                        */
                        USB_Bootloader_MIDI1_InqFlags |= USB_Bootloader_INQ_IDENTITY_REQ_FLAG;
                    }
                }    
			}            
            /* Running Status for Voice and Mode messages only. */
            if((cmd >= USB_Bootloader_NOTE_OFF) && ( cmd <= USB_Bootloader_PITCH_BEND_CHANGE)) 
            {
                if(USB_Bootloader_MIDI1_TxRunStat == epBuf[USB_Bootloader_EVENT_BYTE1])
                {   /* Skip the repeated Status byte */
                    i++;
                }
                else
                {   /* Save Status byte for next event */
                    USB_Bootloader_MIDI1_TxRunStat = epBuf[USB_Bootloader_EVENT_BYTE1];
                }
            }    
            else
            {   /* Clear Running Status */
                USB_Bootloader_MIDI1_TxRunStat = 0u;
            }
            /* Puts data into the MIDI TX output buffer.*/
            do
            {
                MIDI1_UART_PutChar(epBuf[i]);
                i++;
            } while (i <= len);
        }

        /* User code is required at the end of the procedure */
        /* `#START MIDI1_PROCESS_OUT_END` */

        /* `#END` */
    }

#if (USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF)


    /*******************************************************************************
    * Function Name: USB_Bootloader_MIDI2_GetEvent
    ********************************************************************************
    *
    * Summary:
    *  Checks for incoming MIDI data, calls the MIDI event builder if so. 
    *  Returns either empty or with a complete event.
    *
    * Parameters:
    *  None
    *
    * Return:
    *   0, if no complete message
    *   1 - 4, if message complete
    *   MIDI_SYSEX, if start or continuation of system exclusive
    *   MIDI_EOSEX, if end of system exclusive
    *   0xf8 - 0xff, if single byte real time message
    *
    * Global variables:
    *  USB_Bootloader_MIDI2_Event: RX status structure used to parse received 
    *    data.
    *
    *******************************************************************************/
    uint8 USB_Bootloader_MIDI2_GetEvent(void) 
    {
        uint8 msgRtn = 0u;
        uint8 rxData;
        #if (MIDI2_UART_RXBUFFERSIZE >= 256u)
            uint16 loc_rxBufferRead;
            #if CY_PSOC3 /* This local variable required only for PSOC3 and large buffer */
                uint16 loc_rxBufferWrite;
            #endif /* end CY_PSOC3 */    
        #else    
            uint8 loc_rxBufferRead;
        #endif /* End MIDI2_UART_RXBUFFERSIZE >= 256 */    

        if ( ((MIDI2_UART_rxBufferRead != MIDI2_UART_rxBufferWrite) || (MIDI2_UART_rxBufferLoopDetect > 0u)) 
             && (msgRtn == 0u) )
        {
            /* Protect variables that could change on interrupt by disabling Rx interrupt.*/        
            #if ((MIDI2_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                CyIntDisable(MIDI2_UART_RX_VECT_NUM);
            #endif /* End MIDI2_UART_RXBUFFERSIZE >= 256 */    
            loc_rxBufferRead = MIDI2_UART_rxBufferRead;
            #if ((MIDI2_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                loc_rxBufferWrite = MIDI2_UART_rxBufferWrite;
                CyIntEnable(MIDI2_UART_RX_VECT_NUM);
            #endif /* End MIDI2_UART_RXBUFFERSIZE >= 256 */  
            
            /* Stay here until either the buffer is empty or we have a complete message
            *  in the message buffer. Note that we must use a temporary output pointer to
            *   since it takes two instructions to increment with a wrap, and we can't
            *   risk doing that with the real pointer and getting an interrupt in between
            *   instructions. 
            */

            #if ((MIDI2_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                while ( ((loc_rxBufferRead != loc_rxBufferWrite) || (MIDI1_UART_rxBufferLoopDetect > 0u))
                    && (msgRtn == 0u) )
            #else        
                while ( ((loc_rxBufferRead != MIDI2_UART_rxBufferWrite) || (MIDI1_UART_rxBufferLoopDetect > 0u))
                    && (msgRtn == 0u) )
            #endif /* End MIDI1_UART_RXBUFFERSIZE >= 256 && CY_PSOC3 */  
                {
                    rxData = MIDI2_UART_rxBuffer[loc_rxBufferRead];
                    loc_rxBufferRead++;
                    if(loc_rxBufferRead >= MIDI2_UART_RXBUFFERSIZE)
                    {
                        loc_rxBufferRead = 0u;
                    }
                    if(MIDI2_UART_rxBufferLoopDetect > 0u )
                    {
                        MIDI2_UART_rxBufferLoopDetect = 0u;
                    }
                        
                    msgRtn = USB_Bootloader_ProcessMidiIn(rxData, 
                                                    (USB_Bootloader_MIDI_RX_STATUS *)&USB_Bootloader_MIDI2_Event);
        		}
            
            /* Finally, update the real output pointer, then return with
            *  an indication as to whether there's a complete message in the buffer. 
            */
            #if ((MIDI2_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                CyIntDisable(MIDI2_UART_RX_VECT_NUM);
            #endif /* End MIDI2_UART_RXBUFFERSIZE >= 256 */    
        	MIDI2_UART_rxBufferRead = loc_rxBufferRead;
            #if ((MIDI2_UART_RXBUFFERSIZE >= 256u) && (CY_PSOC3))
                CyIntEnable(MIDI2_UART_RX_VECT_NUM);
            #endif /* End MIDI2_UART_RXBUFFERSIZE >= 256 */   
        }
        
        return (msgRtn);
    }


    /*******************************************************************************
    * Function Name: USB_Bootloader_MIDI2_ProcessUsbOut
    ********************************************************************************
    *
    * Summary:
    *  Process a USB MIDI output event.
    *  Puts data into the MIDI TX output buffer.
    *
    * Parameters:
    *  *epBuf: pointer on MIDI event.
    *
    * Return:
    *   None
    *
    * Global variables:
    *  USB_Bootloader_MIDI1_TxRunStat: This variable used to save the MIDI 
    *    status byte and skip to send the repeated status byte in subsequent event.
    *  USB_Bootloader_MIDI1_InqFlags: The following flags are set when SysEx 
    *    message comes. 
    *    USB_Bootloader_INQ_SYSEX_FLAG: Non-Real Time SySEx message received.
    *    USB_Bootloader_INQ_IDENTITY_REQ_FLAG: Identity Request received. 
    *      This bit should be cleared by user when Identity Reply message generated.
    *    
    *******************************************************************************/
    void USB_Bootloader_MIDI2_ProcessUsbOut(uint8 *epBuf) 
    {
        uint8 cmd;
        uint8 len;
        uint8 i;

        /* User code is required at the beginning of the procedure */
        /* `#START MIDI2_PROCESS_OUT_START` */

        /* `#END` */

        cmd = epBuf[USB_Bootloader_EVENT_BYTE0] & USB_Bootloader_CIN_MASK;
        if((cmd != USB_Bootloader_RESERVED0) && (cmd != USB_Bootloader_RESERVED1))
        {
            len = USB_Bootloader_MIDI_SIZE[cmd];
            i = USB_Bootloader_EVENT_BYTE1;
            /* Universal System Exclusive message parsing */
            if(cmd == USB_Bootloader_SYSEX)
            {
                if((epBuf[USB_Bootloader_EVENT_BYTE1] == USB_Bootloader_MIDI_SYSEX) &&
                   (epBuf[USB_Bootloader_EVENT_BYTE2] == USB_Bootloader_MIDI_SYSEX_NON_REAL_TIME))
                {   /* SySEx starts */
                    USB_Bootloader_MIDI2_InqFlags |= USB_Bootloader_INQ_SYSEX_FLAG;
                }
                else
                {
                    USB_Bootloader_MIDI2_InqFlags &= ~USB_Bootloader_INQ_SYSEX_FLAG;
                }
            }
            else if(cmd == USB_Bootloader_SYSEX_ENDS_WITH1)
            {   
                USB_Bootloader_MIDI2_InqFlags &= ~USB_Bootloader_INQ_SYSEX_FLAG;
            }
            else if(cmd == USB_Bootloader_SYSEX_ENDS_WITH2)
            {   
                USB_Bootloader_MIDI2_InqFlags &= ~USB_Bootloader_INQ_SYSEX_FLAG;
            }
            else if(cmd == USB_Bootloader_SYSEX_ENDS_WITH3)
            {
                /* Identify Request support */
		        if((USB_Bootloader_MIDI2_InqFlags & USB_Bootloader_INQ_SYSEX_FLAG) != 0u)
			    {
                    USB_Bootloader_MIDI2_InqFlags &= ~USB_Bootloader_INQ_SYSEX_FLAG;
                    if((epBuf[USB_Bootloader_EVENT_BYTE1] == USB_Bootloader_MIDI_SYSEX_GEN_INFORMATION) &&
                       (epBuf[USB_Bootloader_EVENT_BYTE2] == USB_Bootloader_MIDI_SYSEX_IDENTITY_REQ))
                    {   /* Set the flag about received the Identity Request.
                        *  The Identity Reply message may be send by user code.
                        */
                        USB_Bootloader_MIDI2_InqFlags |= USB_Bootloader_INQ_IDENTITY_REQ_FLAG;
                    }
                }    
			}
            else /* Do nothing for other command */
            {
            }
            /* Running Status for Voice and Mode messages only. */
            if((cmd >= USB_Bootloader_NOTE_OFF) && ( cmd <= USB_Bootloader_PITCH_BEND_CHANGE)) 
            {
                if(USB_Bootloader_MIDI2_TxRunStat == epBuf[USB_Bootloader_EVENT_BYTE1])
                {   /* Skip the repeated Status byte */
                    i++;
                }
                else
                {   /* Save Status byte for next event */
                    USB_Bootloader_MIDI2_TxRunStat = epBuf[USB_Bootloader_EVENT_BYTE1];
                }
            }    
            else
            {   /* Clear Running Status */
                USB_Bootloader_MIDI2_TxRunStat = 0u;
            }
            /* Puts data into the MIDI TX output buffer.*/
            do
            {
                MIDI2_UART_PutChar(epBuf[i]);
                i++;
            } while (i <= len);
        }

        /* User code is required at the end of the procedure */
        /* `#START MIDI2_PROCESS_OUT_END` */

        /* `#END` */
    }
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_TWO_EXT_INTRF */
#endif /* End USB_Bootloader_MIDI_EXT_MODE >= USB_Bootloader_ONE_EXT_INTRF */

#endif  /* End (USB_Bootloader_ENABLE_MIDI_API != 0u) */


/* `#START MIDI_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /* End defined(USB_Bootloader_ENABLE_MIDI_STREAMING) */


/* [] END OF FILE */
