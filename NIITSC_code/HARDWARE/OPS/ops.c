#include "ops.h"
#include "serial.h"
#include "oled.h"
#include "led.h"
#include "flags.h"
#include "string.h"
#include "delay.h"

float OPS_x = 0, OPS_y = 0, OPS_heading = 0;
int32_t OPS_ring = 0;
u8 flag_DMA_running = 0;
OPS OPS_data = {0x00};

void OPS_USART_Config(void);
void OPS_DMA_Init(void);

void OPS_Init(void)
{
	OPS_USART_Config();
	OPS_DMA_Init();
}

void OPS_USART_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(OPS_UART_TX_CLK
						| OPS_UART_RX_CLK, ENABLE); //GPIO_CLK enable
	RCC_APB2PeriphClockCmd(OPS_UART_CLK, ENABLE);	//UART_CLK enable
	
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
	
	new_value.value = x*1000;
	Serial_SendArray(OPS_UART, update_x, 4);
	Serial_SendArray(OPS_UART, &new_value.data[0], 4);
	
	delay_ms(30);
	new_value.value = y*1000;
	Serial_SendArray(OPS_UART, update_y, 4);
	Serial_SendArray(OPS_UART, &new_value.data[0], 4);
	
	delay_ms(30);
	new_value.value = -heading*1000;
	Serial_SendArray(OPS_UART, update_j, 4);
	Serial_SendArray(OPS_UART, &new_value.data[0], 4);
}

void OPS_Display_Specs(void)
{
	OLED_ShowString(1, 2, "OPS:");
	OLED_ShowSignedNum(2, 2, (int)OPS_heading+OPS_ring*360, 12);
	OLED_ShowSignedNum(3, 2, (int)(OPS_x*100), 5); 
	OLED_ShowSignedNum(3, 9, (int)(OPS_y*100), 5); //cm here
	OLED_ShowString(4, 2, "Reset~");
}


/* *********************************************
	������DMA�汾�ĳ�ʼ������������ʹ�õ�OPS���ݴ���
	DMAͨ�����ò鿴��blog.csdn.net/qwsgdjfk/article/details/131401644
************************************************ */

void OPS_DMA_Init(){
	RCC_AHB1PeriphClockCmd(OPS_DMA_CLK, ENABLE);
	
	DMA_InitTypeDef  DMA_InitStructure;	
  DMA_DeInit(OPS_DMA_Stream);
	
	while (DMA_GetCmdStatus(OPS_DMA_Stream) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = OPS_DMA_Channel;                           //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;             //DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&OPS_data.data[2];                  //DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                    //���赽������
  DMA_InitStructure.DMA_BufferSize = OPS_DMA_Buf_Size;               			   //���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           //���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    //�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;            //�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                            //ʹ������ģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                    //���ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;				 //����ͻ�����δ���
  DMA_Init(OPS_DMA_Stream, &DMA_InitStructure);															 //��ʼ��DMA Stream
	
	USART_DMACmd(OPS_UART,USART_DMAReq_Rx,DISABLE);														 //��֡�ɹ�ʱ������
	DMA_Cmd(OPS_DMA_Stream, DISABLE);
}

//����ֻ���ڷ������õ�
/*
void OPS_DMA_Enable(void){
	DMA_Cmd(OPS_DMA_Stream, DISABLE);                      			//�ر�DMA���� 
	while (DMA_GetCmdStatus(OPS_DMA_Stream) != DISABLE){}				//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(OPS_DMA_Stream, OPS_DMA_Buf_Size);   //���ݴ�����  
 
	DMA_Cmd(OPS_DMA_Stream, ENABLE);                    			  //����DMA���� 
}
*/


void USART1_IRQHandler(void) 
{
  u8 data = 0;
  static u8 frame_alignment_state = 0; //0:δ���� 1:��һ��֡ͷ 2:�ڶ���֡ͷ
	//HW BUG handle
	if(USART_GetITStatus(USART1, USART_IT_PE) == SET)
	{//Parity Error interrupt
		USART_ClearFlag(USART1, USART_FLAG_PE);
	}
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) 
	{
		//Frame alignment on start up. ������֡������
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data = USART_ReceiveData(USART1);
		if(frame_alignment_state==1 && data==0x0d)                          
		{
			//��֡�ɹ��������жϹرգ�DMA����
			USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);    
			DMA_Cmd(OPS_DMA_Stream, ENABLE);                                 
	
			flag_DMA_running = 1;  
			frame_alignment_state = 0;  
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);       
		}
		else if(data == 0x0a) {
			frame_alignment_state = 1;
		}
		else frame_alignment_state = 0;
  }
}

//Need to be placed in main loop
void OPS_Data_Process(void){
	if(flag_DMA_running)
	{
		if(OPS_data.data[2] != 0x0D || OPS_data.data[3] != 0x0A) {
			//���¶�֡��
			USART_DMACmd(USART1,USART_DMAReq_Rx,DISABLE);
			DMA_Cmd(OPS_DMA_Stream, DISABLE);                       
			OLED_Clear();
			OLED_ShowString(1,1,"OPS FRM ERR");
			flag_DMA_running = 0;
			USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
		} else {
			//תȦ����
			static u8 ring_lock = 0;
			if (OPS_heading > 170 && -OPS_data.ActVal[1] < -170 && !ring_lock){
				OPS_ring += 1;
				ring_lock = 1;
			}
			else if (OPS_heading < -170 && -OPS_data.ActVal[1] > 170 && !ring_lock){
				OPS_ring -= 1;
				ring_lock = 1;
			}
			else if (OPS_heading >= -170 && OPS_heading <= 170){
				ring_lock = 0;
			}
			//λ������
			OPS_heading = -OPS_data.ActVal[1];//degree
			OPS_x = OPS_data.ActVal[4]/1000;
			OPS_y = OPS_data.ActVal[5]/1000; //m
			//OPS����У׼���
			if(OPS_heading+OPS_x+OPS_y != 0 && !flag_ops_ready){
				flag_ops_ready = 1;
			}
		}
	}
	//�����Ƿ�����أ���������ʱ����DMA������
	/*	
		if(DMA_GetFlagStatus(DMA2_Stream5,DMA_FLAG_TCIF7)!=RESET)//??DMA2_Steam7????
		{ 
			DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF7);//??DMA2_Steam7??????
		}
		
		MYDMA_Enable(DMA2_Stream7,38); //WHY�� 
	*/
}

//�жϰ����ݴ���
/*
u8 OPS_Rx_Buffer[32] = {0x00};
void USART1_IRQHandler()
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
*/
