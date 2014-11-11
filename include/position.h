#ifndef 		__POSITION_H__
#define 		__POSITION_H__

sbit sensorPositon = P3^4; //机械臂定位传感器

extern void maniPosition(void);	
extern void maniPowerOnPosition(void);	

#endif
