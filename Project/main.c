#include "lcd.h"
#include "systemUI.h"
#include "user_config.h"
#include "hw_config.h"
#include "touch.h"
#include "usart.h"
#include "dht11.h"
#include "Systick.h"
#include "bee.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x_it.h"

#define MAX_Temp 30//�趨����¶�
#define MIN_Temp 15//�趨����¶�

#define MAX_EN_Temp 35//�趨������߱����¶�

int i = 15;//Ĭ�Ͽ����趨�¶�ͬʱҲ�Ǳ����趨�¶ȵı���
int last_i;//��һ�ε��趨�¶�

/*

������POR
���ܣ���ǶԿ����¶ȵļӼ�
ֵ	|		1		|		2		|		3		|
��������|��������������|��������������|��������������|
����|	����	|	����	|	δ����|

*/
int POR = 3;

u8 USART2_RX_BUF[250]; 
u8 USART2_RX_CNT=0;
u16 USART2_RX_STA=0;

/***

	��������void USART2_Receive_Data(u8 *buf)
	������u8 *buf
	���ܣ���USART��ȡ����
	
***/
void USART2_Receive_Data(u8 *buf)
{
    u8 rxlen=USART2_RX_CNT;
    u8 i=0;
    Delay_mss(10);        //�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
    while(rxlen!=USART2_RX_CNT)
    {
        rxlen=USART2_RX_CNT;
        Delay_mss(10);
    }
        for(i=0;i<(USART2_RX_CNT);i++)
        {
            buf[i] = USART2_RX_BUF[i];    
            USART2_RX_BUF[i] = 0;
        }        
        USART2_RX_CNT=0;        //����
    
}

int main(void)
{
		int NowTemp = 0;//��ǰ�¶�
		int NowHum = 0;//��ǰʪ��
		u16 x,y,x1,y1;//�����������
		int count = 0;
    /* TFT-LCD��ʼ�� */	
		SZ_STM32_COMInit();//���ڳ�ʼ��
		SZ_STM32_SysTickInit(1000000);//SysTickInit��ʼ��
    SZ_STM32_LCDInit();//LCD��ʾ��ʼ��
		Touch_Configuration();//��������ʼ��
		BEE_init();//��������ʼ��
	  LCD_Clear(LCD_COLOR_WHITE);//�趨������ɫ
		UI_Refresh(i,i,i);//�������ݽ��棬����ֵˢ��
		last_i = i;//����ǰ����ֵ��������
	
		//ѭ�������˶��¶ȿ��ƵĲ���
		while(1)	   
		{
			count++;
			switch(POR)
			{
				case 1:
					if(i<MAX_Temp)
					{
						i++;
						/*
						�˴���Ӷ����µ�Ӳ�����Ʋ���
						*/
						POR = 3;
					}
				break;
				case 0:
					if(i>MIN_Temp)
					{
						i--;
						/*
						�˴���ӶԽ��µ�Ӳ�����Ʋ���
						*/
						POR = 3;
					}
				break;
				case 3:
				break;
			}
			
			//��ʱ�Ի���ĵ�ǰ�¶��뵱ǰʪ�Ƚ���ˢ��
			if(count==1000)
			{
				NowHum =dht11_hum_value(NowHum);//��ȡ��ǰ��ʪ��
				NowTemp =dht11_temp_value(NowTemp);//��ȡ��ǰ���¶�
				JSON_Sender(NowHum,NowTemp,i);//�����ݸ�ʽ��ΪJSON���Ӵ��ڷ���
				count=0;//���������
			}
			
			//�ж��¶��Ƿ񳬹������Ĵ���ֵ
			if(NowTemp>MAX_EN_Temp)
			{
					BEE_start();//��ʼ����
			}
			else
			{
				 BEE_stop();//ֹͣ����
			}
			
			if(!(GPIOC->IDR&0x20))//�жϴ����Ƿ���
			{
				Touch_GetXY(&x,&y,0);
				delay(TOUCH_DELAY);
				Convert_Pos(x,y,&x1,&y1);
				
				//�жϴ�������
				if(y<TOUCH_MINUS_BUTTON_LIFT&&y>TOUCH_MINUS_BUTTON_RIGHT&&i>15)
				{
					i--;
				}
				if(y>TOUCH_PLUS_BUTTON_RIGHT&&y<TOUCH_PLUS_BUTTON_LIFT&&i<30)
				{
					i++;
				}
				
			}
			
			UI_Value_Refresh(NowTemp,NowHum,i);//ˢ����ֵ
			
		}
		
		
    while (1); 
}
	
/***

	��������USART2_IRQHandler(void)
	��������
	���ܣ�����2�ն˷����������ܴ������������
	
***/

 void USART2_IRQHandler(void)
{
    u8 res;        
		int c;
     if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
    {          
        res =USART_ReceiveData(USART2);     //��ȡ���յ�������        
        if(USART2_RX_STA==0)
        {
            USART2_RX_BUF[USART2_RX_CNT] = res;        //��¼���յ���ֵ    
            //�����ݽ�β�յ�0xA0��0xA1�������ݽ�����ɣ���һ������������
            if(USART2_RX_BUF[USART2_RX_CNT-1]==0xA0&&USART2_RX_BUF[USART2_RX_CNT]==0xA1)
						{
							 USART2_RX_STA=1;//��ʾ�������ݽ���
						}  
            USART2_RX_CNT++;						//������������1 
        }
				POR = Str_to_int(USART2_RX_BUF); 
        }  
    //���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)
    {
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2,USART_FLAG_ORE);
    }
     USART_ClearFlag(USART2,USART_IT_RXNE); //һ��Ҫ��������ж�  
		 USART2_RX_CNT = 0;
}

