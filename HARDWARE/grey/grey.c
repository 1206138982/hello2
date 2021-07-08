#include "AllHead.h"

#if defined(BIKING) && BIKING
#include "l298n.h"
extern u8 RUNNING;
#endif

#if defined(FENCHA_TEST) && FENCHA_TEST
u8 max_black_fencha = 80;
#if defined(MAP_TESTA) && MAP_TESTA
extern u8 fencha_times;
#endif
#endif

// u8 MidGreyVal = 0x78;//�ɵ���ֵ
// u8 MidGreyVal = 0x60;//�ɵ���ֵ  for wet day in windows
u8 MidGreyVal = 0x36;//�ɵ���ֵ for night test in windows
// u8 MidGreyVal = 0x48;//�ɵ���ֵ for night test in floor  fail!!
// u8 MidGreyVal = 0x58;//�ɵ���ֵ for day test in floor
// u8 MidGreyVal = 0x45;//�ɵ���ֵ for A4 paper in the night test

//��ȡ������ͼƬ ��ԭͼ��1/8
u8 cutImg[NEEDHEIGHT][NEEDWITH] = {0};
/*�洢���ұ߽�ĺڵ�λ�ã���СΪ�س�����ͼƬ�߶ȳ��Ը���ɨ����*/
u8 leftBlackLoc[(NEEDHEIGHT)/(SKIPLINE)]  = {0};
u8 rightBlackLoc[(NEEDHEIGHT)/(SKIPLINE)] = {0};
/*�洢��Ϊ��Ч�ĶΣ������ұ߽�����ѡ����*/
u8 maxUsefulBlackLine[(NEEDHEIGHT)/(SKIPLINE)] = {0};
u8 maxUsefulLineLen = 0;
//�洢���Ч�εĸ߶�λ��
u8 maxUsefulBlackHeight[(NEEDHEIGHT)/(SKIPLINE)] = {0};
/*��ֱ��ѭ��������ȫ������*/
int cmdByLine = ERR;	//the slope value get from getCmdBySlope()
extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��		
/*����Ժ��б��*/
extern double overK;
extern int b;
/*��⵽��ֱ�����������λ�õ�ƫ��*/
int  lineDeviationLoc;
u8 FLAG_BORDER = 0;
u8 devLocRes = 0;

/*
�������ܣ�ͳһ����ͼ��ɼ������ݴ���ĺ���
*/
void cameraOperation(void)
{  
	u8 res = 0;
	u8 res1 = 0;  
	u8 res2 = 0;
	u8 test_str[20] = {0};
	static u8 line2stop = 0;
	static u8 slope2stop = 0;
    cameraRefresh();//ͼ��ɼ���ֵ���Լ�LCD��ʾ
	/*�����ŷ������أ���ȡ���Һڵ�λ�ã����һ������Ϊ���ʱ�ļ������*/
	getLineEdge(leftBlackLoc,rightBlackLoc,0,NEEDHEIGHT-1,SKIPLINE);
#if defined(MAP_TESTB) && MAP_TESTB	
	find_fencha_move();
#endif
	/*��ȡ�����Ч�Σ�ֻȡһ����Ч�� save in maxUsefulBlackLine and maxUsefulBlackHeight*/
    res = getUsefulLine();
    if(res == BOTHLOST)//��ȫ��ʧ����Ҫ���Ķ���
	{
		//printf("NULL Slope\n");
		line2stop++;
		if(line2stop > 100){
			// printStopMess(1);
#if defined(BIKING) && BIKING
			RUNNING = 0;
#endif
		}
	}
	else //�����������б�ʣ����ż������
	{
		line2stop = 0;
		/*ʹ����С���˷������б��*/
		// res1 = regression(maxUsefulBlackLine,&maxUsefulLineLen,&overK,&b);
		res1 = regression(maxUsefulBlackLine,maxUsefulBlackHeight,&maxUsefulLineLen,&overK,&b);
		if(res1 == GOTSLOPE)
		{
			slope2stop = 0;
			cmdByLine = getCmdBySlope();//��ȡ����
			switch(cmdByLine)
			{
				case RIGHT0_30:
					break;
				case RIGHT30_45:
					break;
				case RIGHT45_60:
					break;
				case RIGHTMORETHAN60:
					break;
				
				case LEFT0_30:
					break;
				case LEFT30_45:
					break;
				case LEFT45_60:
					break;
				case LEFTMORETHAN60:
					break;
				default:printf("ERROR!");
					break;
			}
			/*ר�������ұ߽�ƫ�Ƽ��Ĺ���*/
			res2 = getCmdByDeviLoc();//����ֱ����Ե�ƫ�ƻ�ȡ������
#if defined(BIKING) && BIKING
			motation();
#endif
			print2lcd();
		}
		else
		{
			slope2stop++;
			if(slope2stop > 100){
				printStopMess(2);
#if defined(BIKING) && BIKING
				RUNNING = 0;
#endif
			}
		}
	}
	// printToUart();
	memsetBothBlackLoc();//����һ������ͷ�ɼ�ˢ�²�����Ҫ��գ�
}

