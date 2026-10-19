/**
 * @brief A simple hello world, for aarch64 baremetal.
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdint.h>

#include "opena64/gic.h"
#include "opena64/timer.h"
#include "opena64/uart_pl011.h"

#define UART0 (0x09000000)

struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);

static void print(char *s)
{
  while (s++)
    uart_pl011_poll_out(&uart_dev, *s);
  
}

void timer_cb(void *cb_data)
{
	uint64_t *period_cnt = cb_data;
	print("timer tick!\n");
	timer_set_relative((uint32_t)*period_cnt);
}

int main(void)
{
  char *msg = "hello world!";
	uint64_t period_cnt = timer_ms_to_cnt(1000);

  // EL1 timer interrupts
	gic_v3_int_set_priority(30, 0);
	gic_v3_int_enable(30);

	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

	timer_set_irq_cb(timer_cb, &period_cnt);
	timer_start();

  print("timer start");
	timer_set_relative((uint32_t)period_cnt);

  while(1);

	return 0;
}
