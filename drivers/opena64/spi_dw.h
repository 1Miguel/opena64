/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __SPI_DW__H
#define __SPI_DW__H

#include <stdint.h>
#include <string.h>
#include "spi_dw_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup spi_dw
 * @{
 *
 * @brief Bare-metal driver interface for the Synopsys DesignWare APB SSI.
 *
 * @note	None.
 *
 * modification history:
 *  1. mugsimar, oct 20 2026
 *    > Initial version. No support for interrupts yet.
 */

/** @brief Select single-line SPI data transfers. */
#define SPI_DW_LINE_SINGLE (0U << DW_APB_SSI_CTRLR0_FRF_BIT_POS)
/** @brief Select dual-line SPI data transfers. */
#define SPI_DW_LINE_DUAL   (1U << DW_APB_SSI_CTRLR0_FRF_BIT_POS)
/** @brief Select quad-line SPI data transfers. */
#define SPI_DW_LINE_QUAD   (2U << DW_APB_SSI_CTRLR0_FRF_BIT_POS)

/**
 * @brief Encode an SPI mode (0-3) for the CTRLR0 CPHA and CPOL fields.
 *
 * Mode bit 0 selects CPHA and mode bit 1 selects CPOL.
 *
 * @param _mode SPI mode number in the range 0-3.
 */
#define SPI_DW_SPI_MODE(_mode)                                                                 \
	(((_mode) << DW_APB_SSI_CTRLR0_SCPH_BIT_POS) &                                             \
	 (DW_APB_SSI_CTRLR0_SCPH_BIT_MSK | DW_APB_SSI_CTRLR0_SCPOL_BIT_MSK))

/** @brief Set CTRLR0 for slave operation; clear it for master operation. */
#define SPI_DW_OP_MODE_SLAVE (1U << DW_APB_SSI_CTRLR0_SLV_OE_BIT_POS)

/** @brief Enable the controller's local loopback mode. */
#define SPI_DW_LOOPBACK_MODE (1U << DW_APB_SSI_CTRLR0_SRL_BIT_POS)

#if SPI_DW_DFS_32
/**
 * @brief Encode the data-frame size for a controller with the 32-bit DFS field.
 * @param n Frame size in bits.
 */
#define SPI_DW_DFS(n)                                                                          \
	(((n) << SPI_DW_CTRLR0_DFS_32_BIT_POS) & SPI_DW_CTRLR0_DFS_32_BIT_MSK)
#else
/**
 * @brief Encode the data-frame size for the standard APB SSI DFS field.
 * @param n Frame size in bits (1-16); the register stores the size minus one.
 */
#define SPI_DW_DFS(n)                                                                          \
	(((n - 1) << DW_APB_SSI_CTRLR0_DFS_BIT_POS) & DW_APB_SSI_CTRLR0_DFS_BIT_MSK)
#endif // SPI_DW_DFS_32
       //
/** @brief Select Motorola SPI frame format. */
#define SPI_DW_FRF_SPI       (0U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)
/** @brief Select TI SSP frame format. */
#define SPI_DW_FRF_TI_SSP    (1U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)
/** @brief Select Microwire frame format. */
#define SPI_DW_FRF_MICROWIRE (2U << DW_APB_SSI_CTRLR0_SPI_FRF_BIT_POS)

struct spi_dw;

/**
 * @brief SSI interrupt callback function type.
 *
 * @param[in] ssi SSI controller instance that raised the interrupt.
 * @param[in] cb_data User-provided callback context.
 */
typedef void (*spi_dw_cb_t)(struct spi_dw *ssi, void *cb_data);

/**
 * @brief Runtime configuration for the SSI controller.
 *
 * The @p op_cfg field contains the already-encoded CTRLR0 configuration.
 * Combine the @c SPI_DW_* field macros to construct it. The @p baudrate
 * field is reserved for the requested serial clock rate; support for applying
 * it depends on the controller configuration and driver implementation.
 */
struct spi_dw_cfg {
	/**
	 * Encoded CTRLR0 configuration.
	 *
	 * Example:
	 *
	 * struct spi_dw_cfg cfg = {
	 *     .op_cfg = SPI_DW_SPI_MODE(1) | _LINE_SINGLE |
	 *               SPI_DW_DFS(8),
	 * };
	 */
	uint32_t op_cfg;   /**< Encoded control-register configuration. */
	uint8_t cs_sel;    /**< Chip select */
	uint32_t baudrate; /**< Requested serial clock frequency in hertz. */
};

/**
 * @brief State associated with one DesignWare APB SSI controller instance.
 */
struct spi_dw {
	struct spi_dw_cfg *cfg; /**< current spi transfer operation config */
	uintptr_t reg;              /**< Base address of the controller's registers. */
	spi_dw_cb_t cb;         /**< Optional interrupt callback. */
	void *cb_data;              /**< Context passed to @ref cb. */
};

/**
 * @brief SPI DW error masks.
 *
 */
enum spi_dw_err_msk {
  SPI_DW_TX_FIFO_EMPTY = 1U << 0,
  SPI_DW_TX_FIFO_OVR = 1U << 1,
  SPI_DW_RX_FIFO_UDR = 1U << 2,
  SPI_DW_RX_FIFO_OVR = 1U << 3,
  SPI_DW_RX_FIFO_FULL = 1U << 4,
};

/**
 * @brief Initialize an SSI instance with its register base address.
 *
 * @param _reg Base address of the SSI memory-mapped registers.
 */
#define SPI_DW_INIT(_reg)                                                                      \
	{                                                                                          \
		.reg = _reg,                                                                       \
	}

/**
 * @brief Default test configuration.
 *
 * The configuration selects single-line Motorola SPI, mode 1, 8-bit frames,
 * and local loopback.
 *
 * @param _baudrate Requested serial clock frequency in hertz.
 * @param _cs       Chip select pin selection
 */
#define SPI_DW_CFG_DEFAULT(_baudrate, _cs)                                           \
	{                                                                                          \
		.op_cfg = SPI_DW_SPI_MODE(0) | SPI_DW_LINE_SINGLE |                        \
			  SPI_DW_FRF_SPI | SPI_DW_DFS(8),                            \
		.cs_sel = _cs,                                                                     \
		.baudrate = _baudrate,                                                             \
	}

/**
 * @brief Create a loopback-test configuration for master mode.
 *
 * The configuration selects single-line Motorola SPI, mode 1, 8-bit frames,
 * and local loopback.
 *
 * @param _baudrate Requested serial clock frequency in hertz.
 * @param _cs       Chip select pin selection
 */
#define SPI_DW_CFG_DEFAULT_TEST_MODE(_baudrate, _cs)                                           \
	{                                                                                          \
		.op_cfg = SPI_DW_SPI_MODE(0) | SPI_DW_LINE_SINGLE |                        \
			  SPI_DW_FRF_SPI | SPI_DW_LOOPBACK_MODE | SPI_DW_DFS(8),                            \
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
int spi_dw_init(struct spi_dw *ssi);

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
int spi_dw_set_config(struct spi_dw *ssi, struct spi_dw_cfg *cfg);

/**
 * @brief Enable transmit interrupts.
 *
 * @param[in] ssi SSI controller instance.
 *
 * @retval 0 on success
 * @retval -EINVAL if @p ssi is null.
 */
int spi_dw_tx_irq_enable(struct spi_dw *ssi);

/**
 * @brief Disable transmit interrupts.
 *
 * @param[in] ssi SSI controller instance.
 *
 * @retval 0 on success
 * @retval -EINVAL if @p ssi is null.
 */
int spi_dw_tx_irq_disable(struct spi_dw *ssi);

/**
 * @brief Enable receive interrupts.
 *
 * @param[in] ssi SSI controller instance.
 *
 * @retval 0 on success
 * @retval -EINVAL if @p ssi is null.
 */
int spi_dw_rx_irq_enable(struct spi_dw *ssi);

/**
 * @brief Disable receive interrupts.
 *
 * @param[in] ssi SSI controller instance.
 *
 * @retval 0 on success
 * @retval -EINVAL if @p ssi is null.
 */
int spi_dw_rx_irq_disable(struct spi_dw *ssi);

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
int spi_dw_set_irq_cb(struct spi_dw *ssi, spi_dw_cb_t cb, void *cb_data);

/**
 * @brief Write up to @p len bytes to the SSI transmit FIFO.
 *
 * @note This is expected to be called on IRQ callback ONLY.
 * behaviour is unknown when called at sync code.
 *
 * @param[in] ssi SSI controller instance.
 * @param[in] buf Data to transmit; NULL requests zero-valued data words.
 * @param[in] len Maximum number of bytes to write.
 *
 * @return Number of bytes copied from @p buf and queued
 * @retval 0 if @p buf is null
 * @retval -EINVAL if @p ssi is null.
 */
int spi_dw_tx_fifo_u8(struct spi_dw *ssi, uint8_t *buf, uint16_t len);

/**
 * @brief Check if rx fifo is ready to be read.
 *
 * @note This is expected to be called on IRQ callback ONLY.
 * behaviour is unknown when called at sync code.
 *
 * @param[in] ssi SSI controller instance.
 *
 * @return 1 if rx fifo has data to be read.
 * @retval -EINVAL if @p ssi is null.
 */
int spi_dw_rx_fifo_ready(struct spi_dw *ssi);

/**
 * @brief Check if tx fifo is ready to be written.
 *
 * @note This is expected to be called on IRQ callback ONLY.
 * behaviour is unknown when called at sync code.
 *
 * @param[in] ssi SSI controller instance.
 *
 * @return 1 if rx fifo has space to be written.
 * @retval -EINVAL if @p ssi is null.
 */
int spi_dw_tx_fifo_ready(struct spi_dw *ssi);

/**
 * @brief Read up to @p len bytes from the SSI receive FIFO.
 *
 * @note This is expected to be called on IRQ callback ONLY.
 * behaviour is unknown when called at sync code.
 *
 * @param[in]  ssi SSI controller instance.
 * @param[out] buf Destination buffer; NULL discards received data.
 * @param[in]  len Maximum number of bytes to read.
 *
 * @return Number of bytes copied to @p buf
 * @retval 0 if @p buf is null
 * @retval -EINVAL if @p ssi is null.
 */
int spi_dw_rx_fifo_u8(struct spi_dw *ssi, uint8_t *buf, uint16_t len);

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
int spi_dw_tx_rx_poll_u8(struct spi_dw *ssi, uint8_t *tx_buf, uint8_t *rx_buf,
			     uint16_t len);

/**
 * @brief Start the async/irq-based spi transmit receive.
 *
 * @param[in]  ssi SSI controller instance.
 * @param[in]  tx_buf Transmit data, or NULL when not transmitting.
 * @param[in]  len Number of bytes to transfer.
 *
 * @retval 0 on success
 * @retval -errno at error.
 */
int spi_dw_tx_rx_async_u8(struct spi_dw *ssi, uint8_t *tx_buf, uint16_t len);

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
static inline int spi_dw_tx_poll_u8(struct spi_dw *ssi, uint8_t *buf, uint16_t len)
{
	return spi_dw_tx_rx_poll_u8(ssi, buf, NULL, len);
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
static inline int spi_dw_rx_poll_u8(struct spi_dw *ssi, uint8_t *buf, uint16_t len)
{
	return spi_dw_tx_rx_poll_u8(ssi, NULL, buf, len);
}

/**
 * @brief Check spi errors.
 *
 * @retval 0 no error.
 * @retval err error flags defined in @ref spi_dw_err_msk.
 * @retval -EINVAL invalid args
 */
int spi_dw_err_chk(struct spi_dw *ssi);

/**
 * @brief Dispatch an SSI interrupt to the registered callback.
 *
 * @param[in] ssi SSI controller instance associated with the interrupt.
 */
void spi_dw_irq_handler(struct spi_dw *ssi);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // __SPI_DW__H
