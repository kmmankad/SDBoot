#ifndef INCLUDED_CYFITTER_H
#define INCLUDED_CYFITTER_H
#include <cydevice.h>
#include <cydevice_trm.h>

/* USB_Bootloader_bus_reset */
#define USB_Bootloader_bus_reset__ES2_PATCH 0
#define USB_Bootloader_bus_reset__INTC_CLR_EN_REG CYREG_INTC_CLR_EN2
#define USB_Bootloader_bus_reset__INTC_CLR_PD_REG CYREG_INTC_CLR_PD2
#define USB_Bootloader_bus_reset__INTC_MASK 0x80u
#define USB_Bootloader_bus_reset__INTC_NUMBER 23
#define USB_Bootloader_bus_reset__INTC_PRIOR_NUM 7
#define USB_Bootloader_bus_reset__INTC_PRIOR_REG CYREG_INTC_PRIOR23
#define USB_Bootloader_bus_reset__INTC_SET_EN_REG CYREG_INTC_SET_EN2
#define USB_Bootloader_bus_reset__INTC_SET_PD_REG CYREG_INTC_SET_PD2
#define USB_Bootloader_bus_reset__INTC_VECT (CYREG_INTC_VECT_MBASE+0x2Eu)

/* USB_Bootloader_arb_int */
#define USB_Bootloader_arb_int__ES2_PATCH 0
#define USB_Bootloader_arb_int__INTC_CLR_EN_REG CYREG_INTC_CLR_EN2
#define USB_Bootloader_arb_int__INTC_CLR_PD_REG CYREG_INTC_CLR_PD2
#define USB_Bootloader_arb_int__INTC_MASK 0x40u
#define USB_Bootloader_arb_int__INTC_NUMBER 22
#define USB_Bootloader_arb_int__INTC_PRIOR_NUM 7
#define USB_Bootloader_arb_int__INTC_PRIOR_REG CYREG_INTC_PRIOR22
#define USB_Bootloader_arb_int__INTC_SET_EN_REG CYREG_INTC_SET_EN2
#define USB_Bootloader_arb_int__INTC_SET_PD_REG CYREG_INTC_SET_PD2
#define USB_Bootloader_arb_int__INTC_VECT (CYREG_INTC_VECT_MBASE+0x2Cu)

/* USB_Bootloader_ord_int */
#define USB_Bootloader_ord_int__ES2_PATCH 0
#define USB_Bootloader_ord_int__INTC_CLR_EN_REG CYREG_INTC_CLR_EN3
#define USB_Bootloader_ord_int__INTC_CLR_PD_REG CYREG_INTC_CLR_PD3
#define USB_Bootloader_ord_int__INTC_MASK 0x02u
#define USB_Bootloader_ord_int__INTC_NUMBER 25
#define USB_Bootloader_ord_int__INTC_PRIOR_NUM 7
#define USB_Bootloader_ord_int__INTC_PRIOR_REG CYREG_INTC_PRIOR25
#define USB_Bootloader_ord_int__INTC_SET_EN_REG CYREG_INTC_SET_EN3
#define USB_Bootloader_ord_int__INTC_SET_PD_REG CYREG_INTC_SET_PD3
#define USB_Bootloader_ord_int__INTC_VECT (CYREG_INTC_VECT_MBASE+0x32)

/* USB_Bootloader_sof_int */
#define USB_Bootloader_sof_int__ES2_PATCH 0
#define USB_Bootloader_sof_int__INTC_CLR_EN_REG CYREG_INTC_CLR_EN2
#define USB_Bootloader_sof_int__INTC_CLR_PD_REG CYREG_INTC_CLR_PD2
#define USB_Bootloader_sof_int__INTC_MASK 0x20u
#define USB_Bootloader_sof_int__INTC_NUMBER 21
#define USB_Bootloader_sof_int__INTC_PRIOR_NUM 7
#define USB_Bootloader_sof_int__INTC_PRIOR_REG CYREG_INTC_PRIOR21
#define USB_Bootloader_sof_int__INTC_SET_EN_REG CYREG_INTC_SET_EN2
#define USB_Bootloader_sof_int__INTC_SET_PD_REG CYREG_INTC_SET_PD2
#define USB_Bootloader_sof_int__INTC_VECT (CYREG_INTC_VECT_MBASE+0x2Au)

/* USB_Bootloader_dp_int */
#define USB_Bootloader_dp_int__ES2_PATCH 0
#define USB_Bootloader_dp_int__INTC_CLR_EN_REG CYREG_INTC_CLR_EN1
#define USB_Bootloader_dp_int__INTC_CLR_PD_REG CYREG_INTC_CLR_PD1
#define USB_Bootloader_dp_int__INTC_MASK 0x10u
#define USB_Bootloader_dp_int__INTC_NUMBER 12
#define USB_Bootloader_dp_int__INTC_PRIOR_NUM 7
#define USB_Bootloader_dp_int__INTC_PRIOR_REG CYREG_INTC_PRIOR12
#define USB_Bootloader_dp_int__INTC_SET_EN_REG CYREG_INTC_SET_EN1
#define USB_Bootloader_dp_int__INTC_SET_PD_REG CYREG_INTC_SET_PD1
#define USB_Bootloader_dp_int__INTC_VECT (CYREG_INTC_VECT_MBASE+0x18)

/* USB_Bootloader_ep_0 */
#define USB_Bootloader_ep_0__ES2_PATCH 0
#define USB_Bootloader_ep_0__INTC_CLR_EN_REG CYREG_INTC_CLR_EN3
#define USB_Bootloader_ep_0__INTC_CLR_PD_REG CYREG_INTC_CLR_PD3
#define USB_Bootloader_ep_0__INTC_MASK 0x01u
#define USB_Bootloader_ep_0__INTC_NUMBER 24
#define USB_Bootloader_ep_0__INTC_PRIOR_NUM 7
#define USB_Bootloader_ep_0__INTC_PRIOR_REG CYREG_INTC_PRIOR24
#define USB_Bootloader_ep_0__INTC_SET_EN_REG CYREG_INTC_SET_EN3
#define USB_Bootloader_ep_0__INTC_SET_PD_REG CYREG_INTC_SET_PD3
#define USB_Bootloader_ep_0__INTC_VECT (CYREG_INTC_VECT_MBASE+0x30)

/* USB_Bootloader_ep_1 */
#define USB_Bootloader_ep_1__ES2_PATCH 0
#define USB_Bootloader_ep_1__INTC_CLR_EN_REG CYREG_INTC_CLR_EN0
#define USB_Bootloader_ep_1__INTC_CLR_PD_REG CYREG_INTC_CLR_PD0
#define USB_Bootloader_ep_1__INTC_MASK 0x02u
#define USB_Bootloader_ep_1__INTC_NUMBER 1
#define USB_Bootloader_ep_1__INTC_PRIOR_NUM 7
#define USB_Bootloader_ep_1__INTC_PRIOR_REG CYREG_INTC_PRIOR1
#define USB_Bootloader_ep_1__INTC_SET_EN_REG CYREG_INTC_SET_EN0
#define USB_Bootloader_ep_1__INTC_SET_PD_REG CYREG_INTC_SET_PD0
#define USB_Bootloader_ep_1__INTC_VECT (CYREG_INTC_VECT_MBASE+0x02)

