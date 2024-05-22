/**
 * @file gnss.h
 * @author Paulo Santos (pauloroberto.santos@edge.ufal.br)
 * @brief Ponto de entrada da aplicação.
 * @version 0.1
 * @date 21-08-2023
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef GNSS_H
#define GNSS_H

#include <zephyr/drivers/gnss.h>

/**
 * @brief Get the current GNSS data reference.
 *
 * @return GNSS data reference.
 */
struct gnss_data *gnss_get_ref(void);

#endif /*GNSS_H */
