/***************************************************************************/
// 程序：清洗设备控制器
// 模块：主函数	
// 文件：main.c
// 作者：卜晓旸
// 版本：V0.02
// 日期：2013年11月8日
// 功能：主函数
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
/***************************************************************************/

#include <reg52.h>
//#include <STC12C5A60S2.H>
#include <basefunc.h>
#include <ra8806.h>
#include <parameter.h>
#include <timer.h>

/***************************************************************************/
// 参数定义	
/***************************************************************************/
//sbit OUT_PUT = P3^7; //用于测试输出
uchar currentTP=0; //用于触摸屏消抖动 

/***************************************************************************/
// 显示刷新函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void switchDisplay()
{
	switch(displayMode)
	{
		case MainDisplayMode:
			MainDisplay(0);
		break;
		case Setting:
			SettingDisplay(0);
		break;
		case SettingSaved:
			SettingDisplay(1);
		break;
		case PowerOffing:
			MainDisplay(1);
		break;
		case PowerOffed:
			MainDisplay(2);
		break;
		default:
		break;
	}	
}

/***************************************************************************/
// 设置页面初始化函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void initSettingPage()
{
	uchar i;
	for(i=0; i<8; i++)
	{
		cistern[i].perSettingTime = cistern[i].settingTime;
		cistern[i].isCurrentlSetting = 0;
	}
	cistern[1].isCurrentlSetting = 1;
}

/***************************************************************************/
// 设置页面槽选中函数
// 参数：选中当前槽数
// 返回值：无	
/***************************************************************************/
void touchPadSetChooseCistern(uchar cisternNum)
{
	uchar i;
	for(i=0; i<8; i++)
	{
		cistern[i].isCurrentlSetting = 0;
	}
	cistern[cisternNum].isCurrentlSetting = 1;
}

/***************************************************************************/
// 设置加减时间函数
// 参数：0加一分钟  1减一分钟
// 返回值：无	
/***************************************************************************/
void settingOperationTime(uchar opeartion)
{
	uchar i, cisternNum;
	for(i=0; i<8; i++)
	{
		if(cistern[i].isCurrentlSetting == 1)
		{
			cisternNum = i;
		}
	}
	if(opeartion) //减一分钟
	{
		
		cistern[cisternNum].perSettingTime = (cistern[cisternNum].perSettingTime/60 -1)*60;
		if(cistern[cisternNum].perSettingTime > 1800 || cistern[cisternNum].perSettingTime <= 0)
		{
			cistern[cisternNum].perSettingTime = 1800;
		}		
	}
	else
	{
		cistern[cisternNum].perSettingTime = (cistern[cisternNum].perSettingTime/60 +1)*60;
		if(cistern[cisternNum].perSettingTime > 1800)
		{
			cistern[cisternNum].perSettingTime = 60;
		}
	}
}

/***************************************************************************/
// 触摸屏响应函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void checkTouchPad(uchar tpValue)
{
	switch(tpValue)
	{
		case 0:
			currentTP = 0;
		break;
		case 1:	 //设置界面
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				displayMode = Setting;
				initSettingPage(); //初始化设置界面
				SettingDisplay(0);
			}
		break;
		case 2:	 //手自动
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				if(runMode == ManualMode)
					runMode = AutoMode;
				else
					runMode = ManualMode;
				MainDisplay(0);
			}
		break;
		case 3:	 //下班按键
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				displayMode = PowerOffing;
				MainDisplay(1);
			}
		break;
		case 4:	 //取消下班
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				displayMode = MainDisplayMode;
				MainDisplay(0);
			}
		break;
		case 5:	 //一号槽选中 
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				touchPadSetChooseCistern(1);
				SettingDisplay(0);
			}
		break;
		case 6:	 //二号槽选中 
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				touchPadSetChooseCistern(2);
				SettingDisplay(0);
			}
		break;
		case 7:	 //三号槽选中 
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				touchPadSetChooseCistern(3);
				SettingDisplay(0);
			}
		break;
		case 8:	 //四号槽选中 
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				touchPadSetChooseCistern(4);
				SettingDisplay(0);
			}
		break;
		case 9:	 //五号槽选中 
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				touchPadSetChooseCistern(5);
				SettingDisplay(0);
			}
		break;
		case 10:	 //六号槽选中 
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				touchPadSetChooseCistern(6);
				SettingDisplay(0);
			}
		break;
		case 11:	 //加一分钟
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(70);
				settingOperationTime(0);
				SettingDisplay(0);
			}
		break;
		case 12:	 //减一分钟
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(70);
				settingOperationTime(1);
				SettingDisplay(0);
			}
		break;
		case 13:	 //保存
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				//Save setting to eeprom
				SettingDisplay(3);
				if(parameter_save())
					SettingDisplay(1);
				else
					SettingDisplay(2);
				delay_ms(1000);
				//SettingDisplay(0);
				displayMode = MainDisplayMode;
				MainDisplay(0);
			}
		break;
		case 14:	 //返回
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				displayMode = MainDisplayMode;
				MainDisplay(0);
			}
		break;
		case 15:	 //返回
			if(currentTP == 2) 
			{
				TP_Response(tpValue);
				delay_ms(100);
				displayMode = MainDisplayMode;
				MainDisplay(0);
			}
		break;
		default:
			currentTP = 0;
		break;
	}
	//触摸屏按键消抖
	if(tpValue > 0 && tpValue < 16)
	{
		currentTP++;	
		delay_ms(10);
	}
	else
	{
		currentTP = 0;
	}
}

/***************************************************************************/
// 主函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void main()
{
	lcd_inital();
	PowerOnDisplay();
	delay_ms(2000);
	parameter_init();
	timer_init();
	Enable_TP();
	Enable_Maunal_TP();
	MainDisplay(0);
	while(1)
	{
		
//		OUT_PUT = ~OUT_PUT;
		checkTouchPad(tp_ManualMode());
		//delay_ms(40);
		if(displayFlag == 1) //整屏刷新
		{
			switchDisplay();
			displayFlag = 0;
		}
		else if(displayFlag == 2) //刷新机械臂动作
		{
			if(displayMode != Setting && displayMode != SettingSaved)
			{
				ManipulatorStatusDisplay();
				displayFlag = 0;
				delay_ms(10);
			}			
		}
	}   
}








