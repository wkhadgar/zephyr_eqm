/**
 * @file usb.c
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief //TODO
 * @version 0.1
 * @date 23-05-2025
 *
 * @copyright Copyright (c) 2024 Paulo Santos
 *
 */

#include "infra/guards.h"
#include "usb.h"
#include "zephyr/drivers/gpio.h"

#include <stdint.h>
#include <stdio.h>

#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/usb/usb_device.h>

LOG_MODULE_REGISTER(usbd);

#define ZEQM_USB_VID 0xFFFF
#define ZEQM_USB_PID 0xBEEF

#define ZEQM_MANUFACTURER_STR "wkhadgar"
#define ZEQM_PRODUCT_STR      "zEQM"

static struct usb_metadata {
	bool rx_throttled;
	uint8_t ring_buffer[CONFIG_USB_RING_BUF_SIZE];
	const struct device *const uart_dev;
	struct ring_buf ringbuf;
} self = {
	.uart_dev = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart),
};

K_SEM_DEFINE(recv_sem, 0, K_SEM_MAX_LIMIT);

K_MUTEX_DEFINE(ring_buffer_mutex);
K_MUTEX_DEFINE(uart_tx_mutex);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);

static void usb_data_isr(const struct device *dev, void *user_data)
{
	ARG_UNUSED(user_data);

	while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
		if (!self.rx_throttled && uart_irq_rx_ready(dev)) {
			int recv_len, rb_len;
			uint8_t buffer[64];
			size_t len = MIN(ring_buf_space_get(&self.ringbuf), sizeof(buffer));

			if (len == 0) {
				/* Throttle because ring buffer is full */
				uart_irq_rx_disable(dev);
				self.rx_throttled = true;
				continue;
			}

			recv_len = uart_fifo_read(dev, buffer, len);
			if (recv_len < 0) {
				LOG_ERR("Failed to read UART FIFO");
				recv_len = 0;
			}

			rb_len = ring_buf_put(&self.ringbuf, buffer, recv_len);
			if (rb_len < recv_len) {
				LOG_ERR("Drop %u bytes", recv_len - rb_len);
			}

			LOG_DBG("tty fifo -> ringbuf %d bytes", rb_len);
		}
	}

	if (self.rx_throttled) {
		uart_irq_rx_enable(self.uart_dev);
		self.rx_throttled = false;
	}

	k_sem_give(&recv_sem);
	LOG_DBG("ISR hit!");
}

int zeqm_usb_init(void)
{

	if (!device_is_ready(self.uart_dev)) {
		LOG_ERR("CDC ACM device not ready");
		return 0;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	ZEQM_RETURN_ERROR(usb_enable(NULL), LOG_ERR("Failed to enable USB"));

	ring_buf_init(&self.ringbuf, sizeof(self.ring_buffer), self.ring_buffer);

	LOG_INF("Waiting for DTR");

	while (true) {
		uint32_t dtr = 0U;

		uart_line_ctrl_get(self.uart_dev, UART_LINE_CTRL_DTR, &dtr);
		if (dtr) {
			break;
		} else {
			/* Give CPU resources to low priority threads. */
			k_sleep(K_MSEC(100));
		}
	}

	LOG_INF("DTR set");

	/* They are optional, we use them to test the interrupt endpoint */
	ZEQM_CHECK_ERROR(uart_line_ctrl_set(self.uart_dev, UART_LINE_CTRL_DCD, 1),
			 LOG_WRN("Failed to set DCD: %d", err));
	ZEQM_CHECK_ERROR(uart_line_ctrl_set(self.uart_dev, UART_LINE_CTRL_DSR, 1),
			 LOG_WRN("Failed to set DSR: %d", err));

	/* Wait 100ms for the host to do all settings */
	k_msleep(100);

	uart_irq_callback_set(self.uart_dev, usb_data_isr);
	k_mutex_init(&ring_buffer_mutex);
	k_mutex_init(&uart_tx_mutex);

	/* Enable rx interrupts */
	uart_irq_rx_enable(self.uart_dev);

	return 0;
}

int read_usb_msg(uint8_t *buf, const size_t max_len, const char terminator,
		 const k_timeout_t timeout)
{
	ZEQM_RETURN_ERROR(k_mutex_lock(&ring_buffer_mutex, timeout),
			  LOG_ERR("Unable to lock usb read mutex"));

	for (size_t i = 0; i < max_len; i++) {
		if (ring_buf_get(&self.ringbuf, buf + i, 1) == 0) {
			return -ENOMSG;
		}

		if (buf[i] == terminator) {
			buf[i+1] = '\0';
			break;
		}
	}

	k_mutex_unlock(&ring_buffer_mutex);

	return 0;
}

int send_usb_msg(const uint8_t *data, const size_t len)
{
	k_mutex_lock(&uart_tx_mutex, K_MSEC(100));
	for (size_t i = 0; i < len; i++) {
		uart_poll_out(self.uart_dev, data[i]);
	}
	k_mutex_unlock(&uart_tx_mutex);

	return 0;
}
