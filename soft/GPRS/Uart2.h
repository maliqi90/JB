#ifndef _UART2_H_
#define _UART2_H_

#include "stm32f10x.h"
#include "stdint.h"
#include "def.h"

#ifdef UART2_PRIVATE
	#define UART2_PUBLIC
#else
	#define UART2_PUBLIC extern
#endif
#

#define UART2_BUFFER_COUNT  	10
#define UART2_BUFFER_LENGTH  	64
extern const char gprs_checksignal[];//查询GPRS信号
extern const char gprs_checksim[];//查询sim卡

//任务结构体
#define TASKS    10 //消息队列深度
#define DATAS    10 //动态内存块数
#define DLENGTH  64 //动态内存块字节数
#define UARTLENGTH    64      //、
struct Task
{
	uint8_t Type;//
	uint8_t Date;//有效数据长度
	uint8_t *pDate;// 对应任务可能用到的数据首地址
	struct Task *Next;//指向下一个任务
};	
//串口数据结构体


struct UsartDate
{
	uint8_t State;//串口接收状态标识
	uint8_t Time;//串口接收计时
	uint8_t RXlenth;//串口接收数据包长度
	uint8_t *RXbuf;//串口接收数据所用缓存数据空间的首地址
};
extern struct Task *FirstTask;//头任务指针
extern  struct Task *LastTask;//尾任务指针
extern struct Task  TaskBuf[TASKS];//任务寄存器组
extern struct UsartDate Usat2_data;//串口数据
extern uint8_t usart3_buff[10][3];
UART2_PUBLIC uint8_t Uart2SendBuffer[UART2_BUFFER_COUNT][UART2_BUFFER_LENGTH];
UART2_PUBLIC uint8_t Uart2SendIn;
UART2_PUBLIC uint8_t Uart2SendOut;
UART2_PUBLIC uint8_t Uart2SendStat;
UART2_PUBLIC uint8_t Uart2SendPos;
UART2_PUBLIC uint32_t Uart2SendTimer;

UART2_PUBLIC uint8_t Uart2ReceiveBuffer[UART2_BUFFER_COUNT][UART2_BUFFER_LENGTH];
//UART4_PUBLIC U8 Uart4ReceiveCurrentBuffer[UART4_BUFFER_LENGTH];
UART2_PUBLIC uint8_t Uart2ReceiveIn;
UART2_PUBLIC uint8_t Uart2ReceiveOut;
UART2_PUBLIC uint8_t Uart2ReceiveCount;
UART2_PUBLIC uint8_t Uart2ReceiveSumCheck;
UART2_PUBLIC uint32_t Uart2ReceiveOverTimer;

UART2_PUBLIC uint32_t Uart2_Gprs_PollTimer;
UART2_PUBLIC uint16_t EventRamUart2Pointer;

UART2_PUBLIC uint8_t  switch_timer;

void gprs_init(void);
typedef struct
{
    char Gprs_simflag;//sim卡是否存在
    char Gprs_CSQ;   //信号质量
    char Gprs_flag;//GPRS设置标志位
    
}Gprs_State;


	
    extern Gprs_State gprsstate;
//对外接口函数
void UART4_Init(void);
uint8_t * Uart2SendCommand(void);
void UART4_Task(void);
void  InitTask(void);
void uart2_send(uint8_t len, uint8_t *buff);
//void uart4_send(uint8_t des_addr, uint8_t cmd, uint8_t len, uint8_t *buff);
void Rev_Istr(void);
void UART2_Task(void);
uint8_t   AddTask1(uint8_t Type ,uint8_t  Data,uint8_t  *pData);
extern Gprs_State gprsstate;
extern void Call_PhoneNum(uint8_t *phone);
#endif
