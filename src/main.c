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

#include <zephyr/drivers/gnss.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

struct gpio_dt_spec builtin_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static void gnss_data_cb(const struct device *dev, const struct gnss_data *data)
{
	if (data->info.fix_status != GNSS_FIX_STATUS_NO_FIX) {
		gpio_pin_toggle_dt(&builtin_led);
	}
}

GNSS_DATA_CALLBACK_DEFINE(DEVICE_DT_GET(DT_NODELABEL(gnss)), gnss_data_cb);

int main(void)
{
	LOG_INF("Iniciando Zephyr EQM.\n");
	gpio_pin_configure_dt(&builtin_led, GPIO_OUTPUT_INACTIVE);

	return 0;
}