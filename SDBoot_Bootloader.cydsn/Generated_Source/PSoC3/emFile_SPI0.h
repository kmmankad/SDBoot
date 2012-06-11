/*******************************************************************************
* File Name: emFile_SPI0.h
* Version 2.20
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_emFile_SPI0_H)
#define CY_SPIM_emFile_SPI0_H

#include "cyfitter.h"

/* PSoC3 ES2 or earlier*/
#define emFile_SPI0_PSOC3_ES2   ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A) && \
                     			      (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))
                     
/* PSoC5 ES1 or earlier */
#define emFile_SPI0_PSOC5_ES1   ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A) && \
                     		    	  (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
	
#ifdef emFile_SPI0_TxInternalInterrupt__ES2_PATCH
	#if(emFile_SPI0_PSOC3_ES2 && emFile_SPI0_TxInternalInterrupt__ES2_PATCH)
		#include <intrins.h>
        #define emFile_SPI0_TX_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	#endif /* End PSOC3_ES2 */
#endif /* emFile_SPI0_TxInternalInterrupt__ES2_PATCH */

#ifdef emFile_SPI0_RxInternalInterrupt__ES2_PATCH  
	#if(emFile_SPI0_PSOC3_ES2 && emFile_SPI0_RxInternalInterrupt__ES2_PATCH)
		#include <intrins.h>
        #define emFile_SPI0_RX_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	#endif /* End PSOC3_ES2 */
#endif /* emFile_SPI0_RxInternalInterrupt__ES2_PATCH */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define emFile_SPI0_DataWidth                  (8u)
#define emFile_SPI0_InternalClockUsed          (0u)
#define emFile_SPI0_InternalTxInterruptEnabled (0u)
#define emFile_SPI0_InternalRxInterruptEnabled (0u)
#define emFile_SPI0_ModeUseZero                (1u)
#define emFile_SPI0_BidirectionalMode          (0u)
#define emFile_SPI0_Mode                       (1u)

/* Following definitions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects 
*/
#define emFile_SPI0_DATAWIDHT                (emFile_SPI0_DataWidth)
#define emFile_SPI0_InternalInterruptEnabled (0u)


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct _emFile_SPI0_backupStruct
{
    uint8 enableState;
    uint8 cntrPeriod;
    
    #if(emFile_SPI0_PSOC3_ES2 || emFile_SPI0_PSOC5_ES1) /* PSoC3 ES2 or earlier, PSoC5 ES1 */
    
        uint8 saveSrTxIntMask;
        uint8 saveSrRxIntMask;
        
    #endif /* End emFile_SPI0_PSOC3_ES2 || emFile_SPI0_PSOC5_ES1 */

} emFile_SPI0_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  emFile_SPI0_Init(void);
void  emFile_SPI0_Enable(void) ;
void  emFile_SPI0_Start(void);
void  emFile_SPI0_Stop(void) ;
void  emFile_SPI0_EnableTxInt(void) ;
void  emFile_SPI0_EnableRxInt(void) ;
void  emFile_SPI0_DisableTxInt(void) ;
void  emFile_SPI0_DisableRxInt(void) ;
void  emFile_SPI0_SetTxInterruptMode(uint8 intSource) ;
void  emFile_SPI0_SetRxInterruptMode(uint8 intSource) ;
uint8 emFile_SPI0_ReadTxStatus(void);
uint8 emFile_SPI0_ReadRxStatus(void);
void  emFile_SPI0_WriteTxData(uint8 txData);
uint8 emFile_SPI0_ReadRxData(void);
uint8 emFile_SPI0_GetRxBufferSize(void) ;
uint8 emFile_SPI0_GetTxBufferSize(void) ;
void  emFile_SPI0_ClearRxBuffer(void);
void  emFile_SPI0_ClearTxBuffer(void);
void  emFile_SPI0_ClearFIFO(void) ;
void  emFile_SPI0_PutArray(uint8 *buffer, uint8 byteCount);
void  emFile_SPI0_Sleep(void);
void  emFile_SPI0_Wakeup(void);
void  emFile_SPI0_SaveConfig(void);
void  emFile_SPI0_RestoreConfig(void) ;

#if (emFile_SPI0_BidirectionalMode)

    void  emFile_SPI0_TxEnable(void) ;
    void  emFile_SPI0_TxDisable(void) ;

#endif /* emFile_SPI0_BidirectionalMode == 1u*/

CY_ISR_PROTO(emFile_SPI0_TX_ISR);
CY_ISR_PROTO(emFile_SPI0_RX_ISR);

/* Macros for getting software status of SPIM Statusi Register */
#define emFile_SPI0_GET_STATUS_TX(swTxSts) (uint8)(emFile_SPI0_TX_STATUS_REG | \
                                                       (swTxSts & emFile_SPI0_TX_STS_CLR_ON_RD_BYTES_MASK))
