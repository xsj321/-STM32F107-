#include "bee.h"

 void BEE_init(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    /* ��ʼ��LED��GPIO�ܽţ�����Ϊ������� */				  //���ö˿�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   //������4��led�ƹܽ�
														 	  //���öԶ˿ڵ�ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //����Ϊ ����(Push-Pull)���
														 	  //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�������ٶ�Ϊ50MHz

    GPIO_Init(GPIOD, &GPIO_InitStructure);	                  //����GPIO��ʼ������
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
 }
 
 void BEE_start(void)
 {
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
 }
 
 void BEE_stop(void)
 {
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
 }