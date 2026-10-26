/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#include <stdbool.h>
#include <errno.h>
#include "mmio.h"
#include "dw_apb_ssi.h"
#include "dw_apb_ssi_regs.h"

static inline bool is_busy(struct dw_apb_ssi *ssi)
{
	return mmio_bit_get_u32(DW_APB_SSI_SR_OFFSET(ssi->reg), DW_APB_SSI_SR_BUSY_BIT_MSK);
}

static inline bool is_rx_fifo_ready(struct dw_apb_ssi *ssi)
{
	return mmio_bit_get_u32(DW_APB_SSI_SR_OFFSET(ssi->reg), DW_APB_SSI_SR_RFNE_BIT_MSK);
}

static inline bool is_tx_fifo_empty(struct dw_apb_ssi *ssi)
{
	return mmio_bit_get_u32(DW_APB_SSI_SR_OFFSET(ssi->reg), DW_APB_SSI_SR_TFE_BIT_MSK);
}

static inline bool is_tx_fifo_ready(struct dw_apb_ssi *ssi)
{
	return mmio_bit_get_u32(DW_APB_SSI_SR_OFFSET(ssi->reg), DW_APB_SSI_SR_TFNF_BIT_MSK);
}

static inline void set_transfer_mode(struct dw_apb_ssi *ssi, uint32_t mode)
{
	mmio_bit_clr_u32(DW_APB_SSI_CTRLR0_OFFSET(ssi->reg), DW_APB_SSI_CTRLR0_TMOD_BIT_MSK);
	mmio_bit_set_u32(DW_APB_SSI_CTRLR0_OFFSET(ssi->reg), mode);
}

static inline void ssi_enable(struct dw_apb_ssi *ssi)
{
	mmio_bit_set_u32(DW_APB_SSI_SSIENR_OFFSET(ssi->reg), DW_APB_SSI_SSIENR_SSI_EN_BIT_MSK);
}

static inline void ssi_disable(struct dw_apb_ssi *ssi)
{
	mmio_bit_clr_u32(DW_APB_SSI_SSIENR_OFFSET(ssi->reg), DW_APB_SSI_SSIENR_SSI_EN_BIT_MSK);
}

static inline void ssi_slave_sel(struct dw_apb_ssi *ssi, uint8_t ssel)
{
	mmio_write_u32(DW_APB_SSI_SER_OFFSET(ssi->reg), ssel);
}

int dw_apb_ssi_init(struct dw_apb_ssi *ssi)
{
	return -ENOTSUP;
}

int dw_apb_ssi_set_config(struct dw_apb_ssi *ssi, struct dw_apb_ssi_cfg *cfg)
{
	if (!ssi || !cfg) {
		return -EINVAL;
	}

	if (is_busy(ssi)) {
		return -EBUSY;
	}

	ssi->cfg = cfg;

	ssi_disable(ssi);

	mmio_write_u32(DW_APB_SSI_CTRLR0_OFFSET(ssi->reg), cfg->op_cfg);
	mmio_write_u32(DW_APB_SSI_BAUDR_OFFSET(ssi->reg), cfg->baudrate);
	mmio_write_u32(DW_APB_SSI_TXFTLR_OFFSET(ssi->reg), 1U);
	mmio_write_u32(DW_APB_SSI_RXFTLR_OFFSET(ssi->reg), 1U);

	return 0;
}

int dw_apb_ssi_rx_irq_enable(struct dw_apb_ssi *ssi)
{
	if (!ssi) {
		return -EINVAL;
	}

	mmio_bit_clr_u32(DW_APB_SSI_IMR_OFFSET(ssi->reg), DW_APB_SSI_INT_RXFI_BIT_MSK);

	return 0;
}

int dw_apb_ssi_tx_irq_enable(struct dw_apb_ssi *ssi)
{
	if (!ssi) {
		return -EINVAL;
	}

	mmio_bit_clr_u32(DW_APB_SSI_IMR_OFFSET(ssi->reg), DW_APB_SSI_INT_TXEI_BIT_MSK);

	return 0;
}

int dw_apb_ssi_tx_fifo_u8(struct dw_apb_ssi *ssi, uint8_t *buf, uint16_t len)
{
	int n = 0;

	if (!ssi) {
		return -EINVAL;
	}

	if (!len) {
		return 0;
	}

	while (len--) {
		if (!mmio_bit_get_u32(DW_APB_SSI_SR_OFFSET(ssi->reg), DW_APB_SSI_SR_TFNF_BIT_MSK)) {
			break; // fifo full
		}
		mmio_write_u32(DW_APB_SSI_DR_OFFSET(ssi->reg), buf ? *(buf + n) : 0);
		n++;
	}

	if (n) {
		ssi_slave_sel(ssi, ssi->cfg->cs_sel);
	}

	return n;
}

int dw_apb_ssi_rx_fifo_u8(struct dw_apb_ssi *ssi, uint8_t *buf, uint16_t len)
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
		b = mmio_read_u32(DW_APB_SSI_DR_OFFSET(ssi->reg));
		if (buf) {
			*(buf + n) = b;
		}
		n++;
	}

	return n;
}

static inline void set_num_frames(struct dw_apb_ssi *ssi, uint16_t n_fr)
{
	mmio_write_u16(DW_APB_SSI_CTRLR1_OFFSET(ssi->reg), n_fr - 1);
}

int dw_apb_ssi_tx_rx_async_u8(struct dw_apb_ssi *ssi, uint8_t *tx_buf, uint16_t len)
{
	uint16_t n = 0;

	if (!ssi && !tx_buf && !rx_buf) {
		return -EINVAL;
	}

	if (!is_tx_fifo_ready(ssi) || is_busy(ssi)) {
		return -EBUSY;
	}

	ssi_disable(ssi);
	set_transfer_mode(ssi, DW_APB_SSI_CTRLR0_TMOD_TX_RX);
	ssi_enable(ssi);
	n = dw_apb_ssi_tx_fifo_u8(ssi, tx_buf, len);
	ssi_slave_sel(ssi, ssi->cfg->cs_sel);

	return n;
}

int dw_apb_ssi_tx_async_stop(struct dw_apb_ssi *ssi)
{
	ssi_slave_sel(ssi, 0);
	ssi_disable(ssi);
}

int dw_apb_ssi_tx_rx_poll_u8(struct dw_apb_ssi *ssi, uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len)
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
				n_tx += dw_apb_ssi_tx_fifo_u8(ssi, (tx_buf + n_tx), (len - n_tx));
			}

			ssi_slave_sel(ssi, ssi->cfg->cs_sel);

		} else {
			// rx only, write dummy data in DR only if rx fifo
			// is ready to be read
			if (is_rx_fifo_ready(ssi)) {
				mmio_write_u32(DW_APB_SSI_DR_OFFSET(ssi->reg), 0);
			}
		}

		if (rx_buf) {
			n_rx += dw_apb_ssi_rx_fifo_u8(ssi, (rx_buf + n_rx), (len - n_rx));
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
