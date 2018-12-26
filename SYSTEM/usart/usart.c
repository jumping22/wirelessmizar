#include "sys.h"
#include "usart.h"  


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
u8 USART_RX_BUF[USART_RX_BUF_SIZE];     //���ջ���,���64���ֽ�.
//����״̬
//bit7��������ɱ�־
//bit6�����յ�0x0d
//bit5~0�����յ�����Ч�ֽ���Ŀ
u8 USART_RX_STA=0;       //����״̬���

static recv_callback uart1_recv_callback;


static void _uart1_dma_configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
 
  /* DMA1 Channel6 (triggered by USART1 Rx event) Config */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
 
  /* DMA1 Channel5 (triggered by USART1 Rx event) Config */
  DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART_RX_BUF;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = USART_RX_BUF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
 
  DMA_Cmd(DMA1_Channel5, ENABLE);
 
  //USART_ITConfig(USART1, USART_IT_TC, ENABLE);
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
}


void uart_init(uint32_t bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ն�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	//���߿����ж�
	
	//_uart1_dma_configuration();		//DMA��ʼ��
	
	/* DMA1 Channel5 (triggered by USART1 Rx event) Config */
  DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART_RX_BUF;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = USART_RX_BUF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
 
  DMA_Cmd(DMA1_Channel5, ENABLE);
 
  //USART_ITConfig(USART1, USART_IT_TC, ENABLE);
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}

void uart1_SetRecvCallback(recv_callback cb)
{
	uart1_recv_callback = cb;
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint32_t temp = 0;  
		
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{  
		//USART_ClearFlag(USART1,USART_IT_IDLE);  
		temp = USART1->SR;  
		temp = USART1->DR; //���USART_IT_IDLE��־
		DMA_Cmd(DMA1_Channel5, DISABLE);  

		temp = USART_RX_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);  
		if (uart1_recv_callback) {
			uart1_recv_callback(USART_RX_BUF, temp);
		}

		DMA_SetCurrDataCounter(DMA1_Channel5, USART_RX_BUF_SIZE);  
		DMA_Cmd(DMA1_Channel5, ENABLE);  
	}
} 


void uart_send(uint8_t *buf, uint16_t size)
{
	uint16_t i = 0;
	
	for (; i<size; i++) {
		USART_SendData(USART1, (uint8_t)buf[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}
	}
}


//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//	u8 Res;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������

//		if((USART_RX_STA&0x80)==0)//����δ���
//		{
//			if(USART_RX_STA&0x40)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x80;	//��������� 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x40;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3F]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>63)USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//	} 
//} 

