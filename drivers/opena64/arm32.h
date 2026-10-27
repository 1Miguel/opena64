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
 * @brief aarch32 coprocessor-register and instruction helpers.
 */

/**
 * @brief CPU affinity to u32 format.
 *
 * @param _aff0 affinity level 0 value.
 * @param _aff1 affinity level 1 value.
 * @param _aff2 affinity level 2 value.
 * @param _aff3 affinity level 3 value.
 *
 * @return uint32 format of affinity.
 */
#define CPU_AFFINITY(_aff0, _aff1, _aff2, _aff3)                                                   \
	(((uint32_t)(_aff0) & 0xFFu) | (((uint32_t)(_aff1) & 0xFFu) << 8) |                         \
	 (((uint32_t)(_aff2) & 0xFFu) << 16) | (((uint32_t)(_aff3) & 0xFFu) << 24))

/**
 * @brief Reads an AArch32 coprocessor register.
 *
 * @param cp  coprocessor number.
 * @param op1 opcode 1.
 * @param crn primary register identifier.
 * @param crm secondary register identifier.
 * @param op2 opcode 2.
 *
 * @return Value read from the coprocessor register.
 */
#define __MRC(cp, op1, crn, crm, op2)                                                              \
	({                                                                                         \
		uint32_t __val;                                                                    \
		__asm__ volatile("mrc p" #cp ", " #op1 ", %0, c" #crn ", c" #crm ", " #op2       \
				 : "=r"(__val));                                                   \
		__val;                                                                             \
	})

/**
 * @brief Writes an AArch32 coprocessor register.
 *
 * @param cp  coprocessor number.
 * @param op1 opcode 1.
 * @param val value to write.
 * @param crn primary register identifier.
 * @param crm secondary register identifier.
 * @param op2 opcode 2.
 */
#define __MCR(cp, op1, val, crn, crm, op2)                                                         \
	do {                                                                                       \
		uint32_t __val = (uint32_t)(val);                                                  \
		__asm__ volatile("mcr p" #cp ", " #op1 ", %0, c" #crn ", c" #crm ", " #op2      \
				 : : "r"(__val) : "memory");                                    \
	} while (0)

/**
 * @brief Reads a 64-bit AArch32 coprocessor register.
 *
 * @param cp coprocessor number.
 * @param op opcode.
 * @param crm secondary register identifier.
 *
 * @return Value read from the coprocessor register.
 */
