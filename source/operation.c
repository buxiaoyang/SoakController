#include <reg52.h>
#include <position.h>
#include <parameter.h>
#include <basefunc.h>
#include <operation.h>
#include <intrins.h>

sbit MotorLeft = P2^0; //机械臂向进口移动电机
sbit MotorRight = P2^1; //机械臂向出口移动电机
sbit ManiUp =  P1^0; //机械臂向上移动电机
sbit ManiDown =  P1^1; //机械臂向下移动电机
sbit MotorSpeedUp =  P1^2; //出口到进口空槽机械臂加速

sbit sensorInput = P3^0; //进口位置感应
sbit sensorManiTop = P3^1; //机械臂在顶端位置感应
sbit sensorManiBut = P3^2; //机械臂在低端位置感应
sbit sensorOutput = P3^3; //出位置感应

/*
void A_Move(uchar move_cisternNumber); 		//机械臂A移动到cisternNumber号槽位置
uchar A_Move_Steps = 100; 				//0：机械臂初始状态
										//1：机械臂A定位前等待
										//2：机械臂A定位
										//100：机械臂空操作		
			
							
void A_Down(uchar down_cisternNumber); 		//机械臂A放产品框到cisternNumber号槽
uchar A_Down_Steps = 100; 				//0：机械臂初始状态
										//1：机械臂A定位前等待
										//2：机械臂A定位
										//3：机械臂A向下前等待
										//4：机械臂A向下
										//100：机械臂空操作
										
void A_Up(uchar up_cisternNumber); 		//机械臂A提起cisternNumber号槽里面的产品框
uchar A_Up_Steps = 100; 				//0：机械臂初始状态
										//1：机械臂A定位前等待
										//2：机械臂A定位
										//3：机械臂A向上前等待
										//4：机械臂A向上
										//100：机械臂空操作
*/
void A_Up_Down(uchar upCisternNumber, uchar downCisternNumber); //机械臂提起upCisternNumber槽里面框放到downCisternNumber槽
uchar A_Up_Down_Steps = 100;		   	//0：机械臂初始状态
										//1：机械臂提起upCisternNumber槽中的框
										//2：机械臂放框到downCisternNumber槽中
										//100：机械臂空操作

unsigned int timeBetweenOpeartionDry = timeBetweenOpeartion;		//机械臂调起后悬挂晾干时间


void A_Move_Entrance(void); 		//机械臂A移动到进口槽位置
uchar A_Move_Entrance_Steps = 100; 		//0：机械臂初始状态
										//1：机械臂A定位前等待
										//2：机械臂A定位
										//3：机械臂定位后等待
										//100：机械臂空操作	
										
void ManualMode_Operation(void);  //手动模式																				
										
uint ManiOperationTimer = 0; //机械臂操作定时器

