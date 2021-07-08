#ifndef _OV7670_H
#define _OV7670_H
#include "sys.h"
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ο�������guanfu_wang���롣
//ALIENTEKս��STM32������V3
//OV7670 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/18
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
/* the follow code for macro CHANGE_PIN */
// #if defined(CHANGE_PIN) && CHANGE_PIN
#define OV7670_RCK_H	PEout(4)=1//���ö�����ʱ�Ӹߵ�ƽ
#define OV7670_RCK_L	PEout(4)=0	//���ö�����ʱ�ӵ͵�ƽ
#define OV7670_WREN		PEout(3)		//д��FIFOʹ��
#define OV7670_CS		PEout(2)  		//Ƭѡ�ź�(OE)
#define OV7670_WRST		PEout(1)		//дָ�븴λ
#define OV7670_RRST		PEout(0)  		//��ָ�븴λ
#define OV7670_VSYNC  	PBin(15)			//ͬ���źż��IO  used in exti.c
// #endif
 
// #define OV7670_VSYNC  	PBin(11)			//ͬ���źż��IO
//// #define OV7670_WRST		PCout(15)		//дָ�븴λ
// #define OV7670_WRST		PCout(5)		//дָ�븴λ
// #define OV7670_WREN		PBout(10)		//д��FIFOʹ��
// #define OV7670_RCK_H	GPIOB->BSRR=1<<1//���ö�����ʱ�Ӹߵ�ƽ
// #define OV7670_RCK_L	GPIOB->BRR=1<<1	//���ö�����ʱ�ӵ͵�ƽ
// #define OV7670_RRST		PCout(1)  		//��ָ�븴λ
// #define OV7670_CS		PCout(3)  		//Ƭѡ�ź�(OE)


#define OV7670_DATA   GPIOA->IDR&0x00FF  					//��������˿�
/////////////////////////////////////////									
	    				 
u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);


#endif





















