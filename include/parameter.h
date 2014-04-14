/***************************************************************************/
// 程序：清洗设备控制器
// 模块：参数头文件
// 文件：parameter.h
// 作者：卜晓旸
// 版本：V0.02
// 日期：2013年11月8日
// 功能：参数头文件
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
/***************************************************************************/

#ifndef __PARAMETER_H__
#define __PARAMETER_H__

/***************************************************************************/
// 参数定义
/***************************************************************************/
enum RunMode {AutoMode, ManualMode}; //运行模式 0自动模式 1手动模式

enum DisplayMode {MainDisplayMode, Setting, SettingSaved, PowerOffing, PowerOffed, PowerOnChecking}; //显示模式 0主界面 1设置 2设置成功  3下班键正在关机  4下班键关机完成  5开机归位模式

enum CisternStatus {Empty, InCounting, Ready}; //槽状态 0空闲 1正在计时 2就绪

enum ManipulatorStatus {Stop, GoEntrance, GoOutfall, GoUp, GoDown}; //机械臂状态  0停止 1往进口方向  2往出口方向  3往上  4往下

enum ManipulatorPosition {Top, Center,  Bottom}; //机械状态(竖直方向)  0上方 1中间位置  2下方

enum DrivingDirect {DStop, DGoEntrance, DGoOutfall}; //行车运动方向 	0停止 1往进口方向  2往出口方向 

struct Cistern  //槽结构体
{
	enum CisternStatus cisternStatus;  //槽状态
	unsigned int settingTime;   //槽浸泡设置时间
	unsigned int currentTime;	//槽当前浸泡时间
	unsigned int perSettingTime;	//浸泡设置时间（设置页面未保存值）
	unsigned char isCurrentlSetting; //当前是否为设置状态 0：非设置状态  1：设置状态
};

struct Manipulator //机械臂结构体
{
	enum ManipulatorStatus manipulaterStatus; //机械臂状态
	enum ManipulatorPosition manipulatorPosition;
	unsigned char currentPosition; 	//当前位置（x槽） 
									//0：进口 
									//1：进口一号槽之间
									//2：一号槽
									//3：一号槽二号槽之间
									//4：二号槽
									//5：二号槽三号槽之间
									//6：三号槽
									//7：三号槽四号槽之间
									//8：四号槽
									//9：四号槽五号槽之间
									//10：五号槽
									//11：五号槽六号槽之间
									//12：六号槽
									//13：六号槽出口之间
									//14：出口
};

/***************************************************************************/
// 参数声明
/***************************************************************************/
extern void parameter_init();
extern unsigned char parameter_save();

extern enum RunMode runMode;
extern enum DisplayMode displayMode;
extern enum DrivingDirect drivingDirect; 
extern struct Cistern cistern[8];
extern struct Manipulator manipulator; 

extern unsigned char displayFlag;  //刷新显示标志位  0不刷新 1刷新主页面 2刷新机械臂动作刷新  3复位液晶并初始化显示
extern unsigned char MotorSpeedFlag;  //加速标志位	 0正常速度 1电机加速 2电机减速
extern unsigned char TargetCisternNumber; //机械臂运行目标槽位

#endif