/* USB_Bootloader_ep_2 */
#define USB_Bootloader_ep_2__ES2_PATCH 0
#define USB_Bootloader_ep_2__INTC_CLR_EN_REG CYREG_INTC_CLR_EN0
#define USB_Bootloader_ep_2__INTC_CLR_PD_REG CYREG_INTC_CLR_PD0
#define USB_Bootloader_ep_2__INTC_MASK 0x01u
#define USB_Bootloader_ep_2__INTC_NUMBER 0
#define USB_Bootloader_ep_2__INTC_PRIOR_NUM 7
#define USB_Bootloader_ep_2__INTC_PRIOR_REG CYREG_INTC_PRIOR0
#define USB_Bootloader_ep_2__INTC_SET_EN_REG CYREG_INTC_SET_EN0
#define USB_Bootloader_ep_2__INTC_SET_PD_REG CYREG_INTC_SET_PD0
#define USB_Bootloader_ep_2__INTC_VECT (CYREG_INTC_VECT_MBASE+0x00)

/* USB_Bootloader_ep8 */
#define USB_Bootloader_ep8__DRQ_CTL CYREG_IDMUX_DRQ_CTL1
#define USB_Bootloader_ep8__DRQ_NUMBER 7
#define USB_Bootloader_ep8__NUMBEROF_TDS 0
#define USB_Bootloader_ep8__PRIORITY 2
#define USB_Bootloader_ep8__TERMIN_EN 1
#define USB_Bootloader_ep8__TERMIN_SEL 0
#define USB_Bootloader_ep8__TERMOUT0_EN 0
#define USB_Bootloader_ep8__TERMOUT0_SEL 0
#define USB_Bootloader_ep8__TERMOUT1_EN 0
#define USB_Bootloader_ep8__TERMOUT1_SEL 0

