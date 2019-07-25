/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * �ļ���  ��SZ_STM32F107VC_LIB.c
 * ����    ���ṩSTM32F107VC����IV�ſ�����Ŀ⺯��
 * ʵ��ƽ̨��STM32���ۿ�����
 * ����    ��www.armjishu.com 
**********************************************************************************/
#include "usart.h"
#include "systemUI.h"
#include "string.h"
u8 SendBuff[5200];

/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   �����û���STM32��USART��ʼ������
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART2 ��Ӧ�������ϴ���2
  * @����2  BaudRate ���ڵĲ����ʣ�����"115200"
  * @����ֵ ��
***------------------------------------------------------*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void SZ_STM32_COMInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure; 
  
	/*ʹ��GPIOD�˿�ʱ�ӡ����ù���ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE); 

	/*ʹ��USART2ʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   
 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);  

	/*����2��ӳ��*/
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);   

    USART_InitStructure.USART_BaudRate = 115200;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    USART_Init(USART2, &USART_InitStructure);  
		NVIC_Configuration();
		
		// ʹ�ܴ��ڽ����ж�
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
		// ʹ�ܴ���
    USART_Cmd(USART2, ENABLE);  
}



/**-------------------------------------------------------
  * @������  MYDMA_Config
  * @����   ��ʼ�����ô���2��DMA���䷽ʽ
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void MYDMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/*ʹ��DMAʱ��*/
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
	/*����DMA����������ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  
	
	/*����DMA�����ڴ��ַ(Ҫ����ı�����ָ��)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff; 
	
	/*DMA���䷽�򣺴��ڴ浽����*/	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
	
	/*DMA�����������*/
	DMA_InitStructure.DMA_BufferSize = 5200;  

	/*�����ַ����*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/*�ڴ��ַ����*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 

	/*�������ݵ�λ 8λ*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	
	/*�ڴ����ݵ�λ 8λ*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	
	/*DMAģʽ��һ�δ���*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	
	/*���ȼ�����*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	
	/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��

	/*����DMA1��17ͨ��*/	
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	
	DMA_Cmd(DMA1_Channel7, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 	  	
} 

void JSON_Sender(int NowHum,int NowTemp,int TargetTemp)

{
	printf("{\"temperature\":\"%d\",\"humidity\":\"%d\",\"TargetTemp\":\"%d\"}",NowTemp,NowHum,TargetTemp);
}



/*
    �������´���,֧��printf����,����Ҫѡ��use MicroLIB	  
*/
#ifndef MicroLIB
//#pragma import(__use_no_semihosting)             //û��ʵ��fgetcʱ��Ҫ�����ò���   
/* ��׼����Ҫ��֧�ֺ��� ʹ��printf()���Դ�ӡ����Ҫʵ�ָú��� */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* �ض���fputc���� ���ʹ��MicroLIBֻ��Ҫ�ض���fputc�������� */  
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */

	 /* e.g. write a character to the USART */
    USART_SendData(USART2, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {}

    return ch;
}
/*
����ֱ��ʹ��putchar
����Ҫ�ٶ��� int putchar(int ch)����Ϊstdio.h�������¶���
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
    /* Your implementation of ferror */  
    return EOF;  
} 
#endif 

FILE __stdin;

int fgetc(FILE *fp)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {
    }

    ch = (int)USART2->DR & 0xFF;
	
    putchar(ch); //����
	
	return ch;
}

/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/