/*ר�Ÿ���ֱ�ߵ�ˮƽλ��ƫ���ṩ�Ľӿڣ�����ֵ��������*/
int getCmdByDeviLoc()
{
	// u8 devLocRes = 0;  
	devLocRes = getLineLocCompare2MidLine(&lineDeviationLoc);	
	switch(devLocRes)
	{
		case BOTHLOST:{
			printf("Both lost\r\n");return BOTHLOST;
		}
		case TOOLEFT:{
			printf("TOO LEFT\r\n");return TOOLEFT;
		}
		case TOORIGHT:{
			printf("TOO RIGHT\r\n");return TOORIGHT;
		}
		case NOMIDLOC:{
			printf("NO MID LOC\r\n");return NOMIDLOC;
		}
		case NOLINEWIDTH:{
			printf("NO LINE WIDTH\r\n");return NOLINEWIDTH;
		}
		case GETMIDLOC:{
			printf("DEV: %d \r\n",lineDeviationLoc);
			if((lineDeviationLoc >= 0) && (lineDeviationLoc <= 10)) return RIGHTDEVI0_10;
			if((lineDeviationLoc <= 0) && (lineDeviationLoc >= -10)) return LEFTDEVI0_10;
			if((lineDeviationLoc > 10 ) && (lineDeviationLoc <= 20)) return RIGHTDEVI10_20;
			if((lineDeviationLoc < -10) && (lineDeviationLoc >= -20)) return LEFTDEVI10_20;
			if((lineDeviationLoc > 20 ) && (lineDeviationLoc <= 30)) return RIGHTDEVI20_30;
			if((lineDeviationLoc < -20) && (lineDeviationLoc >= -30)) return LEFTDEVI20_30;
			if((lineDeviationLoc > 30 ) && (lineDeviationLoc <= 40)) return RIGHTDEVI30_40;
			if((lineDeviationLoc < -30) && (lineDeviationLoc >= -40)) return LEFTDEVI30_40;
			if((lineDeviationLoc > 40 )) return RIGHTDEVMORETHAN40;
			if((lineDeviationLoc < -40 )) return LEFTDEVMORETHAN40;
		}
		default:return ERR;
	}
}

/*�������ԭʼ��⵽�ı�Ե*/
void memsetBothBlackLoc()
{  
	memset(leftBlackLoc,0,(NEEDHEIGHT)/(SKIPLINE));
	memset(rightBlackLoc,0,(NEEDHEIGHT)/(SKIPLINE));
}

/*
�������ܣ���OV7670��FIFO�ж������أ��ڷ�ת�Ļ����Ͻ��ж�ֵ��������LCD����ʾ����
*/
void cameraRefresh(void)
{
	u32 m = 0;u32 n = 0;u32 mm = 0;u32 nn = 0;u16 color;
	if(ov_sta)//��֡�жϸ��£�
	{
#if defined(LCD_ON_OFF) && LCD_ON_OFF
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
#if defined(LCD_SHOW_INFO) && LCD_SHOW_INFO
		LCD_Set_Window(50,50,120,80);//����ʾ�������õ���Ļ����
#else
		LCD_Set_Window(100,100,120,80);//����ʾ�������õ���Ļ����
#endif
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
#endif
		  
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
      
		for(m = 0;m < 240;m ++)//��
		{
			for(n = 0;n < 320;n ++)//��
			{
				//��ȡ�����ֽڵ�����
				OV7670_RCK_L;
#if defined(CHANGE_PIN) && CHANGE_PIN
				color=GPIOF->IDR&0XFF;	//�����ݣ�����ͷʹ��PC0-7��Ϊ�������������GPIOC�Ķ˿��������ݼĴ���
#else
				color=GPIOA->IDR&0XFF;	//�����ݣ�����ͷʹ��PC0-7��Ϊ�������������GPIOC�Ķ˿��������ݼĴ���
#endif
				OV7670_RCK_H; 
				color<<=8;  
				OV7670_RCK_L;
#if defined(CHANGE_PIN) && CHANGE_PIN
				color|=GPIOF->IDR&0XFF;	//�����ݣ���Ȼ�ò���������Ҫ�ǲ��������color��������Ļ���������������Ϊ�˵��Է��㣬���Ƕ�һ��
#else
				color|=GPIOA->IDR&0XFF;	//�����ݣ���Ȼ�ò���������Ҫ�ǲ��������color��������Ļ���������������Ϊ�˵��Է��㣬���Ƕ�һ��
#endif
				OV7670_RCK_H; 
				
				//�����ȡ��U��V������գ�������
				color &= 0xff00;
				/*
				�жϷ�ֵ��������ʾ��ֵ
				��Ϊ����ʾ��LCD�Ͽ��Կ�����ʹ�������ֽڽ��б�ʾ��ֵ
				*/
				if( m % 2 == 0)//ѡ����Ҫ�����ˣ���������
				{
					if( n % 4 == 0)//ѡ�е��У���������
					{
						color >>= 8;
						/*��������ķ�ת�洢*/
						if( (u8)color > MidGreyVal) //���ƹ��󣬵Ͱ�λΪ��Ҫ��Yֵ
						{
							cutImg[n / 4][m / 2] = 0xff;//��ɫ
						}
						else 
						{
							cutImg[n / 4][m / 2] = 0x00;//��ɫ									
						}
					}
				}
			}
		}   	
		ov_sta=0;					//����֡�жϱ��
		ov_frame++;
		/*
		���ɼ��������ش����������Ժ�����LCD�����LCDĬ��ʹ�ô������ң��������µķ�ʽ
		��ôLCD����ʾ�����ͼƬ����˵�������������ķ�ת��������ȷ�ġ�
		��������ͼ��Ľ�����ֱ�ӿ���ʹ����������еĲ�����Ĭ��ͼƬ���Ǵ������ң���������
		*/
#if defined(LCD_ON_OFF) && LCD_ON_OFF
		for(mm = 0;mm < 80;mm ++)  //80��
		{
			for(nn = 0;nn < 120;nn ++)  //120��
			{
				if(cutImg[mm][nn] == 0xff)
				{
					color = 0xffff;//��ɫ
					LCD->LCD_RAM = color; 
				}
				else if(cutImg[mm][nn] == 0x00)
				{
					color = 0x0000;//��ɫ
					LCD->LCD_RAM = color; 
				}
			}
		}
#endif
	 }
}	   


