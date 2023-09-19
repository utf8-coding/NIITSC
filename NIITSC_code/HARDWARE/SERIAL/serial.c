#include "serial.h"
#include "led.h"

void Serial_SendByte(USART_TypeDef* USARTx, uint8_t Byte)
{
	USART_SendData(USARTx, Byte);

	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(USART_TypeDef * channel, uint8_t *array, uint16_t length)
{
	for (int i = 0; i < length; i ++)
	{
		Serial_SendByte(channel, array[i]);
	}
}

int Serial_Receive_Data_LH(USART_TypeDef* UARTx, u8 *buffer, u8 buffer_size, u8 data_size, u8 *out_buffer, u16 header, u16 tail)
{
	USART_ClearITPendingBit(UARTx, USART_IT_RXNE);
	static u8 buffer_itr = 0;
	static u8 rx_state = 0;
	static u8 prev_data = 0x00;
	u8 data = USART_ReceiveData(UARTx);
	
	if(rx_state==0 && data == (header&0x00ff) && prev_data == (header>>8))
	{
		rx_state=1;
		buffer[buffer_itr++]=prev_data;
		buffer[buffer_itr++]=data;
	}
	else if(rx_state==1)
	{
		buffer[buffer_itr++]=data;
		if (data == (tail&0x00ff) && prev_data == (tail>>8) && buffer_itr == data_size)
		{	
			rx_state = 2;
			
		}
		if(buffer_itr >= buffer_size)       //buffer接受满了，接受数据结束
		{
			rx_state = 3;
		}
	}
	else if(rx_state >= 2)		
	{
		USART_ITConfig(UARTx,USART_IT_RXNE,DISABLE);
		if(rx_state == 2)
		{
			for(int i = 0; i < data_size; i++)
			{
				out_buffer[i] = buffer[i];
			}
		}
		for(int i=0;i<buffer_size;i++)
		{
			buffer[i]=0x00;      //将存放数据数组清零
		}
		USART_ITConfig(UARTx,USART_IT_RXNE,ENABLE);
		buffer_itr = 0;
		rx_state = 0;
		return 3-rx_state; //2->1, 3->0
	}
	prev_data  = data;
	return 0;
}
