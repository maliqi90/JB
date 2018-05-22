#include "include.h"

volatile uint8_t usart3_flag = 0;
extern uint8_t usart3_buff[10][3];
volatile  uint8_t temp_buf[4];
//UART3 初始化
void UART3_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
   GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 1200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);
	
	/* Enable the USART2 Pins Software Remapping */
//	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	  /* Enable the USART2 */
	USART_Cmd(USART3, ENABLE);
   InitTask();
	
}

void uart3_send(uint8_t len, uint8_t *buff)
{
	uint8_t i;
//	
//	p = Uart2SendCommand();
//	p[0] = len + 0x01;

//	memcpy(p + 1, buff, len);
    for(i = 0;i< len;i++)
    {
        
        USART_SendData(USART3,buff[i]);
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
        //TIMDelay_N10us(900);
    }
	
	//CalCRC(p);
}
void USART3_IRQHandler(void)
{
 
    uint8_t temp;
//	uint8_t isr;
//	uint8_t *p;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)			//????
	{
        if(Usat3_data.RXlenth < UART3_BUFFER_LENGTH)
        {
//            Usat2_data.RXbuf[Usat2_data.RXlenth++] = (uint8_t)USART_ReceiveData(USART3);
           temp= (uint8_t)USART_ReceiveData(USART3);
            #if 0
            if((temp==0xaa)&&(usart3_flag == 0))
            {
                usart3_flag = 1;
                temp_buf[0] = 0xaa;
            }
            else if(usart3_flag == 1)
            {
                usart3_flag = 2;
                temp_buf[1] = temp;
            }
            else if(usart3_flag == 2)
            {
                temp_buf[2] = temp;
                usart3_flag = 0;
                if(temp_buf[2] == (temp_buf[0]+temp_buf[1]))
                {
                    temp_buf[0] = 0xaa;
                    temp_buf[1] = 0x01;
                    if(EventFaultCount!=0)
                    {
                        Led_Ctrl(LEDFAULT_ON);
                        temp_buf[1] = 0x03;
                    }
                    if(EventAlarmCount!=0)
                    {
                        Led_Ctrl(LEDARAM_ON);
                        temp_buf[1] = 0x02;
                    }
                    if((EventFaultCount == 0)&&(EventAlarmCount == 0))
                    {
                        
                       
                    }
                    temp_buf[2] = temp_buf[0]+temp_buf[1];
                    uart3_send(3,(uint8_t *)temp_buf);
                }
            }
            #endif
            if((temp==0xaa)&&(usart3_flag == 0))
            {
                usart3_flag = 1;
                temp_buf[0] = 0xaa;
            }
            else if(usart3_flag == 1)
            {
                usart3_flag = 2;
                temp_buf[1] = temp;
            }
            else if(usart3_flag == 2)
            {
                temp_buf[2] = temp;
                usart3_flag = 3;


            }
            else if(usart3_flag == 3)
              {
                  usart3_flag = 0;
                  temp_buf[3] = temp;
                 if((busaddr_flag == 1)||(temp_buf[1]!=comm_set.Bus_Addr))//更改地址
                 {
                     busaddr_flag = 0;
                     temp_buf[0] = 0x55;
                     //temp_buf[1] = comm_set.Bus_Addr;
                     temp_buf[2] = comm_set.Bus_Addr;
                     temp_buf[3] = temp_buf[0]+ temp_buf[1]+temp_buf[2];
                     uart3_send(4,(uint8_t *)temp_buf);

                }
                 else if(busaddr_flag == 0)
                 {
                   if(temp_buf[3] == (temp_buf[0]+temp_buf[1]+temp_buf[2]))
                    {
                        temp_buf[0] = 0xaa;
                        temp_buf[2] = 0x01;
                        if(EventFaultCount!=0)
                        {
                            Led_Ctrl(LEDFAULT_ON);
                            temp_buf[2] = 0x03;
                        }
                        if(EventAlarmCount!=0)
                        {
                            Led_Ctrl(LEDARAM_ON);
                            temp_buf[2] = 0x02;
                        }
                        if((EventFaultCount == 0)&&(EventAlarmCount == 0))
                        {
                            
                           
                        }
                         temp_buf[3] = temp_buf[0]+ temp_buf[1]+temp_buf[2];
                        uart3_send(4,(uint8_t *)temp_buf);
                    }
                 }
            }
//            Usat3_data.RXbuf[Usat3_data.RXlenth++] = temp;
//            Usat3_data.Time = 100;
        }
    }
}

void Rev3_Istr(void)
{
//   uint8_t *pbuf1;
    uint8_t i;
   if(Usat3_data.Time!=0)
   {
     Usat3_data.Time--;  
   }
   else if(Usat3_data.RXlenth!=0) 
   {
       AddTask1(2,Usat3_data.RXlenth,Usat3_data.RXbuf);//接收完成，送给前台处理
//	   pbuf1=malloc(UARTLENGTH); //从新申请一块动态内存用于串口数据的接收
	   for(i = 0;i < 10;i++)
       {
           if(usart3_buff[i][0]!=0xaa)//判断内存是否为空
           {
               break;
           }
       }
       

		    Usat3_data.RXbuf=&usart3_buff[i][0];

			Usat3_data.RXlenth=0;  
   }       
}
//串口数据处理函数
void shell3(uint8_t *cmd,uint8_t length )//串口3处理函数
{
    uint8_t uart3_buff[3];
    
//	uint8_t i = 0;
//    uint16_t crc = 0xffff;
//	send_char1(cmd,length+2);
    uart3_buff[0] = 0xaa;
	if (cmd[0] == 0xAA)
	{
		switch(cmd[2])
		{
            case 1: 

                if(EventFaultCount!=0)
                {
                     uart3_buff[1] = 0x02;//报故障
                }
                if(EventAlarmCount!=0)
                {
                    uart3_buff[1] = 0x01;//报火警
                }
                uart3_buff[2] = uart3_buff[1]+uart3_buff[0];
                uart3_send(3,uart3_buff);
                break;
            
		}

	}  
    
}


