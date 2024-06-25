#include "uart.h"

void initUart(uart_t *uart)
{
    uart_callback_set(uart_dev, uart_cb, NULL);
}

void uart_send(uart_t *uart, const char *str, size_t len){}