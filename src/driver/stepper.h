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

#if defined(CONFIG_MICRO_STEPPING_16_STEPS)
#define MICRO_STEPPING 16
#elif defined(CONFIG_MICRO_STEPPING_8_STEPS)
#define MICRO_STEPPING 8
#elif defined(CONFIG_MICRO_STEPPING_QUARTER_STEP)
#define MICRO_STEPPING 4
#elif defined(CONFIG_MICRO_STEPPING_HALF_STEP)
#define MICRO_STEPPING 2
#elif defined(CONFIG_MICRO_STEPPING_FULL_STEP)
#define MICRO_STEPPING 1
#endif

/**
 * @brief The absolute maximum number of steps.
 */
#define MAX_STEPS (CONFIG_MOTOR_RAW_STEPS * MICRO_STEPPING)

/**
 * @brief Enumerates the stepper motor rotation directions
 */
enum stepper_direction {
	STEPPER_DIR_CW = 0, /**< Clockwise rotation. */
	STEPPER_DIR_CCW,    /**< Counter-clockwise rotation. */
};

/**
 * @brief Enumerates the stepper axises.
 */
enum stepper_axis {
	STEPPER_AXIS_RA = 0, /**< Right Ascension axis. */
	STEPPER_AXIS_DEC,    /**< Declination axis. */

	STEPPER_AXIS_COUNT, /**< Count of total axises. */
};

/**
 * @brief Defines a stepper motor, based on a A4988 driver.
 */
struct stepper_motor {
	struct {
		const struct gpio_dt_spec enable;    /**< Enables the driver. */
		const struct gpio_dt_spec direction; /**< Sets the rotation direction. */
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
	bool is_on; /**< Indicates if the motor is been driven. */
};

/**
 * @brief Initializes a stepper motor.
 *
 * @param axis Axis to be initialized.
 * @return 0 if successful, an -errno int otherwise.
 */
int axis_init(enum stepper_axis axis);
/**
 * @brief Enables the movement of a desired stepper motor.
 *
 * @param axis Axis to be enabled.
 */
void axis_enable(enum stepper_axis axis);

/**
 * @brief Disables the movement of a desired stepper motor.
 *
 * @param axis Axis to be disabled.
 */
void axis_disable(enum stepper_axis axis);

/**
 * @brief Sets the movement direction of a desired stepper motor.
 *
 * @param axis Axis to have the direction set.
 * @param dir Direction of movement.
 */
void axis_set_direction(enum stepper_axis axis, enum stepper_direction dir);

/**
 * @brief Configures the driving PWM period for the stepper motor, setting it's speed.
 *
 * @param axis Axis to update.
 * @param period_us Period for each step, in microseconds.
 * @return 0 if successful, an -errno int otherwise.
 */
int axis_set_speed(enum stepper_axis axis, uint32_t period_us);

#endif /* ZEPHYR_EQM_STEPPER_H */
