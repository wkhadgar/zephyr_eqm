/**
 * @file usb.h
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief //TODO
 * @version 0.1
 * @date 23-05-2025
 *
 * @copyright Copyright (c) 2024 Paulo Santos
 *
 */

#ifndef USB_H
#define USB_H

#include <zephyr/usb/usbd.h>

/**
 * @brief Data received semaphore.
 */
extern struct k_sem recv_sem;

int zeqm_usb_init(void);

/**
 * @brief Read a message from the USB ring buffer.
 *
 * @param buf Read buffer.
 * @param max_len Size of read buffer.
 * @param terminator Message termination character.
 * @param timeout Timeout to read a message.
 * @return 0 if successfully read a message, an -errno otherwise.
 */
int read_usb_msg(uint8_t *buf, size_t max_len, char terminator, k_timeout_t timeout);

/**
 * @brief Send a message through USB.
 *
 * @param data Data buffer to be sent.
 * @param len Size of data buffer.
 * @return 0 if successfully read a message, an -errno otherwise.
 */
int send_usb_msg(const uint8_t *data, size_t len);

#endif /* USB_H */
