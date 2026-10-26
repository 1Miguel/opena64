/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

/**
 * @defgroup toolchain
 * @{
 *
 * @ingroup assert
 *
 * @brief Toolchain port macro's and api's
 *
 * @version 1.0.0
 *
 * @date Oct 20 2026
 */

/**
 * @brief Provides branch prediction optimization.
 *
 * @param expr likely expression i.e (a == b)
 */
#define LIKELY(expr) (__builtin_expect(!!(expr), 1))

/**
 * @brief Macro for indicating an unused function arg.
 *
 * @param _arg unused arg.
 */
#define ARG_UNUSED(_arg) ((void)_arg)

/**
 * @brief macro wrapper for __asm__.
 */
#ifndef __asm
#define __asm __asm__
#endif

/**
 * @brief Weak attribute.
 */
#ifndef __weak
#define __weak __attribute__((__weak__))
#endif

/**
 * @brief macro wrapper to compiler specific hints for indicating
 *  unreachable codes.
 */
#ifndef __code_unreachable
#define __code_unreachable __builtin_unreachable();
#endif

/**
 * @brief counts the number of 1-bits (set bits) in an integer's
 *    binary representation.
 */
#ifndef __popcount
#define __popcount(x) __builtin_popcount(x)
#endif

/**@}*/
