#include <reg52.h>
#include <position.h>
#include <parameter.h>
#include <basefunc.h>
#include <key.h>

sbit Key_Out = P1^5;  //出口就绪按键
sbit Key_In = P1^6;	  //进口就绪按键

void Key_Scan(void);

uchar Key_Scan_Steps = 0; 		//0：初始状态 如果有按键按下则进入1
								//1：延迟10ms
								//2：如果按键确实按下则进入3
								//3：确定按键
								//4：如果按键释放则进入0

void Key_Scan(void)
{
	switch(Key_Scan_Steps)
	{
		case 0:
			if(Key_Out == 0 || Key_In == 0)
			{
			   	Key_Scan_Steps = 1;
			}
		break;
		case 1:
			if(Key_Out == 0 || Key_In == 0)
			{
			   	Key_Scan_Steps = 2;
			}
			else
			{
				Key_Scan_Steps = 0;
			}
		break;
		case 2:
			if(Key_Out == 0)
			{
			   	cistern[7].cisternStatus = Ready;
				displayFlag = 1;
			}
			if(Key_In == 0)
			{
				cistern[0].cisternStatus = Ready;
				displayFlag = 1;	
			}
			Key_Scan_Steps = 3;
		break;
		case 3:
			if(Key_Out == 1 && Key_In == 1)
			{
			   	Key_Scan_Steps = 0;
			}
		break;
		default:
			 _nop_();
	}
}