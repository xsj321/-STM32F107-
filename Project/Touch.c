/* Includes ------------------------------------------------------------------*/

//#include "usart.h"
#include "lcd.h"
#include "touch.h"
#include "stm32f10x.h"

#include <math.h>
#include <stdlib.h>


/*********************************************************************************************************
** Functoin name:       Touch_Configuration
** Descriptions: 		�����õ���IO��SPI����
** input paraments:     ��
** output paraments:    �� 
** Returned values:    	��
*********************************************************************************************************/
void Touch_Configuration()
{

	SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	//����SPI3ʱ��
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE ) ;

	//��ӳ��
	GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);   
    //////������SPI���GPIO��ʼ��//////
    //Configure SPI3 pins: SCK,  MOSI 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
	GPIO_Init(GPIOC,&GPIO_InitStructure);  

	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//Configure PC9 pin: CS pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOC,GPIO_Pin_9);// ʹ�ܴ���оƬ


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

    //////SPIģ������//////
	/* SPI3 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI3, &SPI_InitStructure);

	/* Enable SPI3  */
	SPI_Cmd(SPI3, ENABLE);
	return;

}
/*SPI��дһ�ֽ�*/
unsigned char SPI_SendByte(unsigned char byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, byte);
	
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3);
}
 		 
/*********************************************************************************************************
** Functoin name:       ADS_Read_AD
** Descriptions:        ����оƬ��һ�ֽ�
** input paraments:     CMD������
** output paraments:    ��    
** Returned values:    	������AD
*********************************************************************************************************/	   
uint16_t ADS_Read_AD(uint8_t CMD)	  
{ 	  	  
	uint16_t Num=0;
	  	 
	GPIO_ResetBits(GPIOC,GPIO_Pin_9); 					//ѡ��ADS7843	 
	SPI_SendByte(CMD);		//���������� 
	
	Num = SPI_SendByte(0x00);
	Num = (Num << 8) | SPI_SendByte(0x00); 
	 	     	   	
	Num>>=4;   					//ֻ�и�12λ��Ч.
	GPIO_SetBits(GPIOC,GPIO_Pin_9);						//�ͷ�ADS7843	 
	return(Num);   
}
/*********************************************************************************************************
** Functoin name:       ADS_Read_XY
** Descriptions:        ��X��Y��ֵ�����˲�
** input paraments:     xy:X��Y�Ķ�ȡ����
** output paraments:    ��    
** Returned values:    	�˲����ֵ
*********************************************************************************************************/
#define READ_TIMES 15 						//��ȡ����
#define LOST_VAL 5	 						//����ֵ
uint16_t ADS_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)			//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])				//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
		sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
/*********************************************************************************************************
** Functoin name:       Read_ADSX
** Descriptions:        ��X��ֵ
** input paraments:     ��
** output paraments:    x:��ȡ���ֵ����ĵ�ַ    
** Returned values:    	1
*********************************************************************************************************/
uint8_t Read_ADSX(uint16_t *x)
{
	uint16_t xtemp;			 	 		  
	xtemp=ADS_Read_XY(0xd0);	  												   
	if(xtemp<100)return 0;					//����ʧ��
	*x=xtemp;

	return 1;								//�����ɹ�
}
/*********************************************************************************************************
** Functoin name:       Read_ADSY
** Descriptions:        ��Y��ֵ
** input paraments:     ��
** output paraments:    y:��ȡ���ֵ����ĵ�ַ    
** Returned values:    	1
*********************************************************************************************************/
uint8_t Read_ADSY(uint16_t *y)
{
	uint16_t ytemp;			 	 		  
	ytemp=ADS_Read_XY(0x90);	  												   
	if(ytemp<100)return 0;					//����ʧ��
	*y=ytemp;

	return 1;								//�����ɹ�
}

/*********************************************************************************************************
** Functoin name:       Read_ADSX
** Descriptions:        
				2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ
				��ܳ���50,��������,����Ϊ������ȷ,�����������.
				�ú����ܴ�����׼ȷ��
** input paraments:     ext:�Ƿ�����ʽ�������������ȴ�ֱ�����´����������������ع̶�ֵ��
** output paraments:    x:��ȡ���ֵ����ĵ�ַ    
** Returned values:    	1
*********************************************************************************************************/

