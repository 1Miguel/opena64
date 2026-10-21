/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __DW_APB_SSI__H
#define __DW_APB_SSI__H

#include <stdint.h>
#include "dw_apb_ssi_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup dw_apb_ssi
 * @{
 *
 * @brief Bare-metal driver interface for the Synopsys DesignWare APB SSI.
 *
 * @note	None.
 *
 * modification history:
 *  1. mugsimar, 01/20/15
 *    > Initial version. No support for interrupts yet.
 */

/** @brief Select single-line SPI data transfers. */
#define DW_APB_SSI_LINE_SINGLE (0U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)
/** @brief Select dual-line SPI data transfers. */
#define DW_APB_SSI_LINE_DUAL   (1U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)
/** @brief Select quad-line SPI data transfers. */
#define DW_APB_SSI_LINE_QUAD   (2U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)

/**
 * @brief Encode an SPI mode (0-3) for the CTRLR0 CPHA and CPOL fields.
 *
 * Mode bit 0 selects CPHA and mode bit 1 selects CPOL.
 *
 * @param _mode SPI mode number in the range 0-3.
 */
#define DW_APB_SSI_SPI_MODE(_mode)                                                                 \
	(((_mode) << DW_APB_SSI_CTRLR0_SCPH_BIT_POS) &                                             \
	 (DW_APB_SSI_CTRLR0_SCPH_BIT_MSK | DW_APB_SSI_CTRLR0_SCPOL_BIT_MSK))

/** @brief Set CTRLR0 for slave operation; clear it for master operation. */
#define DW_APB_SSI_OP_MODE_SLAVE (1U << DW_APB_SSI_CTRLR0_SLV_OE_BIT_POS)

/** @brief Enable the controller's local loopback mode. */
#define DW_APB_SSI_LOOPBACK_MODE (1U << DW_APB_SSI_CTRLR0_SRL_BIT_POS)

#if DW_APB_SSI_DFS_32
/**
 * @brief Encode the data-frame size for a controller with the 32-bit DFS field.
 * @param n Frame size in bits.
 */
#define DW_APB_SSI_DFS(n)                                                                          \
	(((n) << DW_APB_SSI_CTRLR0_DFS_32_BIT_POS) & DW_APB_SSI_CTRLR0_DFS_32_BIT_MSK)
#else
/**
 * @brief Encode the data-frame size for the standard APB SSI DFS field.
 * @param n Frame size in bits (1-16); the register stores the size minus one.
 */
#define DW_APB_SSI_DFS(n)                                                                          \
	(((n - 1) << DW_APB_SSI_CTRLR0_DFS_BIT_POS) & DW_APB_SSI_CTRLR0_DFS_BIT_MSK)
#endif // DW_APB_SSI_DFS_32
       //
/** @brief Select Motorola SPI frame format. */
#define DW_APB_SSI_FRF_SPI       (0U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)
/** @brief Select TI SSP frame format. */
#define DW_APB_SSI_FRF_TI_SSP    (1U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)
/** @brief Select Microwire frame format. */
#define DW_APB_SSI_FRF_MICROWIRE (2U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)

struct dw_apb_ssi;

/**
 * @brief SSI interrupt callback function type.
 *
 * @param[in] ssi SSI controller instance that raised the interrupt.
 * @param[in] cb_data User-provided callback context.
 */
typedef void (*dw_apb_ssi_cb_t)(struct dw_apb_ssi *ssi, void *cb_data);

/**
 * @brief Runtime configuration for the SSI controller.
 *
 * The @p op_cfg field contains the already-encoded CTRLR0 configuration.
 * Combine the @c DW_APB_SSI_* field macros to construct it. The @p baudrate
 * field is reserved for the requested serial clock rate; support for applying
 * it depends on the controller configuration and driver implementation.
 */
struct dw_apb_ssi_cfg {
	/**
	 * Encoded CTRLR0 configuration.
	 *
	 * Example:
	 *
	 * struct dw_apb_ssi_cfg cfg = {
	 *     .op_cfg = DW_APB_SSI_SPI_MODE(1) | DW_APB_SSI_LINE_SINGLE |
	 *               DW_APB_SSI_DFS(8),
	 * };
	 */
	uint32_t op_cfg;   /**< Encoded control-register configuration. */
	uint8_t cs_sel;    /**< Chip select */
	uint32_t baudrate; /**< Requested serial clock frequency in hertz. */
};

/**
 * @brief State associated with one DesignWare APB SSI controller instance.
 */
struct dw_apb_ssi {
	struct dw_apb_ssi_cfg *cfg; /**< current spi transfer operation config */
	uintptr_t reg;              /**< Base address of the controller's registers. */
	dw_apb_ssi_cb_t cb;         /**< Optional interrupt callback. */
	void *cb_data;              /**< Context passed to @ref cb. */
};

/**
 * @brief Initialize an SSI instance with its register base address.
 *
 * @param _reg Base address of the SSI memory-mapped registers.
 */
#define DW_APB_SSI_INIT(_reg)                                                                      \
	{                                                                                          \
		.reg = _reg,                                                                       \
	}

/**
 * @brief Create a loopback-test configuration for master mode.
 *
 * The configuration selects single-line Motorola SPI, mode 1, 8-bit frames,
 * and local loopback.
 *
 * @param _baudrate Requested serial clock frequency in hertz.
 */
#define DW_APB_SSI_CFG_DEFAULT_TEST_MODE(_baudrate, _cs)                                           \
	{                                                                                          \
		.op_cfg = DW_APB_SSI_SPI_MODE(0) | DW_APB_SSI_LINE_SINGLE |                        \
			  DW_APB_SSI_LOOPBACK_MODE | DW_APB_SSI_DFS(8),                            \
		.cs_sel = _cs,                                                                     \
		.baudrate = _baudrate,                                                             \
	}

/**
 * @brief Initialize an SSI controller instance.
 *
 * Initialization is not implemented yet.
 *
 * @param[in] ssi SSI controller instance.
 * @return -ENOTSUP because initialization is not implemented.
 */
int dw_apb_ssi_init(struct dw_apb_ssi *ssi);

/**
 * @brief Apply an encoded configuration to an idle SSI controller.
 *
 * @param[in] ssi SSI controller instance.
 * @param[in]     cfg Configuration to apply.
 *
 * @retval 0 on success
 * @retval -EINVAL for invalid arguments
 * @retval -EBUSY if the controller is transferring data.
 */
int dw_apb_ssi_set_config(struct dw_apb_ssi *ssi, struct dw_apb_ssi_cfg *cfg);

/**
 * @brief Unmask the SSI transmit-empty interrupt.
 *
 * @param[in] ssi SSI controller instance.
 *
 * @retval 0 on success
 * @retval -EINVAL if @p ssi is null.
 */
int dw_apb_ssi_tx_irq_enable(struct dw_apb_ssi *ssi);

/**
 * @brief Unmask the SSI receive-full interrupt.
 *
 * @param[in] ssi SSI controller instance.
 *
 * @retval 0 on success
 * @retval -EINVAL if @p ssi is null.
 */
int dw_apb_ssi_rx_irq_enable(struct dw_apb_ssi *ssi);

/**
 * @brief Register the callback invoked by the SSI interrupt handler.
 *
 * @param[in] ssi SSI controller instance.
 * @param[in] cb Callback function.
 * @param[in] cb_data User context passed to @p cb.
 *
 * @retval 0 on success
 * @retval -errno at error
 */
int dw_apb_ssi_set_irq_cb(struct dw_apb_ssi *ssi, dw_apb_ssi_cb_t cb, void *cb_data);

/**
 * @brief Write up to @p len bytes to the SSI transmit FIFO.
 *
 * @param[in] ssi SSI controller instance.
 * @param[in] buf Data to transmit; NULL requests zero-valued data words.
 * @param[in] len Maximum number of bytes to write.
 *
 * @return Number of bytes copied from @p buf and queued
 * @retval 0 if @p buf is null
 * @retval -EINVAL if @p ssi is null.
 */
int dw_apb_ssi_tx_fifo_u8(struct dw_apb_ssi *ssi, uint8_t *buf, uint16_t len);

/**
 * @brief Read up to @p len bytes from the SSI receive FIFO.
 *
 * @param[in] ssi SSI controller instance.
 * @param[out] buf Destination buffer; NULL discards received data.
 * @param[in] len Maximum number of bytes to read.
 *
 * @return Number of bytes copied to @p buf
 * @retval 0 if @p buf is null
 * @retval -EINVAL if @p ssi is null.
 */
int dw_apb_ssi_rx_fifo_u8(struct dw_apb_ssi *ssi, uint8_t *buf, uint16_t len);

/**
 * @brief Perform a blocking, polling transfer of up to @p len bytes.
 *
 * Either buffer may be NULL. A NULL transmit buffer sends zero-valued words
 * during receive-only transfers; a NULL receive buffer discards received
 * words during transmit-only transfers.
 *
 * @param[in]  ssi SSI controller instance.
 * @param[in]  tx_buf Transmit data, or NULL when not transmitting.
 * @param[out] rx_buf Receive destination, or NULL when not receiving.
 * @param[in]  len Number of bytes to transfer.
 * @return 0 on success, or a negative error code.
 */
int dw_apb_ssi_tx_rx_poll_u8(struct dw_apb_ssi *ssi, uint8_t *tx_buf, uint8_t *rx_buf,
			     uint16_t len);

/**
 * @brief Perform a blocking, polling transmit-only operation.
 *
 * @param[in] ssi SSI controller instance.
 * @param[in] buf Transmit data.
 * @param[in] len Number of bytes to transmit.
 *
 * @retval 0 on success.
 * @retval -errno at error.
 */
static inline int dw_apb_ssi_tx_poll_u8(struct dw_apb_ssi *ssi, uint8_t *buf, uint16_t len)
{
	return dw_apb_ssi_tx_rx_poll_u8(ssi, buf, NULL, len);
}

/**
 * @brief Perform a blocking, polling receive-only operation.
 *
 * @param[in]  ssi SSI controller instance.
 * @param[out] buf Receive destination.
 * @param[in]  len Number of bytes to receive.
 *
 * @retval 0 on success.
 * @retval -errno at error.
 */
static inline int dw_apb_ssi_rx_poll_u8(struct dw_apb_ssi *ssi, uint8_t *buf, uint16_t len)
{
	return dw_apb_ssi_tx_rx_poll_u8(ssi, NULL, buf, len);
}

/**
 * @brief Dispatch an SSI interrupt to the registered callback.
 *
 * @param[in] ssi SSI controller instance associated with the interrupt.
 */
void dw_apb_irq_handler(struct dw_apb_ssi *ssi);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // __DW_APB_SSI__H
