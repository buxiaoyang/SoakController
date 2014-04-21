#include <reg52.h>
#include <position.h>
#include <parameter.h>
#include <basefunc.h>
#include <operation.h>
#include <intrins.h>
#include <dispatch.h>

void ManiDispatch(void); //械臂调度操作
void ManualDispatch(void); //手动状态下机械臂检测

uchar ManiDispatchSteps = 0; 	//0：开机状态（机械臂在最下面，槽1为空）
								//1：机械臂移动到进口位置
								//2：出口空，并且6号槽就绪
								//3：6号槽空，并且5号就绪
								//4：5号槽空，并且4号就绪
								//5：4号槽空，并且3号就绪
								//6：3号槽空，并且2号就绪
								//7：3号槽空，并且1号就绪
								//8：2号槽空，并且入口就绪
								//9：1号槽空，并且入口就绪
								//10：10：入口空，并且出口就绪
								//20：检测空槽
								//100：空操作

uchar ManualDispatchSteps = 0;

uint ManiDispatchTimer = 0; //机械臂调度定时器							
uchar ManiDispatchCurrent = 0; //当前操作的框


void ManiDispatch(void)
{
	//设置机械臂速度等级
	uchar distanceBetweenManiAndTarget = 0;
	if(TargetCisternNumber > manipulator.currentPosition)
	{
		distanceBetweenManiAndTarget = 	TargetCisternNumber - manipulator.currentPosition;
	}
	else
	{
		distanceBetweenManiAndTarget = 	manipulator.currentPosition - TargetCisternNumber;
	}
	if(distanceBetweenManiAndTarget > 2)
	{
		MotorSpeedFlag = 1;
	}
	else
	{
		MotorSpeedFlag = 0;
	}
	//机械臂调度状态机
	switch(ManiDispatchSteps)
	{
		case 0:	//0：开机状态（机械臂在最下面，槽1为空）
			A_Move_Entrance_Steps = 0;
			ManiDispatchSteps = 1;
		break;
		case 1:	//1：机械臂移动到进口位置
			A_Move_Entrance();
			if(A_Move_Entrance_Steps == 100)
			{
				ManiDispatchSteps = 20;
			}
		break;
		case 2:	 //2：出口空，并且6号槽就绪
			A_Up_Down(12,14);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
			} 
		break;
		case 3:	 //3：6号槽空，并且5号就绪
			A_Up_Down(10,12);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
			}
		break;
		case 4:	 //4：5号槽空，并且4号就绪
			A_Up_Down(8,10);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
			}
		break;
		case 5:	 //5：4号槽空，并且3号就绪
			A_Up_Down(6,8);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
			}
		break;
		case 6:	 //6：3号槽空，并且2号就绪
			A_Up_Down(4,6);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
			}
		break;
		case 7:	 //7：3号槽空，并且1号就绪
			A_Up_Down(2,6);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
				//下班检测
				if(displayMode == PowerOffing)
				{
					displayMode = PowerOffed;
				}
			}
		break;
		case 8:	 //8：2号槽空，并且入口就绪
			A_Up_Down(0,4);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
			}
		break;
		case 9:	 //9：1号槽空，并且入口就绪
			A_Up_Down(0,2);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
			}
		break;
		case 10: //10：入口空，并且出口就绪
			A_Up_Down(14,0);
			if(A_Up_Down_Steps == 100)
			{
				ManiDispatchSteps = 20;
			}
		break;
		case 20:
			if(cistern[2].cisternStatus == Empty && cistern[0].cisternStatus == Ready)	 //8：2号槽空，并且入口就绪
			{
				A_Up_Down_Steps = 0;
				timeBetweenOpeartionDry = timeBetweenOpeartion;
				ManiDispatchSteps = 8;
			}
			else if(cistern[1].cisternStatus == Empty && cistern[0].cisternStatus == Ready)	 //9：1号槽空，并且入口就绪
			{
				A_Up_Down_Steps = 0;
				timeBetweenOpeartionDry = timeBetweenOpeartion;
				ManiDispatchSteps = 9;
			}
			else if(cistern[0].cisternStatus == Empty && cistern[7].cisternStatus == Ready)	 //10：入口空，并且出口就绪
			{
				A_Up_Down_Steps = 0;
				timeBetweenOpeartionDry = timeBetweenOpeartion;
				ManiDispatchSteps = 10;
			}
			else if(cistern[7].cisternStatus == Empty && cistern[6].cisternStatus == Ready) //2：出口空，并且6号槽就绪
			{
				A_Up_Down_Steps = 0;
				timeBetweenOpeartionDry = timeBetweenOpeartion;
				ManiDispatchSteps = 2;	 
			}
			else if(cistern[6].cisternStatus == Empty && cistern[5].cisternStatus == Ready)	  //3：6号槽空，并且5号就绪
			{
				A_Up_Down_Steps = 0;
				timeBetweenOpeartionDry = timeBetweenOpeartion;
				ManiDispatchSteps = 3;
			}
			else if(cistern[5].cisternStatus == Empty && cistern[4].cisternStatus == Ready)	//4：5号槽空，并且4号就绪
			{
				A_Up_Down_Steps = 0;
				timeBetweenOpeartionDry = 60;
				ManiDispatchSteps = 4;
			}
			else if(cistern[4].cisternStatus == Empty && cistern[3].cisternStatus == Ready)	  //5：4号槽空，并且3号就绪
			{
				A_Up_Down_Steps = 0;
				timeBetweenOpeartionDry = timeBetweenOpeartion;
				ManiDispatchSteps = 5;
			}
			else if(cistern[3].cisternStatus == Empty)	  //3号槽空，并且1号或者2号就绪
			{
				if(cistern[2].cisternStatus == Ready && cistern[1].cisternStatus == InCounting)	//二号槽就绪
				{
					A_Up_Down_Steps = 0;
					timeBetweenOpeartionDry = 60;
					ManiDispatchSteps = 6;
				}
				else if(cistern[1].cisternStatus == Ready && cistern[2].cisternStatus == InCounting) //一号槽就绪
				{
					A_Up_Down_Steps = 0;
					timeBetweenOpeartionDry = 60;
					ManiDispatchSteps = 7;
				}
				else if(cistern[1].cisternStatus == Ready && cistern[2].cisternStatus == Ready) //一号二号都就绪
				{
					if(cistern[1].currentTime > cistern[2].currentTime)
					{
						A_Up_Down_Steps = 0;
						timeBetweenOpeartionDry = 60;
						ManiDispatchSteps = 7;	
					}
					else
					{
						A_Up_Down_Steps = 0;
						timeBetweenOpeartionDry = 60;
						ManiDispatchSteps = 6;	
					}
				}
			}
		break;
		default:
			 _nop_();
	}
}

void ManualDispatch(void)
{
	ManiDispatchSteps = 20;
	ManualMode_Operation();
}