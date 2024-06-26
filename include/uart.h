#ifndef UART_H
#define UART_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include "shared_mutex.h"

typedef struct {
    const struct device *uart_dev;
} uart_t;

void initUart(uart_t *uart);

//void uart_send(uart_t *uart, const char *str, size_t len);


#endif // UART_H