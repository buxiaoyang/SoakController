#include <reg52.h>
#include <position.h>
#include <parameter.h>
#include <basefunc.h>

sbit sensorPositon = P3^4; //机械臂定位传感器

uchar sensorPosiCheck = 0; //机械臂A位置检测标志位 
							//0：没有感应器没有低电位状态
							//1：检测到低电位进入延迟状态
							//2：等待低电位释放进入延时
							//3：检测低电位确实释放

void maniPosition(void) //机械臂定位
{
	//机械臂A位置检测
	if(sensorPosiCheck==0)
	{
		if(sensorPositon == 0)
		{
			sensorPosiCheck = 1;
		}
	}
	else if(sensorPosiCheck==1)
	{
		if(sensorPositon == 0)
		{
			if(drivingDirect == DGoOutfall)
			{
				//向右机械臂位置加一
				manipulator.currentPosition++;
				displayFlag = 1;
				if(manipulator.currentPosition>14)
				{
					manipulator.currentPosition = 14;
				}
			}
			else if (drivingDirect == DGoEntrance)
			{
				//向右机械臂位置减一
				manipulator.currentPosition--;
				displayFlag = 1;
				if(manipulator.currentPosition>200)
				{
					manipulator.currentPosition = 0;
				}	
			}
			sensorPosiCheck = 2;
		}
		else
		{
			//干扰信号直接释放
			sensorPosiCheck = 0;
		}
	}
	else if(sensorPosiCheck==2)
	{
		if(sensorPositon == 1)
		{
			sensorPosiCheck = 3;
		}	
	}
	else
	{
		if(sensorPositon == 1)
		{
			if(drivingDirect == DGoOutfall)
			{
				//向右机械臂位置加一
				manipulator.currentPosition++;
				displayFlag = 1;
				if(manipulator.currentPosition>14)
				{
					manipulator.currentPosition = 14;
				}
			}
			else if (drivingDirect == DGoEntrance)
			{
				//向右机械臂位置减一
				manipulator.currentPosition--;
				displayFlag = 1;
				if(manipulator.currentPosition>200)
				{
					manipulator.currentPosition = 0;
				}	
			}
			sensorPosiCheck = 0;
		}
	}
}
