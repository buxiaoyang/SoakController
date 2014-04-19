#include <reg52.h>
#include <position.h>
#include <parameter.h>
#include <basefunc.h>

//sbit sensorPositon = P3^4; //机械臂定位传感器
//机械臂定位传感器
#define sensorPositon P3


uchar sensorPosiCheck = 0; //机械臂A位置检测标志位 
							//0：没有感应器没有低电位状态
							//1：检测到低电位进入延迟状态
							//2：等待低电位释放进入延时
							//3：检测低电位确实释放 

uchar getPositonBySensor() //根据传感器获取机械臂位置
{
	if(sensorPositon == 0xFE) 	//进口
	{
		return 	0;
	}
	else if(sensorPositon == 0xFD) //一号槽
	{
		return 	2;
	}
	else if(sensorPositon == 0xFB)	 //二号槽
	{
		return 	4;
	}
	else if(sensorPositon == 0xF7)	//三号槽
	{
		return 	6;
	}
	else if(sensorPositon == 0xEF)	 //四号槽
	{
		return 	8;
	}
	else if(sensorPositon == 0xDF)	  //五号槽
	{
		return 	10;
	}
	else if(sensorPositon == 0xBF)	 //六号槽
	{
		return 	12;
	}
	else if(sensorPositon == 0x7F)	//出口
	{
		return 	14;
	}
	else
	{
		//传感器错误
		runMode = ManualMode;
		return 100;
	}	
}

void maniPosition(void) //机械臂定位
{
	uchar  getPositonBySensorValue = 0;
	//机械臂A位置检测
	if(sensorPosiCheck==0)
	{
		if(sensorPositon != 0xFF)
		{
			sensorPosiCheck = 1;
		}
	}
	else if(sensorPosiCheck==1)
	{
		if(sensorPositon != 0xFF)
		{
			if(drivingDirect == DGoOutfall)
			{
				//向右机械臂位置加一
				manipulator.currentPosition = getPositonBySensor();
			}
			else if (drivingDirect == DGoEntrance)
			{
				//向右机械臂位置减一
				manipulator.currentPosition = getPositonBySensor();	
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
		if(sensorPositon == 0xFF)
		{
			sensorPosiCheck = 3;
		}	
	}
	else
	{
		if(sensorPositon == 0xFF)
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
