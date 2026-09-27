#include <key.H>

unsigned char key_read()//按键扫描
{
	unsigned char temp = 0;
	
	/* 使用位掩码，支持边沿检测及组合键。 */
	if(P1_4==0)temp |= 0x01;//0000_0001
	if(P1_5==0)temp |= 0x02;//0000_0010
	if(P1_6==0)temp |= 0x04;//0000_0100
	if(P1_7==0)temp |= 0x08;//0000_1000
	
	return temp;
}
