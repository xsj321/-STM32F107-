/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * �ļ���  ��BitBand.h
 * ����    ���ṩSTM32F107VC����IV�ſ�����Ŀ⺯��
 * ʵ��ƽ̨��STM32���ۿ�����
 * ����    ��www.armjishu.com 
**********************************************************************************/
#include "stm32f10x.h"
#include <stdio.h>

#ifdef __cplusplus
 extern "C" {
#endif

/* �������궨�� bitband macro ------------------------------------------------*/
/* ʹ��bitband����������bit�����ĳ����Ч�ʣ�����GPIO�ܽŵĿ���Ч����Ϊ���� */
/* ��������� ��32MB����������ķ���ӳ��Ϊ��1MB ����bit-band���ķ���(ʵ�ʴ�С����оƬ�й�) */
#define Periph_BASE         0x40000000  // �������ַ Peripheral 
#define Periph_BB_BASE      0x42000000  // �������������ַ Peripheral bitband

/* ע�⣺���볣������ʱ���ڱ���ʱ��������������������ַ�������ܴﵽ����Ч�ʵ�Ŀ��(�Ƽ�)
         ����������������ֻ��������ʱ��STM32�Լ������������ַ��Ч�ʻ����ۿ�(���Ƽ�) */
#define Periph_BB(PeriphAddr, BitNumber)    \
          *(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2))
	 
#define Periph_ResetBit_BB(PeriphAddr, BitNumber)    \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)) = 0)
   
#define Periph_SetBit_BB(PeriphAddr, BitNumber)       \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)) = 1)

#define Periph_GetBit_BB(PeriphAddr, BitNumber)       \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)))

/* ����GPIO����������������壬nΪbitλ�÷�ΧΪ0��15    */
/* ���Ƕ�GPIOA.15����������Ҫ��ʼ��GPIO��Ȼ��ʹ�÷����� */
/* ��GPIOA.15����͵�ƽ��   PAOutBit(15) = 0;           */
/* ��GPIOA.15����͵�ƽ��   PAOutBit(15) = 1;           */
/* ��ȡGPIOA.15����ĵ�ƽ�� data = PAInBit(15);         */
#define PAOutBit(n)     Periph_BB((uint32_t)&GPIOA->IDR,n)  //��� 
#define PASetBit(n)     (PAOutBit(n) = 1)                   //��� ��
#define PAResetBit(n)   (PAOutBit(n) = 0)                   //��� ��
#define PAInBit(n)      Periph_BB((uint32_t)&GPIOA->IDR,n)  //���� 

#define PBOutBit(n)     Periph_BB((uint32_t)&GPIOB->ODR,n)  //��� 
#define PBSetBit(n)     (PBOutBit(n) = 1)                   //��� ��
#define PBResetBit(n)   (PBOutBit(n) = 0)                   //��� ��
#define PBInBit(n)      Periph_BB((uint32_t)&GPIOB->IDR,n)  //���� 

#define PCOutBit(n)     Periph_BB((uint32_t)&GPIOC->ODR,n)  //��� 
#define PCSetBit(n)     (PCOutBit(n) = 1)                   //��� ��
#define PCResetBit(n)   (PCOutBit(n) = 0)                   //��� ��
#define PCInBit(n)      Periph_BB((uint32_t)&GPIOC->IDR,n)  //���� 

#define PDOutBit(n)     Periph_BB((uint32_t)&GPIOD->ODR,n)  //��� 
#define PDSetBit(n)     (PDOutBit(n) = 1)                   //��� ��
#define PDResetBit(n)   (PDOutBit(n) = 0)                   //��� ��
#define PDInBit(n)      Periph_BB((uint32_t)&GPIOD->IDR,n)  //���� 

#define PEOutBit(n)     Periph_BB((uint32_t)&GPIOE->ODR,n)  //��� 
#define PESetBit(n)     (PEOutBit(n) = 1)                   //��� ��
#define PEResetBit(n)   (PEOutBit(n) = 0)                   //��� ��
#define PEInBit(n)      Periph_BB((uint32_t)&GPIOE->IDR,n)  //����

#define PFOutBit(n)     Periph_BB((uint32_t)&GPIOF->ODR,n)  //��� 
#define PFSetBit(n)     (PFOutBit(n) = 1)                   //��� ��
#define PFResetBit(n)   (PFOutBit(n) = 0)                   //��� ��
#define PFInBit(n)      Periph_BB((uint32_t)&GPIOF->IDR,n)  //����

#define PGOutBit(n)     Periph_BB((uint32_t)&GPIOG->ODR,n)  //��� 
#define PGSetBit(n)     (PGOutBit(n) = 1)                   //��� ��
#define PGResetBit(n)   (PGOutBit(n) = 0)                   //��� ��
#define PGInBit(n)      Periph_BB((uint32_t)&GPIOG->IDR,n)  //����


/* �ڲ�SRAM������ ��32MB SRAM�������ķ���ӳ��Ϊ��1MB SRAMbit-band���ķ���(ʵ�ʴ�С����оƬ�й�) */
#define RAM_BASE            0x20000000  // �ڲ�SRAM����ַ  
#define RAM_BB_BASE         0x22000000  // �ڲ�SRAM����������ַ

#define SRAM_ResetBit_BB(VarAddr, BitNumber)    \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 0)
   
#define SRAM_SetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 1)

#define SRAM_GetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)))



#ifdef __cplusplus
}
#endif
/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/

