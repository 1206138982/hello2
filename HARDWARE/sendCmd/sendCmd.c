#include "AllHead.h"

/*���б�ʣ���������*/
extern double overK;

extern 

/*����б�ʣ���ʼ�ж�ֱ�ߵ�б��*/
//ע�⣬overK�����������0��Ҫ�ǳ��֣���ô������
int getCmdBySlope()
{
	  //��ƫ0.00001-30������ʱ��б��
	  if((overK > 0) && (overK <= 2.5))
		{
	      return RIGHT0_30;
		}
		//��ƫ-0.00001 - -30������ʱ��б��
		if((overK < 0) && (overK >= -2.5))
		{
			  return LEFT0_30;
		}
		
		//��ƫ30 -45�����ҵ�б��
		if((overK > 2.5) && (overK <= 3.8))
		{
        return RIGHT30_45;
		}
		if((overK < -2.5) && (overK >= -3.8))
		{
			  return LEFT30_45;
		}
		
		//��ƫ45-60��
		if((overK > 3.8) && (overK <= 7.1))
		{
        return  RIGHT45_60;
		}
		//��ƫ45-60��
		if((overK < -3.8) && (overK >= -7.1))
		{
			  return LEFT45_60;
		}
		
		/*�ұ߳�����60��*/
		if(overK > 7.1)
		{
			  return RIGHTMORETHAN60;
		}
		
		if(overK < -7.1)
		{
			  return LEFTMORETHAN60;
		}
		//�����ǶȾ����ش���һ��Ϊ�ӽ�ˮƽ�ˡ�
		return ERR;
}





