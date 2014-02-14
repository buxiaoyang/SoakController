#ifndef 		_OPERATION_H__
#define 		_OPERATION_H__

extern void A_Move(unsigned char move_cisternNumber);
extern unsigned char A_Move_Steps;
extern void A_Down(unsigned char down_cisternNumber); 
extern unsigned char A_Down_Steps;
extern void A_Up(unsigned char up_cisternNumber); 
extern unsigned char A_Up_Steps;
extern void A_Up_Down(unsigned char upCisternNumber, unsigned char downCisternNumber);
extern unsigned char A_Up_Down_Steps;

extern void A_Move_Entrance(void); 
extern unsigned char A_Move_Entrance_Steps;

extern void ManualMode_Operation(void); 

extern unsigned int ManiOperationTimer;

#define timeBetweenOpeartion 5  //两个机械臂直接的切换动作等待时间

#endif
