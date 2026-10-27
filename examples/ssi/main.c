/**
 * @brief Demonstrate UART PL011 interrupt based usage.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdio.h>
#include <stdint.h>
#include "unity.h"
#include "opena64/uart_pl011.h"
#include "opena64/spi_dw.h"
#include "opena64/gic.h"

#define UART0 (0x09000000)
#define SPI0  (0x09020000)

struct spi_buf {
	uint8_t *buf;
	uint16_t len;
	uint16_t fill;
};

static struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);
static struct spi_dw ssi_dev = SPI_DW_INIT(SPI0);

int _write(int file, char *ptr, int len)
{
	(void)(file); // unused
	int i;
	for (i = 0; i < len; i++) {
		uart_pl011_poll_out(&uart_dev, (*ptr++));
	}
	return i;
}

void setUp(void)
{
	/* Unity calls this before each test; add per-test initialization here. */
}

void tearDown(void)
{
	/* Unity calls this after each test; add per-test cleanup here. */
}

void spi_irq_cb(struct spi_dw *ssi, void *cb_data)
{
	struct spi_buf *spi_buf = (struct spi_buf *)cb_data;

	if (spi_dw_rx_fifo_ready(ssi)) {
		spi_buf->fill += spi_dw_rx_fifo_u8(ssi, spi_buf->buf, spi_buf->len);
		spi_dw_tx_irq_disable(ssi);
	}
}

void test_spi_dw_irq_mode(void)
{
	uint8_t cmd[4] = {0x9F, 0, 0, 0}; // JEDEC Read id cmd
	uint8_t rsp[4] = {0};             // JEDEC read response buf
	struct spi_buf spi_buf = {.buf = rsp, .len = sizeof(rsp), .fill = 0};

	struct spi_dw_cfg ssi_cfg = SPI_DW_CFG_DEFAULT(1U, 1U);
	spi_dw_set_config(&ssi_dev, &ssi_cfg);
	spi_dw_set_irq_cb(&ssi_dev, spi_irq_cb, &spi_buf);

	// do not enable rx irq, since we're doing transceive
	// lets just wait for tx fifo to be empty, at that point
	// we expect the rx buffer has data
	// spi_dw_rx_irq_enable(&ssi_dev);
	spi_dw_tx_rx_async_u8(&ssi_dev, cmd, sizeof(cmd));
	spi_dw_tx_irq_enable(&ssi_dev);

	printf("start transfer!\n\r");

	TEST_ASSERT_GREATER_OR_EQUAL_INT(0, spi_dw_tx_rx_async_u8(&ssi_dev, cmd, sizeof(cmd)));

	while (!spi_buf.fill)
		;

	printf("transfer done!\n\r");
	printf("JEDEC ID:\n\r");
	printf("  manufacturer id: 0x%x\n\r", rsp[1]);
	printf("  memory type:     0x%x\n\r", rsp[2]);
	printf("  capacity id:     0x%x\n\r", rsp[3]);

	TEST_ASSERT_EQUAL_UINT8(0x52, rsp[1]);
	TEST_ASSERT_EQUAL_UINT8(0x8F, rsp[2]);
	TEST_ASSERT_EQUAL_UINT8(0x19, rsp[3]);
}

void test_spi_dw_polling_mode(void)
{
	uint8_t cmd[4] = {0x9F, 0, 0, 0}; // JEDEC Read id cmd
	uint8_t rsp[4] = {0};             // JEDEC read response buf

	struct spi_dw_cfg ssi_cfg = SPI_DW_CFG_DEFAULT(1U, 1U);
	spi_dw_set_config(&ssi_dev, &ssi_cfg);

	printf("start transfer!\n\r");

	TEST_ASSERT_GREATER_OR_EQUAL_INT(0, spi_dw_tx_rx_poll_u8(&ssi_dev, cmd, rsp, sizeof(cmd)));

	printf("transfer done!\n\r");
	printf("JEDEC ID:\n\r");
	printf("  manufacturer id: 0x%x\n\r", rsp[1]);
	printf("  memory type:     0x%x\n\r", rsp[2]);
	printf("  capacity id:     0x%x\n\r", rsp[3]);

	TEST_ASSERT_EQUAL_UINT8(0x52, rsp[1]);
	TEST_ASSERT_EQUAL_UINT8(0x8F, rsp[2]);
	TEST_ASSERT_EQUAL_UINT8(0x19, rsp[3]);
}

int main(void)
{
	gic_v3_int_set_priority(35, 0);
	gic_v3_int_enable(35);

	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);
	UNITY_BEGIN();
	RUN_TEST(test_spi_dw_polling_mode);
	RUN_TEST(test_spi_dw_irq_mode);
	return UNITY_END();
}

void int_35_irq_handler(void)
{
	spi_dw_irq_handler(&ssi_dev);
}