/*���ݰ������޸ķ�ֵ*/
void changMidGrey()
{
	u8 key;	
	
	key=KEY_Scan(0);//��֧������
	if(key)
	{
		switch(key)
		{				    
			case KEY0_PRES:{
				MidGreyVal += MODIFYVAL;
				if(MidGreyVal >= MAXGREY){MidGreyVal = MINGREY;}
					break;
			}
			case KEY1_PRES:	{
				MidGreyVal -= MODIFYVAL;
				if(MidGreyVal <= MINGREY){MidGreyVal = MAXGREY;}
				break;
			}
			case KEY2_PRES: {
				break;
			}
			case WKUP_PRES:{
				break;
			}
		}
	}	 
}

/*����ʹ�ã���ӡ��������*/
void printToUart()
{
	u8 res;
	u32 mm;
	u32 nn;
    u32 i,j;
	for(i = 0;i < NEEDHEIGHT;i ++)
    {
		printf("****");
		for(j = 0;j < NEEDWITH;j ++)
		{
			if(cutImg[i][j] == 0xff)
			{
				printf("1");
			}
			else
			{
				printf("0");
			}
				
		}
		printf("****\n");
	}
	
	#if 0
	/*��ӡ���ұ߽��*/
	printf("Left:");
	for(mm = 0;mm < (NEEDHEIGHT)/(SKIPLINE);mm ++)
	{
		printf("%d ",leftBlackLoc[mm]);
	}
	printf("\n");
	printf("******************");
	printf("\n");
	printf("Right:");
	for(nn = 0;nn < (NEEDHEIGHT)/(SKIPLINE);nn ++)
	{
		printf("%d ",rightBlackLoc[nn]);
	}
	#endif

////��ӡ��߽�
//		printf("\n");
//		printf("MAX Len:%d",maxUsefulLineLen);
//		printf("\n");
//		printf("the line is :");
//		for(nn = 0;nn < maxUsefulLineLen;nn ++)
//		{
//			  printf("%d ",maxUsefulBlackLine[nn]);
//		}
//		for(nn = 0;nn < maxUsefulLineLen;nn ++)
//		{
//			  printf("%d ",maxUsefulBlackHeight[nn]);
//		}

//     printf("%lf\n",overK);
		
}

