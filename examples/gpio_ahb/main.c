/**
 * @brief Demonstrate periodic generic-timer interrupts with UART output.
 *
 * The timer callback prints a message and schedules the next timeout using
 * the same interval. The interrupt controller must allow the timer interrupt
 * through for the callback to run.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdint.h>
#include <stdio.h>
#include "opena64/gic.h"
#include "opena64/assert.h"
#include "opena64/timer.h"
#include "opena64/uart_pl011.h"
#include "opena64/gpio_arm_ahb.h"

/* UART base address used by the a53_renode platform. */
#define UART0 (0x09000000)
#define GPIO0 (0x09030000)

/* Shared by the callback so it can report each timer tick over UART. */
static struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);
static struct gpio_arm_ahb_port gpio_dev = GPIO_ARM_AHB_PORT_INIT(GPIO0);

/* Route the C library's stdout writes through the PL011 polling transmitter. */
int _write(int file, char *ptr, int len) {
  (void)(file); // unused
  int i;
  for (i = 0; i < len; i++) {
    uart_pl011_poll_out(&uart_dev, (*ptr++));
  }
  return i;
}


void timer_cb(void *cb_data)
{
	uint64_t *period_cnt = cb_data;
	printf("timer tick!\n\r");
	timer_set_relative((uint32_t)*period_cnt);
  gpio_arm_ahb_pin_set(&gpio_dev, 5);
}


int main(void)
{
	/* Convert the desired one-second period into generic-timer counter ticks. */
	uint64_t period_cnt = timer_ms_to_cnt(3000);

	/* Set the timer interrupt priority and enable its delivery through the GIC. */
	gic_v3_int_set_priority(30, 0);
	gic_v3_int_enable(30);

	/* Initialize UART so the callback can report each timer expiration. */
	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

	/* Initialize gpio to low. */
  gpio_arm_ahb_pin_cfg_t gpio_cfg = GPIO_ARM_AHB_OUTPUT_LOW;
  int ret = gpio_arm_ahb_set_port_cfg(&gpio_dev, 0xFFFF, &gpio_cfg);

	timer_set_irq_cb(timer_cb, &period_cnt);
	timer_start();
	timer_set_relative((uint32_t)period_cnt);

	/* Keep the program alive so it can continue servicing timer interrupts. */
	while (1) {
  }

	return 0;
}
