/**
 * @brief Minimal Unity test-runner example with UART-backed output.
 *
 * Initializes the PL011 UART, redirects standard output to it, and runs one
 * sample Unity test. The test currently demonstrates runner/output wiring;
 * it does not yet contain an assertion against application behavior.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdio.h>
#include "unity.h"
#include "opena64/gic.h"
#include "opena64/timer.h"
#include "opena64/uart_pl011.h"

#define UART0 (0x09000000) /* UART base address used by the a53_renode platform. */


/* The UART instance used to print the Unity runner's output. */
static struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);


/* Route the C library's stdout writes through the PL011 polling transmitter. */
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

/* A placeholder test showing where test cases and assertions belong. */
void test_sample(void)
{
  printf("Sample test!\n\r");
}


int main(void)
{
	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;

	/* Initialize UART first so Unity's test results can be sent to the console. */
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

  /* Initialize Unity, execute the registered test, and return its result. */
  UNITY_BEGIN();
  RUN_TEST(test_sample);
  return UNITY_END();
}
