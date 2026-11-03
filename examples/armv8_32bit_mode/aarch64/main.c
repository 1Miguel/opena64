/**
 * @brief Demonstrate 32-bit code execution in armv8.
 *
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 * @date Oct 19 2026
 */

#include <stdint.h>
#include <stdio.h>

#include "opena64/arm.h"
#include "opena64/assert.h"
#include "opena64/uart_pl011.h"


#define SMC_EXEC_32_BIT     (0x1F)
#define CODE_32_BIT_ADDRESS (0x10000)


/**
 * @brief A simple function how to run 32-bit
 *  code. This requires the 32-bit code to be
 *  embedded in flash at a known address.
 *
 * @param address Address start of 32-bit code.
 */
static inline void el3_run_32bit_code(uintptr_t address)
{
  __asm__ volatile (
	  "mrs x10, SCR_EL3\n\t"
	  "bic x10, x10, #(1 << 10)\n\t"
    "msr SCR_EL3, x10\n\t" 
    "isb\n\t"
    "msr ELR_EL3, %0\n\t"
    "mov x10, #0x1f\n\t"
    "msr SPSR_EL3, x10\n\t"
    "isb\n\t"
    "eret"
    :
    : "r"(address)
    : "x10", "memory"
  );
  __code_unreachable();
}

void el_low_el_sync_handler(void)
{
  uint8_t exc_lvl;
  uint64_t esr_el3;
  uint32_t ec, iss;

  exc_lvl = curr_exc_lvl();
  __ASSERT(exc_lvl == 3);


  esr_el3 = sys_reg_read(ESR_EL3);

  // get the syndrome register at EL3
  // read the exception class
  ec = (esr_el3 & ESR_EC_MASK) >> ESR_EC_SHIFT;
  if (ec != ESR_EC_SMC_AARCH64) {
    return;
  }

  // get the specific syndrome value
  iss = esr_el3 & ESR_ISS_MASK;
  if (iss == SMC_EXEC_32_BIT) {
    // the address is stored in x15, see smc1(...)
    uintptr_t address = (uintptr_t)__reg_read(x15);
    el3_run_32bit_code(address);
  }
}

int main(void)
{
  /* smc call with single args */
  smc1(0x1F, CODE_32_BIT_ADDRESS);

	/* Keep the program alive so it can continue servicing timer interrupts. */
	while (1) {}

	return 0;
}
