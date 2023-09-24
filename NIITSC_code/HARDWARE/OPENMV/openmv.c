#include "openmv.h"
#include "oled.h"
#include "serial.h"
#include "flags.h"
#include "delay.h"
#include "buzzer.h"
#include "infrared.h"

//public:
int mv_target_cx = 0, mv_target_cy = 0;
u8 mark1 = 0;

//internal:
int IMAGE_SIZE_X = 320, IMAGE_SIZE_Y = 240;
int SET_RETRY_LIMIT = 3;

void OPENMV_USART_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_AHB1PeriphClockCmd(OPENMV_USART_TX_CLK
                      | OPENMV_USART_RX_CLK, ENABLE); //GPIO_CLK enable
  RCC_APB1PeriphClockCmd(OPENMV_USART_CLK, ENABLE);   //USART_CLK enable
  
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = OPENMV_USART_TX_PIN;
  GPIO_Init(OPENMV_USART_TX_PORT, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = OPENMV_USART_RX_PIN;	
  GPIO_Init(OPENMV_USART_RX_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(OPENMV_USART_TX_PORT, OPENMV_USART_TX_PinSource, GPIO_AF_OPENMV_USART);
  GPIO_PinAFConfig(OPENMV_USART_RX_PORT, OPENMV_USART_RX_PinSource, GPIO_AF_OPENMV_USART);

  USART_InitStructure.USART_BaudRate = OPENMV_USART_BaudRate;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  
  USART_Init(OPENMV_USART,&USART_InitStructure); //初始化串口1
  USART_Cmd(OPENMV_USART,ENABLE); //使能串口1
  
  USART_ITConfig(OPENMV_USART, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = OPENMV_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 0;
  NVIC_Init(&NVIC_InitStructure);
}

u8 OMV_Rx_Buffer[15] = {0x00};
u8 OMV_data[15] = {0x00};
void USART2_IRQHandler()
{
	if(USART_GetITStatus(OPENMV_USART, USART_IT_RXNE) == SET)
	{
		//USART_TypeDef* UARTx, u8 *buffer, u8 buffer_size, u8 data_size, u8 *out_buffer, u16 header, u16 tail
		Serial_Receive_Data_LH(OPENMV_USART, &OMV_Rx_Buffer[0], 15, 10, &OMV_data[0], 0x5A5A, 0xA6A6);
	}
}

void OpenMV_Data_Process(void)
{
//	//openMV mode:
//	if (OMV_data[2] == 0)
//		flag_openmv_mode = 0;
//	else if (0x01&OMV_data[2])
//		flag_openmv_mode = 1;
//	else if (0x02&OMV_data[2])
//		flag_openmv_mode = 2;
//	else if (0x04&OMV_data[2])
//		flag_openmv_mode = 3;
//	else if (0x08&OMV_data[2])
//		flag_openmv_mode = 4;
//	
//	//Target cx, cy data:
//	//possible bug here, MSB/LSB in advance?
//	mv_target_cx = (OMV_data[3]|OMV_data[4]<<8) - IMAGE_SIZE_X/2;
//	mv_target_cy = (OMV_data[5]|OMV_data[6]<<8) - IMAGE_SIZE_Y/2;
//	
//	//mark1
//	mark1 = OMV_data[7];
	mark1 = ((Infrared_Scan()&0x02)>>1) | ((Infrared_Scan()&0x08)>>2);
}

void OpenMV_Display_Specs(void)
{
	OLED_ShowString(1, 2, "OpenMV:");

	OLED_ShowBinNum(3, 3, mark1, 8);
}

//Idle:0, Red:1, G:2, B:3, recog:4
int OpenMV_Change_Mode(u8 mode)
{
	OLED_Clear();
	OLED_ShowString(1, 1, "OpenMV_Change_Mode."); 
	Serial_SendByte(OPENMV_USART, mode);
	
	for(int retry_counter = 0;retry_counter >= SET_RETRY_LIMIT; retry_counter++)
	{
		if (flag_openmv_mode == mode)
			return 1;
		else if (retry_counter >= SET_RETRY_LIMIT)
			//Should be a warning here. Buzzer or something.
			return 0;
		
		delay_ms(100);
		Serial_SendByte(OPENMV_USART, mode);
		retry_counter++;
	}
	return 0;
}


