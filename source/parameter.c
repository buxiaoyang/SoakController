/***************************************************************************/
// 程序：清洗设备控制器
// 模块：参数
// 文件：parameter.c
// 作者：卜晓旸
// 版本：V0.02
// 日期：2013年11月8日
// 功能：参数
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
/***************************************************************************/

#include <parameter.h>
#include <eeprom.h>

/***************************************************************************/
// 参数定义	
/***************************************************************************/
enum RunMode runMode;
enum DisplayMode displayMode; 
enum DrivingDirect drivingDirect; 

struct Cistern cistern[8];
struct Manipulator manipulator;

unsigned char displayFlag; //刷新显示标志位  0不刷新 1刷新主页面 2刷新机械臂动作刷新  3复位液晶并初始化显示
unsigned char speedUpFlag; //加速标志位

/***************************************************************************/
// 读取参数
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_read()
{
	WORD i, result = 1;
	Delay(10); 
	if(IapReadByte(IAP_ADDRESS+256) == 0xEE)
	{
		for(i=0; i<8; i++)
		{
			cistern[i].settingTime = ((IapReadByte(IAP_ADDRESS+(i*2)) << 8) | IapReadByte(IAP_ADDRESS+(i*2+1)));
		}
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

/***************************************************************************/
// 参数初始化函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void parameter_init()
{
	unsigned char i;
	runMode = AutoMode;
	displayMode = MainDisplayMode;
	for(i=0; i<8; i++)
	{
		cistern[i].cisternStatus = InCounting;
		cistern[i].currentTime = 0;
	}
	cistern[1].cisternStatus = Empty; //初始状态一号槽空
	if(!parameter_read())
	{
		cistern[0].settingTime = 1800;
		cistern[1].settingTime = 720;
		cistern[2].settingTime = 720;
		cistern[3].settingTime = 300;
		cistern[4].settingTime = 300;
		cistern[5].settingTime = 300;
		cistern[6].settingTime = 300;
		cistern[7].settingTime = 1800;
	}
	manipulator.manipulaterStatus = Stop;
	manipulator.manipulatorPosition = Bottom;
	manipulator.currentPosition = 0;
	drivingDirect = DStop;
	displayFlag = 0;
	speedUpFlag = 0;
}

/***************************************************************************/
// 参数保存到eeprom
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_save()
{
	WORD i, result = 1;
	Delay(10);
    IapEraseSector(IAP_ADDRESS); //拆除EEPROM
    for (i=0; i<512; i++)           //检测是否擦除成功
    {
        if (IapReadByte(IAP_ADDRESS+i) != 0xff)
            result = 0;
    }
    Delay(10);                      //Delay
	for(i=0; i<8; i++) //写入EEPROM
	{
		IapProgramByte(IAP_ADDRESS+(i*2), (BYTE)(cistern[i].perSettingTime>>8));
		IapProgramByte(IAP_ADDRESS+(i*2+1), (BYTE)cistern[i].perSettingTime);
	}
	IapProgramByte(IAP_ADDRESS+256, 0xEE); //写入标志位
    Delay(10);
	for(i=0; i<8; i++)  //校验保存值
	{
		if(IapReadByte(IAP_ADDRESS+(i*2)) != (BYTE)(cistern[i].perSettingTime>>8))
		{
			result = 0;
		}
		if(IapReadByte(IAP_ADDRESS+(i*2+1)) != (BYTE)cistern[i].perSettingTime)
		{
			result = 0;
		}
	}
	for(i=0; i<8; i++) //设置值为界面值
	{
		cistern[i].settingTime = cistern[i].perSettingTime;
	}
	return result;
}


