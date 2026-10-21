/**
 * @brief Demonstrate UART PL011 interrupt based usage.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdio.h>
#include <stdint.h>

#include "opena64/uart_pl011.h"
#include "opena64/dw_apb_ssi.h"

#define UART0 (0x09000000)
#define SPI0  (0x09020000)

static struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);
static struct dw_apb_ssi ssi_dev = DW_APB_SSI_INIT(SPI0);

int _write(int file, char *ptr, int len)
{
	(void)(file); // unused
	int i;
	for (i = 0; i < len; i++) {
		uart_pl011_poll_out(&uart_dev, (*ptr++));
	}
	return i;
}

int main(void)
{

	uint8_t cmd[4] = {0x9F, 0, 0, 0}; // JEDEC Read id cmd
	uint8_t rsp[4] = {0};             // JEDEC read response buf

	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

	printf("start test\n\r");
	printf("read JEDEC ID\n\r");

	struct dw_apb_ssi_cfg ssi_cfg = DW_APB_SSI_CFG_DEFAULT_TEST_MODE(1U, 1U);
	dw_apb_ssi_set_config(&ssi_dev, &ssi_cfg);

	int ret = dw_apb_ssi_tx_rx_poll_u8(&ssi_dev, cmd, rsp, sizeof(cmd));
	if (ret < 0) {
		printf("(err %d) dw_apb_ssi_tx_rx_poll_u8\n\r", ret);
		return -1;
	}

	printf("transfer done!\n\n\r");

	printf("JEDEC ID:\n\r");
	printf("  manufacturer id: 0x%x\n\r", rsp[1]);
	printf("  memory type:     0x%x\n\r", rsp[2]);
	printf("  capacity id:     0x%x\n\r", rsp[3]);

	while (1)
		;

	return 0;
}
