/*******************************************************************************
 * @file dw_apb_ssi_regs.h
 * @brief Synopsys DesignWare APB SSI register definitions.
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#ifndef DW_APB_SSI_REGS__H
#define DW_APB_SSI_REGS__H

/**
 * @defgroup dw_apb_ssi_regs
 * @{
 *
 * @ingroup dw_apb_ssi
 *
 * @brief dw_apb_ssi register definitions.
 */

/* Register offsets */
#define DW_APB_SSI_CTRLR0(base)         (base + 0x000U)
#define DW_APB_SSI_CTRLR1(base)         (base + 0x004U)
#define DW_APB_SSI_SSIENR(base)         (base + 0x008U)
#define DW_APB_SSI_MWCR(base)           (base + 0x00CU)
#define DW_APB_SSI_SER(base)            (base + 0x010U)
#define DW_APB_SSI_BAUDR(base)          (base + 0x014U)
#define DW_APB_SSI_TXFTLR(base)         (base + 0x018U)
#define DW_APB_SSI_RXFTLR(base)         (base + 0x01CU)
#define DW_APB_SSI_TXFLR(base)          (base + 0x020U)
#define DW_APB_SSI_RXFLR(base)          (base + 0x024U)
#define DW_APB_SSI_SR(base)             (base + 0x028U)
#define DW_APB_SSI_IMR(base)            (base + 0x02CU)
#define DW_APB_SSI_ISR(base)            (base + 0x030U)
#define DW_APB_SSI_RISR(base)           (base + 0x034U)
#define DW_APB_SSI_TXOICR(base)         (base + 0x038U)
#define DW_APB_SSI_RXOICR(base)         (base + 0x03CU)
#define DW_APB_SSI_RXUICR(base)         (base + 0x040U)
#define DW_APB_SSI_MSTICR(base)         (base + 0x044U)
#define DW_APB_SSI_ICR(base)            (base + 0x048U)
#define DW_APB_SSI_DMACR(base)          (base + 0x04CU)
#define DW_APB_SSI_DMATDLR(base)        (base + 0x050U)
#define DW_APB_SSI_DMARDLR(base)        (base + 0x054U)
#define DW_APB_SSI_IDR(base)            (base + 0x058U)
#define DW_APB_SSI_VERSION(base)        (base + 0x05CU)
#define DW_APB_SSI_DR(base)             (base + 0x060U)
#define DW_APB_SSI_RX_SAMPLE_DLY(base)  (base + 0x0F0U)
#define DW_APB_SSI_SPI_CTRLR0(base)     (base + 0x0F4U)
#define DW_APB_SSI_TXD_DRIVE_EDGE(base) (base + 0x0F4U)

/* CTRLR0 */
#define DW_APB_SSI_CTRLR0_DFS_BIT_POS     (0U)
#define DW_APB_SSI_CTRLR0_DFS_BIT_MSK     (0xFU << DW_APB_SSI_CTRLR0_DFS_BIT_POS)
#define DW_APB_SSI_CTRLR0_FRF_BIT_POS     (4U)
#define DW_APB_SSI_CTRLR0_FRF_BIT_MSK     (0x3U << DW_APB_SSI_CTRLR0_FRF_BIT_POS)
#define DW_APB_SSI_CTRLR0_SCPH_BIT_POS    (6U)
#define DW_APB_SSI_CTRLR0_SCPH_BIT_MSK    (1U << DW_APB_SSI_CTRLR0_SCPH_BIT_POS)
#define DW_APB_SSI_CTRLR0_SCPOL_BIT_POS   (7U)
#define DW_APB_SSI_CTRLR0_SCPOL_BIT_MSK   (1U << DW_APB_SSI_CTRLR0_SCPOL_BIT_POS)
#define DW_APB_SSI_CTRLR0_TMOD_BIT_POS    (8U)
#define DW_APB_SSI_CTRLR0_TMOD_BIT_MSK    (0x3U << DW_APB_SSI_CTRLR0_TMOD_BIT_POS)
#define DW_APB_SSI_CTRLR0_SLV_OE_BIT_POS  (10U)
#define DW_APB_SSI_CTRLR0_SLV_OE_BIT_MSK  (1U << DW_APB_SSI_CTRLR0_SLV_OE_BIT_POS)
#define DW_APB_SSI_CTRLR0_SRL_BIT_POS     (11U)
#define DW_APB_SSI_CTRLR0_SRL_BIT_MSK     (1U << DW_APB_SSI_CTRLR0_SRL_BIT_POS)
#define DW_APB_SSI_CTRLR0_CFS_BIT_POS     (12U)
#define DW_APB_SSI_CTRLR0_CFS_BIT_MSK     (0xFU << DW_APB_SSI_CTRLR0_CFS_BIT_POS)
#define DW_APB_SSI_CTRLR0_DFS_32_BIT_POS  (16U)
#define DW_APB_SSI_CTRLR0_DFS_32_BIT_MSK  (0x1FU << DW_APB_SSI_CTRLR0_DFS_32_BIT_POS)
#define DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS (21U)
#define DW_APB_SSI_CTRLR0_SPI_FRF_BIT_MSK (0x3U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)
#define DW_APB_SSI_CTRLR0_SSTE_BIT_POS    (24U)
#define DW_APB_SSI_CTRLR0_SSTE_BIT_MSK    (0x1U << DW_APB_SSI_CTRLR0_SSTE_BIT_POS)

