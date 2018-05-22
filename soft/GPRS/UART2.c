#define UART2_PRIVATE
#include "Uart2.h"
#include "include.h"

#ifdef TRUE
	#undef TRUE
#endif

#ifdef FALSE
	#undef FALSE
#endif


#define UART2_RECEIVE_TIMEOUT		280
Gprs_State gprsstate;
const char gprs_checksignal[] = "AT+CSQ\n";
const char gprs_volue[] = "ATL9\r\n";
struct Task *FirstTask;//头任务指针
struct Task *LastTask;//尾任务指针
struct Task  TaskBuf[TASKS];//任务寄存器组
struct UsartDate Usat2_data;//串口数据
struct UsartDate Usat3_data;//串口数据
uint8_t usart3_buff[10][3];
//static uint8_t   send_buff[10];
//static uint8_t   which_buff = 1;    			//  1:事件发送 Uart4SendBuffer   2:巡检发送send_buff  3:不固定
//static uint8_t   last_buff;
//static uint8_t query_timeout;
//static uint8_t   resend_flag;			// 1:需要重发0:不需要重发
//static uint8_t microphone_state;		//应急广播话筒状态true:话筒通话false:话筒挂机
//static uint8_t emergency_state;
void UART2_Init(void);
void shell(uint8_t *cmd,uint8_t length );
//GPRS开机
void gprs_init(void)
{
    PWR_KEY_ON;
    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
    PWR_KEY_OFF;
    UART2_Init();
    Uart2_Gprs_PollTimer = GetTickCount();
    gprsstate.Gprs_CSQ = 0;
    gprsstate.Gprs_simflag = 2;
    gprsstate.Gprs_flag = 0;
}

//获取发送缓存
uint8_t * Uart2SendCommand(void)
{
    uint8_t Uart2SendInTemp;
    
    Uart2SendInTemp = Uart2SendIn;
    Uart2SendIn++;
    
    if (Uart2SendIn == UART2_BUFFER_COUNT)
    {
        Uart2SendIn = 0;
    }
    
    return (&Uart2SendBuffer[Uart2SendInTemp][0]);
}

//巡检
//不放在大的数据发送缓存中
//void query(uint8_t addr)
//{
//	send_buff[0] = addr; 
//	send_buff[1] = 0x00;
//	send_buff[2] = 0x03;
//	send_buff[3] = 0x00;
//	
//	CalCRC(send_buff);
//	
//	send_buff[0] = 2 * addr + 1;

//	Uart4SendStat  += 0x01;
//	Uart4SendPos    = 1;
//	which_buff      = 2;	

//	SetUart4ToSendMode();

//	LPC_UART4->LCR |= (1 << 3); 	//打开奇偶校验
//	LPC_UART4->LCR |= (1 << 5); 	//设置强制检验为1；
//	LPC_UART4->LCR &=  ~(1 << 4); 	//强制校验位1
//	LPC_UART4->THR  = send_buff[0]; //发送地址位
//}

//应答
//不放在大的数据发送缓存中
//void answer(uint8_t addr)
//{
//	send_buff[0] = addr;
//	send_buff[1] = 0x00;
//	send_buff[2] = 0x03;
//	send_buff[3] = 0x01;
//	
//	CalCRC(send_buff);
//	
//	send_buff[0] = 2 * addr + 1;
//	
//	Uart4SendStat  += 0x01;
//	Uart4SendPos    = 1;
//	which_buff      = 2;
//	
//	SetUart4ToSendMode();

//	LPC_UART4->LCR |= (1 << 3); 	//打开奇偶校验
//	LPC_UART4->LCR |= (1 << 5); 	//设置强制检验为1；
//	LPC_UART4->LCR &=  ~(1 << 4); 	//强制校验位1
//	LPC_UART4->THR  = send_buff[0]; //发送地址位
//}




/*
  des_addr:目的地址
  cmd:         命令字
  len:          buff中数据的长度
  buff:         发送的数据
 */
void uart2_send(uint8_t len, uint8_t *buff)
{
	uint8_t i;
//	
//	p = Uart2SendCommand();
//	p[0] = len + 0x01;

//	memcpy(p + 1, buff, len);
    for(i = 0;i< len;i++)
    {
        
        USART_SendData(USART2,buff[i]);
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
    }
	
	//CalCRC(p);
}







//UART2 初始化
void UART2_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	


  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	/* Enable the USART2 Pins Software Remapping */
