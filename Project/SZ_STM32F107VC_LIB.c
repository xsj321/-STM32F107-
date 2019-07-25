/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * �ļ���  ��SZ_STM32F107VC_LIB.c
 * ����    ���ṩSTM32F107VC����IV�ſ�����Ŀ⺯��
 * ʵ��ƽ̨��STM32���ۿ�����
 * ����    ��www.armjishu.com 
**********************************************************************************/
  
/* Includes ------------------------------------------------------------------*/
#include "SZ_STM32F107VC_LIB.h"

__IO uint32_t TimingDelay;

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};
const uint16_t GPIO_PIN_NUM[LEDn] = {LED1_PIN_NUM, LED2_PIN_NUM, LED3_PIN_NUM, LED4_PIN_NUM};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY1_BUTTON_GPIO_PORT, KEY2_BUTTON_GPIO_PORT, KEY3_BUTTON_GPIO_PORT, KEY4_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN_NUM[BUTTONn] = {KEY1_BUTTON_PIN_NUM, KEY2_BUTTON_PIN_NUM, KEY3_BUTTON_PIN_NUM, KEY4_BUTTON_PIN_NUM}; 
const uint16_t BUTTON_PIN[BUTTONn] = {KEY1_BUTTON_PIN, KEY2_BUTTON_PIN, KEY3_BUTTON_PIN, KEY4_BUTTON_PIN}; 
const uint32_t BUTTON_CLK[BUTTONn] = {KEY1_BUTTON_GPIO_CLK, KEY2_BUTTON_GPIO_CLK, KEY3_BUTTON_GPIO_CLK, KEY4_BUTTON_GPIO_CLK};
const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {KEY1_BUTTON_EXTI_LINE, KEY2_BUTTON_EXTI_LINE, KEY3_BUTTON_EXTI_LINE, KEY4_BUTTON_EXTI_LINE};
const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PORT_SOURCE, KEY2_BUTTON_EXTI_PORT_SOURCE, KEY3_BUTTON_EXTI_PORT_SOURCE, KEY4_BUTTON_EXTI_PORT_SOURCE};
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PIN_SOURCE, KEY2_BUTTON_EXTI_PIN_SOURCE, KEY3_BUTTON_EXTI_PIN_SOURCE, KEY4_BUTTON_EXTI_PIN_SOURCE}; 
const uint16_t BUTTON_IRQn[BUTTONn] = {KEY1_BUTTON_EXTI_IRQn, KEY2_BUTTON_EXTI_IRQn, KEY3_BUTTON_EXTI_IRQn, KEY4_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {SZ_STM32_COM1, SZ_STM32_COM2}; 
GPIO_TypeDef* COM_TX_PORT[COMn] = {SZ_STM32_COM1_TX_GPIO_PORT, SZ_STM32_COM2_TX_GPIO_PORT};
GPIO_TypeDef* COM_RX_PORT[COMn] = {SZ_STM32_COM1_RX_GPIO_PORT, SZ_STM32_COM2_RX_GPIO_PORT};
const uint32_t COM_USART_CLK[COMn] = {SZ_STM32_COM1_CLK, SZ_STM32_COM2_CLK};
const uint32_t COM_TX_PORT_CLK[COMn] = {SZ_STM32_COM1_TX_GPIO_CLK, SZ_STM32_COM2_TX_GPIO_CLK};
const uint32_t COM_RX_PORT_CLK[COMn] = {SZ_STM32_COM1_RX_GPIO_CLK, SZ_STM32_COM2_RX_GPIO_CLK};
const uint16_t COM_TX_PIN[COMn] = {SZ_STM32_COM1_TX_PIN, SZ_STM32_COM2_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {SZ_STM32_COM1_RX_PIN, SZ_STM32_COM2_RX_PIN};

const uint8_t STM32F10x_STR[] = {"\r\n"\
           "  _____ _______ __  __ ____ ___  ______ __  ___\r\n"\
           " / ____|__   __|  \\/  |___ \\__ \\|  ____/_ |/ _ \\\r\n"\
           "| (___    | |  | \\  / | __) | ) | |__   | | | | |_  __\r\n"\
           " \\___ \\   | |  | |\\/| ||__ < / /|  __|  | | | | \\ \\/ /\r\n"\
           " ____) |  | |  | |  | |___) / /_| |     | | |_| |>  <\r\n"\
           "|_____/   |_|  |_|  |_|____/____|_|     |_|\\___//_/\\_\\����ϵ�п�����\r\n"\
           "\r\n"};

					 
uint32_t STM32DeviceSerialID[3]; /* ȫ�ֱ���IntDeviceSerial��Ŷ������豸ID */

/**-------------------------------------------------------
  * @������ delay
  * @����   �򵥵�delay��ʱ����.
  * @����   �ӳ������� 0--0xFFFFFFFF
  * @����ֵ ��
***------------------------------------------------------*/
void delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}

/**-------------------------------------------------------
  * @������ NVIC_GroupConfig
  * @����   ����NVIC�ж����ȼ����麯��.
  *         Ĭ������Ϊ1���ر�ʾ�����ȼ�, 3���ر�ʾ�����ȼ�
  *         �û����Ը�����Ҫ�޸�
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void NVIC_GroupConfig(void)
{
    /* ����NVIC�ж����ȼ�����:
     - 1���ر�ʾ�����ȼ�  �����ȼ��Ϸ�ȡֵΪ 0 �� 1 
     - 3���ر�ʾ�����ȼ�  �����ȼ��Ϸ�ȡֵΪ 0..7
     - ��ֵԽ�����ȼ�Խ�ߣ�ȡֵ�����Ϸ���Χʱȡ��bitλ 
    */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /*==================================================================================
      NVIC_PriorityGroup   |  �����ȼ���Χ  |  �����ȼ���Χ  |   ����
      ==================================================================================
     NVIC_PriorityGroup_0  |      0         |      0-15      |   0 ���ر�ʾ�����ȼ�
                           |                |                |   4 ���ر�ʾ�����ȼ� 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_1  |      0-1       |      0-7       |   1 ���ر�ʾ�����ȼ�
                           |                |                |   3 ���ر�ʾ�����ȼ� 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_2  |      0-3       |      0-3       |   2 ���ر�ʾ�����ȼ�
                           |                |                |   2 ���ر�ʾ�����ȼ� 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_3  |      0-7       |      0-1       |   3 ���ر�ʾ�����ȼ�
                           |                |                |   1 ���ر�ʾ�����ȼ� 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_4  |      0-15      |      0         |   4 ���ر�ʾ�����ȼ�
                           |                |                |   0 ���ر�ʾ�����ȼ�   
    ==================================================================================*/
}

