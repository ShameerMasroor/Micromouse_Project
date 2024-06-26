#ifndef UART_H
#define UART_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)

#if DT_NODE_HAS_STATUS(UART_DEVICE_NODE, okay)
#define UART_DEVICE_LABEL DT_LABEL(UART_DEVICE_NODE)
#else
#error "No usable UART device found"
#endif

typedef struct {
    const struct device *uart_dev;
    uart_callback_t callback;
} uart_t;

void initUart(uart_t *uart);
void uart_send(uart_t *uart, const char *str, size_t len);

#endif // UART_H
static const struct device *uart_dev = DEVICE_DT_GET(UART0_NODE);