#define ERR_RANGE 50 						//��Χ 
uint8_t Touch_GexX(uint16_t *x,uint8_t ext) 
{
	uint16_t x1;
 	uint16_t x2;
 	uint8_t flag; 	
	if(ext){
		/*�ȴ���������*/
		while(T_INT);
	}    
    flag=Read_ADSX(&x1);   
    if(flag==0)return(0);
    flag=Read_ADSX(&x2);	   
    if(flag==0)return(0);  
	if(ext){
		/*�ȴ������ɿ�*/
		while(!(T_INT));
	} 
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE)))//ǰ�����β�����+-50��
    {
        *x=(x1+x2)/2;
//		Printf("x=0x%x\r\n",*x);
        return 1;
    }else return 0;	  
} 
/*********************************************************************************************************
** Functoin name:       Read_ADSY
** Descriptions:        
				2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ
				��ܳ���50,��������,����Ϊ������ȷ,�����������.
				�ú����ܴ�����׼ȷ��
** input paraments:     ext:�Ƿ�����ʽ�������������ȴ�ֱ�����´����������������ع̶�ֵ��
** output paraments:    y:��ȡ���ֵ����ĵ�ַ    
** Returned values:    	1
*********************************************************************************************************/
uint8_t Touch_GexY(uint16_t *y,uint8_t ext) 
{
	uint16_t y1;
 	uint16_t y2;
 	uint8_t flag; 
	if(ext){
		/*�ȴ���������*/
		while(T_INT);
	}   
    flag=Read_ADSY(&y1);   
    if(flag==0)return(0);
    flag=Read_ADSY(&y2);	   
    if(flag==0)return(0);  
	if(ext){
		/*�ȴ������ɿ�*/
		while(!(T_INT));
	} 
    if(((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))//ǰ�����β�����+-50��
    {
        *y=(y1+y2)/2;
//		Printf("x=0x%x\r\n",*x);
        return 1;
    }else return 0;	  
}
/*********************************************************************************************************
** Functoin name:       Touch_GextXY
** Descriptions:        
						�õ�XY��ADֵ
** input paraments:     ext:�Ƿ�����ʽ�������������ȴ�ֱ�����´����������������ع̶�ֵ��
** output paraments:    xy:��ȡ���ֵ����ĵ�ַ    
** Returned values:    	��
*********************************************************************************************************/
void Touch_GetXY(uint16_t *x,uint16_t *y,uint8_t ext)
{
	if(ext){
		/*�ȴ���������*/
		while(T_INT);
	}
	Touch_GexX(x,0);
	Touch_GexY(y,0);
	if(ext){
		/*�ȴ������ɿ�*/
		while(!(T_INT));
	}	
}


/*********************************************************************************************************
** Functoin name:       Drow_Touch_Point
** Descriptions: 		��һ������У׼��
** input paraments:     xy:���������
** output paraments:    �� 
** Returned values:    	��
*********************************************************************************************************/
void Drow_Touch_Point(u16 x,u16 y)
{
	LCD_DrawLine(x-12,y,x+13,y);		//����
	LCD_DrawLine(x,y-12,x,y+13);		//����
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	Draw_Circle(x,y,6);					//������Ȧ
}	  
/*********************************************************************************************************
** Functoin name:       Draw_Big_Point
** Descriptions: 		��һ����� ռ4��λ��
** input paraments:     xy:���������
** output paraments:    �� 
** Returned values:    	��
*********************************************************************************************************/			   
void Draw_Big_Point(u16 x,u16 y)
{	    
	LCD_DrawPoint(x,y);//���ĵ� 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}
/*********************************************************************************************************
** Functoin name:       Refreshes_Screen
** Descriptions: 		���������
** input paraments:     xy:���������
** output paraments:    �� 
** Returned values:    	��
*********************************************************************************************************/
void Refreshes_Screen(void)
{
	LCD_Clear(LCD_COLOR_WHITE);//����  

}
/*********************************************************************************************************
** Functoin name:       Convert_Pos
** Descriptions: 		��ADֵת��Ϊ��Ļ������ֵ
** input paraments:     x_in/y_in:��Ҫת����ADֵ
** output paraments:    x_out,y_out:ת�����ADֵ 
** Returned values:    	��
*********************************************************************************************************/
void Convert_Pos(u16 x_in,u16 y_in, u16* x_out, u16* y_out)
{		 
		*x_out=xfac*x_in+xoff;
		*y_out=yfac*y_in+yoff;  
}
/*********************************************************************************************************
** Functoin name:       Touch_Adjust
** Descriptions: 		У׼����
** input paraments:     ��
** output paraments:    �� 
** Returned values:    	��
*********************************************************************************************************/
 float xfac;
 float yfac;
 short xoff;
 short yoff;
void Touch_Adjust(void)
{	
	u8 i;
						 
	u16 pos_temp[4][2];											//���껺��ֵ
	u16 d1,d2;
	u32 tem1,tem2;
	float fac; 

	while(1){
								 
		for(i=0;i<4;i++){
		
			Refreshes_Screen();
			if(i==0){
				Drow_Touch_Point(20,20);
				Touch_GetXY(&pos_temp[0][0],&pos_temp[0][1],1);
			}
			else if(i == 1){
				Drow_Touch_Point(220,20);	
				Touch_GetXY(&pos_temp[1][0],&pos_temp[1][1],1);	
			}
			else if(i == 2){
				Drow_Touch_Point(20,300);
				Touch_GetXY(&pos_temp[2][0],&pos_temp[2][1],1);	
			}
			else {
			
				Drow_Touch_Point(220,300);
				Touch_GetXY(&pos_temp[3][0],&pos_temp[3][1],1);	
			}
		}

			tem1=abs(pos_temp[0][0]-pos_temp[1][0]);					//x1-x2
			tem2=abs(pos_temp[0][1]-pos_temp[1][1]);					//y1-y2
			tem1*=tem1;
			tem2*=tem2;
			d1=sqrt(tem1+tem2);											//�õ�1,2�ľ���
			
			tem1=abs(pos_temp[2][0]-pos_temp[3][0]);					//x3-x4
			tem2=abs(pos_temp[2][1]-pos_temp[3][1]);					//y3-y4
			tem1*=tem1;
			tem2*=tem2;
			d2=sqrt(tem1+tem2);											//�õ�3,4�ľ���
			fac=(float)d1/d2;
			if(fac<0.95||fac>1.05||d1==0||d2==0)						//���ϸ�
			{
				continue;
			}
			tem1=abs(pos_temp[0][0]-pos_temp[2][0]);					//x1-x3
			tem2=abs(pos_temp[0][1]-pos_temp[2][1]);					//y1-y3
			tem1*=tem1;
			tem2*=tem2;
			d1=sqrt(tem1+tem2);											//�õ�1,3�ľ���
			
			tem1=abs(pos_temp[1][0]-pos_temp[3][0]);					//x2-x4
			tem2=abs(pos_temp[1][1]-pos_temp[3][1]);					//y2-y4
			tem1*=tem1;
			tem2*=tem2;
			d2=sqrt(tem1+tem2);											//�õ�2,4�ľ���
			fac=(float)d1/d2;
			if(fac<0.95||fac>1.05)										//���ϸ�
			{
				continue;
			}
			//��ȷ��
						   
			//�Խ������
			tem1=abs(pos_temp[1][0]-pos_temp[2][0]);				//x1-x3
			tem2=abs(pos_temp[1][1]-pos_temp[2][1]);				//y1-y3
			tem1*=tem1;
			tem2*=tem2;
			d1=sqrt(tem1+tem2);										//�õ�1,4�ľ���

			tem1=abs(pos_temp[0][0]-pos_temp[3][0]);				//x2-x4
			tem2=abs(pos_temp[0][1]-pos_temp[3][1]);				//y2-y4
			tem1*=tem1;
			tem2*=tem2;
			d2=sqrt(tem1+tem2);										//�õ�2,3�ľ���
			fac=(float)d1/d2;
			if(fac<0.95||fac>1.05)//���ϸ�
			{
				continue;
			}
			//��ȷ��

			//������
			xfac=(float)220/(pos_temp[1][0]-pos_temp[0][0]);			//�õ�xfac		 
			xoff=(240-xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;			//�õ�xoff
				  
			yfac=(float)300/(pos_temp[2][1]-pos_temp[1][1]);			//�õ�yfac
			yoff=(320-yfac*(pos_temp[2][1]+pos_temp[1][1]))/2;			//�õ�yoff  

			LCD_Clear(LCD_COLOR_BLACK);
			break;	
	}
}
/*********************************************************************************************************
**                                        End Of File
*********************************************************************************************************/
