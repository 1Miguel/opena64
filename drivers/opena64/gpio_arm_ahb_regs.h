/*******************************************************************************
 * @file gpio_arm_ahb_regs.h
 * @brief ARM PrimeCell AHB GPIO register definitions.
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef GPIO_ARM_AHB_REGS__H
#define GPIO_ARM_AHB_REGS__H

#include <stdint.h>

/**
 * @defgroup gpio_arm_ahb_regs
 * @{
 *
 * @brief ARM PrimeCell GPIO register definitions.
 */

/* DATA is addressed through a pin mask in address bits [9:2]. */
#define GPIO_ARM_AHB_DATA(base)         ((uintptr_t)(base) + 0x0000U)
#define GPIO_ARM_AHB_DATA_OUT(base)     ((uintptr_t)(base) + 0x0004U)
#define GPIO_ARM_AHB_OUTENSET(base)     ((uintptr_t)(base) + 0x0010U)
#define GPIO_ARM_AHB_OUTENCLR(base)     ((uintptr_t)(base) + 0x0014U)
#define GPIO_ARM_AHB_ALTFUNCSET(base)   ((uintptr_t)(base) + 0x0018U)
#define GPIO_ARM_AHB_ALTFUNCCLR(base)   ((uintptr_t)(base) + 0x001CU)
#define GPIO_ARM_AHB_INTENSET(base)     ((uintptr_t)(base) + 0x0020U)
#define GPIO_ARM_AHB_INTENCLR(base)     ((uintptr_t)(base) + 0x0024U)
#define GPIO_ARM_AHB_INTTYPESET(base)   ((uintptr_t)(base) + 0x0028U)
#define GPIO_ARM_AHB_INTTYPECLR(base)   ((uintptr_t)(base) + 0x002CU)
#define GPIO_ARM_AHB_INTPOLSET(base)    ((uintptr_t)(base) + 0x0030U)
#define GPIO_ARM_AHB_INTPOLCLR(base)    ((uintptr_t)(base) + 0x0034U)
#define GPIO_ARM_AHB_INTSTATUS(base)    ((uintptr_t)(base) + 0x0038U)
#define GPIO_ARM_AHB_INTCLEAR(base)     ((uintptr_t)(base) + 0x0038U)
#define GPIO_ARM_AHB_MASKLOWBYTE(base)  ((uintptr_t)(base) + 0x0400U)
#define GPIO_ARM_AHB_MASKHIGHBYTE(base) ((uintptr_t)(base) + 0x0800U)
#define GPIO_ARM_AHB_PIDn(base, n)      ((uintptr_t)(base) + (n < 4 ? 0x00FE0U : 0x00FD0U))
#define GPIO_ARM_AHB_CIDn(base, n)      ((uintptr_t)(base) + 0x0FF0U)

#endif // GPIO_ARM_AHB_REGS__H