//	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	  /* Enable the USART2 */
	USART_Cmd(USART2, ENABLE);
   InitTask();
	
}

void USART2_IRQHandler(void)
{
//	uint8_t temp_buf;
//	uint8_t isr;
//	uint8_t *p;

	//isr = LPC_UART4->IIR & 0x0f;

//	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)			//发送中断
//	{
//		if (which_buff == 1)
//		{
//			p = Uart2SendBuffer[Uart2SendOut];
//		}
//		else
//		{
//			p = send_buff;
//		}
//		
//		if (Uart2SendPos < (p[0]) && Uart2SendPos < UART2_BUFFER_LENGTH)
//		{
//			USART_SendData(USART2,  p[Uart2SendPos++]);
//		}
//		else
//		{
////			while ((LPC_UART4->LSR & (1 << 6)) == 0);
////			
////			SetUart4ToReceiveMode();

////			temp_buf	    = Uart4SendStat;
////			temp_buf       <<= 4;
////			Uart4SendStat    = temp_buf | (Uart4SendStat & 0x0f);			
////			Uart4SendTimer   = GetTickCount();

//			last_buff = which_buff;
//			
//			if (last_buff == 1 && resend_flag == 0)
//			{
//				Uart2SendOut++;
//						
//				if (Uart2SendOut == UART2_BUFFER_COUNT)
//				{
//					Uart2SendOut = 0;
//				}
//			}
//		}
//	}
 
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)			//接收中断
	{
        if(Usat2_data.RXlenth < UART2_BUFFER_LENGTH)
        {
            Usat2_data.RXbuf[Usat2_data.RXlenth++] = (uint8_t)USART_ReceiveData(USART2);
            Usat2_data.Time = 250;
        }
//		temp_buf = (uint8_t)USART_ReceiveData(USART2);;
//		Uart2ReceiveOverTimer = GetTickCount();
//		
//		if (Uart2ReceiveCount == 0)
//		{
//			//字节为0为本地地址
//			if (temp_buf == 0x01)
//			{
//				Uart2ReceiveCount = 1;
//				Uart2ReceiveBuffer[Uart2ReceiveIn][0] = 0x00;
//			}
//			else
//			{
//				Uart2ReceiveCount = 0;
//			}
//		}
//		else
//		{
//			Uart2ReceiveBuffer[Uart2ReceiveIn][Uart2ReceiveCount] = temp_buf;
//			
//			Uart2ReceiveCount++;
//			
//			if (Uart2ReceiveCount >= UART2_BUFFER_LENGTH)
//				Uart2ReceiveCount = 0;
//		}
//		
//		if (((Uart2ReceiveBuffer[Uart2ReceiveIn][2] + 3) == Uart2ReceiveCount) && (Uart2ReceiveCount > 3)) //接收完成
//		{
//			Uart2ReceiveCount	  = 0;
//			Uart2ReceiveOverTimer = 0;
//			
//			Uart2ReceiveIn++;
//			
//			if (Uart2ReceiveIn == UART2_BUFFER_COUNT)
//			{
//				Uart2ReceiveIn = 0;
//			}
//		}
	}
}