void A_Up_Down(uchar upCisternNumber, uchar downCisternNumber)
{
	switch(A_Up_Down_Steps)
	{
		case 0: 	//0：机械臂初始状态
			ManiOperationTimer = 0;
			A_Up_Down_Steps ++;
		break;
		case 1: 	//1：机械臂A定位前等待
			if(ManiOperationTimer > timeBetweenOpeartion) //定时时间到
			{
				if(manipulator.currentPosition > upCisternNumber) //当前机械臂在cisternNumber号槽右边
				{
					MotorRight = 1;
					if(speedUpFlag == 1)//开启加速模式
					{
						MotorLeft = 1; //行车向左
						MotorSpeedUp = 0;	
					}
					else
					{
						MotorLeft = 0; //行车向左
						MotorSpeedUp = 1;
					}
					drivingDirect = DGoEntrance;
					manipulator.manipulaterStatus = GoEntrance;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
				}
				else if(manipulator.currentPosition < upCisternNumber)
				{
					MotorLeft = 1;		
					if(speedUpFlag == 1)
					{
						MotorSpeedUp = 0;
						MotorRight = 1;	
					}
					else
					{
						MotorSpeedUp = 1;
						MotorRight = 0; //行车向右
					}
					drivingDirect = GoOutfall;
					manipulator.manipulaterStatus = DGoOutfall;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
				}
				else
				{
					MotorSpeedUp = 1;
					A_Up_Down_Steps++;	
				}	
			}
		break;
		case 2: 	//2：机械臂A定位
			if(manipulator.currentPosition == upCisternNumber)
			{
				ManiOperationTimer = 0;
				MotorLeft = 1;
				MotorRight = 1;
				//drivingDirect = DStop;
				manipulator.manipulaterStatus = Stop;
				if(displayFlag == 0)
				{
					displayFlag = 2;
				}
				A_Up_Down_Steps ++;
			}
		break;
		case 3: 	//1：机械臂A定位前等待
			if(ManiOperationTimer > timeBetweenOpeartion) //定时时间到
			{
				if(manipulator.currentPosition > upCisternNumber) //当前机械臂在cisternNumber号槽右边
				{					
					MotorRight = 1;
					if(speedUpFlag == 1)
					{
						MotorLeft = 1; //行车向左
						MotorSpeedUp = 0;	
					}
					else
					{
						MotorLeft = 0; //行车向左
						MotorSpeedUp = 1;
					}
					drivingDirect = DGoEntrance;
					manipulator.manipulaterStatus = GoEntrance;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
					A_Up_Down_Steps = 2;
				}
				else if(manipulator.currentPosition < upCisternNumber)
				{
					MotorLeft = 1;
					if(speedUpFlag == 1)
					{
						MotorSpeedUp = 0;
						MotorRight = 1; //行车向右	
					}
					else
					{
						MotorSpeedUp = 1;
						MotorRight = 0; //行车向右
					}
					drivingDirect = GoOutfall;
					manipulator.manipulaterStatus = DGoOutfall;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
					A_Up_Down_Steps = 2;
				}
				else
				{
					MotorSpeedUp = 1;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
					A_Up_Down_Steps ++;	
				}	
			}
		break;
		case 4:
			ManiUp = 0;
			manipulator.manipulaterStatus = GoUp;
			if(displayFlag == 0)
			{
				displayFlag = 2;
			}
			A_Up_Down_Steps++;
		break;
		case 5:		//4：机械臂A向上 
			if(sensorManiTop == 0) //机械臂A顶端感应器感应到
			{
				ManiUp = 1;
				manipulator.manipulatorPosition = Top;
				manipulator.manipulaterStatus = Stop;	
				cistern[upCisternNumber/2].cisternStatus = Empty;//标记当前槽为满
				cistern[upCisternNumber/2].currentTime = 0;//清除当前槽计时时间
				displayFlag = 1;
				A_Up_Down_Steps ++;
			}
		break;
		case 6: 	//0：机械臂初始状态
			ManiOperationTimer = 0;
			A_Up_Down_Steps ++;
		break;
		case 7: 	//1：机械臂A定位前等待
			if(ManiOperationTimer > timeBetweenOpeartionDry) //定时时间到
			{
				if(manipulator.currentPosition > downCisternNumber) //当前机械臂在cisternNumber号槽右边
				{
					MotorRight = 1;
					if(speedUpFlag == 1)
					{
						MotorSpeedUp = 0;
						MotorLeft = 1; //行车向左	
					}
					else
					{
						MotorSpeedUp = 1;
						MotorLeft = 0; //行车向左
					}
					drivingDirect = DGoEntrance;
					manipulator.manipulaterStatus = GoEntrance;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
				}
				else if(manipulator.currentPosition < downCisternNumber)
				{
					MotorLeft = 1;
					if(speedUpFlag == 1)
					{
						MotorSpeedUp = 0;
						MotorRight = 1; //行车向右	
					}
					else
					{
						MotorSpeedUp = 1;
						MotorRight = 0; //行车向右
					}
					drivingDirect = GoOutfall;
					manipulator.manipulaterStatus = DGoOutfall;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
				}
				else
				{
					MotorSpeedUp = 1;
					A_Up_Down_Steps++;	
				}	
			}
		break;
		case 8: 	//2：机械臂A定位
			if(manipulator.currentPosition == downCisternNumber)
			{
				ManiOperationTimer = 0;
				MotorLeft = 1;
				MotorRight = 1;
				//drivingDirect = DStop;
				manipulator.manipulaterStatus = Stop;
				if(displayFlag == 0)
				{
					displayFlag = 2;
				}
				A_Up_Down_Steps ++;
			}
		break;
		case 9: 	//1：机械臂A定位前等待
			if(ManiOperationTimer > timeBetweenOpeartion) //定时时间到
			{
				if(manipulator.currentPosition > downCisternNumber) //当前机械臂在cisternNumber号槽右边
				{
					MotorRight = 1;
					if(speedUpFlag == 1)
					{
						MotorSpeedUp = 0;
						MotorLeft = 1; //行车向左	
					}
					else
					{
						MotorSpeedUp = 1;
						MotorLeft = 0; //行车向左
					}
					drivingDirect = DGoEntrance;
					manipulator.manipulaterStatus = GoEntrance;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
					A_Up_Down_Steps = 8;
				}
				else if(manipulator.currentPosition < downCisternNumber)
				{
					MotorLeft = 1;
					if(speedUpFlag == 1)
					{
						MotorSpeedUp = 0;
						MotorRight = 1; //行车向右	
					}
					else
					{
						MotorSpeedUp = 1;
						MotorRight = 0; //行车向右
					}
					drivingDirect = GoOutfall;
					manipulator.manipulaterStatus = DGoOutfall;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
					A_Up_Down_Steps = 8;
				}
				else
				{
					MotorSpeedUp = 1;
					if(displayFlag == 0)
					{
						displayFlag = 2;
					}
					A_Up_Down_Steps ++;	
				}	
			}
		break;
		case 10:
			ManiDown = 0;
			manipulator.manipulaterStatus = GoDown;
			if(displayFlag == 0)
			{
				displayFlag = 2;
			}
			A_Up_Down_Steps++;
		break;
		case 11:		//4：机械臂A向下 
			if(sensorManiBut == 0) //机械臂A底端感应器感应到
			{
				ManiDown = 1;
				manipulator.manipulatorPosition = Bottom;
				manipulator.manipulaterStatus = Stop;	
				cistern[downCisternNumber/2].cisternStatus = InCounting;//标记当前槽为满
				cistern[downCisternNumber/2].currentTime = 0;//清除当前槽计时时间
				displayFlag = 1;
				A_Up_Down_Steps = 100;
			}
		break;
		default:
			  _nop_();
	}	
}

