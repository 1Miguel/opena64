/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __AARCH32__H
#define __AARCH32__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup arm32 arch specific instruction api's
 * @{
 *
 * @ingroup arm
 *
 * @brief MMIO api's
 */

/**
 * @brief CPU affinity to u32 format.
 *
 * @param _aff0 affinity 0 value.
 * @param _aff1 affinity 1 value.
 * @param _aff2 affinity 2 value.
 * @param _aff3 affinity 3 value.
 *
 * @return uint32 format of affinity.
 */
#define CPU_AFFINITY(_aff0, _aff1, _aff2, _aff3)                                                   \
	((_aff0 & 0xFF) | ((_aff1 & 0xFF) << 16) | ((_aff2 & 0xFF) << 24) | ((_aff3 & 0xFF) << 32))

/**
 * @brief Reads an AArch32 coprocessor register.
 *
 * @param cp  coprocessor register number.
 * @param op1 opcode 1.
 * @param crm primary register identifier.
 * @param crm secondary register identifier.
 * @param op2 opcode 2.
 *
 * @return Value read from the system register.
 */
#define __MRC(cp, op1, crn, crm, op2)                                                              \
	({                                                                                         \
		uint32_t __val;                                                                    \
		__asm__ volatile("mrc p" #cp ", " #op1 ", %0, c" #crn ", c" #crm, ", " #op2        \
				 : "=r"(__val));                                                   \
		__val;                                                                             \
	})

/**
 * @brief Writes to an AArch32 coprocessor register.
 *
 * @param cp  coprocessor register number.
 * @param op1 opcode 1.
 * @param r   source register.
 * @param crm primary register identifier.
 * @param crm secondary register identifier.
 * @param op2 opcode 2.
 */
#define __MCR(cp, op1, r, crn, crm, op2)                                                           \
	({                                                                                         \
		uint32_t __val;                                                                    \
		__asm__ volatile("mrc p" #cp ", " #op1 ", %0, c" #crn ", c" #crm, ", " #op2        \
				 :                                                                 \
				 : "r"(__val));                                                    \
		__val;                                                                             \
	})

#define __SYS_REG_FUNC_RO(reg, cp, op1, crn, crm, op2)                                             \
	static inline uint32_t sys_reg_##reg##_read(void)                                          \
	{                                                                                          \
		return __MCR(cp, op1, crn, crm, op2);                                              \
	}                                                                                          \
	static inline uint32_t sys_reg_##reg##_bit_get(uint64_t bit_msk)                           \
	{                                                                                          \
		return sys_reg_##reg##_read() & bit_msk;                                           \
	}

#define __SYS_REG_FUNC(reg, cp, op1, crn, crm, op2)                                                \
	__SYS_REG_FUNC_RO(reg)                                                                     \
	static inline void sys_reg_##reg##_write(uint32_t val)                                     \
	{                                                                                          \
		__MRC(cp, op1, val, crn, crm, op2);                                                \
	}                                                                                          \
	static inline void sys_reg_##reg##_bit_set(uint32_t bit_msk)                               \
	{                                                                                          \
		sys_reg_##reg##_write(sys_reg_##reg##_read() | bit_msk);                           \
	}                                                                                          \
	static inline void sys_reg_##reg##_bit_clr(uint32_t bit_msk)                               \
	{                                                                                          \
		sys_reg_##reg##_write(sys_reg_##reg##_read() & (~bit_msk));                        \
	}

//__SYS_REG_FUNC(currentEL, )
//__SYS_REG_FUNC(SCR_EL3)
//__SYS_REG_FUNC(MPIDR_EL3)
//__SYS_REG_FUNC(SCR_EL1)
//__SYS_REG_FUNC(MPIDR_EL1)

// GIC cpu interface system registers
//__SYS_REG_FUNC(ICC_CTLR_EL3)
//__SYS_REG_FUNC(ICC_SRE_EL3)
//__SYS_REG_FUNC(ICC_CTLR_EL1)
__SYS_REG_FUNC(ICC_IAR0_EL1, 15, 0, 12, 8, 0)
// __SYS_REG_FUNC(ICC_IAR1_EL1)
// __SYS_REG_FUNC(ICC_EOIR0_EL1)
// __SYS_REG_FUNC(ICC_EOIR1_EL1)
// __SYS_REG_FUNC(ICC_BPR0_EL1)
// __SYS_REG_FUNC(ICC_SRE_EL1)
// __SYS_REG_FUNC(ICC_SGI0R_EL1)
// __SYS_REG_FUNC(ICC_IGRPEN0_EL1)
// __SYS_REG_FUNC(ICC_IGRPEN1_EL1)
// __SYS_REG_FUNC(ICC_PMR_EL1)

// arm generic timer system registers
// __SYS_REG_FUNC(CNTP_CTL_EL0)
// __SYS_REG_FUNC(CNTP_CVAL_EL0)
// __SYS_REG_FUNC(CNTP_TVAL_EL0)
// __SYS_REG_FUNC(CNTPCT_EL0)
// __SYS_REG_FUNC_RO(CNTFRQ_EL0)

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
static inline uint32_t curr_cpu_aff(void)
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
	uint32_t curr_el = sys_reg_read(currentEL);
	return curr_el >> 2;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif // __AARCH32__H
