#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
#include "AllHead.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ο�������guanfu_wang���롣
//ALIENTEKս��STM32������V3
//SCCB ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/18
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
  
#if defined(CHANGE_PIN) && CHANGE_PIN
#define SCCB_SDA_IN()  {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=0X00080000;}
#define SCCB_SDA_OUT() {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=0X00030000;}
#define SCCB_SCL    		PBout(13)	 	//SCL
#define SCCB_SDA    		PBout(12) 		//SDA	 
#define SCCB_READ_SDA    	PBin(12)  		//����SDA    
#else
#define SCCB_SDA_IN()  {GPIOC->CRL&=0XFFFFF0FF;GPIOC->CRL|=0X00000800;}
#define SCCB_SDA_OUT() {GPIOC->CRL&=0XFFFFF0FF;GPIOC->CRL|=0X00000300;}

//IO��������	 
#define SCCB_SCL    		PCout(0)	 	//SCL
#define SCCB_SDA    		PCout(2) 		//SDA	 

#define SCCB_READ_SDA    	PCin(2)  		//����SDA    
#endif
#define SCCB_ID   			0X42  			//OV7670��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif













