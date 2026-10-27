/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __AARCH64__H
#define __AARCH64__H

#include <stdint.h>

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
 * @brief Reads an AArch64 system register.
 *
 * @param reg System register name.
 *
 * @return Value read from the system register.
 */
#define __MRS(reg)                                                                                 \
	({                                                                                         \
		uint64_t __val;                                                                    \
		__asm__ volatile("mrs %0, " #reg : "=r"(__val));                                   \
		__val;                                                                             \
	})

/**
 * @brief Writes a value to an AArch64 system register.
 *
 * @param reg System register name.
 * @param val Value to write.
 */
#define __MSR(reg, val)                                                                            \
	{                                                                                          \
		__asm__ volatile("msr " #reg ", %0" : : "r"(val) : "memory");                      \
	}

/**
 * @brief Executes a Secure Monitor Call with an immediate value.
 *
 * @param imm Immediate value passed to the SMC instruction.
 */
#define smc(imm)                                                                                   \
	{                                                                                          \
		__asm__ volatile("smc #" #imm::: "memory");                                        \
	}

/**
 * @brief Executes a Supervisory Call with an immediate value.
 *
 * @param imm Immediate value passed to the SVC instruction.
 */
#define svc(imm)                                                                                   \
	{                                                                                          \
		__asm__ volatile("svc #" #imm::: "memory");                                        \
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
	__asm__ volatile("daifclr #3" ::: "memory");
}

/**
 * @brief Disable interrupts (irq and fiq).
 */
static inline void irq_disable(void)
{
	__asm__ volatile("daifclr #3" ::: "memory");
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