/* USB_Bootloader_USB */
#define USB_Bootloader_USB__ARB_CFG CYREG_USB_ARB_CFG
#define USB_Bootloader_USB__ARB_EP1_CFG CYREG_USB_ARB_EP1_CFG
#define USB_Bootloader_USB__ARB_EP1_INT_EN CYREG_USB_ARB_EP1_INT_EN
#define USB_Bootloader_USB__ARB_EP1_SR CYREG_USB_ARB_EP1_SR
#define USB_Bootloader_USB__ARB_EP2_CFG CYREG_USB_ARB_EP2_CFG
#define USB_Bootloader_USB__ARB_EP2_INT_EN CYREG_USB_ARB_EP2_INT_EN
#define USB_Bootloader_USB__ARB_EP2_SR CYREG_USB_ARB_EP2_SR
#define USB_Bootloader_USB__ARB_EP3_CFG CYREG_USB_ARB_EP3_CFG
#define USB_Bootloader_USB__ARB_EP3_INT_EN CYREG_USB_ARB_EP3_INT_EN
#define USB_Bootloader_USB__ARB_EP3_SR CYREG_USB_ARB_EP3_SR
#define USB_Bootloader_USB__ARB_EP4_CFG CYREG_USB_ARB_EP4_CFG
#define USB_Bootloader_USB__ARB_EP4_INT_EN CYREG_USB_ARB_EP4_INT_EN
#define USB_Bootloader_USB__ARB_EP4_SR CYREG_USB_ARB_EP4_SR
#define USB_Bootloader_USB__ARB_EP5_CFG CYREG_USB_ARB_EP5_CFG
#define USB_Bootloader_USB__ARB_EP5_INT_EN CYREG_USB_ARB_EP5_INT_EN
#define USB_Bootloader_USB__ARB_EP5_SR CYREG_USB_ARB_EP5_SR
#define USB_Bootloader_USB__ARB_EP6_CFG CYREG_USB_ARB_EP6_CFG
#define USB_Bootloader_USB__ARB_EP6_INT_EN CYREG_USB_ARB_EP6_INT_EN
#define USB_Bootloader_USB__ARB_EP6_SR CYREG_USB_ARB_EP6_SR
#define USB_Bootloader_USB__ARB_EP7_CFG CYREG_USB_ARB_EP7_CFG
#define USB_Bootloader_USB__ARB_EP7_INT_EN CYREG_USB_ARB_EP7_INT_EN
#define USB_Bootloader_USB__ARB_EP7_SR CYREG_USB_ARB_EP7_SR
#define USB_Bootloader_USB__ARB_EP8_CFG CYREG_USB_ARB_EP8_CFG
#define USB_Bootloader_USB__ARB_EP8_INT_EN CYREG_USB_ARB_EP8_INT_EN
#define USB_Bootloader_USB__ARB_EP8_SR CYREG_USB_ARB_EP8_SR
#define USB_Bootloader_USB__ARB_INT_EN CYREG_USB_ARB_INT_EN
#define USB_Bootloader_USB__ARB_INT_SR CYREG_USB_ARB_INT_SR
#define USB_Bootloader_USB__ARB_RW1_DR CYREG_USB_ARB_RW1_DR
#define USB_Bootloader_USB__ARB_RW1_RA CYREG_USB_ARB_RW1_RA
#define USB_Bootloader_USB__ARB_RW1_RA_MSB CYREG_USB_ARB_RW1_RA_MSB
#define USB_Bootloader_USB__ARB_RW1_WA CYREG_USB_ARB_RW1_WA
#define USB_Bootloader_USB__ARB_RW1_WA_MSB CYREG_USB_ARB_RW1_WA_MSB
#define USB_Bootloader_USB__ARB_RW2_DR CYREG_USB_ARB_RW2_DR
#define USB_Bootloader_USB__ARB_RW2_RA CYREG_USB_ARB_RW2_RA
#define USB_Bootloader_USB__ARB_RW2_RA_MSB CYREG_USB_ARB_RW2_RA_MSB
#define USB_Bootloader_USB__ARB_RW2_WA CYREG_USB_ARB_RW2_WA
#define USB_Bootloader_USB__ARB_RW2_WA_MSB CYREG_USB_ARB_RW2_WA_MSB
#define USB_Bootloader_USB__ARB_RW3_DR CYREG_USB_ARB_RW3_DR
#define USB_Bootloader_USB__ARB_RW3_RA CYREG_USB_ARB_RW3_RA
#define USB_Bootloader_USB__ARB_RW3_RA_MSB CYREG_USB_ARB_RW3_RA_MSB
#define USB_Bootloader_USB__ARB_RW3_WA CYREG_USB_ARB_RW3_WA
#define USB_Bootloader_USB__ARB_RW3_WA_MSB CYREG_USB_ARB_RW3_WA_MSB
#define USB_Bootloader_USB__ARB_RW4_DR CYREG_USB_ARB_RW4_DR
#define USB_Bootloader_USB__ARB_RW4_RA CYREG_USB_ARB_RW4_RA
#define USB_Bootloader_USB__ARB_RW4_RA_MSB CYREG_USB_ARB_RW4_RA_MSB
#define USB_Bootloader_USB__ARB_RW4_WA CYREG_USB_ARB_RW4_WA
#define USB_Bootloader_USB__ARB_RW4_WA_MSB CYREG_USB_ARB_RW4_WA_MSB
#define USB_Bootloader_USB__ARB_RW5_DR CYREG_USB_ARB_RW5_DR
#define USB_Bootloader_USB__ARB_RW5_RA CYREG_USB_ARB_RW5_RA
#define USB_Bootloader_USB__ARB_RW5_RA_MSB CYREG_USB_ARB_RW5_RA_MSB
#define USB_Bootloader_USB__ARB_RW5_WA CYREG_USB_ARB_RW5_WA
#define USB_Bootloader_USB__ARB_RW5_WA_MSB CYREG_USB_ARB_RW5_WA_MSB
#define USB_Bootloader_USB__ARB_RW6_DR CYREG_USB_ARB_RW6_DR
#define USB_Bootloader_USB__ARB_RW6_RA CYREG_USB_ARB_RW6_RA
#define USB_Bootloader_USB__ARB_RW6_RA_MSB CYREG_USB_ARB_RW6_RA_MSB
#define USB_Bootloader_USB__ARB_RW6_WA CYREG_USB_ARB_RW6_WA
#define USB_Bootloader_USB__ARB_RW6_WA_MSB CYREG_USB_ARB_RW6_WA_MSB
#define USB_Bootloader_USB__ARB_RW7_DR CYREG_USB_ARB_RW7_DR
#define USB_Bootloader_USB__ARB_RW7_RA CYREG_USB_ARB_RW7_RA
#define USB_Bootloader_USB__ARB_RW7_RA_MSB CYREG_USB_ARB_RW7_RA_MSB
#define USB_Bootloader_USB__ARB_RW7_WA CYREG_USB_ARB_RW7_WA
#define USB_Bootloader_USB__ARB_RW7_WA_MSB CYREG_USB_ARB_RW7_WA_MSB
#define USB_Bootloader_USB__ARB_RW8_DR CYREG_USB_ARB_RW8_DR
#define USB_Bootloader_USB__ARB_RW8_RA CYREG_USB_ARB_RW8_RA
#define USB_Bootloader_USB__ARB_RW8_RA_MSB CYREG_USB_ARB_RW8_RA_MSB
#define USB_Bootloader_USB__ARB_RW8_WA CYREG_USB_ARB_RW8_WA
#define USB_Bootloader_USB__ARB_RW8_WA_MSB CYREG_USB_ARB_RW8_WA_MSB
#define USB_Bootloader_USB__BUF_SIZE CYREG_USB_BUF_SIZE
#define USB_Bootloader_USB__BUS_RST_CNT CYREG_USB_BUS_RST_CNT
#define USB_Bootloader_USB__CR0 CYREG_USB_CR0
#define USB_Bootloader_USB__CR1 CYREG_USB_CR1
#define USB_Bootloader_USB__CWA CYREG_USB_CWA
#define USB_Bootloader_USB__CWA_MSB CYREG_USB_CWA_MSB
#define USB_Bootloader_USB__DMA_THRES CYREG_USB_DMA_THRES
#define USB_Bootloader_USB__DMA_THRES_MSB CYREG_USB_DMA_THRES_MSB
#define USB_Bootloader_USB__DYN_RECONFIG CYREG_USB_DYN_RECONFIG
#define USB_Bootloader_USB__EP0_CNT CYREG_USB_EP0_CNT
#define USB_Bootloader_USB__EP0_CR CYREG_USB_EP0_CR
#define USB_Bootloader_USB__EP0_DR0 CYREG_USB_EP0_DR0
#define USB_Bootloader_USB__EP0_DR1 CYREG_USB_EP0_DR1
#define USB_Bootloader_USB__EP0_DR2 CYREG_USB_EP0_DR2
#define USB_Bootloader_USB__EP0_DR3 CYREG_USB_EP0_DR3
#define USB_Bootloader_USB__EP0_DR4 CYREG_USB_EP0_DR4
#define USB_Bootloader_USB__EP0_DR5 CYREG_USB_EP0_DR5
#define USB_Bootloader_USB__EP0_DR6 CYREG_USB_EP0_DR6
#define USB_Bootloader_USB__EP0_DR7 CYREG_USB_EP0_DR7
#define USB_Bootloader_USB__EP_ACTIVE CYREG_USB_EP_ACTIVE
#define USB_Bootloader_USB__EP_TYPE CYREG_USB_EP_TYPE
#define USB_Bootloader_USB__MEM_DATA CYREG_USB_MEM_DATA_MBASE
#define USB_Bootloader_USB__OSCLK_DR0 CYREG_USB_OSCLK_DR0
#define USB_Bootloader_USB__OSCLK_DR1 CYREG_USB_OSCLK_DR1
#define USB_Bootloader_USB__PM_ACT_CFG CYREG_PM_ACT_CFG5
#define USB_Bootloader_USB__PM_ACT_MSK 0x01u
#define USB_Bootloader_USB__PM_STBY_CFG CYREG_PM_STBY_CFG5
#define USB_Bootloader_USB__PM_STBY_MSK 0x01u
#define USB_Bootloader_USB__SIE_EP1_CNT0 CYREG_USB_SIE_EP1_CNT0
#define USB_Bootloader_USB__SIE_EP1_CNT1 CYREG_USB_SIE_EP1_CNT1
#define USB_Bootloader_USB__SIE_EP1_CR0 CYREG_USB_SIE_EP1_CR0
#define USB_Bootloader_USB__SIE_EP2_CNT0 CYREG_USB_SIE_EP2_CNT0
#define USB_Bootloader_USB__SIE_EP2_CNT1 CYREG_USB_SIE_EP2_CNT1
#define USB_Bootloader_USB__SIE_EP2_CR0 CYREG_USB_SIE_EP2_CR0
#define USB_Bootloader_USB__SIE_EP3_CNT0 CYREG_USB_SIE_EP3_CNT0
#define USB_Bootloader_USB__SIE_EP3_CNT1 CYREG_USB_SIE_EP3_CNT1
#define USB_Bootloader_USB__SIE_EP3_CR0 CYREG_USB_SIE_EP3_CR0
#define USB_Bootloader_USB__SIE_EP4_CNT0 CYREG_USB_SIE_EP4_CNT0
#define USB_Bootloader_USB__SIE_EP4_CNT1 CYREG_USB_SIE_EP4_CNT1
#define USB_Bootloader_USB__SIE_EP4_CR0 CYREG_USB_SIE_EP4_CR0
#define USB_Bootloader_USB__SIE_EP5_CNT0 CYREG_USB_SIE_EP5_CNT0
#define USB_Bootloader_USB__SIE_EP5_CNT1 CYREG_USB_SIE_EP5_CNT1
#define USB_Bootloader_USB__SIE_EP5_CR0 CYREG_USB_SIE_EP5_CR0
#define USB_Bootloader_USB__SIE_EP6_CNT0 CYREG_USB_SIE_EP6_CNT0
#define USB_Bootloader_USB__SIE_EP6_CNT1 CYREG_USB_SIE_EP6_CNT1
#define USB_Bootloader_USB__SIE_EP6_CR0 CYREG_USB_SIE_EP6_CR0
#define USB_Bootloader_USB__SIE_EP7_CNT0 CYREG_USB_SIE_EP7_CNT0
#define USB_Bootloader_USB__SIE_EP7_CNT1 CYREG_USB_SIE_EP7_CNT1
#define USB_Bootloader_USB__SIE_EP7_CR0 CYREG_USB_SIE_EP7_CR0
#define USB_Bootloader_USB__SIE_EP8_CNT0 CYREG_USB_SIE_EP8_CNT0
#define USB_Bootloader_USB__SIE_EP8_CNT1 CYREG_USB_SIE_EP8_CNT1
#define USB_Bootloader_USB__SIE_EP8_CR0 CYREG_USB_SIE_EP8_CR0
#define USB_Bootloader_USB__SIE_EP_INT_EN CYREG_USB_SIE_EP_INT_EN
#define USB_Bootloader_USB__SIE_EP_INT_SR CYREG_USB_SIE_EP_INT_SR
#define USB_Bootloader_USB__SOF0 CYREG_USB_SOF0
#define USB_Bootloader_USB__SOF1 CYREG_USB_SOF1
#define USB_Bootloader_USB__USBIO_CR0 CYREG_USB_USBIO_CR0
#define USB_Bootloader_USB__USBIO_CR1 CYREG_USB_USBIO_CR1
#define USB_Bootloader_USB__USBIO_CR2 CYREG_USB_USBIO_CR2
#define USB_Bootloader_USB__USB_CLK_EN CYREG_USB_USB_CLK_EN

