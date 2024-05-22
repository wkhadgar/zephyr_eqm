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

#include "app/target_controller.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

int main(void)
{
	LOG_INF("Iniciando Zephyr EQM.\n");

	tg_init();
	tg_start_tracking();

	return 0;
}