#define DW_APB_SSI_CTRLR0_TMOD_TX_RX  (0U << DW_APB_SSI_CTRLR0_TMOD_BIT_POS)
#define DW_APB_SSI_CTRLR0_TMOD_TX     (1U << DW_APB_SSI_CTRLR0_TMOD_BIT_POS)
#define DW_APB_SSI_CTRLR0_TMOD_RX     (2U << DW_APB_SSI_CTRLR0_TMOD_BIT_POS)
#define DW_APB_SSI_CTRLR0_TMOD_EEPROM (3U << DW_APB_SSI_CTRLR0_TMOD_BIT_POS)

/* CTRLR1: number of data frames to receive in receive-only mode. */
#define DW_APB_SSI_CTRLR1_NDF_BIT_POS (0U)
#define DW_APB_SSI_CTRLR1_NDF_BIT_MSK (0xFFFFU << DW_APB_SSI_CTRLR1_NDF_BIT_POS)

/* SSIENR */
#define DW_APB_SSI_SSIENR_SSI_EN_BIT_POS (0U)
#define DW_APB_SSI_SSIENR_SSI_EN_BIT_MSK (1U << DW_APB_SSI_SSIENR_SSI_EN_BIT_POS)

/* MWCR */
#define DW_APB_SSI_MWCR_MWMOD_BIT_POS (0U)
#define DW_APB_SSI_MWCR_MWMOD_BIT_MSK (1U << DW_APB_SSI_MWCR_MWMOD_BIT_POS)
#define DW_APB_SSI_MWCR_MDD_BIT_POS   (1U)
#define DW_APB_SSI_MWCR_MDD_BIT_MSK   (1U << DW_APB_SSI_MWCR_MDD_BIT_POS)
#define DW_APB_SSI_MWCR_MHS_BIT_POS   (2U)
#define DW_APB_SSI_MWCR_MHS_BIT_MSK   (1U << DW_APB_SSI_MWCR_MHS_BIT_POS)

/* SER: chip-select enable bits. */
#define DW_APB_SSI_SER_SER_BIT_POS (0U)
#define DW_APB_SSI_SER_SER_BIT_MSK (0xFFFFU << DW_APB_SSI_SER_SER_BIT_POS)

/* BAUDR */
#define DW_APB_SSI_BAUDR_SCKDV_BIT_POS (0U)
#define DW_APB_SSI_BAUDR_SCKDV_BIT_MSK (0xFFFFU << DW_APB_SSI_BAUDR_SCKDV_BIT_POS)

/* TXFTLR and RXFTLR */
#define DW_APB_SSI_TXFTLR_TFT_BIT_POS (0U)
#define DW_APB_SSI_TXFTLR_TFT_BIT_MSK (0x1FU << DW_APB_SSI_TXFTLR_TFT_BIT_POS)
#define DW_APB_SSI_RXFTLR_RFT_BIT_POS (0U)
#define DW_APB_SSI_RXFTLR_RFT_BIT_MSK (0x1FU << DW_APB_SSI_RXFTLR_RFT_BIT_POS)

/* TXFLR and RXFLR */
#define DW_APB_SSI_TXFLR_TXTFL_BIT_POS (0U)
#define DW_APB_SSI_TXFLR_TXTFL_BIT_MSK (0x1FU << DW_APB_SSI_TXFLR_TXTFL_BIT_POS)
#define DW_APB_SSI_RXFLR_RXTFL_BIT_POS (0U)
#define DW_APB_SSI_RXFLR_RXTFL_BIT_MSK (0x1FU << DW_APB_SSI_RXFLR_RXTFL_BIT_POS)

