#include <init.H>

void system_init()
{
	P2 = 0xff;//LED初始化：全部熄灭
	P1 |= 0xf0;//P1.4-P1.7置1作为按键输入
}