/* emFile_SPI0_BSPIM */
#define emFile_SPI0_BSPIM_BitCounter_ST__16BIT_STATUS_AUX_CTL_REG CYREG_B0_UDB08_09_ACTL
#define emFile_SPI0_BSPIM_BitCounter_ST__16BIT_STATUS_REG CYREG_B0_UDB08_09_ST
#define emFile_SPI0_BSPIM_BitCounter_ST__MASK_REG CYREG_B0_UDB08_MSK
#define emFile_SPI0_BSPIM_BitCounter_ST__MASK_ST_AUX_CTL_REG CYREG_B0_UDB08_MSK_ACTL
#define emFile_SPI0_BSPIM_BitCounter_ST__PER_ST_AUX_CTL_REG CYREG_B0_UDB08_MSK_ACTL
#define emFile_SPI0_BSPIM_BitCounter_ST__STATUS_AUX_CTL_REG CYREG_B0_UDB08_ACTL
#define emFile_SPI0_BSPIM_BitCounter_ST__STATUS_CNT_REG CYREG_B0_UDB08_ST_CTL
#define emFile_SPI0_BSPIM_BitCounter_ST__STATUS_CONTROL_REG CYREG_B0_UDB08_ST_CTL
#define emFile_SPI0_BSPIM_BitCounter_ST__STATUS_REG CYREG_B0_UDB08_ST
#define emFile_SPI0_BSPIM_BitCounter__16BIT_CONTROL_AUX_CTL_REG CYREG_B0_UDB08_09_ACTL
#define emFile_SPI0_BSPIM_BitCounter__16BIT_CONTROL_CONTROL_REG CYREG_B0_UDB08_09_CTL
#define emFile_SPI0_BSPIM_BitCounter__16BIT_CONTROL_COUNT_REG CYREG_B0_UDB08_09_CTL
#define emFile_SPI0_BSPIM_BitCounter__16BIT_COUNT_CONTROL_REG CYREG_B0_UDB08_09_CTL
#define emFile_SPI0_BSPIM_BitCounter__16BIT_COUNT_COUNT_REG CYREG_B0_UDB08_09_CTL
#define emFile_SPI0_BSPIM_BitCounter__16BIT_MASK_MASK_REG CYREG_B0_UDB08_09_MSK
#define emFile_SPI0_BSPIM_BitCounter__16BIT_MASK_PERIOD_REG CYREG_B0_UDB08_09_MSK
#define emFile_SPI0_BSPIM_BitCounter__16BIT_PERIOD_MASK_REG CYREG_B0_UDB08_09_MSK
#define emFile_SPI0_BSPIM_BitCounter__16BIT_PERIOD_PERIOD_REG CYREG_B0_UDB08_09_MSK
#define emFile_SPI0_BSPIM_BitCounter__CONTROL_AUX_CTL_REG CYREG_B0_UDB08_ACTL
#define emFile_SPI0_BSPIM_BitCounter__CONTROL_REG CYREG_B0_UDB08_CTL
#define emFile_SPI0_BSPIM_BitCounter__CONTROL_ST_REG CYREG_B0_UDB08_ST_CTL
#define emFile_SPI0_BSPIM_BitCounter__COUNT_REG CYREG_B0_UDB08_CTL
#define emFile_SPI0_BSPIM_BitCounter__COUNT_ST_REG CYREG_B0_UDB08_ST_CTL
#define emFile_SPI0_BSPIM_BitCounter__MASK_CTL_AUX_CTL_REG CYREG_B0_UDB08_MSK_ACTL
#define emFile_SPI0_BSPIM_BitCounter__PERIOD_REG CYREG_B0_UDB08_MSK
#define emFile_SPI0_BSPIM_BitCounter__PER_CTL_AUX_CTL_REG CYREG_B0_UDB08_MSK_ACTL
#define emFile_SPI0_BSPIM_RxStsReg__16BIT_STATUS_AUX_CTL_REG CYREG_B1_UDB08_09_ACTL
#define emFile_SPI0_BSPIM_RxStsReg__16BIT_STATUS_REG CYREG_B1_UDB08_09_ST
#define emFile_SPI0_BSPIM_RxStsReg__4__MASK 0x10u
#define emFile_SPI0_BSPIM_RxStsReg__4__POS 4
#define emFile_SPI0_BSPIM_RxStsReg__5__MASK 0x20u
#define emFile_SPI0_BSPIM_RxStsReg__5__POS 5
#define emFile_SPI0_BSPIM_RxStsReg__6__MASK 0x40u
#define emFile_SPI0_BSPIM_RxStsReg__6__POS 6
#define emFile_SPI0_BSPIM_RxStsReg__MASK 0x70u
#define emFile_SPI0_BSPIM_RxStsReg__MASK_REG CYREG_B1_UDB08_MSK
#define emFile_SPI0_BSPIM_RxStsReg__STATUS_AUX_CTL_REG CYREG_B1_UDB08_ACTL
#define emFile_SPI0_BSPIM_RxStsReg__STATUS_REG CYREG_B1_UDB08_ST
#define emFile_SPI0_BSPIM_TxStsReg__0__MASK 0x01u
#define emFile_SPI0_BSPIM_TxStsReg__0__POS 0
#define emFile_SPI0_BSPIM_TxStsReg__16BIT_STATUS_AUX_CTL_REG CYREG_B0_UDB09_10_ACTL
#define emFile_SPI0_BSPIM_TxStsReg__16BIT_STATUS_REG CYREG_B0_UDB09_10_ST
#define emFile_SPI0_BSPIM_TxStsReg__1__MASK 0x02u
#define emFile_SPI0_BSPIM_TxStsReg__1__POS 1
#define emFile_SPI0_BSPIM_TxStsReg__2__MASK 0x04u
#define emFile_SPI0_BSPIM_TxStsReg__2__POS 2
#define emFile_SPI0_BSPIM_TxStsReg__3__MASK 0x08u
#define emFile_SPI0_BSPIM_TxStsReg__3__POS 3
#define emFile_SPI0_BSPIM_TxStsReg__4__MASK 0x10u
#define emFile_SPI0_BSPIM_TxStsReg__4__POS 4
#define emFile_SPI0_BSPIM_TxStsReg__MASK 0x1Fu
#define emFile_SPI0_BSPIM_TxStsReg__MASK_REG CYREG_B0_UDB09_MSK
#define emFile_SPI0_BSPIM_TxStsReg__STATUS_AUX_CTL_REG CYREG_B0_UDB09_ACTL
#define emFile_SPI0_BSPIM_TxStsReg__STATUS_REG CYREG_B0_UDB09_ST
#define emFile_SPI0_BSPIM_sR8_Dp_u0__16BIT_A0_REG CYREG_B0_UDB09_10_A0
#define emFile_SPI0_BSPIM_sR8_Dp_u0__16BIT_A1_REG CYREG_B0_UDB09_10_A1
#define emFile_SPI0_BSPIM_sR8_Dp_u0__16BIT_D0_REG CYREG_B0_UDB09_10_D0
#define emFile_SPI0_BSPIM_sR8_Dp_u0__16BIT_D1_REG CYREG_B0_UDB09_10_D1
#define emFile_SPI0_BSPIM_sR8_Dp_u0__16BIT_DP_AUX_CTL_REG CYREG_B0_UDB09_10_ACTL
#define emFile_SPI0_BSPIM_sR8_Dp_u0__16BIT_F0_REG CYREG_B0_UDB09_10_F0
#define emFile_SPI0_BSPIM_sR8_Dp_u0__16BIT_F1_REG CYREG_B0_UDB09_10_F1
#define emFile_SPI0_BSPIM_sR8_Dp_u0__A0_A1_REG CYREG_B0_UDB09_A0_A1
#define emFile_SPI0_BSPIM_sR8_Dp_u0__A0_REG CYREG_B0_UDB09_A0
#define emFile_SPI0_BSPIM_sR8_Dp_u0__A1_REG CYREG_B0_UDB09_A1
#define emFile_SPI0_BSPIM_sR8_Dp_u0__D0_D1_REG CYREG_B0_UDB09_D0_D1
#define emFile_SPI0_BSPIM_sR8_Dp_u0__D0_REG CYREG_B0_UDB09_D0
#define emFile_SPI0_BSPIM_sR8_Dp_u0__D1_REG CYREG_B0_UDB09_D1
#define emFile_SPI0_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG CYREG_B0_UDB09_ACTL
#define emFile_SPI0_BSPIM_sR8_Dp_u0__F0_F1_REG CYREG_B0_UDB09_F0_F1
#define emFile_SPI0_BSPIM_sR8_Dp_u0__F0_REG CYREG_B0_UDB09_F0
#define emFile_SPI0_BSPIM_sR8_Dp_u0__F1_REG CYREG_B0_UDB09_F1

