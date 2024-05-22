/**
 * @file target_controller.h
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Interface to the Controller for target selection and handle.
 * @version 0.1
 * @date 12-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TARGET_CONTROLLER_H
#define TARGET_CONTROLLER_H

#include "driver/stepper.h"

/**
 * @brief Enumerates the hemispheres.
 */
enum tg_hemisphere {
	TG_NORTH_HEMISPHERE = 0,
	TG_SOUTH_HEMISPHERE = 1,
};

/**
 * @brief Initializes the EQM motors.
 *
 * @retval 0 if successful.
 * @retval -ENODEV if any axis GPIOs can't be initialized.
 * @retval -EIO if any axis PWM is not ready.
 */
int tg_init(void);

/**
 * @brief Initializes right ascension tracking of the current position.
 */
void tg_start_tracking(void);

#endif /* TARGET_CONTROLLER_H */
