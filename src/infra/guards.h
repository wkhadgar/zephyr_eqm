/**
 * @file guards.h
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief //TODO
 * @version 0.1
 * @date 24-05-2025
 *
 * @copyright Copyright (c) 2024 Paulo Santos
 *
 */

#ifndef GUARDS_H
#define GUARDS_H

/**
 * @brief Checks for a function call, and takes an action upon error.
 *
 * @note a local variable @c err is available for internal handling of errors.
 *
 * @param fn Function call to be verified;
 * @param error_exception Action to be executed if an error in raised.
 */
#define ZEQM_CHECK_ERROR(fn, error_exception)                                                      \
	do {                                                                                       \
		int err = fn;                                                                      \
		if (err < 0) {                                                                     \
			error_exception;                                                           \
		}                                                                                  \
	} while (false)

/**
 * @brief Checks for a function call, and takes an action upon error, finally returns the error.
 *
 * @note a local variable @c err is available for internal handling of errors.
 *
 * @param fn Function call to be verified;
 * @param error_exception Action to be executed if an error in raised.
 */
#define ZEQM_RETURN_ERROR(fn, error_exception)                                                     \
	do {                                                                                       \
		int err = fn;                                                                      \
		if (err < 0) {                                                                     \
			error_exception;                                                           \
			return err;                                                                \
		}                                                                                  \
	} while (false)

#endif /* GUARDS_H */
