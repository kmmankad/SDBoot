/*******************************************************************************
* File Name: USB_Bootloader_drv.c
* Version 2.12
*
* Description:
*  Endpoint 0 Driver for the USBFS Component.
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
* Forward references for the EP0 ISR
***************************************/

void  USB_Bootloader_HandleSetup(void) ;
void  USB_Bootloader_HandleIN(void) ;
void  USB_Bootloader_HandleOUT(void) ;
uint8 USB_Bootloader_InitControlRead(void) ;
void  USB_Bootloader_ControlReadDataStage(void) ;
void  USB_Bootloader_ControlReadStatusStage(void) ;
void  USB_Bootloader_ControlReadPrematureStatus(void)
                                                ;
uint8 USB_Bootloader_InitControlWrite(void) ;
uint8 USB_Bootloader_InitZeroLengthControlTransfer(void)
                                                ;
void  USB_Bootloader_ControlWriteDataStage(void) ;
void  USB_Bootloader_ControlWriteStatusStage(void) ;
void  USB_Bootloader_ControlWritePrematureStatus(void)
                                                ;
uint8 USB_Bootloader_InitNoDataControlTransfer(void) ;
void  USB_Bootloader_NoDataControlStatusStage(void) ;
void  USB_Bootloader_InitializeStatusBlock(void) ;
void  USB_Bootloader_UpdateStatusBlock(uint8 completionCode) ;


/***************************************
* Request Handlers
***************************************/

uint8 USB_Bootloader_HandleStandardRqst(void) ;
uint8 USB_Bootloader_DispatchClassRqst(void) ;
uint8 USB_Bootloader_HandleVendorRqst(void) ;


/***************************************
* External data references
***************************************/


/***************************************
* Global data allocation
***************************************/

volatile T_USB_Bootloader_EP_CTL_BLOCK USB_Bootloader_EP[USB_Bootloader_MAX_EP];
volatile uint8 USB_Bootloader_configuration;
volatile uint8 USB_Bootloader_configurationChanged;
volatile uint8 USB_Bootloader_deviceAddress;
volatile uint8 USB_Bootloader_deviceStatus;
volatile uint8 USB_Bootloader_interfaceSetting[USB_Bootloader_MAX_INTERFACES_NUMBER];
volatile uint8 USB_Bootloader_interfaceSetting_last[USB_Bootloader_MAX_INTERFACES_NUMBER];
volatile uint8 USB_Bootloader_interfaceStatus[USB_Bootloader_MAX_INTERFACES_NUMBER];
volatile uint8 USB_Bootloader_device;
const uint8 CYCODE *USB_Bootloader_interfaceClass;


/***************************************
* Local data allocation
***************************************/

volatile uint8 USB_Bootloader_ep0Toggle;
volatile uint8 USB_Bootloader_lastPacketSize;
volatile uint8 USB_Bootloader_transferState;
volatile T_USB_Bootloader_TD USB_Bootloader_currentTD;
volatile uint8 USB_Bootloader_ep0Mode;
volatile uint8 USB_Bootloader_ep0Count;
volatile uint16 USB_Bootloader_transferByteCount;


