/*******************************************************************************
* File Name: emFile_SPI0_CS.c  
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
#include "emFile_SPI0_CS.h"


/*******************************************************************************
* Function Name: emFile_SPI0_CS_Write
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
void emFile_SPI0_CS_Write(uint8 value) 
{
    uint8 staticBits = emFile_SPI0_CS_DR & ~emFile_SPI0_CS_MASK;
    emFile_SPI0_CS_DR = staticBits | ((value << emFile_SPI0_CS_SHIFT) & emFile_SPI0_CS_MASK);
}


/*******************************************************************************
* Function Name: emFile_SPI0_CS_SetDriveMode
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
void emFile_SPI0_CS_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(emFile_SPI0_CS_0, mode);
}


/*******************************************************************************
* Function Name: emFile_SPI0_CS_Read
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
*  Macro emFile_SPI0_CS_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 emFile_SPI0_CS_Read(void) 
{
    return (emFile_SPI0_CS_PS & emFile_SPI0_CS_MASK) >> emFile_SPI0_CS_SHIFT;
}


/*******************************************************************************
* Function Name: emFile_SPI0_CS_ReadDataReg
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
uint8 emFile_SPI0_CS_ReadDataReg(void) 
{
    return (emFile_SPI0_CS_DR & emFile_SPI0_CS_MASK) >> emFile_SPI0_CS_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(emFile_SPI0_CS_INTSTAT) 

    /*******************************************************************************
    * Function Name: emFile_SPI0_CS_ClearInterrupt
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
    uint8 emFile_SPI0_CS_ClearInterrupt(void) 
    {
        return (emFile_SPI0_CS_INTSTAT & emFile_SPI0_CS_MASK) >> emFile_SPI0_CS_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
