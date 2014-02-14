/***************************************************************************/
// 程序：清洗设备控制器
// 模块：液晶驱动头文件
// 文件：ra8806.h
// 作者：卜晓旸
// 版本：V0.02
// 日期：2013年11月8日
// 功能：液晶驱动头文件
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
/***************************************************************************/

#ifndef __RA8806_H__
#define __RA8806_H__

#include <basefunc.h>


/***************************************************************************/
// 缓存器总表
/***************************************************************************/
#define WLCR	0x00
#define MISC	0x01
#define ADSR	0x03
#define INTR	0x0F
#define WCCR	0x10
#define CHWI    0x11						//new(have change)
#define MAMR	0x12
#define AWRR	0x20
#define DWWR	0x21
#define AWBR	0x30
#define DWHR	0x31						//new(have change)
#define AWLR	0x40
#define AWTR	0x50
#define CURX	0x60						//new(have change)
#define BGSG	0x61
#define EDSG    0x62						//new
#define CURY	0x70						//new(have change)	
#define BGCM	0x71
#define EDCM	0x72
#define	BTMR	0x80
#define ITCR	0x90						//new(have change)
#define KSCR1	0xA0
#define KSCR2	0xA1
#define KSDR0  	0xA2						//new(have change)
#define KSDR1  	0xA3
#define KSDR2  	0xA4
#define MWCR	0xB0						//new(have change)
#define MRCR	0xB1						//new
#define TPCR1	0xC0
#define TPXR	0xC1
#define TPYR	0xC2
#define TPZR	0xC3
#define TPCR2   0xC4
#define PCR     0xD0						//new(have change)
#define PDCR    0xD1						//new
#define PNTR	0xE0
#define FNCR	0xF0
#define FVHT	0xF1

/***************************************************************************/
// 触摸屏参数
/***************************************************************************/
#define x_tps                   -844        //Xmin-Xmax
#define y_tps                   -788        //Ymin-Ymax
#define x_tps0                  936         //Xmax
#define y_tps0                  896         //Ymax

/***************************************************************************/
// 函数定义
/***************************************************************************/
extern void lcd_inital();
extern void clear(void);
extern void Enable_TP(void);
extern void Enable_Maunal_TP(void);
extern uchar tp_ManualMode();
extern void SettingDisplay(uchar settingStatus);
extern void MainDisplay(uchar status);
extern void TP_Response(uchar tpValue);
extern void PowerOnDisplay();
extern void ManipulatorStatusDisplay(void);

#endif