/**-------------------------------------------------------
  * @������ SZ_STM32_SysTickInit
  * @����   ��ʼ��ϵͳ��ʱ��SysTick
  *         �û����Ը�����Ҫ�޸�
  * @����   ÿ���жϴ���
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_SysTickInit(uint32_t HzPreSecond)
{
    /* HzPreSecond = 1000 to Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.
    
    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
       call. The NVIC_SetPriority() is defined inside the core_cm3.h file.

    4. To adjust the SysTick time base, use the following formula:
                            
         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
    */
    if (SysTick_Config(SystemCoreClock / HzPreSecond))
    { 
        /* Capture error */ 
        while (1);
    }
}

/**-------------------------------------------------------
  * @������ SysTickDelay
  * @����   ϵͳ��ʱ��ʵ�ֵ���ʱ�������������û������е���
  *         �û����Ը�����Ҫ�޸�
  * @����   �ӳ����������ӳ�ʱ��ΪnTime/HzPreSecond
  *         ���Ϊ -1/HzPreSecond
  * @����ֵ ��
***------------------------------------------------------*/
void SysTickDelay(__IO uint32_t nTime)
{ 
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/**-------------------------------------------------------
  * @������ TimingDelay_Decrement
  * @����   ϵͳ���Ķ�ʱ�����������õ��Ӻ���
  *         ��ȫ�ֱ���TimingDelay��һ������ʵ����ʱ
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    { 
        TimingDelay--;
    }
}

extern void SysTick_Handler_User(void);
/**-------------------------------------------------------
  * @������ SysTick_Handler
  * @����   ϵͳ���Ķ�ʱ��������������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();

    SysTick_Handler_User();
}

void GetDeviceSerialID(void)
{
    STM32DeviceSerialID[0] = *(__IO uint32_t*)(0x1FFFF7E8);
    STM32DeviceSerialID[1] = *(__IO uint32_t*)(0x1FFFF7EC);
    STM32DeviceSerialID[2] = *(__IO uint32_t*)(0x1FFFF7F0);
}

/**-------------------------------------------------------
  * @������ SZ_STM32_LEDInit
  * @����   ��ʼ��LED��GPIO�ܽţ�����Ϊ�������
  * @����   LED1  ��Ӧ�������ϵ�һ��ָʾ��
  *         LED2  ��Ӧ�������ϵڶ���ָʾ��
  *         LED3  ��Ӧ�������ϵ�����ָʾ��
  *         LED4  ��Ӧ�������ϵ��ĸ�ָʾ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_LEDInit(Led_TypeDef Led)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    /* ʹ��LED��ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

    /* Configure the GPIO_LED pin */
    /* ��ʼ��LED��GPIO�ܽţ�����Ϊ������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**-------------------------------------------------------
  * @������ SZ_STM32_LEDOn
  * @����   ������Ӧ��LEDָʾ��
  * @����   LED1  ��Ӧ�������ϵ�һ��ָʾ��
  *         LED2  ��Ӧ�������ϵڶ���ָʾ��
  *         LED3  ��Ӧ�������ϵ�����ָʾ��
  *         LED4  ��Ӧ�������ϵ��ĸ�ָʾ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_LEDOn(Led_TypeDef Led)
{
    /* ָ���ܽ�����͵�ƽ��������Ӧ��LEDָʾ�� */
    GPIO_PORT[Led]->BRR = GPIO_PIN[Led];  
    //��Ҫ���Ч�ʣ�����ֱ�ӵ��� LEDnOBB = 0;
}

