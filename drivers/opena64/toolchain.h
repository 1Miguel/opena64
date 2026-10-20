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

/**@}*/