/* USB_Bootloader_Dm */
#define USB_Bootloader_Dm__0__MASK 0x80u
#define USB_Bootloader_Dm__0__PC CYREG_IO_PC_PRT15_7_6_PC1
#define USB_Bootloader_Dm__0__PORT 15
#define USB_Bootloader_Dm__0__SHIFT 7
#define USB_Bootloader_Dm__AG CYREG_PRT15_AG
#define USB_Bootloader_Dm__AMUX CYREG_PRT15_AMUX
#define USB_Bootloader_Dm__BIE CYREG_PRT15_BIE
#define USB_Bootloader_Dm__BIT_MASK CYREG_PRT15_BIT_MASK
#define USB_Bootloader_Dm__BYP CYREG_PRT15_BYP
#define USB_Bootloader_Dm__CTL CYREG_PRT15_CTL
#define USB_Bootloader_Dm__DM0 CYREG_PRT15_DM0
#define USB_Bootloader_Dm__DM1 CYREG_PRT15_DM1
#define USB_Bootloader_Dm__DM2 CYREG_PRT15_DM2
#define USB_Bootloader_Dm__DR CYREG_PRT15_DR
#define USB_Bootloader_Dm__INP_DIS CYREG_PRT15_INP_DIS
#define USB_Bootloader_Dm__LCD_COM_SEG CYREG_PRT15_LCD_COM_SEG
#define USB_Bootloader_Dm__LCD_EN CYREG_PRT15_LCD_EN
#define USB_Bootloader_Dm__PORT 15
#define USB_Bootloader_Dm__PRT CYREG_PRT15_PRT
#define USB_Bootloader_Dm__PRTDSI__CAPS_SEL CYREG_PRT15_CAPS_SEL
#define USB_Bootloader_Dm__PRTDSI__DBL_SYNC_IN CYREG_PRT15_DBL_SYNC_IN
#define USB_Bootloader_Dm__PRTDSI__OE_SEL0 CYREG_PRT15_OE_SEL0
#define USB_Bootloader_Dm__PRTDSI__OE_SEL1 CYREG_PRT15_OE_SEL1
#define USB_Bootloader_Dm__PRTDSI__OUT_SEL0 CYREG_PRT15_OUT_SEL0
#define USB_Bootloader_Dm__PRTDSI__OUT_SEL1 CYREG_PRT15_OUT_SEL1
#define USB_Bootloader_Dm__PRTDSI__SYNC_OUT CYREG_PRT15_SYNC_OUT
#define USB_Bootloader_Dm__PS CYREG_PRT15_PS
#define USB_Bootloader_Dm__SLW CYREG_PRT15_SLW

