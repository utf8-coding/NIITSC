#include "ops.h"
#include "serial.h"
#include "oled.h"
#include "led.h"
#include "flags.h"
#include "string.h"
#include "delay.h"

float OPS_x = 0, OPS_y = 0, OPS_heading = 0;
int32_t OPS_ring = 0;

void OPS_UART_Config(void);

void OPS_Init(void)
{
	OPS_UART_Config();
}

void OPS_UART_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(OPS_UART_TX_CLK
						| OPS_UART_RX_CLK, ENABLE); //GPIO_CLK enable
	RCC_APB1PeriphClockCmd(OPS_UART_CLK, ENABLE);	//UART_CLK enable
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = OPS_UART_TX_PIN;
	GPIO_Init(OPS_UART_TX_PORT, &GPIO_InitStructure);		
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = OPS_UART_RX_PIN;
	GPIO_Init(OPS_UART_RX_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(OPS_UART_TX_PORT, OPS_UART_TX_PinSource, GPIO_AF_OPS_UART);
	GPIO_PinAFConfig(OPS_UART_RX_PORT, OPS_UART_RX_PinSource, GPIO_AF_OPS_UART);

	USART_InitStructure.USART_BaudRate = OPS_UART_BaudRate;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(OPS_UART,&USART_InitStructure); 
	USART_Cmd(OPS_UART,ENABLE);
	
	USART_ITConfig(OPS_UART, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = OPS_UART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 2;
	NVIC_Init(&NVIC_InitStructure);
}

u8 OPS_Rx_Buffer[32] = {0x00};
OPS OPS_data = {0x00};
void UART5_IRQHandler()
{
	if(USART_GetITStatus(OPS_UART, USART_IT_RXNE) == SET)
	{
		//USART_TypeDef* UARTx, u8 *buffer, u8 buffer_size, u8 data_size, u8 *out_buffer, u16 header, u16 tail
		if(Serial_Receive_Data_LH(UART5, OPS_Rx_Buffer, 30, 28, &OPS_data.data[2], 0x0d0a, 0x0a0d))
		{
			static u8 ring_lock = 0;
			if (OPS_heading > 170 && -OPS_data.ActVal[1] < -170 && !ring_lock)
			{
				OPS_ring += 1;
				ring_lock = 1;
			}
			else if (OPS_heading < -170 && -OPS_data.ActVal[1] > 170 && !ring_lock)
			{
				OPS_ring -= 1;
				ring_lock = 1;
			}
			else if (OPS_heading >= -170 && OPS_heading <= 170)
			{
				ring_lock = 0;
			}
				
			
			OPS_heading = -OPS_data.ActVal[1];//degree
			OPS_x = OPS_data.ActVal[4]/1000;
			OPS_y = OPS_data.ActVal[5]/1000; //m
		}

		if(OPS_heading+OPS_x+OPS_y != 0 && !flag_ops_ready)
			flag_ops_ready = 1;
	}
}

void OPS_Calibrate(float x, float y, float heading)
{
	u8 update_x[4] = "ACTX";
	u8 update_y[4] = "ACTY";
	u8 update_j[4] = "ACTJ";
	
	static union
	{
		float value;
		u8 data[4];
	}new_value;
	
	new_value.value = x;
	Serial_SendArray(UART5, update_x, 4);
	Serial_SendArray(UART5, new_value.data, 4);
	
	delay_ms(10);
	new_value.value = y;
	Serial_SendArray(UART5, update_y, 4);
	Serial_SendArray(UART5, new_value.data, 4);
	
	delay_ms(10);
	new_value.value = -heading;
	Serial_SendArray(UART5, update_j, 4);
	Serial_SendArray(UART5, new_value.data, 4);
}

void OPS_Display_Specs(void)
{
	OLED_ShowString(1, 1, "OPS:");
	OLED_ShowSignedNum(2, 1, (int)OPS_heading+OPS_ring*360, 14);
	OLED_ShowSignedNum(3, 1, (int)(OPS_x*100), 12); //cm here
	OLED_ShowSignedNum(4, 1, (int)(OPS_y*100), 12);
}
