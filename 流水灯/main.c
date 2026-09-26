#include <REGX52.H>
#include <stdio.h>
#include <intrins.H>


void Delay100ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	i = 180;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}


void system_init()//初始化
{
	P2 = 0x00;
}

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
		P2 = temp;//共阴极接法
		temp_old = temp;
	}
		
}

void led_watering()
{
  static unsigned char i = 0;
	unsigned char j;
	
	// 刷新显示缓冲区
	for (j = 0; j < 8; j++)
	{
		led_disp(j,(j==i));
		Delay100ms();
	}
	
	// 移向下一个 LED
	if (i++ >= 8)
	{
		i = 0;
	}

}

//

int main()
{
	system_init();
	
	while(1)
	{
		led_watering();
	}
	
}



