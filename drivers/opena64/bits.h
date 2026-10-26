/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __BITS__H
#define __BITS__H

/** @brief bit set based on bit position */
#define BIT(n) (1UL << (n))

/** @brief bit set 64-bit */
#define BIT64(_n) (1ULL << (_n))

/** @brief generate a bit mask based on given bit width */
#define BIT_MASK(n) ((1U << (n)) - 1U)

/** @brief generate a 64-bit bit mask based on given bit width */
#define BIT64_MASK(n) (BIT64(n) - 1ULL)

/** @brief set bits based on given mask */
#define BIT_SET(v, m) (v | m)

/** @brief clear bits based on given mask */
#define BIT_CLR(v, m) (v & m)

/** @brief overwrite bits */
#define BIT_WRITE(v, m) BIT_SET(BIT_CLR(v, m), m)

#endif // __BITS__H
