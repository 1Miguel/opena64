/**
 * @brief A simple hello world, for aarch64 baremetal.
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdint.h>

#include "opena64/uart_pl011.h"

#define UART0 (0x09000000)

int main(void)
{
  char *msg = "hello world!";

  struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);
	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;

	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

  char *p = msg;

  do {
    uart_pl011_poll_out(&uart_dev, *p);
  } while (p++);

  // do nothing
  uart_pl011_disable(&uart_dev);

	return 0;
}
