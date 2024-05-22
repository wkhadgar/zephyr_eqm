/**
 * @file target_controller.c
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Controller for the target selection and handle.
 * @version 0.1
 * @date 12-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "target_controller.h"

/**
 * @brief Period for the PWM step signal for tracking, in microseconds.
 */
#define TRACKING_STEP_PERIOD_US 281250

/**
 * @brief Board LED GPIO.
 */
struct gpio_dt_spec builtin_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

/**
 * @brief Target controller data.
 */
static struct tg_controller {
	uint8_t hemisphere: 1;
} self = {
	.hemisphere = TG_SOUTH_HEMISPHERE,
};

int tg_init(void)
{
	int err;

	gpio_pin_configure_dt(&builtin_led, GPIO_OUTPUT_INACTIVE);

	err = axis_init(STEPPER_AXIS_RA);
	if (err) {
		return err;
	}

	err = axis_init(STEPPER_AXIS_DEC);
	if (err) {
		return err;
	}

	return 0;
}

void tg_start_tracking(void)
{
	axis_set_direction(STEPPER_AXIS_RA, self.hemisphere == TG_NORTH_HEMISPHERE
						    ? STEPPER_DIR_CW
						    : STEPPER_DIR_CCW);
	axis_enable(STEPPER_AXIS_RA);
	axis_set_speed(STEPPER_AXIS_RA, TRACKING_STEP_PERIOD_US);
}
