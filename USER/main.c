#include "AllHead.h"

#if defined(BIKING) && BIKING
#include "l298n.h"
#if defined(PID_METHOD) && PID_METHOD
pid_struct	line_pid;
pid_struct	oK_pid;
#endif
#endif
void JTAG_Set(u8 mode);
u8 RUNNING = 1;

int main(void)
{	 
	JTAG_Set(0x01);
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200	
	TIM6_Int_Init(10000,7199);	//10Khz����Ƶ��,1�����ж�  printf N fps message in timer.c  N frames every second  result:15fps
#if defined(LCD_ON_OFF) && LCD_ON_OFF
	LCD_Init();			   		//��ʼ��LCD  
	POINT_COLOR=RED;			//��������Ϊ��ɫ 
	LCD_ShowString(30,230,200,16,16,"OV7670 Init...");	  
#endif
	while(OV7670_Init())//��ʼ��OV7670
	{
#if defined(LCD_ON_OFF) && LCD_ON_OFF
		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
		delay_ms(200);
		LCD_Fill(30,230,239,246,WHITE);
#endif
		delay_ms(200);
	}
#if defined(LCD_ON_OFF) && LCD_ON_OFF
	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");
	delay_ms(500);	 	   
	LCD_Clear(BLACK); 
#endif
	EXTI11_Init();		// PB15 for ov7670 VSYNC interput pin in exti.c
	OV7670_Window_Set(12,176,240,320); 
	OV7670_CS=0;					
#if defined(BIKING) && BIKING
	// Motor_startL();  // for l298n
	MotorA_start();		//for A4950 in motor.c
	RUNNING = 1;
#if defined(PID_METHOD) && PID_METHOD
	PID_init(&line_pid,1.6,0.07,5);
	PID_init(&oK_pid,80,5,90);
/*   �����ȶ�����ֱ��
	PID_init(&line_pid,1.6,0.07,5);
	PID_init(&oK_pid,80,5,90);
*/
#endif
#if defined(START_TEST) && START_TEST
	// motor_test();      // for l298n
	motorA_test();		//for A4950
	RUNNING = 0;
#endif
#endif
	while(1)
	{
#if defined(BIKING) && BIKING
		if(RUNNING){
#endif
			cameraOperation();//����ͷ������ʾ�Լ�������ͼ��������ӿ�
#if defined(BIKING) && BIKING
		}
		else{
			Motor_Stop();
			printf("monitor stop\r\n");
			delay_ms(1000);
		} 
#endif
	}
}

/*��Ե��Ժ�ʹ�ó�ͻ����*/
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp = mode;
	temp <<= 25;
	RCC->APB2ENR |= 1<<0;
	AFIO->MAPR &= 0xF8FFFFFF;
	AFIO->MAPR |= temp;
}
