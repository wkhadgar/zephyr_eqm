/**
 * @file gnss.c
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief GNSS data wrapper.
 * @version 0.1
 * @date 21-08-2023
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "gnss.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gnss);

#define GNSS_MODULE DEVICE_DT_GET(DT_NODELABEL(gnss))

/**
 * @brief GNSS data callback.
 *
 * @param dev GNSS device, as defined on the DTS.
 * @param data[out] Updated GNSS data.
 */
static void gnss_data_cb(const struct device *dev, const struct gnss_data *data);

GNSS_DATA_CALLBACK_DEFINE(GNSS_MODULE, gnss_data_cb);

/**
 * @brief Internal GNSS data.
 */
static struct gnss_data gnss_data;

struct gnss_data *gnss_get_ref(void)
{
	return &gnss_data;
}

static void gnss_data_cb(const struct device *dev, const struct gnss_data *data)
{
	ARG_UNUSED(dev);

	memcpy(&gnss_data, data, sizeof(gnss_data));
}
