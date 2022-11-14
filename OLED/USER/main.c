//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RCT6,����ԭ��MiniSTM32������,��Ƶ72MHZ������12MHZ
//QDtech-OLEDҺ������ for STM32
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/8/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
// OLEDģ��               STM32��Ƭ��
//   VCC         ��       DC 5V/3.3V      //OLED����Դ��
//   GND         ��          GND          //OLED����Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ4����SPI
// OLEDģ��               STM32��Ƭ��
//   D1          ��          PB15        //OLED��SPIд�ź�
//=======================================Һ���������߽���==========================================//
// OLEDģ��               STM32��Ƭ��
//   CS          ��          PB11        //OLED��Ƭѡ�����ź�
//   RES         ��          PB12        //OLED����λ�����ź�
//   DC          ��          PB10        //OLED������/����ѡ������ź�
//   D0          ��          PB13        //OLED��SPIʱ���ź�
//=========================================����������=========================================//
//��ģ�鲻���������ܣ����Բ���Ҫ����������
****************************************************************************************************/	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/	
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "gui.h"
#include "test.h"
#include "AHT20-21_DEMO_V1_3.h" 


//����¶Ⱥ�ʪ��
uint32_t CT_data[2]={0,0};
//ʪ�Ⱥ��¶�
volatile int  c1,t1;

//����LED��ʾ���¶Ⱥ�ʪ��
u8 temp[10];  
u8 hum[10];

//��ʼ��PC13���ڲ���
void GPIOC13_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);

}
//��ʼ���Լ�ǰ��׼��
void Init(void);

//��ȡ��ʪ��
void getData(void);

//��ʾ��ʪ��
void showData(void);

int main(void)
{	
	//��ʼ��
	Init();
	while(1){

		//��ȡ����
		getData();
		//��ʾ����
		showData();

		//��������
		OLED_WR_Byte(0x2F,OLED_CMD);
		
		//��ʱ
		Delay_1ms(3100);
		//OLED_Clear(0); 
	}
	
}

//��ʼ���Լ�ǰ��׼��
void Init(void){
	//��ʼ��PC12
	GPIOC13_Init();		
	
	//��ʱ������ʼ��	  
	delay_init();	   
	
	//��ʼ��OLED 
	OLED_Init();

	//������ȫ�ڣ�	
	OLED_Clear(0);    

	//������ʾ��Ϣ	

	GUI_ShowCHinese(10,0,16,"���",1);
	
	GUI_ShowString(10,24,"631807060431",16,1);
	
	
	Delay_1ms(1000);
	
	AHT20_Init();
	/***********************************************************************************/
	/**///�ٸ��ϵ磬��ƷоƬ�ڲ�������Ҫʱ�䣬��ʱ100~500ms,����500ms
	/***********************************************************************************/
	
	Delay_1ms(1000);
	
	OLED_Clear(0); 
	OLED_WR_Byte(0x2E,OLED_CMD); //�رչ���

	OLED_WR_Byte(0x27,OLED_CMD); //ˮƽ��������ҹ��� 26/27

	OLED_WR_Byte(0x00,OLED_CMD); //�����ֽ�

	OLED_WR_Byte(0x00,OLED_CMD); //��ʼҳ 0

	OLED_WR_Byte(0x07,OLED_CMD); //����ʱ����

	OLED_WR_Byte(0x02,OLED_CMD); //��ֹҳ 2

	OLED_WR_Byte(0x00,OLED_CMD); //�����ֽ�

	OLED_WR_Byte(0xFF,OLED_CMD); //�����ֽ�
	
	
	GUI_ShowCHinese(10,0,16,"һƬ�³�����ɽ",1);	
}

//��ȡ��ʪ��
void getData(){
	//AHT20_Read_CTdata(CT_data);       //������CRCУ�飬ֱ�Ӷ�ȡAHT20���¶Ⱥ�ʪ������    �Ƽ�ÿ������1S��һ��
		AHT20_Read_CTdata(CT_data);;  //crcУ��󣬶�ȡAHT20���¶Ⱥ�ʪ������ 
		c1 = CT_data[0]*1000/1024/1024;  //����õ�ʪ��ֵc1���Ŵ���10����
		t1 = CT_data[1]*2000/1024/1024-500;//����õ��¶�ֵt1���Ŵ���10����

		//תΪ�ַ���������ʾ
		temp[0]=t1/100+'0';
		temp[1]=(t1/10)%10+'0';
		temp[2]='.';
		temp[3]=t1%10+'0';
		temp[4]='\0';
		
		hum[0]=c1/100+'0';
		hum[1]=(c1/10)%10+'0';
		hum[2]='.';
		hum[3]=c1%10+'0';
		hum[4]=32;
		hum[5]='%';
		hum[6]='\0';
}


//��ʾ��ʪ��
void showData(){
		//��ʾ�¶�
		GUI_ShowCHinese(15,28,16,"�¶�",1);
		GUI_ShowString(47,28,":",16,1);
		GUI_ShowString(62,28,temp,16,1);
		
		

		//��ʾʪ��
		GUI_ShowCHinese(15,48,16,"ʪ��",1);
		GUI_ShowString(47,48,":",16,1);
		GUI_ShowString(62,48,hum,16,1);
}
