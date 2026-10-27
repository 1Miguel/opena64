/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#include <stdbool.h>
#include <errno.h>
#include "mmio.h"
#include "spi_dw.h"
#include "spi_dw_regs.h"

static inline bool is_busy(struct spi_dw *ssi)
{
	return mmio_bit_get_u32(DW_APB_SSI_SR(ssi->reg), DW_APB_SSI_SR_BUSY_BIT_MSK);
}

static inline bool is_rx_fifo_ready(struct spi_dw *ssi)
{
	return mmio_bit_get_u32(DW_APB_SSI_SR(ssi->reg), DW_APB_SSI_SR_RFNE_BIT_MSK);
}

static inline bool is_tx_fifo_empty(struct spi_dw *ssi)
{
	return mmio_bit_get_u32(DW_APB_SSI_SR(ssi->reg), DW_APB_SSI_SR_TFE_BIT_MSK);
}

static inline bool is_tx_fifo_ready(struct spi_dw *ssi)
{
	return mmio_bit_get_u32(DW_APB_SSI_SR(ssi->reg), DW_APB_SSI_SR_TFNF_BIT_MSK);
}

static inline void set_transfer_mode(struct spi_dw *ssi, uint32_t mode)
{
	mmio_bit_clr_u32(DW_APB_SSI_CTRLR0(ssi->reg), DW_APB_SSI_CTRLR0_TMOD_BIT_MSK);
	mmio_bit_set_u32(DW_APB_SSI_CTRLR0(ssi->reg), mode);
}

static inline void ssi_enable(struct spi_dw *ssi)
{
	mmio_bit_set_u32(DW_APB_SSI_SSIENR(ssi->reg), DW_APB_SSI_SSIENR_SSI_EN_BIT_MSK);
}

static inline void ssi_disable(struct spi_dw *ssi)
{
	mmio_bit_clr_u32(DW_APB_SSI_SSIENR(ssi->reg), DW_APB_SSI_SSIENR_SSI_EN_BIT_MSK);
}

static inline void ssi_slave_sel(struct spi_dw *ssi, uint8_t ssel)
{
	mmio_write_u32(DW_APB_SSI_SER(ssi->reg), ssel);
}

int spi_dw_init(struct spi_dw *ssi)
{
	return -ENOTSUP;
}

int spi_dw_set_config(struct spi_dw *ssi, struct spi_dw_cfg *cfg)
{
	if (!ssi || !cfg) {
		return -EINVAL;
	}

	if (is_busy(ssi)) {
		return -EBUSY;
	}

	ssi->cfg = cfg;

	ssi_disable(ssi);

	mmio_write_u32(DW_APB_SSI_CTRLR0(ssi->reg), cfg->op_cfg);
	mmio_write_u32(DW_APB_SSI_BAUDR(ssi->reg), cfg->baudrate);
	mmio_write_u32(DW_APB_SSI_TXFTLR(ssi->reg), 1U);
	mmio_write_u32(DW_APB_SSI_RXFTLR(ssi->reg), 1U);

	return 0;
}

int spi_dw_rx_irq_enable(struct spi_dw *ssi)
{
	if (!ssi) {
		return -EINVAL;
	}
	mmio_bit_set_u32(DW_APB_SSI_IMR(ssi->reg), DW_APB_SSI_INT_RXFI_BIT_MSK);
	return 0;
}

int spi_dw_rx_irq_disable(struct spi_dw *ssi)
{
	if (!ssi) {
		return -EINVAL;
	}
	mmio_bit_clr_u32(DW_APB_SSI_IMR(ssi->reg), DW_APB_SSI_INT_RXFI_BIT_MSK);
	return 0;
}

int spi_dw_rx_fifo_ready(struct spi_dw *ssi)
{
	if (!ssi) {
		return -EINVAL;
	}
	return (int)is_rx_fifo_ready(ssi);
}

int spi_dw_tx_fifo_ready(struct spi_dw *ssi)
{
	if (!ssi) {
		return -EINVAL;
	}
	return (int)is_tx_fifo_ready(ssi);
}

int spi_dw_tx_irq_enable(struct spi_dw *ssi)
{
	if (!ssi) {
		return -EINVAL;
	}
	mmio_bit_set_u32(DW_APB_SSI_IMR(ssi->reg), DW_APB_SSI_INT_TXEI_BIT_MSK);
	return 0;
}

int spi_dw_tx_irq_disable(struct spi_dw *ssi)
{
	if (!ssi) {
		return -EINVAL;
	}
	mmio_bit_clr_u32(DW_APB_SSI_IMR(ssi->reg), DW_APB_SSI_INT_TXEI_BIT_MSK);
	return 0;
}

int spi_dw_tx_fifo_u8(struct spi_dw *ssi, uint8_t *buf, uint16_t len)
{
	int n = 0;

	if (!ssi) {
		return -EINVAL;
	}

	if (!len) {
		return 0;
	}

	while (len--) {
		if (!mmio_bit_get_u32(DW_APB_SSI_SR(ssi->reg), DW_APB_SSI_SR_TFNF_BIT_MSK)) {
			break; // fifo full
		}
		mmio_write_u32(DW_APB_SSI_DR(ssi->reg), buf ? *(buf + n) : 0);
		n++;
	}

	if (n) {
		ssi_slave_sel(ssi, ssi->cfg->cs_sel);
	}

	return n;
}

