/*******************************************************************************
* File Name: USB_Bootloader_Dp.h  
* Version 1.60
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PINS_USB_Bootloader_Dp_H) /* Pins USB_Bootloader_Dp_H */
#define CY_PINS_USB_Bootloader_Dp_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "USB_Bootloader_Dp_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    USB_Bootloader_Dp_Write(uint8 value) ;
void    USB_Bootloader_Dp_SetDriveMode(uint8 mode) ;
uint8   USB_Bootloader_Dp_ReadDataReg(void) ;
uint8   USB_Bootloader_Dp_Read(void) ;
uint8   USB_Bootloader_Dp_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define USB_Bootloader_Dp_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define USB_Bootloader_Dp_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define USB_Bootloader_Dp_DM_RES_UP          PIN_DM_RES_UP
#define USB_Bootloader_Dp_DM_RES_DWN         PIN_DM_RES_DWN
#define USB_Bootloader_Dp_DM_OD_LO           PIN_DM_OD_LO
#define USB_Bootloader_Dp_DM_OD_HI           PIN_DM_OD_HI
#define USB_Bootloader_Dp_DM_STRONG          PIN_DM_STRONG
#define USB_Bootloader_Dp_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define USB_Bootloader_Dp_MASK               USB_Bootloader_Dp__MASK
#define USB_Bootloader_Dp_SHIFT              USB_Bootloader_Dp__SHIFT
#define USB_Bootloader_Dp_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define USB_Bootloader_Dp_PS                     (* (reg8 *) USB_Bootloader_Dp__PS)
/* Data Register */
#define USB_Bootloader_Dp_DR                     (* (reg8 *) USB_Bootloader_Dp__DR)
/* Port Number */
#define USB_Bootloader_Dp_PRT_NUM                (* (reg8 *) USB_Bootloader_Dp__PRT) 
/* Connect to Analog Globals */                                                  
#define USB_Bootloader_Dp_AG                     (* (reg8 *) USB_Bootloader_Dp__AG)                       
/* Analog MUX bux enable */
#define USB_Bootloader_Dp_AMUX                   (* (reg8 *) USB_Bootloader_Dp__AMUX) 
/* Bidirectional Enable */                                                        
#define USB_Bootloader_Dp_BIE                    (* (reg8 *) USB_Bootloader_Dp__BIE)
/* Bit-mask for Aliased Register Access */
#define USB_Bootloader_Dp_BIT_MASK               (* (reg8 *) USB_Bootloader_Dp__BIT_MASK)
/* Bypass Enable */
#define USB_Bootloader_Dp_BYP                    (* (reg8 *) USB_Bootloader_Dp__BYP)
/* Port wide control signals */                                                   
#define USB_Bootloader_Dp_CTL                    (* (reg8 *) USB_Bootloader_Dp__CTL)
/* Drive Modes */
#define USB_Bootloader_Dp_DM0                    (* (reg8 *) USB_Bootloader_Dp__DM0) 
#define USB_Bootloader_Dp_DM1                    (* (reg8 *) USB_Bootloader_Dp__DM1)
#define USB_Bootloader_Dp_DM2                    (* (reg8 *) USB_Bootloader_Dp__DM2) 
/* Input Buffer Disable Override */
#define USB_Bootloader_Dp_INP_DIS                (* (reg8 *) USB_Bootloader_Dp__INP_DIS)
/* LCD Common or Segment Drive */
#define USB_Bootloader_Dp_LCD_COM_SEG            (* (reg8 *) USB_Bootloader_Dp__LCD_COM_SEG)
/* Enable Segment LCD */
#define USB_Bootloader_Dp_LCD_EN                 (* (reg8 *) USB_Bootloader_Dp__LCD_EN)
/* Slew Rate Control */
#define USB_Bootloader_Dp_SLW                    (* (reg8 *) USB_Bootloader_Dp__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define USB_Bootloader_Dp_PRTDSI__CAPS_SEL       (* (reg8 *) USB_Bootloader_Dp__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define USB_Bootloader_Dp_PRTDSI__DBL_SYNC_IN    (* (reg8 *) USB_Bootloader_Dp__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define USB_Bootloader_Dp_PRTDSI__OE_SEL0        (* (reg8 *) USB_Bootloader_Dp__PRTDSI__OE_SEL0) 
#define USB_Bootloader_Dp_PRTDSI__OE_SEL1        (* (reg8 *) USB_Bootloader_Dp__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define USB_Bootloader_Dp_PRTDSI__OUT_SEL0       (* (reg8 *) USB_Bootloader_Dp__PRTDSI__OUT_SEL0) 
#define USB_Bootloader_Dp_PRTDSI__OUT_SEL1       (* (reg8 *) USB_Bootloader_Dp__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define USB_Bootloader_Dp_PRTDSI__SYNC_OUT       (* (reg8 *) USB_Bootloader_Dp__PRTDSI__SYNC_OUT) 


#if defined(USB_Bootloader_Dp__INTSTAT)  /* Interrupt Registers */

    #define USB_Bootloader_Dp_INTSTAT                (* (reg8 *) USB_Bootloader_Dp__INTSTAT)
    #define USB_Bootloader_Dp_SNAP                   (* (reg8 *) USB_Bootloader_Dp__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins USB_Bootloader_Dp_H */


/* [] END OF FILE */
