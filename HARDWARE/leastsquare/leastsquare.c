#include "AllHead.h"

double overK = 0;//��б��
// u8 b = 0;//����
int b = 0;//����


/*�洢��Ϊ��Ч�ĶΣ������ұ߽�����ѡ����*/
extern u8 maxUsefulBlackLine[(NEEDHEIGHT)/(SKIPLINE)];
extern u8 maxUsefulLineLen;

/*��С���˷����ֱ��*/
// int regression(u8 *maxLine,u8 *maxLen,double *oK,int *ob)
int regression(u8 *maxLine,u8 *maxLineH,u8 *maxLen,double *oK,int *ob)
{
	  /*�ۻ�����*/
	  double x_sum = 0,y_sum = 0,xy_sum = 0,xx_sum = 0;
		double x_avr = 0,y_avr = 0,xy_avr = 0,xx_avr = 0;
		int i;
		x_sum=0;y_sum=0;xy_sum=0;xx_sum=0;
	 	
		for(i=0;i < *maxLen;i++)
		{
				x_sum+=maxLine[i];
				// y_sum+= i;
				y_sum+= (80 - maxLineH[i]);
				// xy_sum+=maxLine[i]*(i);
				xy_sum+=maxLine[i]*(80-maxLineH[i]);
				xx_sum+=maxLine[i]*maxLine[i];
		}
		x_avr=x_sum/(*maxLen);
		y_avr=y_sum/(*maxLen);
		xy_avr=xy_sum/(*maxLen);
		xx_avr=xx_sum/(*maxLen);
		
		/*��ʽ�з���Ϊ0*/
		if((xy_avr-x_avr*y_avr)==0)
		{
			  /*****����*****/
			  //printToUart();//�����֮ǰ��ӡ
			  
			  *oK = 0;
			  /*����������õ����Ч�Σ��Ա���һ�βɼ�*/
			  *maxLen = 0;
	      memset(maxLine,0,(NEEDHEIGHT)/(SKIPLINE));	  
			
				return NOSLOPE;
		}
		else
		{
				*oK = (xx_avr-x_avr*x_avr)/(xy_avr-x_avr*y_avr);//��б��
				// *oK = (xy_avr-x_avr*y_avr)/(xx_avr-x_avr*x_avr);//��б��
				// *oK = 0 - (*oK);
//				realK=(xy_avr-x_avr*y_avr)/(xx_avr-x_avr*x_avr);//��б��
				// *ob = maxLine[0];//��ȡK
				*ob = 80 - maxLineH[0]-(1/(*oK))*(maxLine[0]);//��ȡK
			
			  			  /*****����*****/
			  //printToUart();//�����֮ǰ��ӡ
			
			   //�˳�����б��
			  if(((*oK) >= 20) || ((*oK) <= -20))
				{
						(*oK) = 0;
				}
			
        /*����������õ����Ч�Σ��Ա���һ�βɼ�*/
			  *maxLen = 0;
	      memset(maxLine,0,(NEEDHEIGHT)/(SKIPLINE));		
			  
				return GOTSLOPE;
		}
}