/* SR */
#define DW_APB_SSI_SR_BUSY_BIT_POS (0U)
#define DW_APB_SSI_SR_BUSY_BIT_MSK (1U << DW_APB_SSI_SR_BUSY_BIT_POS)
#define DW_APB_SSI_SR_TFNF_BIT_POS (1U)
#define DW_APB_SSI_SR_TFNF_BIT_MSK (1U << DW_APB_SSI_SR_TFNF_BIT_POS)
#define DW_APB_SSI_SR_TFE_BIT_POS  (2U)
#define DW_APB_SSI_SR_TFE_BIT_MSK  (1U << DW_APB_SSI_SR_TFE_BIT_POS)
#define DW_APB_SSI_SR_RFNE_BIT_POS (3U)
#define DW_APB_SSI_SR_RFNE_BIT_MSK (1U << DW_APB_SSI_SR_RFNE_BIT_POS)
#define DW_APB_SSI_SR_RFF_BIT_POS  (4U)
#define DW_APB_SSI_SR_RFF_BIT_MSK  (1U << DW_APB_SSI_SR_RFF_BIT_POS)
#define DW_APB_SSI_SR_TXE_BIT_POS  (5U)
#define DW_APB_SSI_SR_TXE_BIT_MSK  (1U << DW_APB_SSI_SR_TXE_BIT_POS)
#define DW_APB_SSI_SR_DCOL_BIT_POS (6U)
#define DW_APB_SSI_SR_DCOL_BIT_MSK (1U << DW_APB_SSI_SR_DCOL_BIT_POS)

/* Interrupt bits shared by IMR, ISR, and RISR. */
#define DW_APB_SSI_INT_TXEI_BIT_POS (0U)
#define DW_APB_SSI_INT_TXEI_BIT_MSK (1U << DW_APB_SSI_INT_TXEI_BIT_POS)
#define DW_APB_SSI_INT_TXOI_BIT_POS (1U)
#define DW_APB_SSI_INT_TXOI_BIT_MSK (1U << DW_APB_SSI_INT_TXOI_BIT_POS)
#define DW_APB_SSI_INT_RXUI_BIT_POS (2U)
#define DW_APB_SSI_INT_RXUI_BIT_MSK (1U << DW_APB_SSI_INT_RXUI_BIT_POS)
#define DW_APB_SSI_INT_RXOI_BIT_POS (3U)
#define DW_APB_SSI_INT_RXOI_BIT_MSK (1U << DW_APB_SSI_INT_RXOI_BIT_POS)
#define DW_APB_SSI_INT_RXFI_BIT_POS (4U)
#define DW_APB_SSI_INT_RXFI_BIT_MSK (1U << DW_APB_SSI_INT_RXFI_BIT_POS)
#define DW_APB_SSI_INT_MSTI_BIT_POS (5U)
#define DW_APB_SSI_INT_MSTI_BIT_MSK (1U << DW_APB_SSI_INT_MSTI_BIT_POS)
#define DW_APB_SSI_INT_ALL_BIT_MSK  (0x3FU)

/* Interrupt clear registers and ICR return/clear bit. */
#define DW_APB_SSI_ICR_IC_BIT_POS (0U)
#define DW_APB_SSI_ICR_IC_BIT_MSK (1U << DW_APB_SSI_ICR_IC_BIT_POS)

/* DMACR */
#define DW_APB_SSI_DMACR_RDMAE_BIT_POS (0U)
#define DW_APB_SSI_DMACR_RDMAE_BIT_MSK (1U << DW_APB_SSI_DMACR_RDMAE_BIT_POS)
#define DW_APB_SSI_DMACR_TDMAE_BIT_POS (1U)
#define DW_APB_SSI_DMACR_TDMAE_BIT_MSK (1U << DW_APB_SSI_DMACR_TDMAE_BIT_POS)

/* DMATDLR and DMARDLR */
#define DW_APB_SSI_DMATDLR_DMATDL_BIT_POS (0U)
#define DW_APB_SSI_DMATDLR_DMATDL_BIT_MSK (0x1FU << DW_APB_SSI_DMATDLR_DMATDL_BIT_POS)
#define DW_APB_SSI_DMARDLR_DMARDL_BIT_POS (0U)
#define DW_APB_SSI_DMARDLR_DMARDL_BIT_MSK (0x1FU << DW_APB_SSI_DMARDLR_DMARDL_BIT_POS)

/* IDR, VERSION, and DR */
#define DW_APB_SSI_IDR_ID_BIT_POS          (0U)
#define DW_APB_SSI_IDR_ID_BIT_MSK          (0xFFFFFFFFU << DW_APB_SSI_IDR_ID_BIT_POS)
#define DW_APB_SSI_VERSION_VERSION_BIT_POS (0U)
#define DW_APB_SSI_VERSION_VERSION_BIT_MSK (0xFFFFFFFFU << DW_APB_SSI_VERSION_VERSION_BIT_POS)
#define DW_APB_SSI_DR_DR_BIT_POS           (0U)
#define DW_APB_SSI_DR_DR_BIT_MSK           (0xFFFFU << DW_APB_SSI_DR_DR_BIT_POS)

/* Optional RX_SAMPLE_DLY register. */
#define DW_APB_SSI_RX_SAMPLE_DLY_RSD_BIT_POS (0U)
#define DW_APB_SSI_RX_SAMPLE_DLY_RSD_BIT_MSK (0xFFU << DW_APB_SSI_RX_SAMPLE_DLY_RSD_BIT_POS)

/** @} */

#endif /* DW_APB_SSI_REGS__H */