/**-------------------------------------------------------
  * @������ SZ_STM32_LEDOn
  * @����   Ϩ���Ӧ��LEDָʾ��
  * @����   LED1  ��Ӧ�������ϵ�һ��ָʾ��
  *         LED2  ��Ӧ�������ϵڶ���ָʾ��
  *         LED3  ��Ӧ�������ϵ�����ָʾ��
  *         LED4  ��Ӧ�������ϵ��ĸ�ָʾ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_LEDOff(Led_TypeDef Led)
{
    /* ָ���ܽ�����ߵ�ƽ��Ϩ���Ӧ��LEDָʾ�� */
    GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];   
    //��Ҫ���Ч�ʣ�����ֱ�ӵ��� LEDnOBB = 1;
}

/**-------------------------------------------------------
  * @������ SZ_STM32_LEDToggle
  * @����   ����Ӧ��LEDָʾ��״̬ȡ��
  * @����   LED1  ��Ӧ�������ϵ�һ��ָʾ��
  *         LED2  ��Ӧ�������ϵڶ���ָʾ��
  *         LED3  ��Ӧ�������ϵ�����ָʾ��
  *         LED4  ��Ӧ�������ϵ��ĸ�ָʾ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_LEDToggle(Led_TypeDef Led)
{
    /* ָ���ܽ������� 1��ʵ�ֶ�Ӧ��LEDָʾ��״̬ȡ��Ŀ�� */
    GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
	//��Ҫ���Ч�ʣ�����ֱ�ӵ��� LEDnOBB = !LEDnOBB;
}


/**-------------------------------------------------------
  * @������ SZ_STM32_BEEPInit
  * @����   ��ʼ����������GPIO�ܽţ�����Ϊ�������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_BEEPInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    /* ʹ�ܷ�������ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);

    /* Configure the GPIO_LED pin */
    /* ��ʼ����������GPIO�ܽţ�����Ϊ������� */
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);

    BEEPOBB = 1;//��ʼ��ʱ�رշ�����
}

/**-------------------------------------------------------
  * @������ SZ_STM32_BEEPOn
  * @����   ʹ��������ʼ����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_BEEPOn(void)
{
    /* ָ���ܽ�����͵�ƽ��ʹ��������ʼ���� */
    //BEEP_GPIO_PORT->BRR = BEEP_PIN;  
    BEEPOBB = 0;
}

/**-------------------------------------------------------
  * @������ SZ_STM32_BEEPOff
  * @����   ʹ������ֹͣ����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_BEEPOff(void)
{
    /* ָ���ܽ�����ߵ�ƽ��ʹ������ֹͣ���� */
    //BEEP_GPIO_PORT->BSRR = BEEP_PIN;   
    BEEPOBB = 1;
}

/**-------------------------------------------------------
  * @������ SZ_STM32_BEEPToggle
  * @����   ʹ������״̬ȡ��
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_BEEPToggle(void)
{
    /* ָ���ܽ������� 1��ʵ�ֶԷ�����״̬ȡ��Ŀ�� */
    //BEEP_GPIO_PORT->ODR ^= BEEP_PIN;
    BEEPOBB = !BEEPOBB;
}


