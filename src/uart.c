#include "../include/uart.h"

#define UART0_NODE DT_NODELABEL(usart1)  // as in our device tree

#if !DT_NODE_HAS_STATUS(UART0_NODE, okay)
#error "Unsupported board: uart0 devicetree alias is not defined"
#endif

K_MUTEX_DEFINE(uart_mutex);

static uart_t uart = {
    .uart_dev = DEVICE_DT_GET(UART0_NODE)
};

void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data) {
    dev = &uart.uart_dev;
    k_mutex_lock(&uart_mutex, K_FOREVER);

    if (!device_is_ready(dev)) {
        printk("UART device not found!");
        k_mutex_unlock(&uart_mutex);
        return;
    }

    struct uart_config uart_cfg = {
        .baudrate = 115200,
        .parity = UART_CFG_PARITY_NONE,
        .stop_bits = UART_CFG_STOP_BITS_1,
        .data_bits = UART_CFG_DATA_BITS_8,
        .flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
    };

    int err = uart_configure(dev, &uart_cfg);
    if (err) {
        printk("Failed to configure UART device!");
        k_mutex_unlock(&uart_mutex);
        return;
    }

    printk("UART initialized.\n");
    switch (evt->type) {
    case UART_TX_DONE:
        printk("UART_TX_DONE\n");
        break;
    case UART_RX_RDY:
        printk("Received data: %s\n", evt->data.rx.buf);
        break;
    default:
        break;
    }

    k_mutex_unlock(&uart_mutex);
}

void initUart(uart_t *uart)
{
    uart_callback_set(uart->uart_dev, uart_cb, NULL);
}