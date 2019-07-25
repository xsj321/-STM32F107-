/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * �ļ���  ��SZ_STM32F107VC_LIB.h
 * ����    ���ṩSTM32F107VC����IV�ſ�����Ŀ⺯��
 * ʵ��ƽ̨��STM32���ۿ�����
 * ����    ��www.armjishu.com 
**********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

#define  DEBUG_USART_IRQ                USART2_IRQn

#ifdef __cplusplus
 extern "C" {
#endif 


//void __SZ_STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void SZ_STM32_COMInit(void);

void MYDMA_Config(void);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
void JSON_Sender(int NowHum,int NowTemp,int TargetTemp);

#ifdef __cplusplus
}
#endif
/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/