/*******************************************************************************
* Function Name: USB_Bootloader_ep_0_Interrupt
********************************************************************************
*
* Summary:
*  This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.
*  It dispactches setup requests and handles the data and status stages.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USB_Bootloader_EP_0_ISR)
{
    uint8 bRegTemp;
    uint8 modifyReg;


    bRegTemp = CY_GET_REG8(USB_Bootloader_EP0_CR_PTR);
    if (bRegTemp & USB_Bootloader_MODE_ACKD)
    {
        modifyReg = 1u;
        if (bRegTemp & USB_Bootloader_MODE_SETUP_RCVD)
        {
            if((bRegTemp & USB_Bootloader_MODE_MASK) != USB_Bootloader_MODE_NAK_IN_OUT)
            {
                modifyReg = 0u;                                     /* When mode not NAK_IN_OUT => invalid setup */
            }
            else
            {
                USB_Bootloader_HandleSetup();
                if(USB_Bootloader_ep0Mode & USB_Bootloader_MODE_SETUP_RCVD)
                {
                    modifyReg = 0u;                         /* if SETUP bit set -> exit without modifying the mode */
                }

            }
        }
        else if (bRegTemp & USB_Bootloader_MODE_IN_RCVD)
        {
            USB_Bootloader_HandleIN();
        }
        else if (bRegTemp & USB_Bootloader_MODE_OUT_RCVD)
        {
            USB_Bootloader_HandleOUT();
        }
        else
        {
            modifyReg = 0u;
        }
        if(modifyReg != 0u)
        {
            bRegTemp = CY_GET_REG8(USB_Bootloader_EP0_CR_PTR);    /* unlock registers */
            if((bRegTemp & USB_Bootloader_MODE_SETUP_RCVD) == 0)  /* Check if SETUP bit is not set, otherwise exit */
            {
                /* Update the count register */
                bRegTemp = USB_Bootloader_ep0Toggle | USB_Bootloader_ep0Count;
                CY_SET_REG8(USB_Bootloader_EP0_CNT_PTR, bRegTemp);
                if(bRegTemp == CY_GET_REG8(USB_Bootloader_EP0_CNT_PTR))   /* continue if writing was succesful */
                {
                    do
                    {
                        modifyReg = USB_Bootloader_ep0Mode;       /* init temporary variable */
                        /* unlock registers */
                        bRegTemp = CY_GET_REG8(USB_Bootloader_EP0_CR_PTR) & USB_Bootloader_MODE_SETUP_RCVD;
                        if(bRegTemp == 0)                           /* Check if SETUP bit is not set */
                        {
                            /* Set the Mode Register  */
                            CY_SET_REG8(USB_Bootloader_EP0_CR_PTR, USB_Bootloader_ep0Mode);
                            /* writing check */
                            modifyReg = CY_GET_REG8(USB_Bootloader_EP0_CR_PTR) & USB_Bootloader_MODE_MASK;
                        }
                    }while(modifyReg != USB_Bootloader_ep0Mode);  /* repeat if writing was not successful */
                }
            }
        }
    }
    /* PSoC3 ES1, ES2 RTC ISR PATCH  */
    #if(CY_PSOC3_ES2 && (USB_Bootloader_ep_0__ES2_PATCH))
        USB_Bootloader_ISR_PATCH();
    #endif /* End CY_PSOC3_ES2*/
}


