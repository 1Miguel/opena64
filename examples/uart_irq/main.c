/**
 * @brief Demonstrate UART PL011 interrupt based usage.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdio.h>
#include <stdint.h>

#include "opena64/gic.h"
#include "opena64/uart_pl011.h"

/* UART base address used by the a53_renode platform. */
#define UART0 (0x09000000)

/* Shared by the callback so it can report each timer tick over UART. */
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

/**
 * @brief weak interrupt handler implementation
 * that calls the uart_pl011 driver callback.
 */
void int_33_irq_handler(void)
{
  uart_pl011_irq_handler(&uart_dev);
}

/**
 * @brief app/user defined uart irq callback.
 */
void uart_cb(struct uart_pl011 *uart, void *cb_data)
{
	int n;
  uint8_t buf[32]; // intermediate buffer

  // check if there is any data in rx fifo, if there
  // is any, put  it in a buffer and push it in tx fifo
	if (uart_pl011_rx_fifo_ready(uart)) {
		n = uart_pl011_rx_fifo(uart, buf, sizeof(buf));
	}

  if (uart_pl011_tx_fifo_ready(uart)) {
		n = uart_pl011_tx_fifo(uart, buf, n);
  }
}

int main(void) {

  /* Set uart pl011 interrupt priority and enable it */
  gic_v3_int_set_priority(33, 0);
  gic_v3_int_enable(33);

  /* Initialize UART so the callback can report each
   * timer expiration. */
  struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
  uart_pl011_init(&uart_dev, &uart_cfg);
  uart_pl011_enable(&uart_dev);
  uart_pl011_set_irq_cb(&uart_dev, uart_cb, NULL);
  // this is a cli loopback test, we dont need a tx done
  // uart_pl011_tx_irq_enable(&uart_dev);

  printf("user start typing in terminal\n\r");

  // start receiving
  uart_pl011_rx_irq_enable(&uart_dev);
  while (1);

  return 0;
}

