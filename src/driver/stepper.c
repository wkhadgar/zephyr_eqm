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

int stepper_init(struct stepper_motor *stepper_motor)
{
	if (gpio_pin_configure_dt(&stepper_motor->control.enable, GPIO_OUTPUT_INACTIVE) ||
	    gpio_pin_configure_dt(&stepper_motor->control.direction, GPIO_OUTPUT_INACTIVE)) {
		return -ENODEV;
	}

	return 0;
}

void stepper_enable(struct stepper_motor *stepper_motor)
{
	gpio_pin_set_dt(&stepper_motor->control.enable, true);
}

void stepper_disable(struct stepper_motor *stepper_motor)
{
	gpio_pin_set_dt(&stepper_motor->control.enable, false);
}

void stepper_set_direction(struct stepper_motor *stepper_motor, enum stepper_direction dir)
{
	gpio_pin_set_dt(&stepper_motor->control.direction, dir);
}

int stepper_set_speed(struct stepper_motor *stepper_motor, uint16_t steps_per_second)
{
	uint32_t micro_seconds_period;

	if (steps_per_second == 0) {
		return -EDOM;
	}

	micro_seconds_period = 1000000 / steps_per_second;
	pwm_set_dt(&stepper_motor->control.step, micro_seconds_period, micro_seconds_period >> 1);
	stepper_motor->steps_per_second = steps_per_second;

	return 0;
}
