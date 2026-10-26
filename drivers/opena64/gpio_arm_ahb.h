/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __GPIO_ARM_AHB__H
#define __GPIO_ARM_AHB__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configures a GPIO pin as input.
 */
#define GPIO_ARM_AHB_INPUT (0U << 0)

/**
 * @brief Configures a GPIO pin as output.
 */
#define GPIO_ARM_AHB_OUTPUT (1U << 0)

/**
 * @brief Selects the default low output state on reset or initialization.
 */
#define GPIO_ARM_AHB_OUTPUT_INIT_LOW (0U << 1)

/**
 * @brief Selects the default high output state on reset or initialization.
 */
#define GPIO_ARM_AHB_OUTPUT_INIT_HIGH (1U << 1)

/**
 * @brief Configures a GPIO pin as output and initializes it to low.
 */
#define GPIO_ARM_AHB_OUTPUT_LOW (GPIO_ARM_AHB_OUTPUT | GPIO_ARM_AHB_OUTPUT_INIT_LOW)

/**
 * @brief Configures a GPIO pin as output and initializes it to high.
 */
#define GPIO_ARM_AHB_OUTPUT_HIGH (GPIO_ARM_AHB_OUTPUT | GPIO_ARM_AHB_OUTPUT_INIT_HIGH)

/**
 * @brief Configures a GPIO pin for alternate-function operation.
 */
#define GPIO_ARM_AHB_ALT_FUNC (1U << 2)

/**
 * @brief Interrupt type bit mask for GPIO edge/level selection.
 */
#define GPIO_ARM_AHB_INT_TYPE (1U << 5)

/**
 * @brief Interrupt polarity bit mask for GPIO active-high/active-low selection.
 */
#define GPIO_ARM_AHB_INT_POL (2U << 5)

/**
 * @brief Interrupt enable bit mask.
 */
#define GPIO_ARM_AHB_INT_ENABLE (4U << 5)

/**
 * @brief Configures a GPIO interrupt as active-low level trigger.
 */
#define GPIO_ARM_AHB_INT_LOW_LEVEL (GPIO_ARM_AHB_INT_ENABLE)

/**
 * @brief Configures a GPIO interrupt as falling-edge trigger.
 */
#define GPIO_ARM_AHB_INT_FALLING_EDGE (GPIO_ARM_AHB_INT_TYPE | GPIO_ARM_AHB_INT_LOW_LEVEL)

/**
 * @brief Configures a GPIO interrupt as active-high level trigger.
 */
#define GPIO_ARM_AHB_INT_HIGH_LEVEL (GPIO_ARM_AHB_INT_ENABLE | GPIO_ARM_AHB_INT_POL)

/**
 * @brief Configures a GPIO interrupt as rising-edge trigger.
 */
#define GPIO_ARM_AHB_INT_RISING_EDGE (GPIO_ARM_AHB_INT_TYPE | GPIO_ARM_AHB_INT_HIGH_LEVEL)

/**
 * @brief Extracts the interrupt mode field from a GPIO configuration value.
 *
 * @param cfg GPIO configuration value.
 * @return Interrupt mode bits encoded in the lower three bits of cfg.
 */
#define GPIO_ARM_AHB_GET_INT_TYPE(cfg) (cfg & 7U)

/**
 * @defgroup gpio_arm_ahb
 * @{
 *
 * @brief Bare-metal driver interface for the ARM AHB-Lite GPIO component.
 *
 * @note	None.
 *
 * modification history:
 *  1. mugsimar, oct 20 2026
 *    > Initial version.
 */

/**
 * @brief Opaque handle for a GPIO port instance.
 */
struct gpio_arm_ahb_port;

/**
 * @brief Bitmask type used for GPIO port pin selection and values.
 */
typedef uint16_t gpio_arm_ahb_port_val_t;

/**
 * @brief GPIO pin configuration value type.
 */
typedef uint32_t gpio_arm_ahb_pin_cfg_t;

/**
 * @brief GPIO interrupt callback signature.
 *
 * @param[in] gpio GPIO port that raised the interrupt.
 * @param[in] pins Bitmask of pins that triggered the callback.
 * @param[in] cb_data User-provided callback context.
 */
typedef void (*gpio_arm_ahb_cb_t)(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins,
				  void *cb_data);

/**
 * @brief GPIO port instance data.
 */
struct gpio_arm_ahb_port {
	uintptr_t reg;        /**< Memory-mapped register base address. */
	gpio_arm_ahb_cb_t cb; /**< Optional interrupt callback. */
	void *cb_data;        /**< Context passed to the interrupt callback. */
};

/**
 * @brief Initializes a GPIO port descriptor.
 *
 * @param _reg Memory-mapped register base address.
 */
#define GPIO_ARM_AHB_PORT_INIT(_reg) {.reg = _reg, .cb = NULL, .cb_data = NULL}

/**
 * @brief Dispatches the GPIO interrupt handler for the port.
 *
 * This routine must be called from the interrupt service routine of the
 * system interrupt controller that services the GPIO block.
 *
 * @param[in] gpio Pointer to the GPIO port instance.
 */
void gpio_arm_ahb_irq_handler(struct gpio_arm_ahb_port *gpio);

/**
 * @brief Installs a GPIO interrupt callback.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] cb User callback invoked when a GPIO interrupt is raised.
 * @param[in] cb_data User-provided callback context.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance or callback is invalid.
 */
int gpio_arm_ahb_set_irq_cb(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_cb_t cb, void *cb_data);

/**
 * @brief Acknowledges the selected GPIO interrupts.
 *
 * This must be called from the interrupt callback to clear the handled
 * interrupt sources before returning from the ISR.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] pins Bitmask of pins whose interrupts are acknowledged.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance is invalid.
 */
int gpio_arm_ahb_set_irq_ack(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins);

/**
 * @brief Applies a configuration value to a masked set of GPIO pins.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] msk Bitmask of pins to configure.
 * @param[in] cfg Pointer to the configuration value to apply.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance or configuration pointer is invalid.
 */
int gpio_arm_ahb_set_port_cfg(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t msk,
			      gpio_arm_ahb_pin_cfg_t *cfg);

/**
 * @brief Sets or clears GPIO output levels using a raw bitmask.
 *
 * Bits set to 1 drive the corresponding pins high, while bits cleared to 0
 * drive the corresponding pins low.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] val Bitmask of output levels to apply.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance is invalid.
 */
int gpio_arm_ahb_port_set_pins_raw(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t val);

/**
 * @brief Sets the selected GPIO pins to a high output level.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] pins Bitmask of pins to set.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance is invalid.
 */
int gpio_arm_ahb_port_set_pins(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins);

/**
 * @brief Clears the selected GPIO pins to a low output level.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] pins Bitmask of pins to clear.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance is invalid.
 */
int gpio_arm_ahb_port_clr_pins(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins);

/**
 * @brief Toggles the selected GPIO pins.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] pins Bitmask of pins to toggle.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance is invalid.
 */
int gpio_arm_ahb_port_toggle_pins(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins);

/**
 * @brief Applies a configuration value to a single GPIO pin.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] pin Pin number within the port.
 * @param[in] cfg Pointer to the configuration value to apply.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance or configuration pointer is invalid.
 */
static inline int gpio_arm_ahb_set_pin_cfg(struct gpio_arm_ahb_port *gpio, uint8_t pin,
					   gpio_arm_ahb_pin_cfg_t *cfg)
{
	return gpio_arm_ahb_set_port_cfg(gpio, (1U << pin), cfg);
}

/**
 * @brief Sets a single GPIO pin to a high output level.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] pin Pin number within the port.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance is invalid.
 */
static inline int gpio_arm_ahb_pin_set(struct gpio_arm_ahb_port *gpio, uint8_t pin)
{
	return gpio_arm_ahb_port_set_pins(gpio, (1U << pin));
}

/**
 * @brief Clears a single GPIO pin to a low output level.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] pin Pin number within the port.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance is invalid.
 */
static inline int gpio_arm_ahb_pin_clr(struct gpio_arm_ahb_port *gpio, uint8_t pin)
{
	return gpio_arm_ahb_port_clr_pins(gpio, (1U << pin));
}

/**
 * @brief Toggles a single GPIO pin.
 *
 * @param[in] gpio GPIO port instance.
 * @param[in] pin Pin number within the port.
 *
 * @retval 0 on success.
 * @retval -EINVAL when the GPIO instance is invalid.
 */
static inline int gpio_arm_ahb_pin_toggle(struct gpio_arm_ahb_port *gpio, uint8_t pin)
{
	return gpio_arm_ahb_port_toggle_pins(gpio, (1U << pin));
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif // __GPIO_ARM_AHB__H
