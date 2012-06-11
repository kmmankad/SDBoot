/*******************************************************************************
* File Name: USB_Bootloader_Dp.c  
* Version 1.60
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "USB_Bootloader_Dp.h"


/*******************************************************************************
* Function Name: USB_Bootloader_Dp_Write
********************************************************************************
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  void 
*  
*******************************************************************************/
void USB_Bootloader_Dp_Write(uint8 value) 
{
    uint8 staticBits = USB_Bootloader_Dp_DR & ~USB_Bootloader_Dp_MASK;
    USB_Bootloader_Dp_DR = staticBits | ((value << USB_Bootloader_Dp_SHIFT) & USB_Bootloader_Dp_MASK);
}


/*******************************************************************************
* Function Name: USB_Bootloader_Dp_SetDriveMode
********************************************************************************
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  void
*
*******************************************************************************/
void USB_Bootloader_Dp_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(USB_Bootloader_Dp_0, mode);
}


/*******************************************************************************
* Function Name: USB_Bootloader_Dp_Read
********************************************************************************
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro USB_Bootloader_Dp_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 USB_Bootloader_Dp_Read(void) 
{
    return (USB_Bootloader_Dp_PS & USB_Bootloader_Dp_MASK) >> USB_Bootloader_Dp_SHIFT;
}


/*******************************************************************************
* Function Name: USB_Bootloader_Dp_ReadDataReg
********************************************************************************
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 USB_Bootloader_Dp_ReadDataReg(void) 
{
    return (USB_Bootloader_Dp_DR & USB_Bootloader_Dp_MASK) >> USB_Bootloader_Dp_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(USB_Bootloader_Dp_INTSTAT) 

    /*******************************************************************************
    * Function Name: USB_Bootloader_Dp_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  void 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 USB_Bootloader_Dp_ClearInterrupt(void) 
    {
        return (USB_Bootloader_Dp_INTSTAT & USB_Bootloader_Dp_MASK) >> USB_Bootloader_Dp_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
