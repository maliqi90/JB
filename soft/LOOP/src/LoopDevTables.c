/*
********************************************************************************
*                             ������̩�����������޹�˾
* ��    ַ �� �����з�̨���żҿ���·8��
* ��    �� �� 100072
* ��    ַ �� Http://www.guotaiyian.com
*
* �� �� �� :  LoopDevTables.c
* ��    �� :  ʯ��
* ����ʱ�� :  2010/05/05
* ��    �� :  ��·�������͵��б�
*
* ע    �� �� 
********************************************************************************
*/
//#include "DeviceTypeDefine.h"
#include "stm32f10x.h"
#include "LoopDevTables.h"

/*******************************************************************************
*      �������ʾ��̽������ģ��ֱ��ڲ�ͬ��������(B0-B3)ʱ�Ļ���ĸ�����  
*      ע��Ϊ�˿��Լ������ʹ������� 0x05ͳһ��Ϊ��5������  
*******************************************************************************/ 
const unsigned char AnsCodeNUM[2][16] = {
//�������00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
           { 2, 3, 3, 0, 0, 5, 0, 0, 0, 0, 3, 0, 0, 2, 0, 0}, //̽����
           { 3, 0, 0, 0, 0, 5, 3, 3, 3, 3, 3, 0, 0, 3, 0, 0}  //ģ��
 };  
  
/*****************************************************************************
-------------------  ���ֺ�ʱ�䳤�̶��ձ�   ----------------------------
��0x00��                ��000us ��
��0x01��                ��300us ��             
��0x02��                ��600us ��
��0x03��                ��900us ��
��0x04��                ��1200us��
��0x05��                ��1500us��
******************************************************************************/

/*******************************************************************************
��0x05�� ���ͼ������ ��̽�����ࡿ  ���� PW0 PW1 PW2 PW3(������û��)
        ˵��: �����������ж� ������û��PW3���룬û��PW3��Ϊ����������Ϊ������ 
        ���͸��ݵ�ַ �� PW0 ��������б�����������������̽��������
                                   ��ַΪ 1  - 127    PW0 ���� 300us;
                                   ��ַΪ 128- 254    PW0 ���� 900us    
*******************************************************************************/

#if 0
//̽��������        ����
#define D_NONE      0x1f     //�����ڵ�һ��ֵ��PW2 = 0ʱ

#define D_GD        0x00     //���̽����     <----��
#define D_CDW       0x01     //���̽����   <----��
#define D_DW        0x02     //û�ж���̽����     <----��
#define D_YWFH      0x03     //���¸���̽���� <----��
#define D_KRQT      0x04     //��ȼ����̽���� <----��

#define ND_GD       0x00     //�¹��̽����				23	
#define ND_CDW      0x01     //�²��̽����			31
#define ND_DW       0x02     //û���¶���̽����		31
#define ND_YWFH     0x03     //�����¸���̽����		50
#define ND_KRQT     0x04     //�¿�ȼ����̽����		102
#define ND_ZWHY     0x05     //���������̽����		40
#define ND_XCHJ     0x07     //���ֳ�����̽����		235
#define ND_HWHY     0x05     //�º������̽����		40
#define ND_GLMD     0x07     //̽������						241
#endif


/*********************  ̽�������Ͷ��ձ� **************************************/
const unsigned char DetTypePW3PW2[6][6] = {
//|  PW2   |  PW2   |  PW2    | PW2    |  PW2    |  PW2    |
//|0(0us)  |1(300us)| 2(600us)|3(900us)|4(1200us)|4(1500us)|
	{D_NONE,  D_GD,     D_CDW,    D_DW,    D_YWFH,   D_KRQT }, //PW3 = 0(0us   )���ϡ�
	{D_NONE,  ND_GD,    ND_CDW,   ND_DW,   ND_YWFH,  ND_KRQT}, //PW3 = 1(300us )���¡�
	{D_NONE,  ND_ZWHY,  ND_XCHJ,  ND_HWHY, ND_GLMD,  D_NONE }, //PW3 = 2(600us )���¡�
	{D_NONE,  D_NONE,   D_NONE,   D_NONE,  D_NONE,   D_NONE }, //PW3 = 3(900us )���¡�
	{D_NONE,  D_NONE,   D_NONE,   D_NONE,  D_NONE,   D_NONE }, //PW3 = 4(1200us)���¡�
	{D_NONE,  D_NONE,   D_NONE,   D_NONE,  D_NONE,   D_NONE }  //PW3 = 5(1500us)���¡�
};

