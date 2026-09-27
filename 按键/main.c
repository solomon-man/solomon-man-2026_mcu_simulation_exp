#include <REGX52.H>
#include <stdio.H>
#include <intrins.H>
#include <key.H>
#include <led.H>
#include <init.H>

volatile unsigned char key_slow;//按键减速专用
unsigned char key_val,key_down,key_up,key_old;
volatile unsigned char led_pos;
unsigned char led[4] = {0};
volatile unsigned char led_pwm = 0;

unsigned char mode;//0-流水灯 1-呼吸灯
unsigned char gear = 0;//呼吸灯挡位，共5档
volatile unsigned int Time_500ms;

bit led_flash = 0;//LED闪烁使能
volatile bit Time_500ms_flag;
bit led_water = 0;//0-从上到下 1-从下到上

//按键处理函数
void key_proc()
{
	//每10ms扫描一次，实现软件消抖
	if(key_slow)return;
	key_slow = 1;
	
	key_val = key_read();
	key_down = key_val & (key_old ^ key_val);
	key_up = ~key_val & (key_old ^ key_val);
	key_old = key_val;
	
	//key1 模式切换
	if(key_down==1)
	{
		if(++mode == 2)mode = 0;
	}
		
	//key2 流水方向切换
	if(key_down==2 && mode == 0)
	{
		led_water = ~led_water;
	}
	
	//key3和key4同时按下：切换0.5秒闪烁
	if(key_down==0x0c && mode == 1)
	{
		led_flash = ~led_flash;
	}
	else if(key_down==4 && mode == 1)
	{
		if(++gear == 5)gear = 4;
	}
	
	//key3 增强呼吸灯亮度，key4 减弱亮度
	else if(key_down==8 && mode == 1)
	{
		if(--gear == 255)gear = 0;
	}
	
}

void led_proc()
{
	unsigned char i;
	static unsigned char water_pos = 0;
	static bit Time_500ms_old = 0;
	
	switch(mode)
	{
		case 0://流水灯
			if(Time_500ms_flag != Time_500ms_old)
			{
				Time_500ms_old = Time_500ms_flag;
				for(i=0;i<4;i++)led[i] = (i == water_pos);

				if(led_water)
				{
					if(++water_pos == 4)water_pos = 0;
				}
				else
				{
					if(water_pos == 0)water_pos = 3;
					else water_pos--;
				}
			}
		break;
		
		case 1://呼吸灯
			for(i=0;i<4;i++)
			{
				led[i] = (led_pwm < gear*25) && (!led_flash || Time_500ms_flag);
			}
		break;
	}
}

void Timer0_Init(void)
{
    TMOD &= 0xF0;       // 清除定时器0控制位
    TMOD |= 0x01;       // 定时器0，方式1，16位定时器

    // 11.0592 MHz 标准12T 51，机器周期约为1.085 us
    // 1 ms计数约922次
    TH0 = 0xFC;
    TL0 = 0x66;

    TF0 = 0;            // 清除溢出标志
    ET0 = 1;            // 允许定时器0中断
    EA  = 1;            // 开总中断
    TR0 = 1;            // 启动定时器0
}

void Timer0_Service() interrupt 1
{
	TH0 = 0xFC;
	TL0 = 0x66;

	if(++key_slow == 10)key_slow = 0;
	led_disp(led_pos,led[led_pos]);
	if(++led_pos == 4)led_pos = 0;
	
	if(++led_pwm==100)led_pwm = 0;
	
	if(++Time_500ms==500)
	{
		Time_500ms = 0;
		Time_500ms_flag ^= 1;
	}
}

int main()
{

	system_init();
	Timer0_Init();
	
	while(1)
	{
		key_proc();
		led_proc();
	}
}



