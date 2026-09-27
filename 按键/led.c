#include <led.H>

void led_disp(unsigned char addr,unsigned char enable)
{
	static unsigned char temp;
	static unsigned char temp_old;
	
	if(enable)
		temp |= 0x01<<addr;
	else
		temp &= ~(0x01<<addr);
	
	if(temp != temp_old)
	{
		P2 = ~temp;//LED阳极接VCC，低电平点亮
		temp_old = temp;
	}
		
}

