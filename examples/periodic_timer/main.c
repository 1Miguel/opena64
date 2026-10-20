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

#include "opena64/gic.h"
#include "opena64/timer.h"
#include "opena64/uart_pl011.h"

/* UART base address used by the a53_renode platform. */
#define UART0 (0x09000000)

/* Shared by the callback so it can report each timer tick over UART. */
static struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);

/* Minimal polling output helper used by the interrupt callback. */
static void print(char *s) {
  while (*s)
    uart_pl011_poll_out(&uart_dev, *s++);
}

/* Runs when the generic timer interrupt expires. */
void timer_cb(void *cb_data) {
  /* cb_data points to the interval value kept alive in main(). */
  uint64_t *period_cnt = cb_data;
  print("timer tick!\n\r");
  /* Schedule another timeout relative to the current timer count. */
  timer_set_relative((uint32_t)*period_cnt);
}

int main(void) {
  /* Convert the desired one-second period into generic-timer counter ticks. */
  uint64_t period_cnt = timer_ms_to_cnt(1000);

  /* Set the timer interrupt priority and enable its delivery through the GIC.
   */
  gic_v3_int_set_priority(30, 0);
  gic_v3_int_enable(30);

  /* Initialize UART so the callback can report each timer expiration. */
  struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
  uart_pl011_init(&uart_dev, &uart_cfg);
  uart_pl011_enable(&uart_dev);

  print("hello! timer start\n\r");
  /* Register the callback and its interval, then start the first period. */
  timer_set_irq_cb(timer_cb, &period_cnt);
  timer_start();
  timer_set_relative((uint32_t)period_cnt);

  /* Keep the program alive so it can continue servicing timer interrupts. */
  while (1)
    ;

  return 0;
}