#define emFile_SPI0_GET_STATUS_RX(swRxSts) (uint8)(emFile_SPI0_RX_STATUS_REG | \
                                                       (swRxSts & emFile_SPI0_RX_STS_CLR_ON_RD_BYTES_MASK))                                                  

/* Following definitions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects 
*/
#define emFile_SPI0_WriteByte (emFile_SPI0_WriteTxData)
#define emFile_SPI0_ReadByte  (emFile_SPI0_ReadRxData)
void  emFile_SPI0_SetInterruptMode(uint8 intSource) ;
uint8 emFile_SPI0_ReadStatus(void);
void  emFile_SPI0_EnableInt(void) ;
void  emFile_SPI0_DisableInt(void) ;


/***************************************
*           API Constants
***************************************/

#define emFile_SPI0_TX_ISR_NUMBER     (emFile_SPI0_TxInternalInterrupt__INTC_NUMBER)
#define emFile_SPI0_RX_ISR_NUMBER     (emFile_SPI0_RxInternalInterrupt__INTC_NUMBER)
#define emFile_SPI0_TX_ISR_PRIORITY   (emFile_SPI0_TxInternalInterrupt__INTC_PRIOR_NUM)
#define emFile_SPI0_RX_ISR_PRIORITY   (emFile_SPI0_RxInternalInterrupt__INTC_PRIOR_NUM)


/***************************************
*    Initial Parameter Constants
***************************************/
                                               
#define emFile_SPI0_INT_ON_SPI_DONE    (0u << emFile_SPI0_STS_SPI_DONE_SHIFT)
#define emFile_SPI0_INT_ON_TX_EMPTY    (0u << emFile_SPI0_STS_TX_FIFO_EMPTY_SHIFT)
#define emFile_SPI0_INT_ON_TX_NOT_FULL (0u << emFile_SPI0_STS_TX_FIFO_NOT_FULL_SHIFT)
#define emFile_SPI0_INT_ON_BYTE_COMP   (0u << emFile_SPI0_STS_BYTE_COMPLETE_SHIFT)
#define emFile_SPI0_INT_ON_SPI_IDLE    (0u << emFile_SPI0_STS_SPI_IDLE_SHIFT)

#define emFile_SPI0_TX_INIT_INTERRUPTS_MASK  (emFile_SPI0_INT_ON_SPI_DONE | \
                                            emFile_SPI0_INT_ON_TX_EMPTY | emFile_SPI0_INT_ON_TX_NOT_FULL | \
                                            emFile_SPI0_INT_ON_BYTE_COMP | emFile_SPI0_INT_ON_SPI_IDLE)
            
#define emFile_SPI0_INT_ON_RX_FULL      (0u << emFile_SPI0_STS_RX_FIFO_FULL_SHIFT)
#define emFile_SPI0_INT_ON_RX_NOT_EMPTY (0u << emFile_SPI0_STS_RX_FIFO_NOT_EMPTY_SHIFT)
#define emFile_SPI0_INT_ON_RX_OVER      (0u << emFile_SPI0_STS_RX_FIFO_OVERRUN_SHIFT)

#define emFile_SPI0_RX_INIT_INTERRUPTS_MASK  (emFile_SPI0_INT_ON_RX_FULL | \
                                            emFile_SPI0_INT_ON_RX_NOT_EMPTY | emFile_SPI0_INT_ON_RX_OVER)
                                               
#define emFile_SPI0_BITCTR_INIT           ( (emFile_SPI0_DataWidth << 1u) - 1u)

#define emFile_SPI0_TXBUFFERSIZE          (4u)
#define emFile_SPI0_RXBUFFERSIZE          (4u)

/* Following definitions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects 
*/
#define emFile_SPI0_INIT_INTERRUPTS_MASK  (emFile_SPI0_INT_ON_SPI_DONE | emFile_SPI0_INT_ON_TX_EMPTY | \
                                            emFile_SPI0_INT_ON_TX_NOT_FULL | emFile_SPI0_INT_ON_RX_FULL | \
                                            emFile_SPI0_INT_ON_RX_NOT_EMPTY | emFile_SPI0_INT_ON_RX_OVER | \
                                            emFile_SPI0_INT_ON_BYTE_COMP)


/***************************************
*             Registers
***************************************/

