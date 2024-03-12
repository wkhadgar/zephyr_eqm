/**
 * @file stepper.h
 * @author Paulo Santos (pauloroberto.santos@edge.ufal.br)
 * @brief Stepper motor driver for a A988 driven stepper motor.
 * @version 0.1
 * @date 12-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef ZEPHYR_EQM_STEPPER_H
#define ZEPHYR_EQM_STEPPER_H

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

/**
 * @brief The true resolution of the stepper motor, in steps.
 */
#define MOTOR_RAW_STEPS 200

/**
 * @brief If the driver has micro-stepping functionality, this indicates how many micro-steps are in
 * a single raw step.
 */
#define MICRO_STEPPING 16

/**
 * @brief The absolute maximum number of steps.
 */
#define MAX_STEPS (MOTOR_RAW_STEPS * MICRO_STEPPING)

/**
 * @brief Enumerates the stepper motor rotation directions
 */
enum stepper_direction {
	STEPPER_CW = 0, /**< Clockwise rotation. */
	STEPPER_CCW,    /**< Counter-clockwise rotation. */
};

/**
 * @brief Defines a stepper motor, based on a A4988 driver.
 */
struct stepper_motor {
	struct {
		struct gpio_dt_spec enable;    /**< Enables the driver. */
		struct gpio_dt_spec direction; /**< Sets the rotation direction. */
		struct pwm_dt_spec
			step; /**< PWM signal for steps, each rising edge is a step to be taken. */
	} control;            /**< Encapsulates the control hardware interface. */

	struct {
		uint32_t current; /**< Current position, in steps. */
		uint32_t target;  /**< Target position, in steps.*/
	} position; /**< Encapsulates the positional information, the steps are considered as motor
		       physical step (if micro-stepping is enabled, the micro step is considered).
		       It must be noted that the step counts should be interpreted as cyclical, but
		       we wont consider the crossing of a boundary (0 to MAX_STEPS) as a valid path.
		       This makes sure that the stepper motor wont ever travel to a target position
		       crossing a boundary. This is needed for safety reasons. */

	uint16_t steps_per_second; /**< The rate of rotation, in steps per second. */
	bool is_on;                /**< Indicates if the motor is been driven. */
};

#endif /* ZEPHYR_EQM_STEPPER_H */
