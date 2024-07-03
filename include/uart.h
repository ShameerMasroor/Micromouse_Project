#ifndef UART_H
#define UART_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

extern struct k_mutex uart_mutex;

typedef struct {
    const struct device *uart_dev;
} uart_t;

void initUart(uart_t *uart);

#endif // UART_H