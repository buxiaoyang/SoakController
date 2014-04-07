/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC12C5Axx Series 16-bit Timer Demo -------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-755-82905966 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

#include <reg52.h>
#include <timer.h>
#include <parameter.h>
#include <position.h>
#include <operation.h>
#include <dispatch.h>
#include <key.h>

typedef unsigned char BYTE;
typedef unsigned int WORD;

//-----------------------------------------------

#define T1MS 53536      //1ms timer calculation method in 1T mode
#define T10MS 5536      //1ms timer calculation method in 1T mode

/* define SFR */
sfr AUXR = 0x8e;                    //Auxiliary register

//sbit TEST_LED = P2^0;               //work LED, flash once per second
//sbit TEST_LED1 = P2^1;               //work LED, flash once per second
/* define variables */
//-----------------------------------------------
BYTE count1s; // 1s时间计数
BYTE countRestDisplay; //液晶屏定时复位计数器
BYTE countManiPosition; //机械表位置检测有效时间

/***************************************************************************/
// 槽计时函数 每秒调用一次
// 参数：无
// 返回值：无	
/***************************************************************************/
void cisternCount()
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		if(cistern[i].cisternStatus == InCounting)
		{
			cistern[i].currentTime ++;
			//cistern[i].currentTime += 60;
			if(cistern[i].currentTime >= cistern[i].settingTime)
			{
				if(i != 0 && i != 7)// 进口出口不安时间就绪
				{
					cistern[i].cisternStatus = Ready;
				}
				else
				{
					cistern[i].currentTime = 0;
				}
			}
			if(cistern[i].currentTime % 60 == 0)
			{
				displayFlag = 1;
			}
		}
	}
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1  //1ms
{
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
//	TEST_LED = ! TEST_LED;
	countManiPosition ++;
	if(countManiPosition > 2)
	{
		maniPosition();
		countManiPosition = 0;
	}
}

/* Timer1 interrupt routine */
void tm1_isr() interrupt 3 using 1  //5ms
{
    TL1 = T10MS;                     //reload timer1 low byte
    TH1 = T10MS >> 8;                //reload timer1 high byte
	count1s ++;
//	TEST_LED1 = ~TEST_LED1;
	if(displayMode != PowerOffed)
	{
		if(count1s == 200) //1s
		{
			count1s = 0;
			cisternCount();
//			TEST_LED1 = ! TEST_LED1;      //work LED flash
			ManiOperationTimer ++; //机械臂操作定时器
			//液晶定时复位操作
			countRestDisplay ++;
			if(countRestDisplay == 5)
			{
				displayFlag = 3; //初始化液晶并刷新显示
				countRestDisplay = 0;	
			}
		}
		if(count1s%2==0) //10ms
		{
			if(runMode == AutoMode)
			{
				ManiDispatch();
			}
			else
			{
				ManualDispatch();
			}
			Key_Scan();
		}
	}
}

//-----------------------------------------------

/* timer_init program */
void timer_init()
{
    AUXR = 0x80|0x40;                    //timer0 work in 1T mode
    TMOD = 0x01|0x10;                    //set timer0 as mode1 (16-bit)
    TL0 = T1MS;                     //initial timer0 low byte
    TH0 = T1MS >> 8;                //initial timer0 high byte
    TR0 = 1;                        //timer0 start running
    ET0 = 1;                        //enable timer0 interrupt

	TL1 = T10MS;                     //initial timer1 low byte
    TH1 = T10MS >> 8;                //initial timer1 high byte
    TR1 = 1;                        //timer1 start running
    ET1 = 1;                        //enable timer1 interrupt

    EA = 1;                         //open global interrupt switch

	count1s = 0;
	countManiPosition = 0;
	countRestDisplay = 0;
}