#define __MRRC(cp, op, crm)                                                                        \
	({                                                                                         \
		uint64_t __val;                                                                    \
		__asm__ volatile("mrrc p" #cp ", " #op ", %Q0, %R0, c" #crm : "=r"(__val));       \
		__val;                                                                             \
	})

/**
 * @brief Writes a 64-bit AArch32 coprocessor register.
 *
 * @param cp coprocessor number.
 * @param op opcode.
 * @param val value to write.
 * @param crm secondary register identifier.
 */
#define __MCRR(cp, op, val, crm)                                                                   \
	do {                                                                                       \
		uint64_t __val = (uint64_t)(val);                                                  \
		__asm__ volatile("mcrr p" #cp ", " #op ", %Q0, %R0, c" #crm                       \
				 : : "r"(__val) : "memory");                                    \
	} while (0)

#define __SYS_REG_FUNC_RO(reg, cp, op1, crn, crm, op2)                                             \
	static inline uint32_t sys_reg_##reg##_read(void)                                          \
	{                                                                                          \
		return __MRC(cp, op1, crn, crm, op2);                                              \
	}                                                                                          \
	static inline uint32_t sys_reg_##reg##_bit_get(uint32_t bit_msk)                           \
	{                                                                                          \
		return sys_reg_##reg##_read() & bit_msk;                                           \
	}

#define __SYS_REG_FUNC(reg, cp, op1, crn, crm, op2)                                                \
	__SYS_REG_FUNC_RO(reg, cp, op1, crn, crm, op2)                                             \
	static inline void sys_reg_##reg##_write(uint32_t val)                                     \
	{                                                                                          \
		__MCR(cp, op1, val, crn, crm, op2);                                                \
	}                                                                                          \
	static inline void sys_reg_##reg##_bit_set(uint32_t bit_msk)                               \
	{                                                                                          \
		sys_reg_##reg##_write(sys_reg_##reg##_read() | bit_msk);                           \
	}                                                                                          \
	static inline void sys_reg_##reg##_bit_clr(uint32_t bit_msk)                               \
	{                                                                                          \
		sys_reg_##reg##_write(sys_reg_##reg##_read() & (~bit_msk));                        \
	}

#define __SYS_REG_FUNC_RO_64(reg, cp, op, crm)                                                     \
	static inline uint64_t sys_reg_##reg##_read(void)                                          \
	{                                                                                          \
		return __MRRC(cp, op, crm);                                                        \
	}                                                                                          \
	static inline uint64_t sys_reg_##reg##_bit_get(uint64_t bit_msk)                           \
	{                                                                                          \
		return sys_reg_##reg##_read() & bit_msk;                                           \
	}

#define __SYS_REG_FUNC_64(reg, cp, op, crm)                                                       \
	__SYS_REG_FUNC_RO_64(reg, cp, op, crm)                                                      \
	static inline void sys_reg_##reg##_write(uint64_t val)                                     \
	{                                                                                          \
		__MCRR(cp, op, val, crm);                                                          \
	}                                                                                          \
	static inline void sys_reg_##reg##_bit_set(uint64_t bit_msk)                               \
	{                                                                                          \
		sys_reg_##reg##_write(sys_reg_##reg##_read() | bit_msk);                           \
	}                                                                                          \
	static inline void sys_reg_##reg##_bit_clr(uint64_t bit_msk)                               \
	{                                                                                          \
		sys_reg_##reg##_write(sys_reg_##reg##_read() & (~bit_msk));                        \
	}

/*
 * AArch32 has no CurrentEL system register. Return the equivalent encoded
 * value using the CPSR mode field so the common helper keeps its AArch64 API.
 */
static inline uint64_t sys_reg_currentEL_read(void)
{
	uint32_t cpsr;
	uint32_t mode;

	__asm__ volatile("mrs %0, cpsr" : "=r"(cpsr));
	mode = cpsr & 0x1Fu;

	switch (mode) {
	case 0x10: /* User */
		return 0;
	case 0x16: /* Monitor */
		return 3u << 2;
	case 0x1A: /* Hyp */
		return 2u << 2;
	default:
		return 1u << 2;
	}
}

static inline uint64_t sys_reg_currentEL_bit_get(uint64_t bit_msk)
{
	return sys_reg_currentEL_read() & bit_msk;
}

/* AArch32 access to the processor identification and secure configuration. */
__SYS_REG_FUNC_RO(MPIDR_EL1, 15, 0, 0, 0, 5)
__SYS_REG_FUNC_RO(SCR_EL3, 15, 0, 1, 1, 0)

/* GICv3 CPU interface registers (AArch32 CP15 encodings). */
__SYS_REG_FUNC(ICC_CTLR_EL1, 15, 0, 12, 12, 4)
__SYS_REG_FUNC(ICC_SRE_EL1, 15, 0, 12, 12, 5)
__SYS_REG_FUNC_RO(ICC_IAR0_EL1, 15, 0, 12, 8, 0)
__SYS_REG_FUNC_RO(ICC_IAR1_EL1, 15, 0, 12, 12, 0)
__SYS_REG_FUNC(ICC_EOIR0_EL1, 15, 0, 12, 8, 1)
__SYS_REG_FUNC(ICC_EOIR1_EL1, 15, 0, 12, 12, 1)
__SYS_REG_FUNC(ICC_BPR0_EL1, 15, 0, 12, 8, 3)
__SYS_REG_FUNC(ICC_BPR1_EL1, 15, 0, 12, 12, 3)
__SYS_REG_FUNC(ICC_IGRPEN0_EL1, 15, 0, 12, 12, 6)
__SYS_REG_FUNC(ICC_IGRPEN1_EL1, 15, 0, 12, 12, 7)
__SYS_REG_FUNC(ICC_PMR_EL1, 15, 0, 4, 6, 0)
__SYS_REG_FUNC_64(ICC_SGI0R_EL1, 15, 2, 12)

/* ARM generic timer registers (available on implementations with the timer). */
__SYS_REG_FUNC(CNTP_CTL_EL0, 15, 0, 14, 2, 1)
__SYS_REG_FUNC(CNTP_TVAL_EL0, 15, 0, 14, 2, 0)
__SYS_REG_FUNC_64(CNTP_CVAL_EL0, 15, 2, 14)
__SYS_REG_FUNC_RO_64(CNTPCT_EL0, 15, 0, 14)
__SYS_REG_FUNC_RO(CNTFRQ_EL0, 15, 0, 14, 0, 0)

/**
 * @brief Helper to read an ARM system register.
 *
 * @param reg name of system register.
 */
#define sys_reg_read(reg) sys_reg_##reg##_read()

/**
 * @brief Helper to write an ARM system register.
 *
 * @param reg name of system register.
 * @param val value to write.
 */
#define sys_reg_write(reg, val) sys_reg_##reg##_write(val)

/**
 * @brief Helper to set bits in an ARM system register.
 *
 * @param reg name of system register.
 * @param bit_msk bit mask selecting the bits to set.
 */
#define sys_reg_bit_set(reg, bit_msk) sys_reg_##reg##_bit_set(bit_msk)

/**
 * @brief Helper to clear bits in an ARM system register.
 *
 * @param reg name of system register.
 * @param bit_msk bit mask selecting the bits to clear.
 */
#define sys_reg_bit_clr(reg, bit_msk) sys_reg_##reg##_bit_clr(bit_msk)

/**
 * @brief Helper to get bits from an ARM system register.
 *
 * @param reg name of system register.
 * @param bit_msk bit mask selecting the bits to get.
 */
#define sys_reg_bit_get(reg, bit_msk) sys_reg_##reg##_bit_get(bit_msk)

/**
 * @brief Enable IRQ and FIQ exceptions.
 */
static inline void irq_enable(void)
{
	__asm__ volatile("cpsie if" ::: "memory");
}

/**
 * @brief Disable IRQ and FIQ exceptions.
 */
static inline void irq_disable(void)
{
	__asm__ volatile("cpsid if" ::: "memory");
}

/**
 * @brief Get the current CPU affinity.
 *
 * AArch32 exposes Aff0-Aff2 in MPIDR.
 *
 * @return Current CPU affinity.
 */
static inline uint32_t curr_cpu_aff(void)
{
	return (uint32_t)sys_reg_read(MPIDR_EL1) & 0x00FFFFFFu;
}

/**
 * @brief Get the current CPU id.
 *
 * @return Current CPU id.
 */
static inline uint8_t curr_cpu_id(void)
{
	return (uint8_t)sys_reg_bit_get(MPIDR_EL1, 0xFFu);
}

/**
 * @brief Get the current exception level.
 *
 * @return Current exception level.
 */
static inline uint8_t curr_exc_lvl(void)
{
	return (uint8_t)(sys_reg_read(currentEL) >> 2);
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AARCH32__H */