/* USB_Bootloader_Dp */
#define USB_Bootloader_Dp__0__MASK 0x40u
#define USB_Bootloader_Dp__0__PC CYREG_IO_PC_PRT15_7_6_PC0
#define USB_Bootloader_Dp__0__PORT 15
#define USB_Bootloader_Dp__0__SHIFT 6
#define USB_Bootloader_Dp__AG CYREG_PRT15_AG
#define USB_Bootloader_Dp__AMUX CYREG_PRT15_AMUX
#define USB_Bootloader_Dp__BIE CYREG_PRT15_BIE
#define USB_Bootloader_Dp__BIT_MASK CYREG_PRT15_BIT_MASK
#define USB_Bootloader_Dp__BYP CYREG_PRT15_BYP
#define USB_Bootloader_Dp__CTL CYREG_PRT15_CTL
#define USB_Bootloader_Dp__DM0 CYREG_PRT15_DM0
#define USB_Bootloader_Dp__DM1 CYREG_PRT15_DM1
#define USB_Bootloader_Dp__DM2 CYREG_PRT15_DM2
#define USB_Bootloader_Dp__DR CYREG_PRT15_DR
#define USB_Bootloader_Dp__INP_DIS CYREG_PRT15_INP_DIS
#define USB_Bootloader_Dp__INTSTAT CYREG_PICU15_INTSTAT
#define USB_Bootloader_Dp__LCD_COM_SEG CYREG_PRT15_LCD_COM_SEG
#define USB_Bootloader_Dp__LCD_EN CYREG_PRT15_LCD_EN
#define USB_Bootloader_Dp__MASK 0x40u
#define USB_Bootloader_Dp__PORT 15
#define USB_Bootloader_Dp__PRT CYREG_PRT15_PRT
#define USB_Bootloader_Dp__PRTDSI__CAPS_SEL CYREG_PRT15_CAPS_SEL
#define USB_Bootloader_Dp__PRTDSI__DBL_SYNC_IN CYREG_PRT15_DBL_SYNC_IN
#define USB_Bootloader_Dp__PRTDSI__OE_SEL0 CYREG_PRT15_OE_SEL0
#define USB_Bootloader_Dp__PRTDSI__OE_SEL1 CYREG_PRT15_OE_SEL1
#define USB_Bootloader_Dp__PRTDSI__OUT_SEL0 CYREG_PRT15_OUT_SEL0
#define USB_Bootloader_Dp__PRTDSI__OUT_SEL1 CYREG_PRT15_OUT_SEL1
#define USB_Bootloader_Dp__PRTDSI__SYNC_OUT CYREG_PRT15_SYNC_OUT
#define USB_Bootloader_Dp__PS CYREG_PRT15_PS
#define USB_Bootloader_Dp__SHIFT 6
#define USB_Bootloader_Dp__SLW CYREG_PRT15_SLW
#define USB_Bootloader_Dp__SNAP CYREG_PICU15_SNAP

/* emFile_Clock_1 */
#define emFile_Clock_1__CFG0 CYREG_CLKDIST_DCFG0_CFG0
#define emFile_Clock_1__CFG1 CYREG_CLKDIST_DCFG0_CFG1
#define emFile_Clock_1__CFG2 CYREG_CLKDIST_DCFG0_CFG2
#define emFile_Clock_1__CFG2_SRC_SEL_MASK 0x07u
#define emFile_Clock_1__PM_ACT_CFG CYREG_PM_ACT_CFG2
#define emFile_Clock_1__PM_ACT_MSK 0x01u
#define emFile_Clock_1__PM_STBY_CFG CYREG_PM_STBY_CFG2
#define emFile_Clock_1__PM_STBY_MSK 0x01u

/* emFile_SPI0_CS */
#define emFile_SPI0_CS__0__MASK 0x02u
#define emFile_SPI0_CS__0__PC CYREG_PRT5_PC1
#define emFile_SPI0_CS__0__PORT 5
#define emFile_SPI0_CS__0__SHIFT 1
#define emFile_SPI0_CS__AG CYREG_PRT5_AG
#define emFile_SPI0_CS__AMUX CYREG_PRT5_AMUX
#define emFile_SPI0_CS__BIE CYREG_PRT5_BIE
#define emFile_SPI0_CS__BIT_MASK CYREG_PRT5_BIT_MASK
#define emFile_SPI0_CS__BYP CYREG_PRT5_BYP
#define emFile_SPI0_CS__CTL CYREG_PRT5_CTL
#define emFile_SPI0_CS__DM0 CYREG_PRT5_DM0
#define emFile_SPI0_CS__DM1 CYREG_PRT5_DM1
#define emFile_SPI0_CS__DM2 CYREG_PRT5_DM2
#define emFile_SPI0_CS__DR CYREG_PRT5_DR
#define emFile_SPI0_CS__INP_DIS CYREG_PRT5_INP_DIS
#define emFile_SPI0_CS__LCD_COM_SEG CYREG_PRT5_LCD_COM_SEG
#define emFile_SPI0_CS__LCD_EN CYREG_PRT5_LCD_EN
#define emFile_SPI0_CS__MASK 0x02u
#define emFile_SPI0_CS__PORT 5
#define emFile_SPI0_CS__PRT CYREG_PRT5_PRT
#define emFile_SPI0_CS__PRTDSI__CAPS_SEL CYREG_PRT5_CAPS_SEL
#define emFile_SPI0_CS__PRTDSI__DBL_SYNC_IN CYREG_PRT5_DBL_SYNC_IN
#define emFile_SPI0_CS__PRTDSI__OE_SEL0 CYREG_PRT5_OE_SEL0
#define emFile_SPI0_CS__PRTDSI__OE_SEL1 CYREG_PRT5_OE_SEL1
#define emFile_SPI0_CS__PRTDSI__OUT_SEL0 CYREG_PRT5_OUT_SEL0
#define emFile_SPI0_CS__PRTDSI__OUT_SEL1 CYREG_PRT5_OUT_SEL1
#define emFile_SPI0_CS__PRTDSI__SYNC_OUT CYREG_PRT5_SYNC_OUT
#define emFile_SPI0_CS__PS CYREG_PRT5_PS
#define emFile_SPI0_CS__SHIFT 1
#define emFile_SPI0_CS__SLW CYREG_PRT5_SLW

