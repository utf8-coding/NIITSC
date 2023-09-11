#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f4xx.h"                  // Device header

void Serial_SendByte(USART_TypeDef* USARTx, uint8_t Byte);
void Serial_SendArray(USART_TypeDef * channel, uint8_t *array, uint16_t length);
int Serial_Receive_Data_LH(USART_TypeDef* UARTx, u8 *buffer, u8 buffer_size, u8 data_size, u8 *out_buffer, u16 header, u16 tail);

#endif
