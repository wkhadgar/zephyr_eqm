/**
 * @file main.c
 * @author Paulo Santos (pauloroberto.santos@edge.ufal.br)
 * @brief Ponto de entrada da aplicação.
 * @version 0.1
 * @date 21-08-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(main);

int main(void) {
    LOG_INF("Iniciando Zephyr EQM.\n");

    while (1) {
        k_msleep(1000);
    }

    return 0;
}