/*
�������ܣ���ú��ߵ����Ҳ�ڵ�λ��
���������洢��������λ�õ�����  ��ʼ��  ������  �������
*/
void getLineEdge(u8 *leftBlackLoc,u8 *rightBlackLoc,u16 startLine,u16 endLine,u16 skipLine)
{
	u16 i = 0;
	//u16 j = 0;
	u16 tmpHeight = 0; 
#if defined(MAP_TESTA) && MAP_TESTA
	u8 get_fencha = 0;
	u8 fencha_start = 0;
#endif
	FLAG_BORDER = 0;
	
    /*���ɨ�輸��*/	
	for(tmpHeight = startLine;tmpHeight < endLine;tmpHeight += skipLine)
	{
#if defined(MAP_TESTA) && MAP_TESTA
		get_fencha = 0;
#endif
		/*һ���еļ�����䣬ÿ���оͼ��һ����㣬һ���ҵ�*/
		for(i = 0;i < NEEDWITH - 3;i ++)  //�����ж������㣬���������������ȥ
		{
#if defined(MAP_TESTA) && MAP_TESTA
			if(cutImg[tmpHeight][i] == 0){
				if(get_fencha == 0)
					fencha_start = i;
				get_fencha++;
			}
#endif
			if(i==NEEDWITH-4 && cutImg[tmpHeight][NEEDWITH-3]==0 && cutImg[tmpHeight][NEEDWITH-2]==0)
			{
				*rightBlackLoc = NEEDWITH-2;
				FLAG_BORDER = 2;
			}
				/*��⵽�����䣬�����ž�����ͬ�ĸߵ�ƽ����ô�����ұߵĺ�ɫ�㱻��⵽�� i+1���Ǻڵ��λ��*/
			else if(  ((cutImg[tmpHeight][i] - cutImg[tmpHeight][i+1] ) <= UPJUMP) && 
					(cutImg[tmpHeight][i+1] == cutImg[tmpHeight][i+2])  )
			{*rightBlackLoc = i+1;}

			if(i==0 && cutImg[tmpHeight][1]==0 && cutImg[tmpHeight][2]==0)
			{
				*leftBlackLoc = 1;
				FLAG_BORDER = 1;
			}
			/*��⵽�����䣬�����ž�����ͬ�ĵ͵�ƽ����ô�������ĺڵ㱻��⵽�� i+1���Ǻڵ��λ��*/
			else if( (cutImg[tmpHeight][i] - cutImg[tmpHeight][i+1] ) >= DOWNJUMP && 
					((cutImg[tmpHeight][i+1] == cutImg[tmpHeight][i+2])) )
			//�洢��ڵ��λ��
			{*leftBlackLoc = i+1;}
		}
			//׼����һ��
		rightBlackLoc ++;leftBlackLoc  ++;				
#if defined(MAP_TESTA) && MAP_TESTA
		if(get_fencha > max_black_fencha){
			fencha_times++;
			if(fencha_times < 5){
				move_for_fencha(fencha_times);
				if(fencha_times == 5)
					RUNNING = 0;
				if(fencha_start <= 20)
					printStopMess(5);
				else
					printStopMess(6);
				break;
			}
		}
#endif
	}
}

