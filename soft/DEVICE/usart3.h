#ifndef _USART3_H_
#define _USART3_H_

#include "Uart2.h"
#define UART3_RECEIVE_TIMEOUT		280

#define UART3_BUFFER_COUNT  	10
#define UART3_BUFFER_LENGTH  	3

//struct Task *FirstTask;//ͷ����ָ��
//struct Task *LastTask;//β����ָ��
//struct Task  TaskBuf[TASKS];//����Ĵ�����
extern struct UsartDate Usat3_data;//��������
void UART3_Init(void);
void uart3_send(uint8_t len, uint8_t *buff);
void Rev3_Istr(void);
void shell3(uint8_t *cmd,uint8_t length );
#endif


