/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __TOOLCHAIN__H
#define __TOOLCHAIN__H

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

#ifdef __GNUC__
#include "toolchain_gcc.h"
#else
#error "Unsupported toolchain"
#endif

/**@}*/

#endif // __TOOLCHAIN__H