/*******************************************************************************
��0x05�� ���ͼ������ ��ģ���ࡿ    ���� PW0 PW1 PW2 PW3(������û��)
         ˵��: �����������ж� ������û��PW3���룬û��PW3��Ϊ����������Ϊ������ 
               ���͸��ݵ�ַ �� PW0 ��������б�����������������ģ����
                                   ��ַΪ 1  - 127    PW0 ���� 900us;
                                   ��ַΪ 128- 254    PW0 ���� 300us    
*******************************************************************************/

#if 0
//ģ������          ����
#define M_NONE      0x1f     //�����ڵ�һ��ֵ��PW3 = 0ʱ

#define M_SRSBQB    0x10     //����ģ�顢�ֶ�������ť������˩���ð�ť    <----��
#define M_SC        0x18     //���ģ��                                  <----��
#define M_SRSC      0x19     //�������ģ��                              <----��
#define M_BZXXQ     0x1a     //��ַѶ����                                <----��

#define NM_SR       0x10     //������ģ��					72
#define NM_SB       0x11     //���ֶ�������ť			61
#define NM_QB       0x12     //������˩���ð�ť		62
#define NM_SC       0x18     //�����ģ��					73
#define NM_SRSC     0x19     //���������ģ��			74
#define NM_BZXXQ    0x1a     //�±�ַѶ����				82
#define NM_CDI      0x13     //��CDIģ��						75
#define NM_ZYSR     0x10     //��ר������ģ��			72
#define NM_MD       0x19     //�ŵ�060430�����������	74
#define NM_GBQH     0x08     //�㲥�л�ģ��				166
#define NM_HZXS     0x0f     //������ʾ��					10
#define NM_JJQT     0x20     //������ͣ��ť				65
#define NM_ZDZH     0x21     //�Զ�ת������				242
#endif


/**************************** ģ�����Ͷ��ձ�  *********************************/
const unsigned char ModTypePW4PW3[6][5] = {
//     |  PW3   |  PW3    |  PW3     | PW3    |  PW3    |
//     |0(0us)  |1(300us) | 2(600us) |3(900us)|4(1200us)|
	{M_NONE,   M_SRSBQB, M_SC,     M_SRSC,  M_BZXXQ }, //PW4 = 0(0us   )���ϡ�
	{M_NONE,   NM_SR,    NM_SC,    NM_SRSC, NM_BZXXQ}, //PW4 = 1(300us )���¡�
	{M_NONE,   NM_SB,    NM_JJQT,  M_NONE,  NM_MD   }, //PW4 = 2(600us )���¡�
	{M_NONE,   NM_QB,    NM_GBQH,  M_NONE,  M_NONE  }, //PW4 = 3(900us )���¡�
	{M_NONE,   NM_CDI,   NM_ZDZH,  M_NONE,  NM_HZXS }, //PW4 = 4(1200us)���¡�
	{M_NONE,   NM_ZYSR,  M_NONE,   M_NONE,  M_NONE  }, //PW4 = 5(1500us)���¡�
};

uc8 detector_bit_map[32] = {
	0x00, 0x00, 0xF0, 0xC3, 0x0F, 0x0F, 0x3C, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00
};

uc8 module_bit_map[32] = {
	0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
	0x00, 0x0F, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x00, 0x00, 0x00, 0x40, 0x10, 0x00, 0x80,
	0x48, 0xA4, 0x45, 0x00, 0x00, 0xC0, 0x01, 0x00
};

/*
u8 type_from_gb(u8 type)
{
	u8 temp;
	
	switch (type)
	{
	case 23:
		temp = 0x00;
		break;
	case 31:
		temp = 0x01;
		break;
	case 50:
		temp = 0x03;
		break;
	case 102:
		temp = 0x04;
		break;
	case 40:
		temp = 0x05;
		break;
	case 235:
		temp = 0x07;
		break;
	case 166:
		temp = 0x08;
		break;
	case 10:
		temp = 0x0f;
		break;
	case 72:
		temp = 0x10;
		break;
	case 61:
		temp = 0x11;
		break;
	case 62:
		temp = 0x12;
		break;
	case 75:
		temp = 0x13;
		break;
	case 73:
		temp = 0x18;
		break;
	case 74:
		temp = 0x19;
		break;
	case 82:
		temp = 0x1a;
		break;
	case 65:
		temp = 0x20;
		break;
	case 236:
		temp = 0x21;
		break;
	default:
		temp = 0x1f;
		break;	
	}

	return temp;
}


u8 type_to_gb(u8 type)
{
	u8 temp;
	
	switch (type)
	{
	case 0x00:
		temp = 23;
		break;
	case 0x01:
	case 0x02:
		temp = 31;
		break;
	case 0x03:
		temp = 50;
		break;
	case 0x04:
		temp = 102;
		break;
	case 0x05:
		temp = 40;
		break;
	case 0x07:
		temp = 241;
		break;
	case 0x08:
		temp = 166;
		break;
	case 0x0f:
		temp = 10;
		break;
	case 0x10:
		temp = 72;
		break;
	case 0x11:
		temp = 61;
		break;
	case 0x12:
		temp = 62;
		break;
	case 0x13:
		temp = 75;
		break;
	case 0x18:
		temp = 73;
		break;
	case 0x19:
		temp = 74;
		break;
	case 0x1a:
		temp = 82;
		break;
	case 0x20:
		temp = 65;
		break;
	case 0x21:
		temp = 242;
		break;
	default:
		temp = 0xff;
		break;	
	}

	return temp;
}
*/