/*******************************************************************************
* Function Name: USB_Bootloader_HandleSetup
********************************************************************************
*
* Summary:
*  This Routine dispatches requests for the four USB request types
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
void USB_Bootloader_HandleSetup(void) 
{
    uint8 requestHandled;

    requestHandled = CY_GET_REG8(USB_Bootloader_EP0_CR_PTR);      /* unlock registers */
    CY_SET_REG8(USB_Bootloader_EP0_CR_PTR, requestHandled);       /* clear setup bit */
    requestHandled = CY_GET_REG8(USB_Bootloader_EP0_CR_PTR);      /* reread register */
    if(requestHandled & USB_Bootloader_MODE_SETUP_RCVD)
    {
        USB_Bootloader_ep0Mode = requestHandled;        /* if SETUP bit set -> exit without modifying the mode */
    }
    else
    {
        /* In case the previous transfer did not complete, close it out */
        USB_Bootloader_UpdateStatusBlock(USB_Bootloader_XFER_PREMATURE);

        switch (CY_GET_REG8(USB_Bootloader_bmRequestType) & USB_Bootloader_RQST_TYPE_MASK)
        {
            case USB_Bootloader_RQST_TYPE_STD:
                requestHandled = USB_Bootloader_HandleStandardRqst();
                break;
            case USB_Bootloader_RQST_TYPE_CLS:
                requestHandled = USB_Bootloader_DispatchClassRqst();
                break;
            case USB_Bootloader_RQST_TYPE_VND:
                requestHandled = USB_Bootloader_HandleVendorRqst();
                break;
            default:
                requestHandled = USB_Bootloader_FALSE;
                break;
        }
        if (requestHandled == USB_Bootloader_FALSE)
        {
            USB_Bootloader_ep0Mode = USB_Bootloader_MODE_STALL_IN_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USB_Bootloader_HandleIN
********************************************************************************
*
* Summary:
*  This routine handles EP0 IN transfers.
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
void USB_Bootloader_HandleIN(void) 
{
    switch (USB_Bootloader_transferState)
    {
        case USB_Bootloader_TRANS_STATE_IDLE:
            break;
        case USB_Bootloader_TRANS_STATE_CONTROL_READ:
            USB_Bootloader_ControlReadDataStage();
            break;
        case USB_Bootloader_TRANS_STATE_CONTROL_WRITE:
            USB_Bootloader_ControlWriteStatusStage();
            break;
        case USB_Bootloader_TRANS_STATE_NO_DATA_CONTROL:
            USB_Bootloader_NoDataControlStatusStage();
            break;
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USB_Bootloader_HandleOUT
********************************************************************************
*
* Summary:
*  This routine handles EP0 OUT transfers.
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
void USB_Bootloader_HandleOUT(void) 
{
    switch (USB_Bootloader_transferState)
    {
        case USB_Bootloader_TRANS_STATE_IDLE:
            break;
        case USB_Bootloader_TRANS_STATE_CONTROL_READ:
            USB_Bootloader_ControlReadStatusStage();
            break;
        case USB_Bootloader_TRANS_STATE_CONTROL_WRITE:
            USB_Bootloader_ControlWriteDataStage();
            break;
        case USB_Bootloader_TRANS_STATE_NO_DATA_CONTROL:
            /* Update the completion block */
            USB_Bootloader_UpdateStatusBlock(USB_Bootloader_XFER_ERROR);
            /* We expect no more data, so stall INs and OUTs */
            USB_Bootloader_ep0Mode = USB_Bootloader_MODE_STALL_IN_OUT;
            break;
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USB_Bootloader_LoadEP0
********************************************************************************
*
* Summary:
*  This routine loads the EP0 data registers for OUT transfers.  It uses the
*  currentTD (previously initialized by the _InitControlWrite function and
*  updated for each OUT transfer, and the bLastPacketSize) to determine how
*  many uint8s to transfer on the current OUT.
*
*  If the number of uint8s remaining is zero and the last transfer was full,
*  we need to send a zero length packet.  Otherwise we send the minimum
*  of the control endpoint size (8) or remaining number of uint8s for the
*  transaction.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USB_Bootloader_transferByteCount - Update the transfer byte count from the
*     last transaction.
*  USB_Bootloader_ep0Count - counts the data loaded to the SIE memory in
*     current packet.
*  USB_Bootloader_lastPacketSize - remebers the USBFS_ep0Count value for the
*     next packet.
*  USB_Bootloader_transferByteCount - suma of the previos bytes transferred
*     on previous packets(suma of USBFS_lastPacketSize)
*  USB_Bootloader_ep0Toggle - inverted
*  USB_Bootloader_ep0Mode  - prepare for mode register content.
*  USB_Bootloader_transferState - set to TRANS_STATE_CONTROL_READ
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_LoadEP0(void) 
{
    /* Update the transfer byte count from the last transaction */
    USB_Bootloader_transferByteCount += USB_Bootloader_lastPacketSize;
    /* Now load the next transaction */
    USB_Bootloader_ep0Count = 0u;
    while ((USB_Bootloader_currentTD.count > 0u) && (USB_Bootloader_ep0Count < 8u))
    {
        USB_Bootloader_EP0_DR0_PTR[USB_Bootloader_ep0Count] = *USB_Bootloader_currentTD.pData++;
        USB_Bootloader_ep0Count++;
        USB_Bootloader_currentTD.count--;
    }
    /* Support zero-length packet*/
    if( (USB_Bootloader_ep0Count > 0u) || (USB_Bootloader_lastPacketSize == 8u) )
    {
        /* Update the data toggle */
        USB_Bootloader_ep0Toggle ^= USB_Bootloader_EP0_CNT_DATA_TOGGLE;
        /* Set the Mode Register  */
        USB_Bootloader_ep0Mode = USB_Bootloader_MODE_ACK_IN_STATUS_OUT;
        /* Update the state (or stay the same) */
        USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_CONTROL_READ;
    }
    else
    {
        /* Expect Status Stage Out */
        USB_Bootloader_ep0Mode = USB_Bootloader_MODE_STATUS_OUT_ONLY;
        /* Update the state (or stay the same) */
        USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_CONTROL_READ;
    }

    /* Save the packet size for next time */
    USB_Bootloader_lastPacketSize = USB_Bootloader_ep0Count;
}


/*******************************************************************************
* Function Name: USB_Bootloader_InitControlRead
********************************************************************************
*
* Summary:
*  Initialize a control read transaction, usable to send data to the host.
*  The following global variables shold be initialized before this function
*  called. To send zero length packet use InitZeroLengthControlTransfer
*  function.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USB_Bootloader_currentTD.count - counts of data to be sent.
*  USB_Bootloader_currentTD.pData - data pointer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_InitControlRead(void) 
{
    uint16 xferCount;
    if(USB_Bootloader_currentTD.count == 0)
    {
        USB_Bootloader_InitZeroLengthControlTransfer();
    }
    else
    {
        /* Set up the state machine */
        USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_CONTROL_READ;
        /* Set the toggle, it gets updated in LoadEP */
        USB_Bootloader_ep0Toggle = 0u;
        /* Initialize the Status Block */
        USB_Bootloader_InitializeStatusBlock();
        xferCount = (((uint16)CY_GET_REG8(USB_Bootloader_lengthHi) << 8u) | (CY_GET_REG8(USB_Bootloader_lengthLo)));

        if (USB_Bootloader_currentTD.count > xferCount)
        {
            USB_Bootloader_currentTD.count = xferCount;
        }
        USB_Bootloader_LoadEP0();
    }

    return(USB_Bootloader_TRUE);
}


/*******************************************************************************
* Function Name: USB_Bootloader_InitZeroLengthControlTransfer
********************************************************************************
*
* Summary:
*  Initialize a zero length data IN transfer.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USB_Bootloader_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USB_Bootloader_ep0Mode  - prepare for mode register content.
*  USB_Bootloader_transferState - set to TRANS_STATE_CONTROL_READ
*  USB_Bootloader_ep0Count - cleared, means the zero-length packet.
*  USB_Bootloader_lastPacketSize - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_InitZeroLengthControlTransfer(void)
                                                
{
    /* Update the state */
    USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_CONTROL_READ;
    /* Set the data toggle */
    USB_Bootloader_ep0Toggle = USB_Bootloader_EP0_CNT_DATA_TOGGLE;
    /* Set the Mode Register  */
    USB_Bootloader_ep0Mode = USB_Bootloader_MODE_ACK_IN_STATUS_OUT;
    /* Save the packet size for next time */
    USB_Bootloader_lastPacketSize = 0u;
    USB_Bootloader_ep0Count = 0u;

    return(USB_Bootloader_TRUE);
}


/*******************************************************************************
* Function Name: USB_Bootloader_ControlReadDataStage
********************************************************************************
*
* Summary:
*  Handle the Data Stage of a control read transfer.
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
void USB_Bootloader_ControlReadDataStage(void) 

{
    USB_Bootloader_LoadEP0();
}


/*******************************************************************************
* Function Name: USB_Bootloader_ControlReadStatusStage
********************************************************************************
*
* Summary:
*  Handle the Status Stage of a control read transfer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USB_Bootloader_USBFS_transferByteCount - updated with last packet size.
*  USB_Bootloader_transferState - set to TRANS_STATE_IDLE.
*  USB_Bootloader_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_ControlReadStatusStage(void) 
{
    /* Update the transfer byte count */
    USB_Bootloader_transferByteCount += USB_Bootloader_lastPacketSize;
    /* Go Idle */
    USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_IDLE;
    /* Update the completion block */
    USB_Bootloader_UpdateStatusBlock(USB_Bootloader_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USB_Bootloader_ep0Mode =  USB_Bootloader_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USB_Bootloader_InitControlWrite
********************************************************************************
*
* Summary:
*  Initialize a control write transaction
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USB_Bootloader_USBFS_transferState - set to TRANS_STATE_CONTROL_WRITE
*  USB_Bootloader_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USB_Bootloader_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_InitControlWrite(void) 
{
    uint16 xferCount;

    /* Set up the state machine */
    USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_CONTROL_WRITE;
    /* This migh not be necessary */
    USB_Bootloader_ep0Toggle = USB_Bootloader_EP0_CNT_DATA_TOGGLE;
    /* Initialize the Status Block */
    USB_Bootloader_InitializeStatusBlock();

    xferCount = (((uint16)CY_GET_REG8(USB_Bootloader_lengthHi) << 8u) | (CY_GET_REG8(USB_Bootloader_lengthLo)));

    if (USB_Bootloader_currentTD.count > xferCount)
    {
        USB_Bootloader_currentTD.count = xferCount;
    }

    /* Expect Data or Status Stage */
    USB_Bootloader_ep0Mode = USB_Bootloader_MODE_ACK_OUT_STATUS_IN;

    return(USB_Bootloader_TRUE);
}


/*******************************************************************************
* Function Name: USB_Bootloader_ControlWriteDataStage
********************************************************************************
*
* Summary:
*  Handle the Data Stage of a control write transfer
*       1. Get the data (We assume the destination was validated previously)
*       2. Update the count and data toggle
*       3. Update the mode register for the next transaction
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USB_Bootloader_transferByteCount - Update the transfer byte count from the
*    last transaction.
*  USB_Bootloader_ep0Count - counts the data loaded from the SIE memory
*    in current packet.
*  USB_Bootloader_transferByteCount - suma of the previos bytes transferred
*    on previous packets(suma of USBFS_lastPacketSize)
*  USB_Bootloader_ep0Toggle - inverted
*  USB_Bootloader_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_ControlWriteDataStage(void) 
{
    uint8 *pReg = (uint8 *)USB_Bootloader_EP0_DR0_PTR;

    USB_Bootloader_ep0Count = (CY_GET_REG8(USB_Bootloader_EP0_CNT_PTR ) & 0x0Fu) - 2u;

    USB_Bootloader_transferByteCount += USB_Bootloader_ep0Count;

    while ((USB_Bootloader_currentTD.count > 0u) && (USB_Bootloader_ep0Count > 0u))
    {
        *USB_Bootloader_currentTD.pData++ = CY_GET_REG8(pReg++);
        USB_Bootloader_ep0Count--;
        USB_Bootloader_currentTD.count--;
    }
    /* Update the data toggle */
    USB_Bootloader_ep0Toggle ^= USB_Bootloader_EP0_CNT_DATA_TOGGLE;
    /* Expect Data or Status Stage */
    USB_Bootloader_ep0Mode = USB_Bootloader_MODE_ACK_OUT_STATUS_IN;
}


/*******************************************************************************
* Function Name: USB_Bootloader_ControlWriteStatusStage
********************************************************************************
*
* Summary:
*  Handle the Status Stage of a control write transfer
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USB_Bootloader_transferState - set to TRANS_STATE_IDLE.
*  USB_Bootloader_USBFS_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_ControlWriteStatusStage(void) 
{
    /* Go Idle */
    USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_IDLE;
    /* Update the completion block */
    USB_Bootloader_UpdateStatusBlock(USB_Bootloader_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USB_Bootloader_ep0Mode = USB_Bootloader_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USB_Bootloader_InitNoDataControlTransfer
********************************************************************************
*
* Summary:
*  Initialize a no data control transfer
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USB_Bootloader_transferState - set to TRANS_STATE_NO_DATA_CONTROL.
*  USB_Bootloader_ep0Mode  - set to MODE_STATUS_IN_ONLY.
*  USB_Bootloader_ep0Count - cleared.
*  USB_Bootloader_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USB_Bootloader_InitNoDataControlTransfer(void) 
{
    USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_NO_DATA_CONTROL;
    USB_Bootloader_ep0Mode = USB_Bootloader_MODE_STATUS_IN_ONLY;
    USB_Bootloader_ep0Toggle = USB_Bootloader_EP0_CNT_DATA_TOGGLE;
    USB_Bootloader_ep0Count = 0u;

    return(USB_Bootloader_TRUE);
}


/*******************************************************************************
* Function Name: USB_Bootloader_NoDataControlStatusStage
********************************************************************************
* Summary:
*  Handle the Status Stage of a no data control transfer.
*
*  SET_ADDRESS is special, since we need to receive the status stage with
*  the old address.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USB_Bootloader_transferState - set to TRANS_STATE_IDLE.
*  USB_Bootloader_ep0Mode  - set to MODE_STALL_IN_OUT.
*  USB_Bootloader_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USB_Bootloader_deviceAddress - used to set new address and cleared
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_NoDataControlStatusStage(void) 
{
    /* Change the USB address register if we got a SET_ADDRESS. */
    if (USB_Bootloader_deviceAddress != 0u)
    {
        CY_SET_REG8(USB_Bootloader_CR0_PTR, USB_Bootloader_deviceAddress | USB_Bootloader_CR0_ENABLE);
        USB_Bootloader_deviceAddress = 0u;
    }
    /* Go Idle */
    USB_Bootloader_transferState = USB_Bootloader_TRANS_STATE_IDLE;
    /* Update the completion block */
    USB_Bootloader_UpdateStatusBlock(USB_Bootloader_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USB_Bootloader_ep0Mode = USB_Bootloader_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USB_Bootloader_UpdateStatusBlock
********************************************************************************
*
* Summary:
*  Update the Completion Status Block for a Request.  The block is updated
*  with the completion code the USB_Bootloader_transferByteCount.  The
*  StatusBlock Pointer is set to NULL.
*
* Parameters:
*  completionCode - status.
*
* Return:
*  None.
*
* Global variables:
*  USB_Bootloader_currentTD.pStatusBlock->status - updated by the 
*    completionCode parameter.
*  USB_Bootloader_currentTD.pStatusBlock->length - updated.
*  USB_Bootloader_currentTD.pStatusBlock - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_UpdateStatusBlock(uint8 completionCode) 
{
    if (USB_Bootloader_currentTD.pStatusBlock != NULL)
    {
        USB_Bootloader_currentTD.pStatusBlock->status = completionCode;
        USB_Bootloader_currentTD.pStatusBlock->length = USB_Bootloader_transferByteCount;
        USB_Bootloader_currentTD.pStatusBlock = NULL;
    }
}


/*******************************************************************************
* Function Name: USB_Bootloader_InitializeStatusBlock
********************************************************************************
*
* Summary:
*  Initialize the Completion Status Block for a Request.  The completion
*  code is set to USB_XFER_IDLE.
*
*  Also, initializes USB_Bootloader_transferByteCount.  Save some space,
*  this is the only consumer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USB_Bootloader_currentTD.pStatusBlock->status - set to XFER_IDLE.
*  USB_Bootloader_currentTD.pStatusBlock->length - cleared.
*  USB_Bootloader_transferByteCount - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Bootloader_InitializeStatusBlock(void) 
{
    USB_Bootloader_transferByteCount = 0u;
    if(USB_Bootloader_currentTD.pStatusBlock != NULL)
    {
        USB_Bootloader_currentTD.pStatusBlock->status = USB_Bootloader_XFER_IDLE;
        USB_Bootloader_currentTD.pStatusBlock->length = 0u;
    }
}


/* [] END OF FILE */
