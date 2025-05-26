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
#include "driver/usb.h"
#include "infra/guards.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

int main(void)
{
	LOG_INF("Iniciando Zephyr EQM.\n");
	uint8_t buf[128] = {0};

	LOG_INF("Init USB: %d", zeqm_usb_init());

	while (true) {
		/* Wait for message. */
		LOG_INF("Waiting for message...\n");
		k_sem_take(&recv_sem, K_FOREVER);

		ZEQM_CHECK_ERROR(read_usb_msg(buf, sizeof(buf), '\n', K_SECONDS(5)),
				 LOG_WRN("Unable to read succesfully: %d", err));

		ZEQM_CHECK_ERROR(send_usb_msg(buf, strlen(buf)),
				 LOG_WRN("Unable to send succesfully: %d", err));
	}

	return 0;
}