/*
  *   ��̽����
 */
BOOL is_detector(u8 type)
{
	u8 byte = type / 8;
	u8 bit  = type % 8;

	if (detector_bit_map[byte] & (1 << bit))
		return TRUE;
	else
		return FALSE;
}

/******************************************************************************
                       ̽��������������
 ******************************************************************************

0. ��PW0�� ̽���������ж�     ��ַΪ1-127��PW0����300us �� ��ַΪ128-254��PW0����900usΪ̽����
                              ��ַΪ1-127��PW0����900us �� ��ַΪ128-254��PW0����300usΪģ��
   ��PW1�� ̽����״̬��־     300us :̽����������600us : ̽����Ԥ��
                              900us :̽�����𾯣�1200us:̽��������
                              1500us:̽�������ҹ��ϡ�
         
1. Ѳ�죺��������̽��������Ѳ�죬����ʵ�ֶ�̽������������ء� 
         ��ÿ���λΪ 00�� 
         ��̽�������յ�����������λش� PW0��PW1��  
         ��PW1 Ϊ̽����״̬��־��         
2. ģ������⣺�鿴̽��������ʱ�۲�ģ����ֵ�ı仯�� 
         ��ÿ���λΪ 01�� 
         ��̽�������յ�����������λش� PW0��PW1��PW2�� 
         ��PW1 Ϊ̽����״̬��־��
         ��PW2 Ϊ̽����ģ����ֵ����Χ�����������루200��8.79uS�� �� 
3. ������⣺�������Ͳ鿴̽��������ʱ�ı�����ֵ�� 
         ��ÿ���λΪ 02�� 
         ��̽�������յ�����������λش� PW0��PW1��PW2�� 
         ��PW1 Ϊ̽����״̬��־��
         ��PW2 Ϊ������ֵ����Χ�����������루�Ը���̽���� 103��8.79uS�� �� 
4. ��ȷ�ϣ�̽������⵽�𾯡�������ȷ�Ϻ󣬿�������ͣ���͸��������Ѳ����ں���״̬��
             ���𾯻ָ���̽����ȷ�ϵƴ���һ������һ����״̬��̽�������յ�Ѳ���������������ƣ��ָ�����״̬�� 
         ��ÿ���λΪ 0AH�� 
         ��̽�������յ�����������λش� PW0��PW1��PW2�� 
         ��PW1 Ϊ̽����״̬��־��
         ��PW2 Ϊ 300��S��̽����δȷ�ϻ𾯣�PW2 Ϊ 600��S��̽����ȷ�ϻ𾯡� 
5. �۲챳����ֵ���۲�̽�����ı����⣬��ȷ��̽���·��һ���ԡ� 
         ��ÿ���λΪ 03�� 
         ��̽�������յ�����������λش� PW0��PW1��PW2�� 
         ��PW1 Ϊ̽����״̬��־��
         ��PW2 Ϊ������ֵ����Լ 20��8.79uS�� �� 
6. �ı�����ֵ���������������������̽�����ı�����ֵ�� 
         ��ÿ���λΪ 04�� 
         ��̽�������յ�����������λش� PW0��PW1��PW2�� 
         ��PW1 Ϊ̽����״̬��־��
         ��PW2 ���д�뱳����ֵʧ�ܻ�������Ϊ 300��S�����д�뱳����ֵ�ɹ���������Ϊ 600��S�� 
7. ͨ������������������Ͳ��ԣ� 
         ��ÿ���λΪ 05�� 
         ��̽�������յ�����������λش� PW0��PW1��PW2��PW3(���������룬����������) 
         ��PW1 Ϊ̽����״̬��־�� 
         ��PW2 (ֻ��PW2)��ʾ��ͬ��̽���� 300��S ��ʾ��磬600��S ��ʾ��£�900��S ��ʾ����̽������1200��S
             ��ʾ���¸���̽������1500 ��ȼ����̽������
         ��PW2 PW3��PW3�л��룩 �����̽�������Ͷ��ձ�
8.���Ѳ��ָ��ø�ָ��Ѳ���ֳ��豸ʱ���ֳ��豸��������ָ���00����һ�����������������;��
         ��ÿ���λΪ 0D��
         ��̽�������յ�����������λش� PW0��PW1��PW2��
9��(��չ��)ͨ�������̽���������Ƚ��е��� 
         ��ÿ���λΪ 0BH�� 
         ��̽�������յ�����������λش� PW0��PW1��PW2�� 
         ��PW1 Ϊ̽����״̬��־��PW2 Ϊ 300��S��������δ������PW2 Ϊ 600��S���������ѵ����� 
10. (��չ��)ͨ���������������Զ�̲��� 
         ��ÿ���λΪ 0C�� 
         ��̽�������յ�����������λش� PW0��PW1��PW2�� 
         ��PW1 Ϊ̽����״̬��־��PW2 Ϊ 300��S������δ�򿪣�PW2 Ϊ 600��S�����Դ򿪡�

******************************************************************************
                       ģ������������
*******************************************************************************
0. ��PW0�� ģ�������ж�     ��ַΪ1-127��PW0����300us �� ��ַΪ128-254��PW0����900usΪ̽����
                            ��ַΪ1-127��PW0����900us �� ��ַΪ128-254��PW0����300usΪģ��
   ��PW1�� ����ڵ���ӡ�300us����, 600us����ڵ㿪·, 900us����ڵ��·, 1200us�𾯡�
   ��PW2�� ����ڵ���ӣ�300us����, 600us����ڵ㿪·, 900us����ڵ��·, 1200us��, 1500us����ڵ㶯����

1. Ѳ�죺��������ģ�����Ѳ�죬����ʵ�ֶ�ģ���������ء� 
        ��ÿ���λΪ 00�� 
        ��ģ����յ�����������λش� PW0��PW1��PW2����λ����ͬ��  
        ����ģ��: PW1����ڵ������ǰ��ʾ��PW2���������̶�Ϊ300us
        ���ģ��: PW1���������̶�Ϊ300us�� PW2����ڵ������ǰ��ʾ��
        �������ģ��: PW1����ڵ������ǰ��ʾ��PW2����ڵ������ǰ��ʾ��������ڵ㶯��ʱ������ڵ����Ӧ�б仯��

2. ͨ����������ģ�顢�������ģ�� 
        ��ÿ���λΪ 06�� 
        ��ģ����յ�����������λش� PW0��PW1��PW2����λ����ͬ�� 
3. ͨ������ر����ģ�顢�������ģ�� 
        ��ÿ���λΪ 07�� 
        ��ģ����յ�����������λش� PW0��PW1��PW2����λ����ͬ�� 
4. ͨ������ʹ����ģ�鴦�����������ʽ�������㴦��һ�뿪һ���״̬ 
        ��ÿ���λΪ 08�� 
        ��ģ����յ�����������λش� PW0��PW1��PW2����λ����ͬ��  
5��ģ�鶯��ѡͨ��� 
        ��ÿ���λΪ 09�� 
        ��ģ����յ�����������λش� PW0��PW1��PW2��PW3��Զ�̲����Ƿ� ����֤��
          ������ڵ��·ʱ��PW1��PW2 �Ƿ��Ϊ��·״̬��PW3 ��ȷ�ϵ�ǰ״̬ΪԶ�̲�
          �ԣ�����Ϊ 900��S 
       ��ʹ����豸������ģ����������������ͬ������ָ�������£�������������Ϊ
       ѡָͨ����ܶ���������ڵ㶯������ֻ���ڵ���������ʱ����仯�� 
6.  ͨ������������������Ͳ��ԣ� 
        ��ÿ���λΪ 05�� 
        ��ģ����յ�����������λش� PW0��PW1��PW2��PW3��PW4(���������룬����������)
        ��PW0��PW1��PW2����λ����ͬ��  
        ��PW3(ֻ��PW3)��ֵͬ��ʾ��ͬ��ģ�顣300us��ʾ����ģ�飨����ģ�顢�ֶ�������ť������˨��ð�ť��
                                            600us����ģ�飬900us��ʾ�������ģ�顣 
        ��PW3 PW4��PW4�л��룩 �����ģ�����Ͷ��ձ� 
7�����Ѳ��ָ��ø�ָ��Ѳ���ֳ��豸ʱ���ֳ��豸��������ָ���00����һ�����������������;��
         ��ÿ���λΪ 0D��
         ��ģ����յ�����������λش� PW0��PW1��PW2��

*******************************************************************************/