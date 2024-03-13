/**
 * @file stepper.c
 * @author Paulo Santos (pauloroberto.santos@edge.ufal.br)
 * @brief Stepper motor driver for a A988 driven stepper motor.
 * @version 0.1
 * @date 12-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stepper.h"

#include <zephyr/drivers/gpio.h>

/**
 * @brief Defines the mount axises as stepper motors.
 */
static struct stepper_motor axises[STEPPER_AXIS_COUNT] = {
	[STEPPER_AXIS_RA] =
		{
			.control =
				{
					.direction = GPIO_DT_SPEC_GET(
						DT_NODELABEL(stepper_ra_direction), gpios),
					.enable = GPIO_DT_SPEC_GET(DT_NODELABEL(stepper_ra_enable),
								   gpios),
					.step = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
				},
			.position =
				{
					.current = MAX_STEPS / 2,
					.target = MAX_STEPS / 2,
				},
			.is_on = false,
		},
	[STEPPER_AXIS_DEC] =
		{
			.control =
				{
					.direction = GPIO_DT_SPEC_GET(
						DT_NODELABEL(stepper_dec_direction), gpios),
					.enable = GPIO_DT_SPEC_GET(DT_NODELABEL(stepper_dec_enable),
								   gpios),
					.step = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
				},
			.position =
				{
					.current = MAX_STEPS / 2,
					.target = MAX_STEPS / 2,
				},
			.is_on = false,
		},
};

int axis_init(enum stepper_axis axis)
{
	if (gpio_pin_configure_dt(&axises[axis].control.enable, GPIO_OUTPUT_INACTIVE) ||
	    gpio_pin_configure_dt(&axises[axis].control.direction, GPIO_OUTPUT_INACTIVE)) {
		return -ENODEV;
	}

	if (!pwm_is_ready_dt(&axises[axis].control.step)) {
		return -EIO;
	}

	return 0;
}
void axis_enable(enum stepper_axis axis)
{
	gpio_pin_set_dt(&axises[axis].control.enable, true);
	axises[axis].is_on = true;
}
void axis_disable(enum stepper_axis axis)
{
	gpio_pin_set_dt(&axises[axis].control.enable, false);
	axises[axis].is_on = false;
}
void axis_set_direction(enum stepper_axis axis, enum stepper_direction dir)
{
	gpio_pin_set_dt(&axises[axis].control.direction, dir);
}

int axis_set_speed(enum stepper_axis axis, uint32_t period_us)
{
	if (period_us < CONFIG_MIN_PERIOD_US) {
		return -EDOM;
	}

	pwm_set_dt(&axises[axis].control.step, PWM_USEC(period_us), PWM_USEC(period_us) >> 1);
	return 0;
}
