/**
 * @brief 32-bit app.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdint.h>
#include <stdio.h>

#include "opena64/uart_pl011.h"

/* UART base address used by the a53_renode platform. */
#define UART0 (0x09000000)

/* Peripheral instances shared with the timer callback. */
static struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);

/* Route the C library's stdout writes through the PL011 polling transmitter. */
int _write(int file, char *ptr, int len) {
  (void)(file); // unused
  int i;
  for (i = 0; i < len; i++) {
    uart_pl011_poll_out(&uart_dev, (*ptr++));
  }
  return i;
}


int main(void)
{
	/* Initialize UART so the callback can report each timer expiration. */
	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

  printf("Hello from 32-bit mode!");

	while (1) {}

	return 0;
}

