/***************************************************************************/
// 程序：清洗设备控制器
// 模块：液晶驱动
// 文件：ra8806.c
// 作者：卜晓旸
// 版本：V0.02
// 日期：2013年11月8日
// 功能：液晶驱动
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
/***************************************************************************/

#include <reg52.h>
#include <basefunc.h>
#include <fonts.h>
#include <ra8806.h> 
#include <parameter.h>

/***************************************************************************/
// 液晶引脚定义	
/***************************************************************************/
#define lcd_data P0
sbit REST	= P2^7;   // REST就是接口中的RST
sbit BUSY	= P2^6;   //BUSY就是接口中的BUSY
sbit CS		= P2^5;   // CS就是接口中的CS1
sbit RD0  	= P2^4;  // RD0就是接口中的RD
sbit WR0  	= P2^3;  // WR0就是接口中的WR
sbit CD		= P2^2;   // CD 就是接口中的RS






/***************************************************************************/
// 液晶写入命令函数
// 参数：命令
// 返回值：无	
/***************************************************************************/
void LCD_CmdWrite(uchar cmd)
{
	CS=0;
	RD0 = 1;
	CD = 1;		
	WR0 = 0;
	lcd_data = cmd;
	//delay_us(2); 
	WR0 = 0;
	WR0 = 1;
	CS = 1;
}

/***************************************************************************/
// 液晶写入数据函数
// 参数：数据
// 返回值：无	
/***************************************************************************/
void LCD_DataWrite(uchar wdata)
{
	while(!BUSY);
	CS=0;
	RD0 = 1;
	CD = 0;		
	lcd_data = wdata; 
	WR0 = 0;
	//delay_us(10); 
	WR0 = 1;
	CS = 1;
}

/***************************************************************************/
// 液晶读取命令函数
// 参数：地址
// 返回值：命令
/***************************************************************************/
uchar LCD_CmdRead(uchar Addr)
{  
	uchar Data;  
	CS=0;
	CD=1;
	RD0=1;
	lcd_data=Addr;
	WR0=0;
	WR0=1;
	lcd_data=0XFF;
	CD=0;
	WR0=1;
	RD0=1;
	RD0=0;
	Data=lcd_data;
	return Data;
}

/***************************************************************************/
// 液晶读取数据函数
// 参数：无
// 返回值：数据
/***************************************************************************/
uchar LCD_DataRead()
{
	uchar Data;
	lcd_data = 0x00;
	WR0 = 1;
	CS = 0;
	CD  = 0;
	RD0 = 0;
	Data = lcd_data;
	RD0 = 1;	
	CS = 1;			
	//CD = 1;
	lcd_data = 0xff;
	return Data;
}

/***************************************************************************/
// 液晶初始化函数
// 参数：无
// 返回值：无
/***************************************************************************/
void lcd_inital()
{ 
	REST = 0;
	delay_ms(50);
	REST = 1;
	delay_ms(50);
	LCD_CmdWrite(WLCR);		//[00H] , Default --> 0x00
	LCD_DataWrite(0x04);
	
	LCD_CmdWrite(MISC);		//[01H] , Default --> 0x00
	LCD_DataWrite(0x08);
	
	LCD_CmdWrite(ADSR);		//[03H] , Default --> 0x00 
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(INTR);		//[0FH] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(WCCR);		//[10H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(CHWI);		//[11H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(MAMR);		//[12H] , Default --> 0x11 
	LCD_DataWrite(0x11);
	
	LCD_CmdWrite(AWRR);		//[20H] , Default --> 0x27
	LCD_DataWrite(39);
	
	LCD_CmdWrite(DWWR);		//[21H] , Default --> 0x27
	LCD_DataWrite(39);
	
	LCD_CmdWrite(AWBR);		//[30H] , Default --> 0xEF	
	LCD_DataWrite(239);
	
	LCD_CmdWrite(DWHR);		//[31H] , Default --> 0xEF
	LCD_DataWrite(239);
	
	LCD_CmdWrite(AWLR);		//[40H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(AWTR);		//[50H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(CURX);		//[60H] , Default --> 0x00 
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(BGSG);		//[61H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(EDSG);		//[62H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(CURY);		//[70H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(BGCM);		//[71H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(EDCM);		//[72H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(BTMR);		//[80H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(ITCR);		//[90H] , Default --> 0x00    需要调整
	LCD_DataWrite(0x80);
	
	LCD_CmdWrite(PNTR);		//[E0H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(FNCR);		//[F0H] , Default --> 0x00
	LCD_DataWrite(0x00);
	
	LCD_CmdWrite(FVHT);		//[F1H] , Default --> 0x00
	LCD_DataWrite(0x00);
}

/***************************************************************************/
// 液晶X,Y坐标定位
// 参数：x坐标  y坐标
// 返回值：无
/***************************************************************************/
void LCD_GotoXY(uchar xx,uchar yy)
{
	LCD_CmdWrite(CURX); 
	LCD_DataWrite(xx);
	LCD_CmdWrite(CURY);  
	LCD_DataWrite(yy);
}

