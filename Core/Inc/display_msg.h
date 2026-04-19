#ifndef UART_TX_MSG_H
#define UART_TX_MSG_H

#include <stdint.h>
typedef struct {
    uint16_t countValue; // 计数值
    uint16_t lightValue;    // 光照转换值
} UartTxMsg_t;

#endif // UART_TX_MSG_H 