void UART2_Task(void)
{
//	uint8_t i;
//	static uint8_t    addr;
//	static uint32_t   last_timer = 0;
	//SYSTEM_EVENT pEvent;	
	//CAN_DATA_BUFFER *pCommand;
//	uint32_t temp;
//	uint8_t  SendStat_H;
//	uint8_t  SendStat_L;
//	int floor;
//	static uint8_t broadcast_or_sound_light = 1;	
		
	//巡检GPRS信号
    if((GetTickCount()-Uart2_Gprs_PollTimer)>3000)
    {
         Uart2_Gprs_PollTimer = GetTickCount();
        if(call_flag == 0)
        {
            if(gprsstate.Gprs_simflag == 0)
            {
             
              uart2_send(strlen(gprs_checksim),(uint8_t*)gprs_checksim);
                //uart2_send(strlen(gprs_checksim),(uint8_t*)gprs_checksim);
            }
            else if(gprsstate.Gprs_simflag == 1) 
            {
                if(gprsstate.Gprs_simflag == 1)//查询到sim卡，开始查询信号质量
                {
                  uart2_send(strlen(gprs_checksignal),(uint8_t*)gprs_checksignal);
                }
            }
            if(gprsstate.Gprs_flag == 0)
            {
                gprsstate.Gprs_simflag = 0;
                gprsstate.Gprs_flag = 1;
                uart2_send(strlen(gprs_volue),(uint8_t*)gprs_volue);
            }
       }
        
    }


	

	if(FirstTask!=NULL)
	{
        if(FirstTask->Type == 1)
        {
           shell(FirstTask->pDate,FirstTask->Date);
		  
        }
        else if(FirstTask->Type == 2)
        {
	//		send0(0xdd);
		  shell3(FirstTask->pDate,FirstTask->Date);

        }
		free(FirstTask->pDate);//释放内存空间
		FirstTask->Type=0;//释放已完成的任务结构体
		FirstTask=FirstTask->Next;
	}







}
//初始化任务结构体****************************************************************************
void  InitTask(void)
{
    memset(&Usat2_data,0,sizeof(Usat2_data));
	Usat2_data.RXbuf=malloc(UARTLENGTH);//给串口接收缓存区申请一块内存
    
     memset(&Usat3_data,0,sizeof(Usat3_data));
	 Usat3_data.RXbuf=&usart3_buff[0][0];//给串口接收缓存区申请一块内存
   // 
	
}
uint8_t   AddTask1(uint8_t Type ,uint8_t  Data,uint8_t  *pData)
{
	uint8_t   i;
	for(i=0;i<TASKS;i++) //搜索空闲结构体
	{
		if(TaskBuf[i].Type==0)
		break;
	}
	
	if(i<TASKS)  //
	{
		if(FirstTask==NULL)
		{ FirstTask =&TaskBuf[i];
			LastTask  =&TaskBuf[i];
		}
		else
		{	 LastTask->Next=&TaskBuf[i];
			LastTask      =&TaskBuf[i];
		}
		
		LastTask->Type=Type;
		LastTask->Date=Data;
		LastTask->pDate=pData;
		LastTask->Next=NULL;
		
	}
	
	return   0;
}//
//串口数据处理函数
void shell(uint8_t *cmd,uint8_t length )
{
//	uint8_t i = 0;
//    uint16_t crc = 0xffff;
//	send_char1(cmd,length+2);
	if ((cmd[0] == 0x41)&&(cmd[1] == 0x54)&&(cmd[2] == 0x2B))//判断是否是AT+指令
	{
         if((cmd[3] == 0x43)&&(cmd[4] == 0x53)&&(cmd[5] == 0x51))//"CSQ?"
         {
             if((cmd[9] == 0x45)&&(cmd[10] == 0x52)&&(cmd[11] == 0x52))//"ERR"
             {
                 gprsstate.Gprs_CSQ = 0;
             }
             else
             {
                 
                 gprsstate.Gprs_CSQ = cmd[14]-0x30;
             }
         }
         if((cmd[3] == 0x43)&&(cmd[4] == 0x43)&&(cmd[5] == 0x49)&&(cmd[6] == 0x44))//ccid?
         {
            if((cmd[9] == 0x45)&&(cmd[10] == 0x52)&&(cmd[11] == 0x52))//"ERR"
            {
                gprsstate.Gprs_simflag = 0;//没有检测到SIM卡
            }
            else 
            {
                gprsstate.Gprs_simflag = 1;
                //检测到了sim卡
            }
             
         }

	}  
    //if()    
}
//串口接收中断服务函数
void Rev_Istr(void)
{
   uint8_t *pbuf1;
   if(Usat2_data.Time!=0)
   {
     Usat2_data.Time--;  
   }
   else if(Usat2_data.RXlenth!=0) 
   {
       AddTask1(1,Usat2_data.RXlenth,Usat2_data.RXbuf);//接收完成，送给前台处理
	   pbuf1=malloc(UARTLENGTH); //从新申请一块动态内存用于串口数据的接收
			
		if(pbuf1!=NULL)
		{
		    Usat2_data.RXbuf=pbuf1;
		}
			Usat2_data.RXlenth=0;  
   }       
}
//拨打电话
void Call_PhoneNum(uint8_t *phone)
{   char i;
    uint8_t buf[20];
    uint8_t phone1[11];
    //norflash_read_phone(1,(uint8_t *)phone);
    for(i = 0;i < 11;i ++)
    {
      phone1[i] = phone[i] + 0x30;  
    }
    sprintf((char*)buf,"ATD%s;\r\n",phone1);
    uart2_send(17,buf);
  
     AUDIO_SECL_ON;
    AUDIO_CTR_ON;
}









