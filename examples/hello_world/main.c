/**
 * @brief Print a message over the PL011 UART on a bare-metal AArch64 target.
 *
 * This example shows the basic UART setup sequence: describe the UART's
 * memory-mapped register base, initialize it with the default serial settings,
 * enable it, and transmit "hello world!" using polling. It targets the
 * a53_renode platform, where PL011 UART0 is mapped at 0x09000000; observe the
 * output on the emulated UART console.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdint.h>

#include "opena64/uart_pl011.h"

/* UART base address used by the a53_renode platform. */
#define UART0 (0x09000000)

int main(void)
{
	char *msg = "hello world!";

	/* Bind the PL011 driver instance to the platform's UART registers. */
	struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);
	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;

	/* Configure and enable the UART before sending data. */
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

	/* Poll the transmitter one character at a time until the string ends. */
	char *p = msg;

	while (*p) {
		uart_pl011_poll_out(&uart_dev, *p++);
	}

	/* No more output is needed, so disable the UART before returning. */
	uart_pl011_disable(&uart_dev);

	return 0;
}
