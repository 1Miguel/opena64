/**
 * @brief Minimal Unity test-runner example with UART-backed output.
 *
 * Built for the a53_renode platform with PL011 UART0 at 0x09000000. Initializes
 * the UART, routes Unity's standard output to the emulated UART console, and
 * runs a placeholder test to demonstrate the test-runner lifecycle. The
 * sample currently has no assertions against application behavior.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include "opena64/gic.h"
#include "opena64/timer.h"
#include "opena64/uart_pl011.h"
#include "unity.h"
#include <stdio.h>

/* UART base address used by the a53_renode platform. */
#define UART0 (0x09000000)

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

/* Placeholder test showing where application assertions can be added. */
void test_sample(void)
{
	printf("Sample test!\n\r");
}

int main(void)
{
	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;

	/* Initialize UART first so Unity's test results can be sent to the console.
	 */
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

	/* Initialize Unity, execute the registered test, and return its result. */
	UNITY_BEGIN();
	RUN_TEST(test_sample);

	uart_pl011_disable(&uart_dev);
	return UNITY_END();
}