/* emFile_miso0 */
#define emFile_miso0__0__MASK 0x20u
#define emFile_miso0__0__PC CYREG_PRT5_PC5
#define emFile_miso0__0__PORT 5
#define emFile_miso0__0__SHIFT 5
#define emFile_miso0__AG CYREG_PRT5_AG
#define emFile_miso0__AMUX CYREG_PRT5_AMUX
#define emFile_miso0__BIE CYREG_PRT5_BIE
#define emFile_miso0__BIT_MASK CYREG_PRT5_BIT_MASK
#define emFile_miso0__BYP CYREG_PRT5_BYP
#define emFile_miso0__CTL CYREG_PRT5_CTL
#define emFile_miso0__DM0 CYREG_PRT5_DM0
#define emFile_miso0__DM1 CYREG_PRT5_DM1
#define emFile_miso0__DM2 CYREG_PRT5_DM2
#define emFile_miso0__DR CYREG_PRT5_DR
#define emFile_miso0__INP_DIS CYREG_PRT5_INP_DIS
#define emFile_miso0__LCD_COM_SEG CYREG_PRT5_LCD_COM_SEG
#define emFile_miso0__LCD_EN CYREG_PRT5_LCD_EN
#define emFile_miso0__MASK 0x20u
#define emFile_miso0__PORT 5
#define emFile_miso0__PRT CYREG_PRT5_PRT
#define emFile_miso0__PRTDSI__CAPS_SEL CYREG_PRT5_CAPS_SEL
#define emFile_miso0__PRTDSI__DBL_SYNC_IN CYREG_PRT5_DBL_SYNC_IN
#define emFile_miso0__PRTDSI__OE_SEL0 CYREG_PRT5_OE_SEL0
#define emFile_miso0__PRTDSI__OE_SEL1 CYREG_PRT5_OE_SEL1
#define emFile_miso0__PRTDSI__OUT_SEL0 CYREG_PRT5_OUT_SEL0
#define emFile_miso0__PRTDSI__OUT_SEL1 CYREG_PRT5_OUT_SEL1
#define emFile_miso0__PRTDSI__SYNC_OUT CYREG_PRT5_SYNC_OUT
#define emFile_miso0__PS CYREG_PRT5_PS
#define emFile_miso0__SHIFT 5
#define emFile_miso0__SLW CYREG_PRT5_SLW

/* emFile_mosi0 */
#define emFile_mosi0__0__MASK 0x08u
#define emFile_mosi0__0__PC CYREG_PRT5_PC3
#define emFile_mosi0__0__PORT 5
#define emFile_mosi0__0__SHIFT 3
#define emFile_mosi0__AG CYREG_PRT5_AG
#define emFile_mosi0__AMUX CYREG_PRT5_AMUX
#define emFile_mosi0__BIE CYREG_PRT5_BIE
#define emFile_mosi0__BIT_MASK CYREG_PRT5_BIT_MASK
#define emFile_mosi0__BYP CYREG_PRT5_BYP
#define emFile_mosi0__CTL CYREG_PRT5_CTL
#define emFile_mosi0__DM0 CYREG_PRT5_DM0
#define emFile_mosi0__DM1 CYREG_PRT5_DM1
#define emFile_mosi0__DM2 CYREG_PRT5_DM2
#define emFile_mosi0__DR CYREG_PRT5_DR
#define emFile_mosi0__INP_DIS CYREG_PRT5_INP_DIS
#define emFile_mosi0__LCD_COM_SEG CYREG_PRT5_LCD_COM_SEG
#define emFile_mosi0__LCD_EN CYREG_PRT5_LCD_EN
#define emFile_mosi0__MASK 0x08u
#define emFile_mosi0__PORT 5
#define emFile_mosi0__PRT CYREG_PRT5_PRT
#define emFile_mosi0__PRTDSI__CAPS_SEL CYREG_PRT5_CAPS_SEL
#define emFile_mosi0__PRTDSI__DBL_SYNC_IN CYREG_PRT5_DBL_SYNC_IN
#define emFile_mosi0__PRTDSI__OE_SEL0 CYREG_PRT5_OE_SEL0
#define emFile_mosi0__PRTDSI__OE_SEL1 CYREG_PRT5_OE_SEL1
#define emFile_mosi0__PRTDSI__OUT_SEL0 CYREG_PRT5_OUT_SEL0
#define emFile_mosi0__PRTDSI__OUT_SEL1 CYREG_PRT5_OUT_SEL1
#define emFile_mosi0__PRTDSI__SYNC_OUT CYREG_PRT5_SYNC_OUT
#define emFile_mosi0__PS CYREG_PRT5_PS
#define emFile_mosi0__SHIFT 3
#define emFile_mosi0__SLW CYREG_PRT5_SLW

/* emFile_sclk0 */
#define emFile_sclk0__0__MASK 0x80u
#define emFile_sclk0__0__PC CYREG_PRT5_PC7
#define emFile_sclk0__0__PORT 5
#define emFile_sclk0__0__SHIFT 7
#define emFile_sclk0__AG CYREG_PRT5_AG
#define emFile_sclk0__AMUX CYREG_PRT5_AMUX
#define emFile_sclk0__BIE CYREG_PRT5_BIE
#define emFile_sclk0__BIT_MASK CYREG_PRT5_BIT_MASK
#define emFile_sclk0__BYP CYREG_PRT5_BYP
#define emFile_sclk0__CTL CYREG_PRT5_CTL
#define emFile_sclk0__DM0 CYREG_PRT5_DM0
#define emFile_sclk0__DM1 CYREG_PRT5_DM1
#define emFile_sclk0__DM2 CYREG_PRT5_DM2
#define emFile_sclk0__DR CYREG_PRT5_DR
#define emFile_sclk0__INP_DIS CYREG_PRT5_INP_DIS
#define emFile_sclk0__LCD_COM_SEG CYREG_PRT5_LCD_COM_SEG
#define emFile_sclk0__LCD_EN CYREG_PRT5_LCD_EN
#define emFile_sclk0__MASK 0x80u
#define emFile_sclk0__PORT 5
#define emFile_sclk0__PRT CYREG_PRT5_PRT
#define emFile_sclk0__PRTDSI__CAPS_SEL CYREG_PRT5_CAPS_SEL
#define emFile_sclk0__PRTDSI__DBL_SYNC_IN CYREG_PRT5_DBL_SYNC_IN
#define emFile_sclk0__PRTDSI__OE_SEL0 CYREG_PRT5_OE_SEL0
#define emFile_sclk0__PRTDSI__OE_SEL1 CYREG_PRT5_OE_SEL1
#define emFile_sclk0__PRTDSI__OUT_SEL0 CYREG_PRT5_OUT_SEL0
#define emFile_sclk0__PRTDSI__OUT_SEL1 CYREG_PRT5_OUT_SEL1
#define emFile_sclk0__PRTDSI__SYNC_OUT CYREG_PRT5_SYNC_OUT
#define emFile_sclk0__PS CYREG_PRT5_PS
#define emFile_sclk0__SHIFT 7
#define emFile_sclk0__SLW CYREG_PRT5_SLW

