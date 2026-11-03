/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __AARCH64__H
#define __AARCH64__H

#include <stdint.h>
#include "toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup arm64 arch specific instruction api's
 * @{
 *
 * @ingroup arm
 *
 * @brief aarch64 armv8/9 arch specific instruction api's
 */

/**
 * @brief CPU affinity to u64 format.
 *
 * @param _aff0 affinity 0 value.
 * @param _aff1 affinity 1 value.
 * @param _aff2 affinity 2 value.
 * @param _aff3 affinity 3 value.
 *
 * @return uint64 format of affinity.
 */
#define CPU_AFFINITY(_aff0, _aff1, _aff2, _aff3)                                                   \
	((_aff0 & 0xFF) | ((_aff1 & 0xFF) << 16) | ((_aff2 & 0xFF) << 24) | ((_aff3 & 0xFF) << 32))

/**
 * @brief Write register value.
 *
 * @param _reg Register name i.e "x0", "w0"
 * @param _val Value to write to register
 */
#define __reg_write(_reg, _val)                                                                    \
	{                                                                                          \
		uint64_t __val = (uint64_t)_val;                                                   \
		__asm volatile("mov " #_reg ", %0" : : "r"(__val));                                \
	}

/**
 * @brief Read register value.
 *
 * @param _reg Register name i.e "x0", "w0"
 */
#define __reg_read(_reg)                                                                           \
	({                                                                                         \
		uint64_t __val;                                                                    \
		__asm volatile("mov %0, " #_reg : "=r"(__val));                                    \
		__val;                                                                             \
	})

/**
 * @brief Reads an AArch64 system register.
 *
 * @param reg System register name.
 *
 * @return Value read from the system register.
 */
#define __MRS(reg)                                                                                 \
	({                                                                                         \
		uint64_t __val;                                                                    \
		__asm volatile("mrs %0, " #reg : "=r"(__val));                                     \
		__val;                                                                             \
	})

/**
 * @brief Writes a value to an AArch64 system register.
 *
 * @param reg System register name.
 * @param val Value to write.
 */
#define __MSR(reg, val) __asm volatile("msr " #reg ", %0" : : "r"(val) : "memory")

/**
 * @brief Executes a Secure Monitor Call with an immediate value.
 *
 * @param imm Immediate value passed to the SMC instruction.
 */
#define smc(imm) __asm volatile("smc #" #imm::: "memory")

/**
 * @brief Executes a Secure Monitor (SMC) Call with an immediate value.
 *  and a single argument.
 *
 * @param imm  Immediate value passed to the SVC instruction.
 * @param arg0 Single argument.
 */
#define smc1(imm, arg0)                                                                            \
	{                                                                                          \
		__reg_write(x15, arg0);                                                            \
		smc(imm);                                                                          \
	}

#define smc2(imm, arg0, arg1)                                                                      \
	{                                                                                          \
		__reg_write(x15, arg0);                                                            \
		__reg_write(x16, arg1);                                                            \
		smc(imm);                                                                          \
	}

#define smc3(imm, arg0, arg1, arg2)                                                                \
	{                                                                                          \
		__reg_write(x15, arg0);                                                            \
		__reg_write(x16, arg1);                                                            \
		__reg_write(x17, arg2);                                                            \
		smc(imm);                                                                          \
	}

/**
 * @brief Executes a Supervisory Call with an immediate value.
 *
 * @param imm Immediate value passed to the SVC instruction.
 */
#define svc(imm)                                                                                   \
	{                                                                                          \
		__asm volatile("svc #" #imm::: "memory");                                          \
	}

#define __SYS_REG_FUNC_RO(reg)                                                                     \
	static inline uint64_t sys_reg_##reg##_read(void)                                          \
	{                                                                                          \
		return __MRS(reg);                                                                 \
	}                                                                                          \
	static inline uint64_t sys_reg_##reg##_bit_get(uint64_t bit_msk)                           \
	{                                                                                          \
		return sys_reg_##reg##_read() & bit_msk;                                           \
	}

#define __SYS_REG_FUNC(reg)                                                                        \
	__SYS_REG_FUNC_RO(reg)                                                                     \
	static inline void sys_reg_##reg##_write(uint64_t val)                                     \
	{                                                                                          \
		__MSR(reg, val);                                                                   \
	}                                                                                          \
	static inline void sys_reg_##reg##_bit_set(uint64_t bit_msk)                               \
	{                                                                                          \
		sys_reg_##reg##_write(sys_reg_##reg##_read() | bit_msk);                           \
	}                                                                                          \
	static inline void sys_reg_##reg##_bit_clr(uint64_t bit_msk)                               \
	{                                                                                          \
		sys_reg_##reg##_write(sys_reg_##reg##_read() & (~bit_msk));                        \
	}

__SYS_REG_FUNC(currentEL)
__SYS_REG_FUNC(SCR_EL3)
__SYS_REG_FUNC(MPIDR_EL3)
__SYS_REG_FUNC(SCR_EL1)
__SYS_REG_FUNC(MPIDR_EL1)
__SYS_REG_FUNC_RO(ESR_EL3)
__SYS_REG_FUNC_RO(ESR_EL1)

/**
 * @brief ESR_ELx exception class (EC) field.
 *
 * Use ESR_EC(esr) to extract the EC field from an ESR_ELx value.
 */
#define ESR_EC_SHIFT  26U
#define ESR_EC_MASK   (0x3FU << ESR_EC_SHIFT)
#define ESR_EC(esr)   (((uint32_t)(esr) >> ESR_EC_SHIFT) & 0x3FU)
#define ESR_ISS_SHIFT (0)
#define ESR_ISS_MASK  (0x01FFFFFF << ESR_ISS_SHIFT)

#define ESR_EC_UNKNOWN                    0x00U
#define ESR_EC_TRAPPED_WFI_WFE            0x01U
#define ESR_EC_TRAPPED_CP15_MCR_MRC       0x03U
#define ESR_EC_TRAPPED_CP15_MCRR_MRRC     0x04U
#define ESR_EC_TRAPPED_CP14_MCR_MRC       0x05U
#define ESR_EC_TRAPPED_CP14_LDC_STC       0x06U
#define ESR_EC_TRAPPED_SIMD_FP_ACCESS     0x07U
#define ESR_EC_TRAPPED_CP10               0x08U
#define ESR_EC_ILLEGAL_EXECUTION_STATE    0x0EU
#define ESR_EC_SVC_AARCH32                0x11U
#define ESR_EC_HVC_AARCH32                0x12U
#define ESR_EC_SMC_AARCH32                0x13U
#define ESR_EC_SVC_AARCH64                0x15U
#define ESR_EC_HVC_AARCH64                0x16U
#define ESR_EC_SMC_AARCH64                0x17U
#define ESR_EC_TRAPPED_MSR_MRS            0x18U
#define ESR_EC_INSTRUCTION_ABORT_LOWER_EL 0x20U
#define ESR_EC_INSTRUCTION_ABORT_SAME_EL  0x21U
#define ESR_EC_PC_ALIGNMENT_FAULT         0x22U
#define ESR_EC_DATA_ABORT_LOWER_EL        0x24U
#define ESR_EC_DATA_ABORT_SAME_EL         0x25U
#define ESR_EC_SP_ALIGNMENT_FAULT         0x26U
#define ESR_EC_FP_EXCEPTION_AARCH32       0x28U
#define ESR_EC_FP_EXCEPTION_AARCH64       0x2CU
#define ESR_EC_SERROR                     0x2FU
#define ESR_EC_BREAKPOINT_LOWER_EL        0x30U
#define ESR_EC_BREAKPOINT_SAME_EL         0x31U
#define ESR_EC_SOFTWARE_STEP_LOWER_EL     0x32U
#define ESR_EC_SOFTWARE_STEP_SAME_EL      0x33U
#define ESR_EC_WATCHPOINT_LOWER_EL        0x34U
#define ESR_EC_WATCHPOINT_SAME_EL         0x35U
#define ESR_EC_BKPT_AARCH32               0x38U
#define ESR_EC_VECTOR_CATCH_AARCH32       0x3AU
#define ESR_EC_BRK_AARCH64                0x3CU

// GIC cpu interface system registers
__SYS_REG_FUNC(ICC_CTLR_EL3)
__SYS_REG_FUNC(ICC_SRE_EL3)
__SYS_REG_FUNC(ICC_CTLR_EL1)
__SYS_REG_FUNC(ICC_IAR0_EL1)
__SYS_REG_FUNC(ICC_IAR1_EL1)
__SYS_REG_FUNC(ICC_EOIR0_EL1)
__SYS_REG_FUNC(ICC_EOIR1_EL1)
__SYS_REG_FUNC(ICC_BPR0_EL1)
__SYS_REG_FUNC(ICC_SRE_EL1)
__SYS_REG_FUNC(ICC_SGI0R_EL1)
__SYS_REG_FUNC(ICC_IGRPEN0_EL1)
__SYS_REG_FUNC(ICC_IGRPEN1_EL1)
__SYS_REG_FUNC(ICC_PMR_EL1)

// arm generic timer system registers
__SYS_REG_FUNC(CNTP_CTL_EL0)
__SYS_REG_FUNC(CNTP_CVAL_EL0)
__SYS_REG_FUNC(CNTP_TVAL_EL0)
__SYS_REG_FUNC(CNTPCT_EL0)
__SYS_REG_FUNC_RO(CNTFRQ_EL0)

/**
 * @brief helper function to read arm system register.
 *
 * @param reg name of system register.
 */
#define sys_reg_read(reg) sys_reg_##reg##_read()

/**
 * @brief helper function to write to an arm system register.
 *
 * @param reg name of system register.
 * @param val  value to write
 */
#define sys_reg_write(reg, val) sys_reg_##reg##_write(val)

/**
 * @brief helper function to set arm system register bit field.
 *
 * @param reg     name of system register.
 * @param bit_msk bit mask selecting the bits to set.
 */
#define sys_reg_bit_set(reg, bit_msk) sys_reg_##reg##_bit_set(bit_msk)

/**
 * @brief helper function to set arm system register bit field.
 *
 * @param reg     name of system register.
 * @param bit_msk bit mask selecting the bits to clear.
 */
#define sys_reg_bit_clr(reg, bit_msk) sys_reg_##reg##_bit_clr(bit_msk)

/**
 * @brief helper function to set arm system register bit field.
 *
 * @param reg     name of system register.
 * @param bit_msk bit mask selecting the bits to get.
 */
#define sys_reg_bit_get(reg, bit_msk) sys_reg_##reg##_bit_get(bit_msk)

/**
 * @brief Enable interrupts (irq and fiq).
 */
static inline void irq_enable(void)
{
	__asm volatile("daifclr #3" ::: "memory");
}

/**
 * @brief Disable interrupts (irq and fiq).
 */
static inline void irq_disable(void)
{
	__asm volatile("daifclr #3" ::: "memory");
}

/**
 * @brief Get the current cpu affinity.
 *
 * @return Current cpu affinity.
 */
static inline uint64_t curr_cpu_aff(void)
{
	return sys_reg_bit_get(MPIDR_EL1, 0xff00ffffff);
}

/**
 * @brief Get the current cpu id.
 *
 * @return Current cpu id.
 */
static inline uint8_t curr_cpu_id(void)
{
	return sys_reg_bit_get(MPIDR_EL1, 0xff);
}

/**
 * @brief Get the current exception level (EL).
 *
 * @return Current exception level.
 */
static inline uint8_t curr_exc_lvl(void)
{
	uint64_t curr_el = sys_reg_read(currentEL);
	return curr_el >> 2;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif // __AARCH64__H