/**-------------------------------------------------------
  * @������ SZ_STM32_KEYInit
  * @����   ��ʼ��KEY������GPIO�ܽţ�����Ϊ������������
  *         ��������ʱΪ�͵�ƽ0
  * @����1  KEY1  ��Ӧ�������ϵ�һ��KEY����
  *         KEY2  ��Ӧ�������ϵڶ���KEY����
  *         KEY3  ��Ӧ�������ϵ�����KEY����
  *         KEY4  ��Ӧ�������ϵ��ĸ�KEY����
  * @����2  BUTTON_MODE_GPIO  ����Ϊ��ͨ����ܽ�ʹ��
  *         BUTTON_MODE_EXTI  ����Ϊ�ⲿEXTI�ж�ʹ��
  *                           ��Ҫ�����жϴ�����
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_KEYInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the BUTTON Clock */
    /* ʹ��KEY������ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(BUTTON_CLK[Button] | RCC_APB2Periph_AFIO, ENABLE);

    /* Configure Button pin as input floating */
    /* ��ʼ��KEY������GPIO�ܽţ�����Ϊ������������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
    GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

    /* ��ʼ��KEY����Ϊ�ж�ģʽ */
    if (Button_Mode == BUTTON_MODE_EXTI)
    {
        /* Connect Button EXTI Line to Button GPIO Pin */
        /* ��KEY������Ӧ�Ĺܽ����ӵ��ڲ��ж��� */    
        GPIO_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

        /* Configure Button EXTI line */
        /* ��KEY��������Ϊ�ж�ģʽ���½��ش����ж� */    
        EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable and set Button EXTI Interrupt to the lowest priority */
        /* ��KEY�������ж����ȼ�����Ϊ��� */  
        NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure); 
    }
}

/**-------------------------------------------------------
  * @������ SZ_STM32_KEYGetState
  * @����   ��ȡKEY�����������ƽ״̬����������ʱΪ�͵�ƽ0
  * @����   KEY1  ��Ӧ�������ϵ�һ��KEY����
  *         KEY2  ��Ӧ�������ϵڶ���KEY����
  *         KEY3  ��Ӧ�������ϵ�����KEY����
  *         KEY4  ��Ӧ�������ϵ��ĸ�KEY����
  * @����ֵ ��
***------------------------------------------------------*/
uint32_t SZ_STM32_KEYGetState(Button_TypeDef Button)
{
    return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
	//��Ҫ���Ч�ʣ�����ֱ�ӵ��� KeyState = KEYnIBB;
}

/**-------------------------------------------------------
  * @������ SZ_STM32_KEYScan
  * @����   ��ȡKEY�����������ƽ״̬����������ʱΪ�͵�ƽ0
  * @����   ��
  * @����ֵ 1  ��Ӧ�������ϵ�һ��KEY�������ڰ���״̬
  *         2  ��Ӧ�������ϵڶ���KEY�������ڰ���״̬
  *         3  ��Ӧ�������ϵ�����KEY�������ڰ���״̬
  *         4  ��Ӧ�������ϵ��ĸ�KEY�������ڰ���״̬
  *         0  ��������û��KEY�������ڰ���״̬
***------------------------------------------------------*/
uint32_t SZ_STM32_KEYScan(void)
{
    /* ��ȡKEY�����������ƽ״̬����������ʱΪ�͵�ƽ0 */
    if(0 == KEY1IBB)
    {
         /* �ӳ�ȥ�� */
        delay(150000);
        if(0 == KEY1IBB)
        {
            return 1;
        }
    }

    /* ��ȡKEY�����������ƽ״̬����������ʱΪ�͵�ƽ0 */
    if(0 == KEY2IBB)
    {
         /* �ӳ�ȥ�� */
        delay(150000);
        if(0 == KEY2IBB)
        {
            return 2;
        }
    }

    /* ��ȡKEY�����������ƽ״̬����������ʱΪ�͵�ƽ0 */
    if(0 == KEY3IBB)
    {
         /* �ӳ�ȥ�� */
        delay(150000);
        if(0 == KEY3IBB)
        {
            return 3;
        }
    }

    /* ��ȡKEY�����������ƽ״̬����������ʱΪ�͵�ƽ0 */
    if(0 == KEY4IBB)
    {
         /* �ӳ�ȥ�� */
        delay(150000);
        if(0 == KEY4IBB)
        {
            return 4;
        }
    }
    
    return 0;
}


/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   ��STM32��USART��ʼ���ײ㺯��
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART2 ��Ӧ�������ϴ���2
  * @����2  ָ��һ����Ա�Ѹ�ֵUSART_InitTypeDef�ṹ���ָ��
  * @����ֵ ��
***------------------------------------------------------*/
void __SZ_STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    /* ʹ��STM32��USART��ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

    if (COM == COM1)
    {
        /* ʹ��STM32��USART1��Clockʱ�� */
        RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
    }
    else
    {
        /* Enable the USART2 Pins Software Remapping */
        /* ʹ��STM32��USART2�Ĺܽ���ӳ�� */
        GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
        /* ʹ��STM32��USART��Clockʱ�� */
        RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    }

    /* Configure USART Tx as alternate function push-pull */
    /* ��ʼ��STM32��USART��TX�ܽţ�����Ϊ���ù���������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    /* ��ʼ��STM32��USART��RX�ܽţ�����Ϊ���ù������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    /* ���ݴ���Ĳ�����ʼ��STM32��USART���� */
    USART_Init(COM_USART[COM], USART_InitStruct);

    /* Enable USART */
    /* ʹ��STM32��USART����ģ�� */
    USART_Cmd(COM_USART[COM], ENABLE);
}