#define emFile_SPI0_TXDATA_REG         (* (reg8 *) \
        emFile_SPI0_BSPIM_sR8_Dp_u0__F0_REG)
#define emFile_SPI0_TXDATA_PTR         (  (reg8 *) \
        emFile_SPI0_BSPIM_sR8_Dp_u0__F0_REG)
#define emFile_SPI0_RXDATA_REG         (* (reg8 *) \
        emFile_SPI0_BSPIM_sR8_Dp_u0__F1_REG)
#define emFile_SPI0_RXDATA_PTR         (  (reg8 *) \
        emFile_SPI0_BSPIM_sR8_Dp_u0__F1_REG)

#define emFile_SPI0_AUX_CONTROL_DP0_REG (* (reg8 *) \
        emFile_SPI0_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define emFile_SPI0_AUX_CONTROL_DP0_PTR (  (reg8 *) \
        emFile_SPI0_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)        

#if (emFile_SPI0_DataWidth > 8u)

    #define emFile_SPI0_AUX_CONTROL_DP1_REG  (* (reg8 *) \
            emFile_SPI0_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define emFile_SPI0_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
            emFile_SPI0_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* emFile_SPI0_DataWidth > 8u */

#define emFile_SPI0_COUNTER_PERIOD_REG  (* (reg8 *)  emFile_SPI0_BSPIM_BitCounter__PERIOD_REG)
#define emFile_SPI0_COUNTER_PERIOD_PTR  (  (reg8 *)  emFile_SPI0_BSPIM_BitCounter__PERIOD_REG)
#define emFile_SPI0_COUNTER_CONTROL_REG (* (reg8 *)  emFile_SPI0_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define emFile_SPI0_COUNTER_CONTROL_PTR (  (reg8 *)  emFile_SPI0_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define emFile_SPI0_TX_STATUS_REG       (* (reg8 *)  emFile_SPI0_BSPIM_TxStsReg__STATUS_REG)
#define emFile_SPI0_TX_STATUS_PTR       (  (reg8 *)  emFile_SPI0_BSPIM_TxStsReg__STATUS_REG)
#define emFile_SPI0_RX_STATUS_REG       (* (reg8 *)  emFile_SPI0_BSPIM_RxStsReg__STATUS_REG)
#define emFile_SPI0_RX_STATUS_PTR       (  (reg8 *)  emFile_SPI0_BSPIM_RxStsReg__STATUS_REG)

#define emFile_SPI0_CONTROL_REG         (* (reg8 *)  \
        emFile_SPI0_BSPIM_BidirMode_SyncCtl_CtrlReg__CONTROL_REG)
#define emFile_SPI0_CONTROL_PTR         (  (reg8 *)  \
        emFile_SPI0_BSPIM_BidirMode_SyncCtl_CtrlReg__CONTROL_REG)
        
#define emFile_SPI0_TX_STATUS_MASK_REG  (* (reg8 *)  emFile_SPI0_BSPIM_TxStsReg__MASK_REG)
#define emFile_SPI0_TX_STATUS_MASK_PTR  (  (reg8 *)  emFile_SPI0_BSPIM_TxStsReg__MASK_REG)
#define emFile_SPI0_RX_STATUS_MASK_REG  (* (reg8 *)  emFile_SPI0_BSPIM_RxStsReg__MASK_REG)
#define emFile_SPI0_RX_STATUS_MASK_PTR  (  (reg8 *)  emFile_SPI0_BSPIM_RxStsReg__MASK_REG)

#define emFile_SPI0_TX_STATUS_ACTL_REG  (* (reg8 *)  emFile_SPI0_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define emFile_SPI0_TX_STATUS_ACTL_PTR  (  (reg8 *)  emFile_SPI0_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define emFile_SPI0_RX_STATUS_ACTL_REG  (* (reg8 *)  emFile_SPI0_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define emFile_SPI0_RX_STATUS_ACTL_PTR  (  (reg8 *)  emFile_SPI0_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

/* Obsolete register names. Not to be used in new designs */
#define emFile_SPI0_TXDATA                 (emFile_SPI0_TXDATA_REG)
#define emFile_SPI0_RXDATA                 (emFile_SPI0_RXDATA_REG)
#define emFile_SPI0_AUX_CONTROLDP0         (emFile_SPI0_AUX_CONTROL_DP0_REG)
#define emFile_SPI0_TXBUFFERREAD           (emFile_SPI0_txBufferRead)
#define emFile_SPI0_TXBUFFERWRITE          (emFile_SPI0_txBufferWrite)
#define emFile_SPI0_RXBUFFERREAD           (emFile_SPI0_rxBufferRead)
#define emFile_SPI0_RXBUFFERWRITE          (emFile_SPI0_rxBufferWrite)

