#ifndef LD_CHIP_H
#define LD_CHIP_H

#include "sys.h"

//	以下三个状态定义用来记录程序是在运行ASR识别还是在运行MP3播放
#define LD_MODE_IDLE		0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 	0x40


//	以下五个状态定义用来记录程序是在运行ASR识别过程中的哪个状态
#define LD_ASR_NONE			0x00	//	表示没有在作ASR识别
#define LD_ASR_RUNING		0x01	//	表示LD3320正在作ASR识别中
#define LD_ASR_FOUNDOK		0x10	//	表示一次识别流程结束后，有一个识别结果
#define LD_ASR_FOUNDZERO 	0x11	//	表示一次识别流程结束后，没有识别结果
#define LD_ASR_ERROR	 	0x31	//	表示一次识别流程中LD3320芯片内部出现不正确的状态


#define CLK_IN   			24	/* 用户注意修改输入的晶振时钟大小 */
#define LD_PLL_11			(u8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(u8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(u8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

#define MIC_VOL 0x43	 //咪头增益

#define DATE_A 50   //数组二维数值
#define DATE_B 30	//数组一维数值

#define CODE_CMD  0x00   //该命令码0x00不可进行修改。

#define CODE_1KL1 0x01
#define CODE_1KL2 0x02
#define CODE_1KL3 0x03
#define CODE_1KL4 0x04
#define CODE_1KL5 0x05
#define CODE_1KL6 0x06
#define CODE_1KL7 0x07

#define CODE_2KL1 0x08
#define CODE_2KL2 0x09
#define CODE_2KL3 0x0A
#define CODE_2KL4 0x0B
#define CODE_2KL5 0x0C
#define CODE_2KL6 0x0D
#define CODE_2KL7 0x0E

#define CODE_3KL1 0x0F
#define CODE_3KL2 0x10
#define CODE_3KL3 0x11
#define CODE_3KL4 0x12
#define CODE_3KL5 0x13
#define CODE_3KL6 0x14
#define CODE_3KL7 0x15

#define CODE_4KL1 0x16
#define CODE_4KL2 0x17
#define CODE_4KL3 0x18
#define CODE_4KL4 0x19
#define CODE_4KL5 0x1A
#define CODE_4KL6 0x1B
#define CODE_4KL7 0x1C

#define CODE_5KL1 0x1D
#define CODE_5KL2 0x1E
#define CODE_5KL3 0x1F
#define CODE_5KL4 0x20
#define CODE_5KL5 0x21
#define CODE_5KL6 0x22
#define CODE_5KL7 0x23

#define CODE_6KL1 0x24
#define CODE_6KL2 0x25
#define CODE_6KL3 0x26
#define CODE_6KL4 0x27
#define CODE_6KL5 0x28
#define CODE_6KL6 0x29
#define CODE_6KL7 0x2A

#define CODE_7KL1 0x2B
#define CODE_7KL2 0x2C
#define CODE_7KL3 0x2D
#define CODE_7KL4 0x2E
#define CODE_7KL5 0x2F
#define CODE_7KL6 0x30
#define CODE_7KL7 0x31

//函数声明
void LD_Reset(void);
void LD_Init_Common(void);
void LD_Init_ASR(void);
void ProcessInt(void);
void LD_AsrStart(void);
u8 LD_AsrRun(void);
u8 LD_AsrAddFixed(void);
u8 RunASR(void);
u8 LD_GetResult(void);

#endif

