/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

/**
 * @defgroup gic_v3
 * @{
 *
 * @ingroup gic
 *
 * @brief gic api's.
 *
 * @version 1.0.0
 *
 * modification history:
 *  1. mugsimar, oct 10, 2026
 *    > First version, only supports routing to EL1 non-secure.
 *    > no support for EL1 secure or EL3. Requires the GICD_CTLR.DS
 *      to be set to 1, which disables security access.
 *
 *  2. mugsimar, oct 20, 2026
 *    > add support for SGI
 */

#ifndef __GIC__H
#define __GIC__H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Initializes the distributor, current PE redistributor, and CPU
 * interface.
 */
void gic_v3_init(void);

/**
 * @brief Enable interrupt.
 *
 * @param int_id interrupt id.
 */
void gic_v3_int_enable(unsigned int int_id);

/**
 * @brief Disable interrupt.
 *
 * @param int_id interrupt id.
 */
void gic_v3_int_disable(unsigned int int_id);

/**
 * @brief Check if a given interrupt id is enabled.
 *
 * @param int_id interrupt id.
 *
 * @retval true if interrupt is enabled.
 */
bool gic_v3_int_is_enable(unsigned int int_id);

/**
 * @brief Check if a given interrupt id is pending.
 *
 * @param int_id interrupt id.
 *
 * @retval true if interrupt is pending.
 */
bool gic_v3_int_is_pending(unsigned int int_id);

/**
 * @brief Set pending interrupt of a given interrupt id.
 *
 * @param int_id interrupt id.
 */
void gic_v3_int_set_pending(unsigned int int_id);

/**
 * @brief Clear any pending interrupt of a given interrupt id.
 *
 * @param int_id interrupt id.
 */
void gic_v3_int_clear_pending(unsigned int int_id);

/**
 * @brief Sets an interrupt's priority.
 *
 * @param int_id GIC interrupt ID.
 * @param priority 8-bit GIC priority value.
 * @return False when the driver is not initialized or the ID is unsupported.
 */
void gic_v3_int_set_priority(unsigned int int_id, uint8_t priority);

/**
 * @brief Sends an SGI to the selected Aff0 targets.
 *
 * @param sgi_id SGI ID in the range 0-15.
 * @param target_aff Target MPIDR affinity value; Aff0[7:4] selects the range.
 * @param target_list Bit mask selecting Aff0[3:0] targets in the affinity
 * group.
 *
 * @return -EINVAL if sgi_id is invalid.
 */
int gic_v3_raise_sgi(unsigned int sgi_id, uint64_t target_aff, uint16_t target_list);

/**
 * @brief Sends an SGI to self.
 *
 * @param sgi_id SGI ID in the range 0-15.
 *
 * @return -EINVAL if sgi_id is invalid.
 */
int gic_v3_raise_sgi_self(unsigned int sgi_id);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GIC__H

/**@}*/