/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   �����û���STM32��USART��ʼ������
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART2 ��Ӧ�������ϴ���2
  * @����2  BaudRate ���ڵĲ����ʣ�����"115200"
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_COMInit(COM_TypeDef COM, uint32_t BaudRate)
{
  
    USART_InitTypeDef USART_InitStructure;
  
    /* USARTx Ĭ������:
          - BaudRate = 115200 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Hardware flow control disabled (RTS and CTS signals)
          - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = BaudRate;              //���ڵĲ����ʣ�����115200 ��ߴ�4.5Mbits/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�����ֳ���(8λ��9λ)
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //�����õ�ֹͣλ-֧��1��2��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;         //����żУ��  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //˫��ģʽ��ʹ�ܷ��ͺͽ���
  
    __SZ_STM32_COMInit(COM, &USART_InitStructure);  // ����STM32��USART��ʼ���ײ㺯��
  
    printf("\r\n\n\n\r WWW.ARMJISHU.COM  %s printf configured....", 
  	                            (COM == COM1)? SZ_STM32_COM1_STR:SZ_STM32_COM2_STR);
    printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");
  
    printf("%s", STM32F10x_STR);
  
    printf(" WWW.ARMJISHU.COM use __STM32F10X_STDPERIPH_VERSION %d.%d.%d",
  			__STM32F10X_STDPERIPH_VERSION_MAIN,
  			__STM32F10X_STDPERIPH_VERSION_SUB1,
  			__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("\n\r ��Ʒ�ڲ�Flash��СΪ��%dK�ֽڣ� \t www.armjishu.com",
              *(__IO uint16_t*)(0x1FFFF7E0));
    SystemCoreClockUpdate();
    printf("\n\r ϵͳ�ں�ʱ��Ƶ��(SystemCoreClock)Ϊ��%dHz.\n\r",
            SystemCoreClock);
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
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(SZ_STM32_COM2, USART_FLAG_TC) == RESET)
    {}

    /* e.g. write a character to the USART */
    USART_SendData(SZ_STM32_COM2, (uint8_t) ch);

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
	
    while(USART_GetFlagStatus(SZ_STM32_COM2, USART_FLAG_RXNE) == RESET)
    {
    }

    ch = (int)SZ_STM32_COM2->DR & 0xFF;
	
    putchar(ch); //����
	
	return ch;
}

#ifdef USE_ADC
/**-------------------------------------------------------
  * @������ ADC_GPIO_Configuration
  * @����   ADCģ��ת����GPIO���ó�ʼ������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void ADC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOC clock */
    /* ʹ��GPIOCʱ��      */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  //

    /* Configure PA.01 (ADC Channel12) as analog input -------------------------*/
    //PC0 ��Ϊģ��ͨ��11��������                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       //�ܽ�0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //����ģʽ
    GPIO_Init(GPIOC, &GPIO_InitStructure);          //GPIO��
}

/* STM32оƬADCת�����DR�Ĵ�������ַ */
#define DR_ADDRESS                  ((uint32_t)0x4001244C) 
/* ���ADCΪ12λģ��ת��������ı�����ֻ��ADCConvertedValue�ĵ�12λ��Ч */
__IO uint16_t ADCConvertedValue;     // 

/**-------------------------------------------------------
  * @������ SZ_STM32_ADC_Configuration
  * @����   ADCģ��ת���Ĳ������ú���
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;        //ADC��ʼ���ṹ������
    DMA_InitTypeDef DMA_InitStructure;        //DMA��ʼ���ṹ������     

    ADC_GPIO_Configuration();

    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //ʹ��DMAʱ��

    /* DMA1 channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);		  //����DMA1�ĵ�һͨ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = DR_ADDRESS;		  //DMA��Ӧ���������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;   //�ڴ�洢����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
    DMA_InitStructure.DMA_BufferSize = 1;		   //DMA�����С��1��
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����һ�����ݺ��豸��ַ��ֹ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;	//�رս���һ�����ݺ�Ŀ���ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�����������ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA�������ݳߴ磬HalfWord����Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //ת��ģʽ��ѭ������ģʽ��
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA���ȼ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2Mģʽ����
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //ʹ��ADCʱ��

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//������ת��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //����ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   //��������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC�ⲿ���أ��ر�״̬
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //���뷽ʽ,ADCΪ12λ�У��Ҷ��뷽ʽ
    ADC_InitStructure.ADC_NbrOfChannel = 1;	 //����ͨ������1��
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel10 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
                          //ADCͨ���飬 ��1��ͨ�� ����˳��1��ת��ʱ�� 
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	  //ADC���ʹ��
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);  //����ADC1

    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);	  //����У׼
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ�����У׼���
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//��ʼУ׼
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));	   //�ȴ�У׼���
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//����ת����ʼ��ADCͨ��DMA��ʽ���ϵĸ���RAM����
}

/**-------------------------------------------------------
  * @������ GetADCConvertedValue
  * @����   ��ȡADCģ��ת���Ľ��
  * @����   ��
  * @����ֵ ADCת������ı�����ֻ�еĵ�12λ��Ч
***------------------------------------------------------*/
uint16_t GetADCConvertedValue(void)
{
    return ADCConvertedValue;
}
#endif