/*����ȡ��Ч�εĺ������ֽ��и���*/
//�����ֱ��� ԭʼ��  ��ǰ0�ĸ���  �洢���Ч�ε�λ������  �洢�����  �洢���Ч�εĸ߶�λ��
void getOneSideUsefulLine(u8 *needBlackLoc,u8 countZero,u8 *maxUBlackLine,u8 *maxUBlackLineLen,u8 *maxUBlackHeight)
{
	int i = 0;
	int j = 0;
	/*��ȡ��ε���ʱ�洢����*/
	u8 tmpMaxUsefulLineLen = 0;//ʵʱ���浱ǰ��Ķγ���
	u8 tmpMaxUsefulLine[(NEEDHEIGHT)/(SKIPLINE)]  = {0}; //���ڱ�����Ч�����
	u8 tmpmaxUBlackHeight[(NEEDHEIGHT)/(SKIPLINE)] = {0};//����߶�
	
	//����ߵ�һ����Ч�ĵط������⣬ֱ�����
	for(i = countZero;i < (NEEDHEIGHT)/(SKIPLINE) - 2;i ++)
	{
		/*
			���������λ�ã�ֱ�Ӿ���0����ô��Ϊ��һ�ξ�Ҫ�ɼ������ˣ�
			����ʱ�Ķδ洢��ȫ�ֵı�����
		*/
		if(needBlackLoc[i+2] == 0)//һ�βɼ�����
		{
			//����αȽϳ��Ļ�����ѡ��
			if((*maxUBlackLineLen) < tmpMaxUsefulLineLen)
			{
				//�洢
				(*maxUBlackLineLen) = tmpMaxUsefulLineLen;
				memset(maxUBlackLine,0,(NEEDHEIGHT)/(SKIPLINE));//����Ա��洢
				memset(maxUBlackHeight,0,(NEEDHEIGHT)/(SKIPLINE));
				for(j = 0;j <= tmpMaxUsefulLineLen;j ++)
				{
					maxUBlackLine[j] = tmpMaxUsefulLine[j];
					maxUBlackHeight[j] = tmpmaxUBlackHeight[j];//�洢�߶�
				}
			}
			//�������
			memset(tmpMaxUsefulLine,0,(NEEDHEIGHT)/(SKIPLINE));
			memset(tmpmaxUBlackHeight,0,(NEEDHEIGHT)/(SKIPLINE));
			tmpMaxUsefulLineLen = 0;
			/*����*/
			//��i+2�Լ�������0��λ��ȫ������
			if((i + 3) < (NEEDHEIGHT)/(SKIPLINE))//����Խ��
			{
				i += 3;//��i + 3λ�ÿ�ʼ
			}
			/*δ����������������һֱΪ0����ô�ͼ���*/
			while((i < ((NEEDHEIGHT)/(SKIPLINE) - 2)) && (needBlackLoc[i] == 0))
			{
				i ++;
			}
			continue;
		}
		//��������λ�õıȽϣ�ʵʱ�洢
		if((needBlackLoc[i] != 0) && (needBlackLoc[i+1] != 0) && (needBlackLoc[i+2] != 0) &&
			(abs((needBlackLoc[i] - needBlackLoc[i+1]) - (needBlackLoc[i+1] - needBlackLoc[i+2])) <= DEVIATION))
		{
			/*�洢����ʱ������*/
			tmpMaxUsefulLine[tmpMaxUsefulLineLen] = needBlackLoc[i];
			// tmpmaxUBlackHeight[tmpMaxUsefulLineLen] = i;//�洢�߶�
			tmpmaxUBlackHeight[tmpMaxUsefulLineLen] = i*SKIPLINE;//�洢�߶�
			tmpMaxUsefulLineLen ++;
		}
	}
	
	/*����Чλ�ÿ�ʼ��ȫ���з��ϵ�λ�ã����洢����ղ���*/
	//����αȽϳ��Ļ�����ѡ��
	if((*maxUBlackLineLen) < tmpMaxUsefulLineLen)
	{
		//�洢
		(*maxUBlackLineLen) = tmpMaxUsefulLineLen;
		memset(maxUBlackHeight,0,(NEEDHEIGHT)/(SKIPLINE));//��ո߶ȣ��Ա��洢
		memset(maxUBlackLine,0,(NEEDHEIGHT)/(SKIPLINE));//����Ա��洢
		for(j = 0;j <= tmpMaxUsefulLineLen;j ++)
		{
			maxUBlackLine[j] = tmpMaxUsefulLine[j];
			maxUBlackHeight[j] = tmpmaxUBlackHeight[j];//�洢�߶�
		}
	}
	//�������
	memset(tmpMaxUsefulLine,0,(NEEDHEIGHT)/(SKIPLINE));
	memset(tmpmaxUBlackHeight,0,(NEEDHEIGHT)/(SKIPLINE));
	tmpMaxUsefulLineLen = 0;
}


/*
�������ܣ��ӱ߽������У���ȡ����Ч�������Σ�ȡ����Ϊ��Ч�ɿ��ĶΡ�
*/
int getUsefulLine()
{	  
	//Ĭ�ϱ߽�û���ҵ�
	u8 LeftDirect = LEFTLOST;
	u8 RightDirect = RIGHTLOST;
	/*��ʱ����*/
	u8 countLeftZero = 0;//ͳ��һ���߽��е�0���������ݸ�������ʱ����
	u8 countRightZero = 0;//ͳ��һ���߽��е�0���������ݸ�������ʱ����

/*Ԥ�ȶ����ұ߽���д������û�м�⵽�ͱ��뷵�أ������˺���*/
	/*����˳����鿪ʼ��0����*/
	while((countLeftZero < 20) && (leftBlackLoc[countLeftZero] == 0))
	{
		countLeftZero ++;//��һ������
	}
	if(countLeftZero == 20)//���ȫ��Ϊ0����ô����û�б߽磬ֱ�ӽ���־λ��Ϊlost��ʧ
	{
		LeftDirect = LEFTLOST;
	}
	else
	{
		LeftDirect = GETDIRECT;
	}
	
	/*�ұ��˳�0����*/
	while((countRightZero < 20) && (rightBlackLoc[countRightZero] == 0))
	{
		//��һ������
		countRightZero ++;
	}
	if(countRightZero == 20)//���ȫ��Ϊ0����ô����û�б߽磬ֱ�ӽ���־λ��Ϊlost��ʧ
	{
		RightDirect = RIGHTLOST;
	}
	else
	{
		RightDirect = GETDIRECT;
	}
	//����ʧ����ô��ֱ�ӽ�������
	if((LeftDirect == LEFTLOST) && (RightDirect == RIGHTLOST))
	{
		return BOTHLOST;
	}
	
	if(LeftDirect==GETDIRECT && RightDirect==GETDIRECT && FLAG_BORDER!=0){
		if(FLAG_BORDER == 1){
			getOneSideUsefulLine(rightBlackLoc,countRightZero,maxUsefulBlackLine,&maxUsefulLineLen,maxUsefulBlackHeight);
		}
		else if(FLAG_BORDER == 2){
			getOneSideUsefulLine(leftBlackLoc,countLeftZero,maxUsefulBlackLine,&maxUsefulLineLen,maxUsefulBlackHeight);
		}
	}
	else{
		/*���ұ߽��⿪ʼ*/
		/*�����߽߱��⵽�ˣ���ô�ż���������*/
		if(LeftDirect == GETDIRECT)
		{
			getOneSideUsefulLine(leftBlackLoc,countLeftZero,maxUsefulBlackLine,&maxUsefulLineLen,maxUsefulBlackHeight);
		}
		/*����ұ߽߱��⵽�ˣ���ô�ſ�ʼ�������*/
		if(RightDirect == GETDIRECT)
		{
			getOneSideUsefulLine(rightBlackLoc,countRightZero,maxUsefulBlackLine,&maxUsefulLineLen,maxUsefulBlackHeight);
		}
	}
	
	/*Ҫ��ֻ��һ�����߸�����û�л�ȡ��������ݣ���ô��εĲɼ�����ʧ�ܵģ�����ʧ�ܵ��ź�*/
	if((maxUsefulLineLen == 0) || (maxUsefulLineLen == 1) || (maxUsefulLineLen == 2) || (maxUsefulLineLen == 3))
	{
			/*��շ���*/
		maxUsefulLineLen = 0;
		memset(maxUsefulBlackHeight,0,(NEEDHEIGHT)/(SKIPLINE));
		memset(maxUsefulBlackLine,0,(NEEDHEIGHT)/(SKIPLINE));			
		return BOTHLOST;
	}
	return GETDIRECT;
}

