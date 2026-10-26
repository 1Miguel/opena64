/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __TIMER__H
#define __TIMER__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup timer ARM generic timer API
 * @{
 *
 * @brief API for configuring and querying the ARM Generic Timer.
 */

/**
 * @brief Timer interrupt callback type.
 *
 * @param[in] cb_data User-provided callback context passed to the interrupt handler.
 */
typedef void (*timer_irq_cb_t)(void *cb_data);

/**
 * @brief Installs the timer interrupt callback and its context.
 *
 * @param[in] irq_cb Interrupt callback function pointer.
 * @param[in] cb_data Pointer to user data passed to the callback.
 */
void timer_set_irq_cb(timer_irq_cb_t irq_cb, void *cb_data);

/**
 * @brief Polls until the configured timer event fires.
 *
 * This is a blocking wait helper for timer expiration.
 */
void timer_poll(void);

/**
 * @brief Starts the generic timer.
 */
void timer_start(void);

/**
 * @brief Stops the generic timer.
 */
void timer_stop(void);

/**
 * @brief Returns the current timer count value.
 *
 * @return Current timer count.
 */
uint64_t timer_get_count(void);

/**
 * @brief Sets a relative timeout from the current timer count.
 *
 * @param[in] cnt Number of timer ticks to wait relative to the current count.
 */
void timer_set_relative(uint32_t cnt);

/**
 * @brief Sets an absolute timeout value.
 *
 * @param[in] cnt Absolute tick count at which the timeout triggers.
 */
void timer_set_absolute(uint64_t cnt);

/**
 * @brief Converts milliseconds to timer ticks.
 *
 * @param[in] ms Time in milliseconds.
 * @return Equivalent timer tick count.
 */
uint64_t timer_ms_to_cnt(uint32_t ms);

/**
 * @brief Converts timer ticks to milliseconds.
 *
 * @param[in] ticks Timer tick count.
 * @return Equivalent time in milliseconds.
 */
uint32_t timer_cnt_to_ms(uint64_t ticks);

/**
 * @brief Converts microseconds to timer ticks.
 *
 * @param[in] us Time in microseconds.
 * @return Equivalent timer tick count.
 */
uint64_t timer_us_to_cnt(uint32_t us);

/**
 * @brief Converts timer ticks to microseconds.
 *
 * @param[in] cnt Timer tick count.
 * @return Equivalent time in microseconds.
 */
uint32_t timer_cnt_to_us(uint64_t cnt);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // __TIMER__H
