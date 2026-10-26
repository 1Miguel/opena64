/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#include <errno.h>
#include "gpio_arm_ahb_regs.h"
#include "gpio_arm_ahb.h"
#include "mmio.h"

int gpio_arm_ahb_set_port_cfg(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t msk,
			      gpio_arm_ahb_pin_cfg_t *cfg)
{
	uintptr_t reg;

	if (!gpio || !cfg || !msk) {
		return -EINVAL;
	}

	// set output mode
	reg = (*cfg & GPIO_ARM_AHB_OUTPUT) ? GPIO_ARM_AHB_OUTENSET(gpio->reg)
					   : GPIO_ARM_AHB_OUTENCLR(gpio->reg);
	mmio_bit_set_u16(reg, msk);

	// alternate functon set
	reg = (*cfg & GPIO_ARM_AHB_OUTPUT) ? GPIO_ARM_AHB_ALTFUNCSET(gpio->reg)
					   : GPIO_ARM_AHB_ALTFUNCCLR(gpio->reg);
	mmio_bit_set_u16(reg, msk);

	// init to high or low logic level
	(*cfg & GPIO_ARM_AHB_OUTPUT_INIT_HIGH)
		? mmio_bit_set_u16(GPIO_ARM_AHB_DATA_OUT(gpio->reg), msk)
		: mmio_bit_clr_u16(GPIO_ARM_AHB_DATA_OUT(gpio->reg), msk);

	// set interrupt type (falling / level)
	reg = (*cfg & GPIO_ARM_AHB_INT_TYPE) ? GPIO_ARM_AHB_INTTYPESET(gpio->reg)
					     : GPIO_ARM_AHB_INTTYPECLR(gpio->reg);
	mmio_bit_set_u16(reg, msk);

	// set interrupt polarity (high / low)
	reg = (*cfg & GPIO_ARM_AHB_INT_POL) ? GPIO_ARM_AHB_INTPOLSET(gpio->reg)
					    : GPIO_ARM_AHB_INTPOLCLR(gpio->reg);
	mmio_bit_set_u16(reg, msk);

	// interrupt enable
	reg = (*cfg & GPIO_ARM_AHB_INT_ENABLE) ? GPIO_ARM_AHB_INTENSET(gpio->reg)
					       : GPIO_ARM_AHB_INTENCLR(gpio->reg);
	mmio_bit_set_u16(reg, msk);

	return 0;
}

int gpio_arm_ahb_set_irq_cb(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_cb_t cb, void *cb_data)
{
	if (!gpio || !cb) {
		return -EINVAL;
	}

	gpio->cb = cb;
	gpio->cb_data = cb_data;

	return 0;
}

int gpio_arm_ahb_port_set_pins(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins)
{
	if (!gpio || !pins) {
		return -EINVAL;
	}
	mmio_bit_set_u16(GPIO_ARM_AHB_DATA_OUT(gpio->reg), pins);
	return 0;
}

int gpio_arm_ahb_port_set_pins_raw(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins)
{
	if (!gpio || !pins) {
		return -EINVAL;
	}
	mmio_write_u16(GPIO_ARM_AHB_DATA_OUT(gpio->reg), pins);
	return 0;
}

int gpio_arm_ahb_port_clr_pins(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins)
{
	if (!gpio || !pins) {
		return -EINVAL;
	}
	mmio_bit_clr_u16(GPIO_ARM_AHB_DATA_OUT(gpio->reg), pins);
	return 0;
}

int gpio_arm_ahb_port_toggle_pins(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins)
{
	if (!gpio || !pins) {
		return -EINVAL;
	}
	mmio_bit_toggle_u16(GPIO_ARM_AHB_DATA_OUT(gpio->reg), pins);
	return 0;
}

int gpio_arm_ahb_set_irq_ack(struct gpio_arm_ahb_port *gpio, gpio_arm_ahb_port_val_t pins)
{
	if (!gpio || !pins) {
		return -EINVAL;
	}
	mmio_bit_set_u16(GPIO_ARM_AHB_INTSTATUS(gpio->reg), pins);
}

void gpio_arm_ahb_irq_handler(struct gpio_arm_ahb_port *gpio)
{
	gpio_arm_ahb_port_val_t pins = mmio_read_u16(GPIO_ARM_AHB_INTSTATUS(gpio->reg));

	if (gpio->cb) {
		gpio->cb(gpio, pins, gpio->cb_data);
	} else {
		// auto ack
		mmio_write_u16(GPIO_ARM_AHB_INTSTATUS(gpio->reg), pins);
	}
}