void A_Move_Entrance(void)
{
	switch(A_Move_Entrance_Steps)
	{
		case 0: 	//0：机械臂初始状态
			ManiOperationTimer = 0;
			A_Move_Entrance_Steps ++;
		break;
		case 1: 	//1：机械臂A定位前等待
			if(ManiOperationTimer > timeBetweenOpeartion) //定时时间到
			{
				MotorLeft = 0; //行车向左
				drivingDirect = DGoEntrance;
				manipulator.manipulaterStatus = GoEntrance;
				if(displayFlag == 0)
				{
					displayFlag = 2;
				}
				A_Move_Entrance_Steps++;
			}
		break;
		case 2: 	//2：机械臂A定位
			if(sensorInput == 0)
			{
				ManiOperationTimer = 0;
				MotorLeft = 1;
				MotorRight = 1;
				drivingDirect = DStop;
				manipulator.manipulaterStatus = Stop;
				if(displayFlag == 0)
				{
					displayFlag = 2;
				}
				manipulator.currentPosition = 0;
				A_Move_Entrance_Steps ++;
			}
		break;
		case 3: 	//2：机械臂A定位
			if(ManiOperationTimer > timeBetweenOpeartion) //定时时间到
			{
				ManiOperationTimer = 0;
				manipulator.currentPosition = 0;
				A_Move_Entrance_Steps = 100;
			}
		break;
		default:
			  _nop_();
	}
}

void ManualMode_Operation(void)
{
 	MotorLeft = 1;
 	MotorRight = 1;
 	ManiUp =  1;
 	ManiDown =  1;
 	MotorSpeedUp =  1;	
}