/***************************************************************************/
// 液晶绘图模式
// 参数：无
// 返回值：无
/***************************************************************************/
void LCD_Graphic(void)
{
	uchar temp;
	temp=LCD_CmdRead(WLCR);
	temp &=0xf7;
	LCD_CmdWrite(WLCR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶文字模式
// 参数：无
// 返回值：无
/***************************************************************************/
void LCD_Text(void)
{
	uchar temp;
	temp=LCD_CmdRead(WLCR);
	temp |=0x08;
	LCD_CmdWrite(WLCR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶DDRAM1 显示
// 参数：无
// 返回值：无
/***************************************************************************/
void Only_Show_Page1(void)
{
	uchar temp;
	temp=LCD_CmdRead(MAMR);
	temp &=0x9f;
	temp |=0x10;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶DDRAM2 显示
// 参数：无
// 返回值：无
/***************************************************************************/
void Only_Show_Page2(void)
{
	uchar temp;
	temp=LCD_CmdRead(MAMR);
	temp &=0xaf;
	temp |=0x20;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶DDRAM1和DDRAM2 双图层显示
// 参数：无
// 返回值：无
/***************************************************************************/
void Show_Two_Layer(void)
{
	uchar temp;	
	temp=LCD_CmdRead(MAMR);
	temp &=0xbf;
	temp |=0x30;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶DDRAM1和DDRAM2 OR方式显示
// 参数：无
// 返回值：无
/***************************************************************************/
void OR_Two_Layer(void)
{
	uchar temp;	
	temp=LCD_CmdRead(MAMR);
	temp &=0xf3;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶DDRAM1和DDRAM2 XOR方式显示
// 参数：无
// 返回值：无
/***************************************************************************/
void XOR_Two_Layer(void)
{
	uchar temp;	
	temp=LCD_CmdRead(MAMR);
	temp &=0xf7;
	temp |=0x04;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶DDRAM1和DDRAM2 NOR方式显示
// 参数：无
// 返回值：无
/***************************************************************************/
void NOR_Two_Layer(void)
{
	uchar temp;	
	temp=LCD_CmdRead(MAMR);
	temp &=0xfb;
	temp |=0x08;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶DDRAM1和DDRAM2 AND方式显示
// 参数：无
// 返回值：无
/***************************************************************************/
void AND_Two_Layer(void)
{
	uchar temp;	
	temp=LCD_CmdRead(MAMR);
	temp |=0x0c;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶存取DDRAM1
// 参数：无
// 返回值：无
/***************************************************************************/
void Access_Page1(void)
{
	uchar temp;	
	temp=LCD_CmdRead(MAMR);
	temp &=0xfd;
	temp |=0x01;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 存取DDRAM2
// 参数：无
// 返回值：无
/***************************************************************************/
void Access_Page2(void)
{
	uchar temp;	
	temp=LCD_CmdRead(MAMR);
	temp &=0xfe;
	temp |=0x02;
	LCD_CmdWrite(MAMR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶清屏
// 参数：无
// 返回值：无
/***************************************************************************/
void clear(void)
{ 
	LCD_Graphic();     //选择绘图模式     
	LCD_CmdWrite(PNTR);
	LCD_DataWrite(0x00);
	LCD_CmdWrite(FNCR);
	LCD_DataWrite(0xa8);
	//delay_ms(3);
}


/***************************************************************************/
// 液晶满屏显示
// 参数：无
// 返回值：无
/***************************************************************************/
void display_all()
{
	LCD_Graphic();
	LCD_CmdWrite(PNTR);
	LCD_DataWrite(0xff);     //全部填0xff
	LCD_CmdWrite(FNCR);
	LCD_DataWrite(0xa8);
	//delay_ms(3);
}

/***************************************************************************/
// 液晶全屏正常显示
// 参数：无
// 返回值：无
/***************************************************************************/
void normal_data(void)
{
	uchar temp;
	temp=LCD_CmdRead(WLCR);  //读WLCR寄存器数据
	temp &=0xfe;
	LCD_CmdWrite(WLCR);
	LCD_DataWrite(temp);     
}

/***************************************************************************/
// 液晶全屏反白显示
// 参数：无
// 返回值：无
/***************************************************************************/
void reverse_data(void)
{
	uchar temp;
	temp=LCD_CmdRead(WLCR);  //读WLCR寄存器数据
	temp |=0x01;
	LCD_CmdWrite(WLCR);
	LCD_DataWrite(temp);     
}

/***************************************************************************/
// 液晶数据反白输入
// 参数：无
// 返回值：无
/***************************************************************************/
void LCD_Inv(void)
{
	uchar temp;	
	temp=LCD_CmdRead(0x10);
	temp |=0x20;
	LCD_CmdWrite(0x10);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶数据正常输入
// 参数：无
// 返回值：无
/***************************************************************************/
void LCD_NoInv(void)
{
	uchar temp;	
	temp=LCD_CmdRead(WCCR);
	temp &=0xdf;
	LCD_CmdWrite(WCCR);
	LCD_DataWrite(temp); 
}

/***************************************************************************/
// 液晶字符放大i倍
// 参数：放大倍数1~4
// 返回值：无
/***************************************************************************/
void enlarge(uchar i)
{  
	switch(i)
	{
		case 0x01:
			LCD_CmdWrite(FVHT);
			LCD_DataWrite(0x00);  
		break;
		case 0x02:
			LCD_CmdWrite(FVHT);
			LCD_DataWrite(0x50);  
		break;
		case 0x03:
			LCD_CmdWrite(FVHT);
			LCD_DataWrite(0xa0); 
		break;
		case 0x04:
			LCD_CmdWrite(FVHT);
			LCD_DataWrite(0xf0); 
		break;
	}
}

/***************************************************************************/
// 液晶画一个表格
// 参数：无
// 返回值：无
/***************************************************************************/
void display_biao(void)
{
	uint n0,n1;
	LCD_Graphic();   //选择绘图模式
	LCD_GotoXY(0x00,0x00);   //设定X,Y坐标位置
	LCD_CmdWrite(MWCR);
	for(n0=0;n0<40;n0++)
	{
		LCD_DataWrite(0xff);
	}
	for(n0=0;n0<31;n0++)		
	{
		for(n1=0;n1<40;n1++)
		{
			if (n1==0) LCD_DataWrite(0x80);
			else if (n1==39) LCD_DataWrite(0x01);
			else LCD_DataWrite(0x00);
		}
	}
	for(n0=0;n0<40;n0++)
	{
		LCD_DataWrite(0xff);
	}
	for(n0=0;n0<(125);n0++)		
	{
		for(n1=0;n1<40;n1++)
		{
			if (n1==0)          LCD_DataWrite(0x80);
			else if (n1==39) LCD_DataWrite(0x01);
			else                LCD_DataWrite(0x00);
		}
	}
	for(n0=0;n0<40;n0++)
	{
		LCD_DataWrite(0xff);
	}
}

/***************************************************************************/
// 液晶在指定的坐标处显示字符串
// 参数：x坐标  y坐标  字符串内容
// 返回值：无
/***************************************************************************/
void LCD_ShowStr(uchar xxx,uchar yyy,uchar *text)
{ 
	LCD_Text();   //选择文本模式
	LCD_GotoXY(xxx,yyy);   //设定X,Y坐标位置 
	LCD_CmdWrite(MWCR);
	while(*text != '\0')          
	{
		LCD_DataWrite(*text++);
	}
}

/***************************************************************************/
// 液晶在指定的坐标处显示字符
// 参数：x坐标  y坐标  字符
// 返回值：无
/***************************************************************************/
void LCD_ShowChar(uchar xxx,uchar yyy,uchar text)
{ 
	LCD_Text();   //选择文本模式
	LCD_GotoXY(xxx,yyy);   //设定X,Y坐标位置 
	LCD_CmdWrite(MWCR);
	LCD_DataWrite(text);
}

/***************************************************************************/
// 液晶显示中文字库和字符
// 参数：
// 返回值：无
/***************************************************************************/
void display_word(const uchar *cha, int count )
{
	uint iiii=0;
	LCD_Text();  //选择文本模式
	LCD_GotoXY(0x00,0x00);   //设定X,Y坐标位置 
	LCD_CmdWrite(MWCR); 
	for(iiii=0;iiii<count;iiii++)
	{
		LCD_DataWrite(*cha++);
	}   
}

/***************************************************************************/
// 显示一行边框
// 参数：x起始坐标0~39  y起始坐标0~239  宽度  高度 颜色1正常2反白  
// 返回值：无
/***************************************************************************/
void display_kuang(uchar x_star,uchar y_star,uchar w,uchar h, uchar color)
{
	uchar width,heigh;      //i表示行,j表示列
	LCD_Graphic();   //选择绘图模式
	for(heigh=0;heigh<h;heigh++)
	{  
		if(heigh==0)
		{
			LCD_GotoXY(x_star,y_star+heigh);   //设定X,Y开始坐标位置  
			for(width=0;width<w;width++)
			{ 
				LCD_CmdWrite(MWCR); 
				LCD_DataWrite(0xff);
			}
		}
		else if((heigh>0)&&(heigh<(h-1)))
		{
			LCD_GotoXY(x_star,y_star+heigh);   //设定X,Y开始坐标位置    
			LCD_CmdWrite(MWCR);   
			if(color)
			{
				LCD_DataWrite(0x80);
				for(width=1;width<(w-1);width++)
				{
					LCD_DataWrite(0x00);
				}
				LCD_DataWrite(0x01);
			}
			else
			{
				LCD_DataWrite(0xff);
				for(width=1;width<(w-1);width++)
				{
					LCD_DataWrite(0xff);
				}
				LCD_DataWrite(0xff);				
			}
		}
		else if(heigh==(h-1))
		{
			LCD_GotoXY(x_star,y_star+heigh);   //设定X,Y开始坐标位置  
			for(width=0;width<w;width++)
			{      
				LCD_CmdWrite(MWCR);
				LCD_DataWrite(0xff);
			}
		}
	}
}       

/***************************************************************************/
// 显示图片
// 参数：x_star:0~39;y_star:0~239 w:0~39;h:0~239 
// 返回值：无
/***************************************************************************/
void drow_picture(uchar x, uchar y, uchar w, uchar h, uchar *pData)
{
	uchar i, j;
	LCD_Graphic(); 
	for(i=0;i<h;i++)
	{
		for(j=0; j<w; j++)
		{
			LCD_GotoXY(x+j,y+i); 
			LCD_CmdWrite(MWCR);   
			LCD_DataWrite(*pData++); 
		}
	}
}

/***************************************************************************/
// 绘点函数
// 参数：x坐标 y坐标
// 返回值：无
/***************************************************************************/
void drow_point(uint x, uint y)
{
	uchar temp, temp1= x/8;
	LCD_Graphic();
	LCD_GotoXY(temp1,y); 
	LCD_CmdWrite(MRCR);	//内存读出 
	temp = LCD_DataRead();
	LCD_GotoXY(temp1,y); 
	LCD_CmdWrite(MWCR);   
	LCD_DataWrite(temp | (1<<7-x%8)); 
}

/***************************************************************************/
// 绘横线函数
// 参数：x起始坐标 y起始坐标 x终止坐标
// 返回值：无
/***************************************************************************/
void drow_row_line(uint x0, uint y0, uint x1)
{
	uint temp;
	if(x0>x1)
	{
		temp = x1;
		x1 = x0;
		x0 = temp;
	}
	while(x1>=x0)
	{
		drow_point(x0, y0);
		x0++;
	}
}

/***************************************************************************/
// 绘竖线函数
// 参数：x起始坐标 y起始坐标 y终止坐标
// 返回值：无
/***************************************************************************/
void drow_column_line(uint x0, uint y0, uint y1)
{
	uint temp;
	if(y0>y1)
	{
		temp = y1;
		y1 = y0;
		y0 = temp;
	}
	do
	{
		drow_point(x0, y0);
		y0++;
	}
	while(y1>=y0);
}

/***************************************************************************/
// 绘制圆角矩形
// 参数：x起始坐标 y起始坐标 宽度 高度 颜色1正常2反白
// 返回值：无
/***************************************************************************/
void display_r_kuang(uchar x_star,uchar y_star,uchar w,uchar h,uchar color)
{
	if(color)
	{
		display_kuang(x_star,y_star,w,h,1);
		drow_picture(x_star, y_star, 1, 8, RoundedTLB);
		drow_picture(x_star,  y_star+h-8, 1, 8, RoundedBLB);
		drow_picture(x_star+w-1, y_star, 1, 8, RoundedTRB);
		drow_picture(x_star+w-1, y_star+h-8, 1, 8, RoundedBRB);
	}
	else
	{
		display_kuang(x_star,y_star,w,h,0);
		drow_picture(x_star, y_star, 1, 8, RoundedTLF);
		drow_picture(x_star,  y_star+h-8, 1, 8, RoundedBLF);
		drow_picture(x_star+w-1, y_star, 1, 8, RoundedTRF);
		drow_picture(x_star+w-1, y_star+h-8, 1, 8, RoundedBRF);		
	}
}

/***************************************************************************/
// 绘制左侧圆角矩形
// 参数：x起始坐标 y起始坐标 宽度 高度 颜色1正常2反白
// 返回值：无
/***************************************************************************/
void display_rl_kuang(uchar x_star,uchar y_star,uchar w,uchar h,uchar color)
{
	if(color)
	{
		display_kuang(x_star,y_star,w,h,1);
		drow_picture(x_star, y_star, 1, 8, RoundedTLB);
		drow_picture(x_star,  y_star+h-8, 1, 8, RoundedBLB);
	}
	else
	{
		display_kuang(x_star,y_star,w,h,0);
		drow_picture(x_star, y_star, 1, 8, RoundedTLF);
		drow_picture(x_star,  y_star+h-8, 1, 8, RoundedBLF);
	}
}

/***************************************************************************/
// 绘制右侧圆角矩形
// 参数：x起始坐标 y起始坐标 宽度 高度 颜色1正常2反白
// 返回值：无
/***************************************************************************/
void display_rr_kuang(uchar x_star,uchar y_star,uchar w,uchar h,uchar color)
{
	if(color)
	{
		display_kuang(x_star,y_star,w,h,1);
		drow_picture(x_star+w-1, y_star, 1, 8, RoundedTRB);
		drow_picture(x_star+w-1, y_star+h-8, 1, 8, RoundedBRB);
	}
	else
	{
		display_kuang(x_star,y_star,w,h,0);
		drow_picture(x_star+w-1, y_star, 1, 8, RoundedTRF);
		drow_picture(x_star+w-1, y_star+h-8, 1, 8, RoundedBRF);		
	}
}

/***************************************************************************/
// 允许触摸屏
// 参数：无
// 返回值：无
/***************************************************************************/
void Enable_TP(void)
{
	uchar temp;
	temp=LCD_CmdRead(TPCR1);
	temp|=0x80;
	LCD_CmdWrite(TPCR1);
	LCD_DataWrite(temp);
}

/***************************************************************************/
// 允许触摸屏非中断方式
// 参数：无
// 返回值：无
/***************************************************************************/
void Enable_Maunal_TP(void)
{
	uchar temp;
	temp=LCD_CmdRead(TPCR2);
	temp|=0x80;
	LCD_CmdWrite(TPCR2);
	LCD_DataWrite(temp);
}

/***************************************************************************/
// 允许触摸屏中断方式
// 参数：无
// 返回值：无
/***************************************************************************/
void Enable_interrput_TP(void)
{
	uchar temp;
	temp=LCD_CmdRead(INTR);
	temp|=0x10;
	LCD_CmdWrite(INTR);
	LCD_DataWrite(temp);
}

/***************************************************************************/
// 禁用触摸屏中断
// 参数：无
// 返回值：无
/***************************************************************************/
void Disable_interrput_TP(void)
{
	uchar temp;
	temp=LCD_CmdRead(INTR);
	temp&=0xef;
	LCD_CmdWrite(INTR);
	LCD_DataWrite(temp);
}

/***************************************************************************/
// 
// 参数：无
// 返回值：无
/***************************************************************************/
void Switch_Latch_X_data(void)
{
	uchar temp;
	temp=LCD_CmdRead(TPCR2);
	temp&=0xFE;
	temp|=0x02;
	LCD_CmdWrite(TPCR2);
	LCD_DataWrite(temp);
}

/***************************************************************************/
// 
// 参数：无
// 返回值：无
/***************************************************************************/
void Switch_Latch_Y_data(void)
{
	uchar temp;
	temp=LCD_CmdRead(TPCR2);
	temp|=0x01;
	temp|=0x02;
	LCD_CmdWrite(TPCR2);
	LCD_DataWrite(temp);
}

/***************************************************************************/
// x坐标值转换
// 参数：无
// 返回值：无
/***************************************************************************/
uint trans_tpx(uint x)
{
	long tp_x;
	long temp;
	temp=(((long)x)-x_tps0)*320;
	tp_x=temp/x_tps;
	return tp_x;
}

/***************************************************************************/
// y坐标值转换
// 参数：无
// 返回值：无
/***************************************************************************/
uint trans_tpy(uint y)
{
	long tp_y;
	long temp;
	temp=(((long)y)-y_tps0)*240;
	tp_y=temp/y_tps;
	return tp_y;
}

/***************************************************************************/
// 获取触摸屏值
// 参数：触摸x坐标 触摸y坐标
// 返回值：result_data
//						0无触摸 
//						1主界面定时设置 
//						2主界面手自动 
//						3主界面下班按键 
//						4主界面关机状态取消下班 
//						5设置界面一号槽选中  
//						6设置界面二号槽选中 
//						7设置界面三号槽选中 
//						8设置界面四号槽选中  
//						9设置界面五号槽选中  
//						10设置界面六号槽选中
//						11设置界面加一分钟
//						12设置界面减一分钟
//						13设置界面保存
//						14设置界面返回
//						15设置取消界面返回
/***************************************************************************/
uchar getTP_Value(uint x_temp, uint y_temp)
{
	uchar result_data = 0;
	if(displayMode == MainDisplayMode) //主界面正常显示
	{
		if(x_temp>0 && x_temp<96 && y_temp>208 && y_temp<240) //定时设置
		{
			if(runMode == ManualMode)
			{
				result_data = 1;
			}
		}
		else if(x_temp>104 && x_temp<208 && y_temp>208 && y_temp<240) //手自动
		{
			result_data = 2;
		}
		else if(x_temp>216 && x_temp<312 && y_temp>208 && y_temp<240) //下班按键
		{
			if(runMode == AutoMode) //只有自动模式下面才能点击下班按钮
			{
				result_data = 3;
			}
		}
	}
	else if(displayMode == PowerOffing) //主界面正在关机
	{
		if(x_temp>216 && x_temp<312 && y_temp>208 && y_temp<240) //取消下班
		{
			result_data = 4;
		}
	}
	else if(displayMode == Setting) //设置界面正常显示
	{
		if(x_temp>40 && x_temp<72 && y_temp>74 && y_temp<174) //一号槽选中 
		{
			result_data = 5;
		}
		else if(x_temp>80 && x_temp<112 && y_temp>74 && y_temp<174) //二号槽选中 
		{
			result_data = 6;
		}
		else if(x_temp>120 && x_temp<152 && y_temp>74 && y_temp<174) //三号槽选中 
		{
			result_data = 7;
		}
		else if(x_temp>160 && x_temp<192 && y_temp>74 && y_temp<174) //四号槽选中  
		{
			result_data = 8;
		}
		else if(x_temp>200 && x_temp<232 && y_temp>74 && y_temp<174) //五号槽选中  
		{
			result_data = 9;
		}
		else if(x_temp>240 && x_temp<272 && y_temp>74 && y_temp<174) //六号槽选中
		{
			result_data = 10;
		}
		else if(x_temp>160 && x_temp<216 && y_temp>12 && y_temp<60) //加一分钟
		{
			result_data = 11;
		}
		else if(x_temp>232 && x_temp<288 && y_temp>12 && y_temp<60) //减一分钟
		{
			result_data = 12;
		}
		else if(x_temp>112 && x_temp<208 && y_temp>208 && y_temp<240) //保存
		{
			result_data = 13;
		}
		else if(x_temp>216 && x_temp<312 && y_temp>208 && y_temp<240) //返回
		{
			result_data = 14;
		}
	}
	else if(displayMode == SettingSaved)//设置保存成功
	{
		if(x_temp>216 && x_temp<312 && y_temp>208 && y_temp<240) //返回
		{
			result_data = 15;
		}
	}
	return result_data;
}

/***************************************************************************/
// 手动模式轮询测试触摸屏
// 参数：无
// 返回值：result_data 
/***************************************************************************/
uchar tp_ManualMode()
{
	uchar Touch_Sta_Valid,INT_Sta,count,X1,Y1,X2,Y2,temp,result_data=0;
	uint X,Y,x_temp,y_temp;
	Touch_Sta_Valid=0;
	LCD_CmdWrite(TPCR2);
	LCD_DataWrite(0x81);
	LCD_CmdWrite(0xff); 
	LCD_DataWrite(0xff);  //写这段语句的意思是利用写数据时的判忙来确定此时ra8806是否处于内部工作状态
	INT_Sta=LCD_CmdRead(INTR);
	if((INT_Sta&0x08)==0x08)
	{
		for(count=0;count<50;count++)
		{
			INT_Sta=LCD_CmdRead(INTR);
			if(INT_Sta==0)
			{
				Touch_Sta_Valid=0;
				break;
			}
			if(count==49)
			{
				Touch_Sta_Valid=1;
			}
		}
		if(Touch_Sta_Valid)
		{
			Switch_Latch_X_data();
			delay_us(100);
			Switch_Latch_Y_data();	
			delay_us(100);
			X1=LCD_CmdRead(TPXR);
			LCD_CmdWrite(0xff); 
			LCD_DataWrite(0xff);
			Y1=LCD_CmdRead(TPYR);
			LCD_CmdWrite(0xff); 
			LCD_DataWrite(0xff);
			X2=LCD_CmdRead(TPZR)&0x03;
			LCD_CmdWrite(0xff); 
			LCD_DataWrite(0xff);
			Y2=LCD_CmdRead(TPZR)&0x0c;
			LCD_CmdWrite(0xff); 
			LCD_DataWrite(0xff);
			X = (((unsigned int)X1&0x00ff)<<2) | ((unsigned int)X2&0x0003);
			Y = (((unsigned int)Y1&0x00ff)<<2) | (((unsigned int)Y2&0x000C) >> 2);
			if((X<948)&&(X>94)&&(Y<944)&&(Y>92))
			{
				x_temp=trans_tpx(X);
				y_temp=trans_tpy(Y);
				result_data = getTP_Value(x_temp, y_temp);
			}
		}
		temp=LCD_CmdRead(INTR)&0xfe;
		LCD_CmdWrite(INTR);
		LCD_DataWrite(temp);
	}
	return result_data;
}

/***************************************************************************/
// 显示设置界面
// 参数：0正常设置  1保存成功提示  2保存失败提示 3正在保存提示
// 返回值：无
/***************************************************************************/
void SettingDisplay(uchar settingStatus)
{	
	uchar i, currentTimeCount;
	Access_Page2(); //存取DDRAM2
	clear(); 
	//加减时间按钮
	display_r_kuang(20,12,7,48,1);
	display_r_kuang(29,12,7,48,1);
	//设置槽框
	for(i=0; i<8; i++)
	{
		if(cistern[i].isCurrentlSetting)
		{
			display_kuang(i*5,74,4,100,0);
		}
		else
		{
			display_kuang(i*5,74,4,100,1);
		}
	}
	//按建
	if(displayMode == Setting)
	{
		display_r_kuang(14,208,12,32,1);
	}
	display_r_kuang(27,208,12,32,1);
	//提示框
	if(settingStatus)
	{
		display_r_kuang(9,72,22,64,1);	//保存成功提示
	}

	Access_Page1(); //存取DDRAM1
	clear();
	normal_data();
	LCD_NoInv();
	//标题
	LCD_ShowStr(1,8,"定时设置:");
	//加减时间按钮
	LCD_ShowStr(21,18,"加 一");
	LCD_ShowStr(21,38,"分 钟");
	LCD_ShowStr(30,18,"减 一");
	LCD_ShowStr(30,38,"分 钟");
	//设置槽框
	LCD_ShowStr(1,96,"未");
	LCD_ShowStr(1,116,"设");
	LCD_ShowStr(1,136,"置");
	//设置槽框
	for(i=1; i<7; i++)
	{
		currentTimeCount = cistern[i].perSettingTime/60;
		LCD_ShowChar(1+5*i,96,currentTimeCount/10+48);
		LCD_ShowChar(2+5*i,96,currentTimeCount%10+48);
		LCD_ShowStr(1+5*i,116,"分");
		LCD_ShowStr(1+5*i,136,"钟");
	}
	LCD_ShowStr(36,96,"未");
	LCD_ShowStr(36,116,"设");
	LCD_ShowStr(36,136,"置");
	//槽号
	LCD_ShowStr(1,180,"进");
	LCD_ShowStr(6,180,"1");
	LCD_ShowStr(11,180,"2");
	LCD_ShowStr(16,180,"3");
	LCD_ShowStr(21,180,"4");
	LCD_ShowStr(26,180,"5");
	LCD_ShowStr(31,180,"6");
	LCD_ShowStr(36,180,"出");
	//按建
	if(displayMode == Setting)
	{
		LCD_ShowStr(17,216,"保  存");
	}
	LCD_ShowStr(30,216,"返  回");
	//提示
	if(settingStatus)
	{
		display_r_kuang(9,72,22,64,0);	//保存成功提示
		display_kuang(10,80,20,48,1);	//保存成功提示
		if(settingStatus == 1)
			LCD_ShowStr(14,96,"保 存 成 功");   //保存成功提示
		else if(settingStatus == 2)
			LCD_ShowStr(14,96,"保 存 失 败");   //保存失败提示
		else
			LCD_ShowStr(14,96,"保 存 中...");   //保存失败提示
	}

	XOR_Two_Layer();//DDRAM1 OR DDRAM2
	Show_Two_Layer();//DDRAM1与DDRAM2同时显示
}

/***************************************************************************/
// 开机界面
// 参数：无
// 返回值：无
/***************************************************************************/
void PowerOnDisplay()
{
	Access_Page2();
	LCD_NoInv();
	clear();
	Access_Page1();
	LCD_NoInv();
	clear();
	Only_Show_Page1();
	//enlarge(2);
	//LCD_ShowStr(0x02,0x00,"慈溪星越纺机配件厂");
	enlarge(1);
	LCD_ShowStr(0x04,0x35,"联系人:陆志东");
	LCD_ShowStr(0x04,0x53,"联系电话: 13567402367");
}

/***************************************************************************/
// 显示主界面
// 参数：0正常显示 1正在关机 2关机成功
// 返回值：无
/***************************************************************************/
void MainDisplay(uchar displayStatus)
{
	uchar i, currentTimeCount;
	Access_Page2(); //存取DDRAM2
	clear(); 
	//状态显示
	display_kuang(0,0,8, 32, 1);
	display_kuang(8,0,8, 32, 1);
	//机械臂位置显示
	for(i=0; i<8; i++)
	{
		drow_picture(2+i*5, 42, 1, 8, HollowPrism);	
		drow_picture(2+i*5, 58, 1, 8, HollowPrism);	
	}
	if(manipulator.currentPosition%2 == 0)
	{
		if(manipulator.manipulatorPosition == Top)
		{
			drow_picture(2+manipulator.currentPosition/2*5, 42, 1, 8, SolidPrism);
		}
		else if(manipulator.manipulatorPosition == Bottom)
		{
			drow_picture(2+manipulator.currentPosition/2*5, 58, 1, 8, SolidPrism);	
		}
	}
	else
	{
		if(manipulator.manipulatorPosition == Top)
		{
			drow_picture(4+manipulator.currentPosition/2*5, 42, 1, 8, SolidPrism);
		}
		else if(manipulator.manipulatorPosition == Bottom)
		{
			drow_picture(4+manipulator.currentPosition/2*5, 58, 1, 8, SolidPrism);	
		}
	}
	//槽状态显示
	for(i=0; i<8; i++)
	{
		if(cistern[i].cisternStatus)
		{
			display_kuang(5*i,74,4,100,0);	
		}
		else
		{
			display_kuang(5*i,74,4,100,1);
		}
	}
	//按键显示
	if(displayStatus == 0)
	{
		if(runMode == ManualMode)
		{
			display_r_kuang(0,208,12,32,1);	
		}
		display_r_kuang(13,208,13,32,1);
	}
	if(runMode == AutoMode && displayStatus != 2) //只有自动模式下面才能点击下班按钮
	{
		display_r_kuang(27,208,12,32,1);
	}
	
	//提示信息
	if(displayStatus)
	{
		display_r_kuang(9,72,22,64,1);
	}

	Access_Page1();//存取DDRAM1
	clear();
	normal_data();
	LCD_NoInv();
	//状态显示
	LCD_ShowStr(2,8,"状态");
	if(runMode == AutoMode)
		LCD_ShowStr(10,8,"自动");
	else
		LCD_ShowStr(10,8,"手动");

	//机械臂动作显示
	LCD_ShowStr(22,8,"机械臂动作:");
	switch(manipulator.manipulaterStatus)
	{
		case Stop:
			LCD_ShowStr(34,8,"空闲");
		break;
		case GoEntrance:
			LCD_ShowStr(34,8,"<---");
		break;
		case GoOutfall:
			LCD_ShowStr(34,8,"--->");
		break;
		case GoUp:
			LCD_ShowStr(34,8,"向上");
		break;
		case GoDown:
			LCD_ShowStr(34,8,"向下");
		break;
		default:
			LCD_ShowStr(34,8,"错误");
	}
	//槽状态显示
	for(i=0; i<8; i++)
	{
		if(cistern[i].cisternStatus == Empty)
		{
			LCD_ShowStr(1+5*i,100,"空");
			LCD_ShowStr(1+5*i,132,"闲");
		}
		else if(cistern[i].cisternStatus == Ready)
		{
			LCD_ShowStr(1+5*i,100,"就");
			LCD_ShowStr(1+5*i,132,"绪");
		}
		else
		{	
			if(i != 0 && i != 7)// 进口出口不安时间就绪
			{
				currentTimeCount = (cistern[i].settingTime - cistern[i].currentTime)/60;
				LCD_ShowChar(1+5*i,96,currentTimeCount/10+48);
				LCD_ShowChar(2+5*i,96,currentTimeCount%10+48);
				LCD_ShowStr(1+5*i,116,"分");
				LCD_ShowStr(1+5*i,136,"钟");	
			}
			else
			{
				LCD_ShowStr(1+5*i,100,"忙");
				LCD_ShowStr(1+5*i,132,"碌");
			}		
		}
	}
	LCD_ShowStr(1,180,"进");
	LCD_ShowStr(6,180,"1");
	LCD_ShowStr(11,180,"2");
	LCD_ShowStr(16,180,"3");
	LCD_ShowStr(21,180,"4");
	LCD_ShowStr(26,180,"5");
	LCD_ShowStr(31,180,"6");
	LCD_ShowStr(36,180,"出");
	//按键显示
	if(displayStatus == 0)
	{
		if(runMode == ManualMode)
		{
			LCD_ShowStr(2,216,"定时设置");
		}
		LCD_ShowStr(15,216,"手动/自动");
	}
	if(displayStatus == 1)
	{
		LCD_ShowStr(29,216,"取消下班");
	}
	else if(displayStatus == 0 && runMode == AutoMode)//只有自动模式下面才能点击下班按钮
	{
		LCD_ShowStr(29,216,"下班按键");
	}
	//提示信息
	if(displayStatus)
	{
		display_r_kuang(9,72,22,64,0);
		display_kuang(10,80,20,48,1);
		if(displayStatus == 1)
			LCD_ShowStr(14,96,"关 机 中...");
		else
			LCD_ShowStr(14,96,"关 机 成 功"); 
	}

	XOR_Two_Layer();//DDRAM1 OR DDRAM2
	Show_Two_Layer();//DDRAM1与DDRAM2同时显示
}

/***************************************************************************/
// 机械臂状态刷新
// 参数：无
// 返回值：无
/**************************************************************************/
void ManipulatorStatusDisplay(void)
{
	Access_Page1();//存取DDRAM1
	normal_data();
	LCD_NoInv();
	//机械臂动作显示
	switch(manipulator.manipulaterStatus)
	{
		case Stop:
			LCD_ShowStr(34,8,"空闲");
		break;
		case GoEntrance:
			LCD_ShowStr(34,8,"<---");
		break;
		case GoOutfall:
			LCD_ShowStr(34,8,"--->");
		break;
		case GoUp:
			LCD_ShowStr(34,8,"向上");
		break;
		case GoDown:
			LCD_ShowStr(34,8,"向下");
		break;
		default:
			LCD_ShowStr(34,8,"错误");
	}
}

/***************************************************************************/
// 触摸按钮反显函数
// 参数：触摸值
// 返回值：无
/***************************************************************************/
void TP_Response(uchar tpValue)
{
	Access_Page2(); //存取DDRAM2	
	switch(tpValue)
	{
		case 1:
			display_r_kuang(0,208,12,32,0);	
		break;
		case 2:
			display_r_kuang(13,208,13,32,0);
		break;
		case 3:
			display_r_kuang(27,208,12,32,0);
		break;
		case 4:
			display_r_kuang(27,208,12,32,0);
		break;
		case 11:
			display_r_kuang(20,12,7,48,0);
		break;
		case 12:
			display_r_kuang(29,12,7,48,0);
		break;
		case 13:
			display_r_kuang(14,208,12,32,0);
		break;
		case 14:
			display_r_kuang(27,208,12,32,0);
		break;
		case 15:
			display_r_kuang(27,208,12,32,0);
		break;
	}
}