/* Miscellaneous */
/* -- WARNING: define names containting LEOPARD or PANTHER are deprecated and will be removed in a future release */
#define CYDEV_BOOTLOADER_IO_COMP_CUSTOM_IO 0
#define CYDEV_DEBUGGING_DPS_SWD 2
#define CYDEV_CONFIG_UNUSED_IO_AllowButWarn 0
#define CYDEV_CONFIGURATION_MODE_DMA 2
#define CYDEV_CONFIG_FASTBOOT_ENABLED 1
#define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3
#define CYDEV_CHIP_REVISION_3A_PRODUCTION 3
#define CYDEV_CHIP_MEMBER_3A 1
#define CYDEV_CHIP_FAMILY_PSOC3 1
#define CYDEV_CHIP_DIE_LEOPARD 1
#define CYDEV_CHIP_DIE_EXPECT CYDEV_CHIP_DIE_LEOPARD
#define CYDEV_BOOTLOADER_IO_COMP_USB_Bootloader 1
#define CYDEV_BOOTLOADER_CHECKSUM_BASIC 0
#define BCLK__BUS_CLK__HZ 48000000U
#define BCLK__BUS_CLK__KHZ 48000U
#define BCLK__BUS_CLK__MHZ 48U
#define CYDEV_APPLICATION_ID 0x0000
#define CYDEV_APPLICATION_VERSION 0x0000
#define CYDEV_BOOTLOADER_APPLICATIONS 1
#define CYDEV_BOOTLOADER_CHECKSUM CYDEV_BOOTLOADER_CHECKSUM_BASIC
#define CYDEV_BOOTLOADER_CHECKSUM_CRC 1
#define CYDEV_BOOTLOADER_FAST_VERIFY 0
#define CYDEV_BOOTLOADER_IO_COMP CYDEV_BOOTLOADER_IO_COMP_USB_Bootloader
#define CYDEV_BOOTLOADER_VERSION 0x0000
#define CYDEV_BOOTLOADER_WAIT_COMMAND 1
#define CYDEV_BOOTLOADER_WAIT_TIME 225
#define CYDEV_CHIP_DIE_ACTUAL CYDEV_CHIP_DIE_EXPECT
#define CYDEV_CHIP_DIE_PANTHER 2
#define CYDEV_CHIP_DIE_UNKNOWN 0
#define CYDEV_CHIP_FAMILY_PSOC4 2
#define CYDEV_CHIP_FAMILY_PSOC5 3
#define CYDEV_CHIP_FAMILY_UNKNOWN 0
#define CYDEV_CHIP_FAMILY_USED CYDEV_CHIP_FAMILY_PSOC3
#define CYDEV_CHIP_JTAG_ID 0x1E028069
#define CYDEV_CHIP_MEMBER_5A 2
#define CYDEV_CHIP_MEMBER_UNKNOWN 0
#define CYDEV_CHIP_MEMBER_USED CYDEV_CHIP_MEMBER_3A
#define CYDEV_CHIP_REVISION_3A_ES1 0
#define CYDEV_CHIP_REVISION_3A_ES2 1
#define CYDEV_CHIP_REVISION_3A_ES3 3
#define CYDEV_CHIP_REVISION_5A_ES0 0
#define CYDEV_CHIP_REVISION_5A_ES1 1
#define CYDEV_CHIP_REVISION_5A_PRODUCTION 1
#define CYDEV_CHIP_REVISION_USED CYDEV_CHIP_REVISION_3A_PRODUCTION
#define CYDEV_CHIP_REV_EXPECT CYDEV_CHIP_REV_LEOPARD_PRODUCTION
#define CYDEV_CHIP_REV_LEOPARD_ES1 0
#define CYDEV_CHIP_REV_LEOPARD_ES2 1
#define CYDEV_CHIP_REV_LEOPARD_ES3 3
#define CYDEV_CHIP_REV_PANTHER_ES0 0
#define CYDEV_CHIP_REV_PANTHER_ES1 1
#define CYDEV_CHIP_REV_PANTHER_PRODUCTION 1
#define CYDEV_CONFIGURATION_CLEAR_SRAM 1
#define CYDEV_CONFIGURATION_COMPRESSED 0
#define CYDEV_CONFIGURATION_DMA 1
#define CYDEV_CONFIGURATION_ECC 0
#define CYDEV_CONFIGURATION_IMOENABLED CYDEV_CONFIG_FASTBOOT_ENABLED
#define CYDEV_CONFIGURATION_MODE CYDEV_CONFIGURATION_MODE_DMA
#define CYDEV_CONFIGURATION_MODE_COMPRESSED 0
#define CYDEV_CONFIGURATION_MODE_UNCOMPRESSED 1
#define CYDEV_CONFIG_FORCE_ROUTE 0
#define CYDEV_CONFIG_UNUSED_IO CYDEV_CONFIG_UNUSED_IO_AllowButWarn
#define CYDEV_CONFIG_UNUSED_IO_AllowWithInfo 1
#define CYDEV_CONFIG_UNUSED_IO_Disallowed 2
#define CYDEV_CUSTOM_ID 0x00000000
#define CYDEV_DEBUGGING_DPS CYDEV_DEBUGGING_DPS_SWD
#define CYDEV_DEBUGGING_DPS_Disable 3
#define CYDEV_DEBUGGING_DPS_JTAG_4 1
#define CYDEV_DEBUGGING_DPS_JTAG_5 0
#define CYDEV_DEBUGGING_ENABLE 1
#define CYDEV_DEBUGGING_REQXRES 1
#define CYDEV_DEBUGGING_XRES 0
#define CYDEV_DEBUG_ENABLE_MASK 0x01
#define CYDEV_DEBUG_ENABLE_REGISTER CYREG_MLOGIC_DEBUG
#define CYDEV_DMA_CHANNELS_AVAILABLE 24
#define CYDEV_ECC_ENABLE 0
#define CYDEV_INSTRUCT_CACHE_ENABLED 1
#define CYDEV_PROJ_TYPE 1
#define CYDEV_PROJ_TYPE_BOOTLOADER 1
#define CYDEV_PROJ_TYPE_LOADABLE 2
#define CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER 3
#define CYDEV_PROJ_TYPE_STANDARD 0
#define CYDEV_PROTECTION_ENABLE 0
#define CYDEV_USB_CLK_OSC_LOCKING_ENABLED_AT_PWR_UP 
#define CYDEV_VDDA 3.3
#define CYDEV_VDDA_MV 3300
#define CYDEV_VDDD 3.3
#define CYDEV_VDDD_MV 3300
#define CYDEV_VDDIO0 3.3
#define CYDEV_VDDIO0_MV 3300
#define CYDEV_VDDIO1 3.3
#define CYDEV_VDDIO1_MV 3300
#define CYDEV_VDDIO2 3.3
#define CYDEV_VDDIO2_MV 3300
#define CYDEV_VDDIO3 3.3
#define CYDEV_VDDIO3_MV 3300
#define CYDEV_VIO0 3.3
#define CYDEV_VIO0_MV 3300
#define CYDEV_VIO1 3.3
#define CYDEV_VIO1_MV 3300
#define CYDEV_VIO2 3.3
#define CYDEV_VIO2_MV 3300
#define CYDEV_VIO3 3.3
#define CYDEV_VIO3_MV 3300
#define CyBtldr_Custom_Interface CYDEV_BOOTLOADER_IO_COMP_CUSTOM_IO
#define CyBtldr_USB_Bootloader CYDEV_BOOTLOADER_IO_COMP_USB_Bootloader
#define DMA_CHANNELS_USED__MASK0 0x00000080u
#define CYDEV_BOOTLOADER_ENABLE 1

#endif /* INCLUDED_CYFITTER_H */