#ifdef USE_RTC
/**-------------------------------------------------------
  * @������ RTC_Configuration
  * @����   RTCʵʱʱ�ӵĲ������ú���
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void RTC_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    /* PWRʱ�ӣ���Դ���ƣ���BKPʱ�ӣ�RTC�󱸼Ĵ�����ʹ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    /*ʹ��RTC�ͺ󱸼Ĵ������� */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    /* ������BKP��ȫ���Ĵ�������Ϊȱʡֵ */
    BKP_DeInit();

    /* Enable LSE */
    /* ʹ��LSE���ⲿ32.768KHz���پ���*/
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    /* �ȴ��ⲿ�������ȶ���� */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    /*ʹ���ⲿ32.768KHz������ΪRTCʱ�� */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    /* ʹ�� RTC ��ʱ�ӹ��� */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    /*�ȴ�RTC�Ĵ���ͬ�� */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    /* ʹ��RTC�����ж� */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    /* 32.768KHz����Ԥ��Ƶֵ��32767,����Ծ���Ҫ��ܸ߿����޸Ĵ˷�Ƶֵ��У׼���� */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
    RTC_WaitForLastTask();
}

/**-------------------------------------------------------
  * @������ RTC_NVIC_Configuration
  * @����   RTCʵʱʱ�ӵ��жϲ������ú���
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void RTC_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

__IO uint32_t TimeDisplay = 1;
/**-------------------------------------------------------
  * @������ RTC_IRQHandler
  * @����   RTCʵʱʱ�ӵ��жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void RTC_IRQHandler(void)
{
    /*  �ж�RTC�Ƿ��������жϣ�Ҳ�п�����������������ж�) */
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        /* ������жϱ�־ */
        RTC_ClearITPendingBit(RTC_IT_SEC);

        /* Toggle LED1 �ı�LED1������״̬(ȡ��) */
        SZ_STM32_LEDToggle(LED1);

        /* Enable time update */
        /* ��ȫ�ֱ���Ϊ1��֪ͨ�������ӡʱ�� */
        TimeDisplay = 1;

        /* Wait until last write operation on RTC registers has finished */
        /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
        RTC_WaitForLastTask();
   
    }
}
#endif

/**-------------------------------------------------------
  * @������ Buffercmp
  * @����   �Ƚ������������������ȵ������Ƿ���ͬ�ĺ���
  * @����1  pBuffer1 �� pBuffer2 ��Ҫ�Ƚ�������������ʼ��ַ
  * @����2  BufferLength �����ıȽϳ��ȣ��ֽ�Ϊ��λ
  * @����ֵ PASSED: �������������ȵ�������ͬ
  *         FAILED: �������������ȵ����ݲ���ͬ
***------------------------------------------------------*/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

#ifdef USE_TIM5
/**-------------------------------------------------------
  * @������ NVIC_TIM5Configuration
  * @����   ����TIM5�ж�������������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
static void NVIC_TIM5Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Set the Vector Table base address at 0x08000000 */
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}
    
/**-------------------------------------------------------
  * @������ NVIC_TIM5Configuration
  * @����   ����TIM5����������ÿ��������ж�һ�� 
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TIM5_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* ---------------------------------------------------------------
    TIM4 Configuration: Output Compare Timing Mode:
    TIM2CLK = 36 MHz, Prescaler = 7200, TIM2 counter clock = 7.2 MHz
    --------------------------------------------------------------- */

    /* Time base configuration */
    //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999
    TIM_TimeBaseStructure.TIM_Period = (10000 - 1);
    // �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
    TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);
    // �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
    // ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //��ʼ����ʱ��5
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    /* Clear TIM5 update pending flag[���TIM5����жϱ�־] */
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

    /* TIM IT enable */ //������ж�
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM5, ENABLE);  //������ʹ�ܣ���ʼ����

    /* �жϲ������� */
    NVIC_TIM5Configuration();
}