#if(emFile_SPI0_DataWidth > 8u)    
    #define emFile_SPI0_AUX_CONTROLDP1     (emFile_SPI0_AUX_CONTROL_DP1_REG)    
#endif /* emFile_SPI0_DataWidth > 8u */
    
#define emFile_SPI0_COUNTER_PERIOD         (emFile_SPI0_COUNTER_PERIOD_REG)
#define emFile_SPI0_COUNTER_CONTROL        (emFile_SPI0_COUNTER_CONTROL_REG)
#define emFile_SPI0_STATUS                 (emFile_SPI0_TX_STATUS_REG)
#define emFile_SPI0_CONTROL                (emFile_SPI0_CONTROL_REG)
#define emFile_SPI0_STATUS_MASK            (emFile_SPI0_TX_STATUS_MASK_REG)
#define emFile_SPI0_STATUS_ACTL            (emFile_SPI0_TX_STATUS_ACTL_REG)


/***************************************
*       Register Constants 
***************************************/

/* Status Register Definitions */
#define emFile_SPI0_STS_SPI_DONE_SHIFT          (0x00u)
#define emFile_SPI0_STS_TX_FIFO_EMPTY_SHIFT     (0x01u)
#define emFile_SPI0_STS_TX_FIFO_NOT_FULL_SHIFT  (0x02u)
#define emFile_SPI0_STS_BYTE_COMPLETE_SHIFT     (0x03u)
#define emFile_SPI0_STS_SPI_IDLE_SHIFT          (0x04u)
#define emFile_SPI0_STS_RX_FIFO_FULL_SHIFT      (0x04u)
#define emFile_SPI0_STS_RX_FIFO_NOT_EMPTY_SHIFT (0x05u)
#define emFile_SPI0_STS_RX_FIFO_OVERRUN_SHIFT   (0x06u)

#define emFile_SPI0_STS_SPI_DONE                (0x01u << emFile_SPI0_STS_SPI_DONE_SHIFT)        
#define emFile_SPI0_STS_TX_FIFO_EMPTY           (0x01u << emFile_SPI0_STS_TX_FIFO_EMPTY_SHIFT)    
#define emFile_SPI0_STS_TX_FIFO_NOT_FULL        (0x01u << emFile_SPI0_STS_TX_FIFO_NOT_FULL_SHIFT)    
#define emFile_SPI0_STS_SPI_IDLE                (0x01u << emFile_SPI0_STS_SPI_IDLE_SHIFT)
#define emFile_SPI0_STS_RX_FIFO_FULL            (0x01u << emFile_SPI0_STS_RX_FIFO_FULL_SHIFT)    
#define emFile_SPI0_STS_RX_FIFO_NOT_EMPTY       (0x01u << emFile_SPI0_STS_RX_FIFO_NOT_EMPTY_SHIFT)    
#define emFile_SPI0_STS_RX_FIFO_OVERRUN         (0x01u << emFile_SPI0_STS_RX_FIFO_OVERRUN_SHIFT)  
#define emFile_SPI0_STS_BYTE_COMPLETE           (0x01u << emFile_SPI0_STS_BYTE_COMPLETE_SHIFT)

#define emFile_SPI0_TX_STS_CLR_ON_RD_BYTES_MASK (0x09u)
#define emFile_SPI0_RX_STS_CLR_ON_RD_BYTES_MASK (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define emFile_SPI0_INT_ENABLE                  (0x10u)
#define emFile_SPI0_FIFO_CLR                    (0x03u)
                                                                 
/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define emFile_SPI0_CNTR_ENABLE                 (0x20u)   
                                                                  
/* Bi-Directional mode control bit */
#define emFile_SPI0_CTRL_TX_SIGNAL_EN           (0x01u)

/* Datapath Auxillary Control Register definitions */
#define emFile_SPI0_AUX_CTRL_FIFO0_CLR          (0x01u)
#define emFile_SPI0_AUX_CTRL_FIFO1_CLR          (0x02u)
#define emFile_SPI0_AUX_CTRL_FIFO0_LVL          (0x04u)
#define emFile_SPI0_AUX_CTRL_FIFO1_LVL          (0x08u)
#define emFile_SPI0_STATUS_ACTL_INT_EN_MASK     (0x10u)

#endif  /* CY_SPIM_emFile_SPI0_H */


/* [] END OF FILE */