int spi_dw_rx_fifo_u8(struct spi_dw *ssi, uint8_t *buf, uint16_t len)
{
	int n = 0;
	uint8_t b;

	if (!ssi) {
		return -EINVAL;
	}

	if (!len) {
		return 0;
	}

	while (len--) {
		if (!is_rx_fifo_ready(ssi)) {
			break; // rx fifo empty
		}
		b = mmio_read_u32(DW_APB_SSI_DR(ssi->reg));
		if (buf) {
			*(buf + n) = b;
		}
		n++;
	}

	return n;
}

int spi_dw_set_irq_cb(struct spi_dw *ssi, spi_dw_cb_t cb, void *cb_data)
{
	if (!ssi) {
		return -EINVAL;
	}
	ssi->cb = cb;
	ssi->cb_data = cb_data;
	return 0;
}

static inline void set_num_frames(struct spi_dw *ssi, uint16_t n_fr)
{
	mmio_write_u16(DW_APB_SSI_CTRLR1(ssi->reg), n_fr - 1);
}

static int start_async_transaction_u8(struct spi_dw *ssi, uint8_t *tx_buf, uint16_t len,
				      uint32_t mode)
{
	uint16_t n = 0;

	if (!ssi || !len) {
		return -EINVAL;
	}

	if (!is_tx_fifo_ready(ssi) || is_busy(ssi)) {
		return -EBUSY;
	}

	ssi_disable(ssi);
	set_transfer_mode(ssi, mode);

	if (mode == DW_APB_SSI_CTRLR0_TMOD_RX) {
		set_num_frames(ssi, len);
	}

	ssi_enable(ssi);
	n = spi_dw_tx_fifo_u8(ssi, tx_buf, len);
	ssi_slave_sel(ssi, ssi->cfg->cs_sel);

	return n;
}

int spi_dw_tx_rx_async_u8(struct spi_dw *ssi, uint8_t *tx_buf, uint16_t len)
{
	return start_async_transaction_u8(ssi, tx_buf, len, DW_APB_SSI_CTRLR0_TMOD_TX_RX);
}

int spi_dw_tx_async_u8(struct spi_dw *ssi, uint8_t *tx_buf, uint16_t len)
{
	if (!tx_buf) {
		return -EINVAL;
	}
	return start_async_transaction_u8(ssi, tx_buf, len, DW_APB_SSI_CTRLR0_TMOD_TX);
}

int spi_dw_rx_async_u8(struct spi_dw *ssi, uint16_t len)
{
	return start_async_transaction_u8(ssi, NULL, len, DW_APB_SSI_CTRLR0_TMOD_RX);
}

int spi_dw_tx_async_stop(struct spi_dw *ssi)
{
	ssi_slave_sel(ssi, 0);
	ssi_disable(ssi);
}

int spi_dw_tx_rx_poll_u8(struct spi_dw *ssi, uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len)
{
	uint16_t tx_len;
	uint16_t rx_len;
	uint16_t n_rx;
	uint16_t n_tx;

	if (!ssi && !tx_buf && !rx_buf) {
		return -EINVAL;
	}

	if (!is_tx_fifo_ready(ssi) || is_busy(ssi)) {
		return -EBUSY;
	}

	ssi_disable(ssi);

	if (tx_buf && rx_buf) {
		tx_len = len;
		rx_len = len;
		set_transfer_mode(ssi, DW_APB_SSI_CTRLR0_TMOD_TX_RX);
	} else if (tx_buf) {
		rx_len = 0;
		set_transfer_mode(ssi, DW_APB_SSI_CTRLR0_TMOD_TX);
	} else if (rx_buf) {
		tx_len = 0;
		set_transfer_mode(ssi, DW_APB_SSI_CTRLR0_TMOD_RX);
		set_num_frames(ssi, len);
	}

	n_rx = 0;
	n_tx = 0;

	ssi_enable(ssi);

	while ((n_rx < rx_len) || (n_tx < tx_len)) {

		if (tx_buf) {

			if (is_tx_fifo_ready(ssi)) {
				n_tx += spi_dw_tx_fifo_u8(ssi, (tx_buf + n_tx), (len - n_tx));
			}

			ssi_slave_sel(ssi, ssi->cfg->cs_sel);

		} else {
			// rx only, write dummy data in DR only if rx fifo
			// is ready to be read
			if (is_rx_fifo_ready(ssi)) {
				mmio_write_u32(DW_APB_SSI_DR(ssi->reg), 0);
			}
		}

		if (rx_buf) {
			n_rx += spi_dw_rx_fifo_u8(ssi, (rx_buf + n_rx), (len - n_rx));
		}
	}

	// wait until spi is not busy
	while (is_busy(ssi))
		;

	ssi_slave_sel(ssi, 0);
	// then make sure ssi is disabled
	ssi_disable(ssi);

	return 0;
}

int spi_dw_err_chk(struct spi_dw *ssi)
{
	if (ssi) {
		return mmio_read_u32(DW_APB_SSI_RISR(ssi->reg));
	}
	return -EINVAL;
}

void spi_dw_irq_handler(struct spi_dw *ssi)
{
	uint32_t isr;

	isr = mmio_bit_get_u32(DW_APB_SSI_ISR(ssi->reg), DW_APB_SSI_INT_ALL_BIT_MSK);
	if (!isr) {
		return;
	}

	if (ssi->cb) {
		ssi->cb(ssi, ssi->cb_data);
	}
}