/**-------------------------------------------------------
  * @������ TIM5_IRQHandler
  * @����   TIM5�жϴ�������ÿ���ж�һ�� 
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TIM5_IRQHandler(void)
{
    /* www.armjishu.com ARM������̳ */
    static u32 counter = 0;

    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        /* LED1ָʾ��״̬ȡ�� */
        SZ_STM32_LEDToggle(LED1);

        /* armjishu.com������: ���������ж���ʹ��Printf, ��ʾ��ֻ����ʾ�� */
        printf("\n\rarmjishu.com��ʾ��: ���������ж���ʹ��Printf, ��ʾ��ֻ����ʾ��\n\r");
        printf("ARMJISHU.COM-->TIM5:%d\n\r", counter++);
    }
}
#endif

#ifdef USE_TIM3
/**-------------------------------------------------------
  * @������ NVIC_TIM3Configuration
  * @����   ����TIM3�ж�������������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
static void NVIC_TIM3Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Set the Vector Table base address at 0x08000000 */
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/**-------------------------------------------------------
  * @������ TIM3_PWM_Init
  * @����   ����TIM3ΪPWMģʽ
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TIM3_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
  	const uint16_t Period_Val = 1020;
    __IO uint16_t CCR1_Val = Period_Val/2;
    __IO uint16_t CCR2_Val = Period_Val-20;
    uint16_t PrescalerValue = 0;
         
    /* System Clocks Configuration */ /* PCLK1 = HCLK/2 = 36MHz */
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
    /* -----------------------------------------------------------------------
      TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
      The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
      clock at 24 MHz the Prescaler is computed as following:
       - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
      SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
      and Connectivity line devices and to 24 MHz for Low-Density Value line and
      Medium-Density Value line devices
  
      The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                                    = 24 MHz / 1000 = 24 KHz
      TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
      TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    ----------------------------------------------------------------------- */
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = Period_Val;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* TIM IT enable */
    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

    //TIM_ARRPreloadConfig(TIM3, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);

    NVIC_TIM3Configuration();
}



/**-------------------------------------------------------
  * @������ TIM3_IRQHandler
  * @����   TIM3�жϴ�������PWM����ָʾ������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

        SZ_STM32_LEDOn(LED1);
        SZ_STM32_LEDOn(LED3);
        SZ_STM32_LEDOff(LED2);
        SZ_STM32_LEDOff(LED4);
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

        SZ_STM32_LEDOff(LED1);
        SZ_STM32_LEDOff(LED3);
        SZ_STM32_LEDOn(LED2);
        SZ_STM32_LEDOn(LED4);
    }
}
#endif

#ifdef USE_IWDG
/**-------------------------------------------------------
  * @������ SZ_STM32_IWDGInit
  * @����   STM32�������Ź���ʼ�����ú���
  * @����   TimeoutFreMs  ���Ź���ʱ�����ٺ������һ��
  * @����ֵ ��
  * @ι����ʽ(Reload IWDG counter) IWDG_ReloadCounter(); 
***------------------------------------------------------*/
void SZ_STM32_IWDGInit(uint32_t TimeoutFreMs)/* 2 -  3276*/
{
    uint32_t ReloadValue;

    /* IWDG timeout equal to 280 ms (the timeout may varies due to LSI frequency
    dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    /* Set counter reload value to 349 *//* range from 0 to 4095 */
    //IWDG_SetReload(349);
    if(TimeoutFreMs < 2)
    {
        TimeoutFreMs = 2;
    }

    if(TimeoutFreMs > 3276)
    {
        TimeoutFreMs = 3276;
    }

    ReloadValue = ((TimeoutFreMs * 1250)/1000) - 1;
    IWDG_SetReload(ReloadValue);

    printf("\r\n IWDG Timeout frequency %dms, ReloadValue is set to %d.", TimeoutFreMs, ReloadValue);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();

}
#endif

#ifdef USE_PVD
/**-------------------------------------------------------
  * @������ SZ_STM32_PVDConfiguration
  * @����   �ɱ�̵�ѹ�����(programmable voltage detector)��ʼ��
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_PVDConfiguration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Configure EXTI Line16(PVD Output) to generate an 
     interrupt on rising and falling edges */
    EXTI_ClearITPendingBit(EXTI_Line16);
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable the PVD Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the PVD Output */
    PWR_PVDCmd(ENABLE);
}

/**-------------------------------------------------------
  * @������ PVD_IRQHandler
  * @����   �ɱ�̵�ѹ�����PVD�жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void PVD_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line16) != RESET)
    {
        /* LED1״̬ȡ�� */
        LED1OBB = !LED1OBB;
        /* ����ָʾ��Ϩ�� */
        LED2OBB = 1;        	
        LED3OBB = 1;
        LED4OBB = 1;
        
        printf("PVD 2.9V");
        	
        /* Clear the Key Button EXTI line pending bit */
        EXTI_ClearITPendingBit(EXTI_Line16);
    }
}
  