u8 testLineWidth = 0;

/*��õ�ǰ�߿�*/
int getLineWidth(u8 *lMaxULine,u8 *rMaxULine,u8 lMaxULen,u8 rMaxULen,u8 *lMaxLineHei,u8 *rMaxLineHei,u8 *lineWidth)
{
	u8 i;
	// u8 j;
	// u8 m,n;
	//���һ��ˮƽλ���ϣ�������Ч�ξ��������ˮƽλ������λ��
/*���Ȼ��б�ߵ�ˮƽ�߿�*/
	if(lMaxULen <= rMaxULen)//������Ч�߶ν϶̣��������
	{
		for(i = 0;i < lMaxULen;i ++)//��ʼƥ��ͬһˮƽλ�����Ҿ�������Ч����
		{
			// if(rightBlackLoc[lMaxLineHei[i]] != 0)//�������Чλ�ø߶ȣ������ұ���Ч�߶ν���
			if(rightBlackLoc[lMaxLineHei[i]/SKIPLINE] != 0)//�������Чλ�ø߶ȣ������ұ���Ч�߶ν���
			{
				// *lineWidth = abs(rightBlackLoc[lMaxLineHei[i]] - lMaxULine[i]);
				*lineWidth = abs(rightBlackLoc[lMaxLineHei[i]/SKIPLINE] - lMaxULine[i]);
				/*test*/
				testLineWidth = *lineWidth;
				return GOTLINEWIDTH;
			}
		}
	}
	else //�ұߵ���С��Ч�ν϶̣������ұ�
	{
		for(i = 0;i < rMaxULen;i ++)//��ʼƥ��ͬһˮƽλ�����Ҿ�������Ч����
		{
			if(leftBlackLoc[rMaxLineHei[i]] != 0)//�������Чλ�ø߶ȣ������ұ���Ч�߶ν���
			if(leftBlackLoc[rMaxLineHei[i]/SKIPLINE] != 0)//�������Чλ�ø߶ȣ������ұ���Ч�߶ν���
			{
				// *lineWidth = abs(leftBlackLoc[rMaxLineHei[i]] - rMaxULine[i]);
				*lineWidth = abs(leftBlackLoc[rMaxLineHei[i]/SKIPLINE] - rMaxULine[i]);
				/*test*/
				testLineWidth = *lineWidth;
				return GOTLINEWIDTH;
			}
		}
	}
	//������
	*lineWidth = 0;
	/*test*/
	testLineWidth = *lineWidth;
	return NOLINEWIDTH;
}
/*
�������ܣ���ȡ���ǣ���ǰ��⵽��ֱ�ߣ�������û����б�����������ֱ������Ļ��������ƫ��
��ƫΪ������ƫΪ��
˼·�������˳����ұ߽�һ��ʼ��0��
Ȼ��ʹ�û�ȡ���Ч�εĺ����ӿڣ���������߽�����Ч��
Ȼ�����ݱȽ϶̵����Ч�Σ��������ǰ���߿�
�����е�λ�ã���һ�ߵľ��룬�����߿�Ϳ��Լ����ƫ�����ˡ�
*/
int getLineLocCompare2MidLine(int *realVerticalDevationLoc)//�������ڷ��صı�����Ϊ��Ե�ƫ����
{
	u8 i = 0;
	u8 j = 0;
	u8 res = 0;
/*������ʱ���������Ч�߽������*/
	u8 leftMaxULineLoc[((NEEDHEIGHT)/(SKIPLINE))] = {0};
	u8 rightMaxULineLoc[((NEEDHEIGHT)/(SKIPLINE))] = {0};
	//�����
	u8 leftMaxULen = 0;
	u8 rightMaxULen = 0;
	//��Ч�εĸ߶�λ��
	u8 leftMaxUBlackHeight[((NEEDHEIGHT)/(SKIPLINE))] = {0};
	u8 rightMaxUBlackHeight[((NEEDHEIGHT)/(SKIPLINE))] = {0};
	//��ǰ�߿�
	u8 lineWidth = 0;
/******Ԥ�ȶ����ұ߽���д������û�м�⵽�ͱ��뷵�أ������˺���*****/
/*���Ƚ��ο�ʼ�����������˳�*/
	//Ĭ�ϱ߽�û���ҵ�
	u8 LeftDirect = LEFTLOST;
	u8 RightDirect = RIGHTLOST;
	/*��ʱ����*/
	u8 countLeftZero = 0;//ͳ��һ���߽��е�0���������ݸ�������ʱ����
	u8 countRightZero = 0;//ͳ��һ���߽��е�0���������ݸ�������ʱ����
	u8 left_nums,right_nums;
	/*����˳����鿪ʼ��0����*/
	while((countLeftZero < 20) && (leftBlackLoc[countLeftZero] == 0))
	{ countLeftZero ++;}//��һ������
	if(countLeftZero == 20)//���ȫ��Ϊ0����ô����û�б߽磬ֱ�ӽ���־λ��Ϊlost��ʧ
	{LeftDirect = LEFTLOST;}
	else{LeftDirect = GETDIRECT;}
	/*�ұ��˳�0����*/
	while((countRightZero < 20) && (rightBlackLoc[countRightZero] == 0))
	{countRightZero ++;}//��һ������
	if(countRightZero == 20)//���ȫ��Ϊ0����ô����û�б߽磬ֱ�ӽ���־λ��Ϊlost��ʧ
	{RightDirect = RIGHTLOST;}
	else{RightDirect = GETDIRECT;}
	//����ʧ����ô��ֱ�ӽ�������
	if((LeftDirect == LEFTLOST) && (RightDirect == RIGHTLOST))
	{
		return BOTHLOST;//ֻ���ض�ʧ�źţ�����ƫ��������
	}
/***********************************************************************/
	/*û�м�⵽��ߵı߽� ��ô����̫ƫ���ˣ���ʱ����ȷ������ƫ������ֻ�ܷ������õ����ֵ*/
	if((LeftDirect == LEFTLOST) && (RightDirect == GETDIRECT))
	{
		*realVerticalDevationLoc = -80;
		return TOOLEFT;
	}
	/*û�м�⵽�ұߵı߽�*/
	if((LeftDirect == GETDIRECT) && (RightDirect == RIGHTLOST))
	{
		*realVerticalDevationLoc = 80;
		return TOORIGHT;
	}
	/*************************���¾�Ϊ�����߽߱��*********************************/
	
	/*�����⵽�����ұ߽磬�ٷ�һ�¼������*/
	//���Ȳɼ�һ�����ұ߽����Ч�Ρ�
	getOneSideUsefulLine(leftBlackLoc,countLeftZero,leftMaxULineLoc,&leftMaxULen,leftMaxUBlackHeight);
	getOneSideUsefulLine(rightBlackLoc,countRightZero,rightMaxULineLoc,&rightMaxULen,rightMaxUBlackHeight);
	//��ô����leftMaxULineLoc leftMaxULen �� rightMaxULineLoc  rightMaxULen���Ѿ�����ʹ����
	//������Ч��̫�̵ı߽磬ֱ�ӷ���̫ƫ��ƫ�Ҽ���
	if((leftMaxULen <= 2)){
		*realVerticalDevationLoc = -80;
		return TOOLEFT;
	}
	if((rightMaxULen <= 2)){
		*realVerticalDevationLoc = 80;
		return TOORIGHT;
	}
	/*����߿�*/
	res = getLineWidth(leftMaxULineLoc,rightMaxULineLoc,leftMaxULen,rightMaxULen,leftMaxUBlackHeight,rightMaxUBlackHeight,&lineWidth);
	if(res == NOLINEWIDTH)//���û�л�ȡ���߿�
	{
		lineWidth = 0;
		*realVerticalDevationLoc = 0;
		return NOLINEWIDTH;
	}
	/*��ʼʹ��������Ļ�е�ȥ����ƫ�ƣ�Ĭ�Ͽ϶���⵽������*/
	//����߿�ʼ
	for(i = 0;i < leftMaxULen;i ++)
	{
		// if(leftMaxUBlackHeight[i] == MIDHORIHEIGHT)//���������ˮƽ�����н���
		if(leftMaxUBlackHeight[i]/SKIPLINE == MIDHORIHEIGHT)//���������ˮƽ�����н���
		{
			/*����ֱ��λ���е���ĸ�λ�ã���õ�����ʽ�ӣ�����ʽ�ӽ����Ϊ�෴��*/
		
			//�������߶�ƫ�ң����Ϊ����
			// if((leftMaxULineLoc[leftMaxUBlackHeight[i]] + (lineWidth / 2)) > MIDHORLOC)
			if((leftMaxULineLoc[leftMaxUBlackHeight[i]/SKIPLINE] + (lineWidth / 2)) > MIDHORLOC)
			{
				// *realVerticalDevationLoc = (lineWidth / 2) - MIDHORLOC + leftMaxULineLoc[leftMaxUBlackHeight[i]];
				*realVerticalDevationLoc = (lineWidth / 2) - MIDHORLOC + leftMaxULineLoc[leftMaxUBlackHeight[i]/SKIPLINE];
				return GETMIDLOC;
			}
			else  //�����߶�ƫ�󣬽��Ϊ��
			{
				//��ֵ��ƫ����
				// *realVerticalDevationLoc = MIDHORLOC - leftMaxULineLoc[leftMaxUBlackHeight[i]] - (lineWidth / 2);
				*realVerticalDevationLoc = MIDHORLOC - leftMaxULineLoc[leftMaxUBlackHeight[i]/SKIPLINE] - (lineWidth / 2);
				*realVerticalDevationLoc = 0 - *realVerticalDevationLoc;//ȡ���Ժ󣬷���Ϊ��ֵ����ʾΪ��ƫ
				return GETMIDLOC;
			}
		}
	}
	/*�����ұ�*/
	for(i = 0;i < rightMaxULen;i ++)
	{
		// if(rightMaxUBlackHeight[i] == MIDHORIHEIGHT)//���������ˮƽ�����н���
		if(rightMaxUBlackHeight[i]/SKIPLINE == MIDHORIHEIGHT)//���������ˮƽ�����н���
		{
			/*����ֱ��λ���е���ĸ�λ�ã���õ�����ʽ�ӣ�����ʽ�ӽ����Ϊ�෴��*/
			//����ƫ��
			// if((rightMaxULineLoc[rightMaxUBlackHeight[i]] + (lineWidth / 2)) > MIDHORLOC)
			if((rightMaxULineLoc[rightMaxUBlackHeight[i]/SKIPLINE] + (lineWidth / 2)) > MIDHORLOC)
			{
				// *realVerticalDevationLoc = rightMaxULineLoc[rightMaxUBlackHeight[i]] - MIDHORLOC - (lineWidth / 2);
				*realVerticalDevationLoc = rightMaxULineLoc[rightMaxUBlackHeight[i]/SKIPLINE] - MIDHORLOC - (lineWidth / 2);
				return GETMIDLOC;
			}
			else
			{
				// *realVerticalDevationLoc = (lineWidth / 2) - rightMaxULineLoc[rightMaxUBlackHeight[i]] + MIDHORLOC;
				*realVerticalDevationLoc = (lineWidth / 2) - rightMaxULineLoc[rightMaxUBlackHeight[i]/SKIPLINE] + MIDHORLOC;
				*realVerticalDevationLoc = 0 - *realVerticalDevationLoc;
				return GETMIDLOC;
			}
		}
	}
/* ���½ǡ����½��Ż�����  */
	// left_nums = 0;
	// right_nums = 0;
	// getOneSideUsefulLine(leftBlackLoc,countLeftZero,leftMaxULineLoc,&leftMaxULen,leftMaxUBlackHeight);
	// getOneSideUsefulLine(rightBlackLoc,countRightZero,rightMaxULineLoc,&rightMaxULen,rightMaxUBlackHeight);
	// if(leftMaxUBlackHeight[leftMaxULen] < )     //the lowest point
	// for(i=0;i<leftMaxULen;i++){
	// 	if(leftMaxULineLoc[i] < NEEDWITH/2){
	// 		left_nums++;
	// 	}
	// }
	// for(i=0;i<rightMaxULen;i++){
	// 	if(rightMaxULineLoc[i] > NEEDWITH/2){
	// 		right_nums++;
	// 	}
	// }
	// if(left_nums >= leftMaxULen*2/3){
	// 	*realVerticalDevationLoc = -NEEDWITH/2;
	// }
	// else if(right_nums >= rightMaxULen*2/3){
	// 	*realVerticalDevationLoc = NEEDWITH/2;
	// }
	return NOMIDLOC;
}
