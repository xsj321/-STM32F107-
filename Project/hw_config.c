#include "hw_config.h"
#include "led.h"
/**
	*�������ƣ�void LED_Configuration(void)
	*
	*��ڲ�������
	*
	*���ڲ�������
	*
	*����˵����LED��ʼ������
**/
void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	/*---------��ʼ��״̬�ĸ�LEDȫOFF------------*/
	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
	LED4_OFF();		
}

/**
	*�������ƣ�void USART_Configuration(void)
	*
	*��ڲ�������
	*
	*���ڲ�������
	*
	*����˵����USART��ʼ������ ����GPIO��ʼ�� TX��������Ϊ�������
**/
void USART_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    /*USART1�˿�����
	  PA9 TX ����������� PA10 RX ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin   = USART_TX ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_PORT,&GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin   = USART_RX ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_PORT, &GPIO_InitStructure);
      	
    /*--------------------USART1����----------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);
	USART_Cmd(USARTx, ENABLE);	
}