#endif

#ifdef USE_RTC_ALARM
/**-------------------------------------------------------
  * @������ RTCAlarm_Configuration
  * @����   RTC���ӳ�ʼ��
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void RTCAlarm_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    /* PWRʱ�ӣ���Դ���ƣ���BKPʱ�ӣ�RTC�󱸼Ĵ�����ʹ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    /*ʹ��RTC�ͺ󱸼Ĵ������� */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    /* ������BKP��ȫ���Ĵ�������Ϊȱʡֵ */
    //BKP_DeInit();

    /* Enable LSE */
    /* ʹ��LSE���ⲿ32.768KHz���پ���*/
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    /* �ȴ��ⲿ�������ȶ���� */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    /*ʹ���ⲿ32.768KHz������ΪRTCʱ�� */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    /* ʹ�� RTC ��ʱ�ӹ��� */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    /*�ȴ�RTC�Ĵ���ͬ�� */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
    RTC_WaitForLastTask();

    /* Enable the RTC Alarm interrupt */
    /* ʹ��RTC�������ж� */
    RTC_ITConfig(RTC_IT_ALR, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    /* 32.768KHz����Ԥ��Ƶֵ��32767,����Ծ���Ҫ��ܸ߿����޸Ĵ˷�Ƶֵ��У׼���� */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
    RTC_WaitForLastTask();
}

/**-------------------------------------------------------
  * @������ RTCAlarm_NVIC_Configuration
  * @����   ����RTC�����ж�������������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void RTCAlarm_NVIC_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure EXTI Line17(RTC Alarm) to generate 
                      an interrupt on rising edge */
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 2 bits for Preemption Priority and 2 bits for Sub Priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**-------------------------------------------------------
  * @������ RTC_STANDBY_Configuration
  * @����   RTC��STANDBYģʽ��ʼ��
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void RTC_STANDBY_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    /* PWRʱ�ӣ���Դ���ƣ���BKPʱ�ӣ�RTC�󱸼Ĵ�����ʹ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    /*ʹ��RTC�ͺ󱸼Ĵ������� */
    PWR_BackupAccessCmd(ENABLE);

    /* Enable WKUP pin */
    PWR_WakeUpPinCmd(ENABLE);


    /* Check if the StandBy flag is set */
    if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
    {/* System resumed from STANDBY mode */

        /* Turn on LED2 */
        SZ_STM32_LEDOn(LED2);

        printf("\r\n resumed from StandBy mode..��STANDBY����ģʽ�˳�");

        /* Clear StandBy flag */
        PWR_ClearFlag(PWR_FLAG_SB);

        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();
        /* No need to configure the RTC as the RTC configuration(clock source, enable,
        prescaler,...) is kept after wake-up from STANDBY */
    }
    else
    {/* StandBy flag is not set */
        printf("\r\n resumed from Reset..������ģʽ��λ");
        /* RTC clock source configuration ----------------------------------------*/

        /* Reset Backup Domain */
        /* ������BKP��ȫ���Ĵ�������Ϊȱʡֵ */
        //BKP_DeInit();

        /* Enable LSE */
        /* ʹ��LSE���ⲿ32.768KHz���پ���*/
        RCC_LSEConfig(RCC_LSE_ON);

        /* Wait till LSE is ready */
        /* �ȴ��ⲿ�������ȶ���� */
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
        }

        /* Select LSE as RTC Clock Source */
        /*ʹ���ⲿ32.768KHz������ΪRTCʱ�� */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        /* Enable RTC Clock */
        /* ʹ�� RTC ��ʱ�ӹ��� */
        RCC_RTCCLKCmd(ENABLE);

        /* RTC configuration -----------------------------------------------------*/
        /* Wait for RTC APB registers synchronisation */
        /*�ȴ�RTC�Ĵ���ͬ�� */
        RTC_WaitForSynchro();


        /* Set RTC prescaler: set RTC period to 1sec */
        /* 32.768KHz����Ԥ��Ƶֵ��32767,����Ծ���Ҫ��ܸ߿����޸Ĵ˷�Ƶֵ��У׼���� */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

        /* Wait until last write operation on RTC registers has finished */
        /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
        RTC_WaitForLastTask();
    }
}
#endif

#ifdef  USE_FULL_ASSERT
// ��Ҫ�ڹ�������Option(��ݼ�ALT+F7)C++����ҳ��define������"USE_FULL_ASSERT"
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/
