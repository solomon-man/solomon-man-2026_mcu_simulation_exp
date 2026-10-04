#include <reg52.h>
#include <intrins.h>

// 引脚定义（依据原理图连接）
sbit LCD_RS = P3^0;
sbit LCD_RW = P3^1;
sbit LCD_E  = P3^2;
#define LCD_DATA P2

// 延时函数（约 1ms @ 12MHz）
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 114; j++);
}

// 简易短延时
void delay_us(unsigned char us) {
    while (us--) {
        _nop_();
    }
}

// 写指令函数
void LCD_WriteCmd(unsigned char cmd) {
    LCD_RS = 0;       // 0: 指令寄存器
    LCD_RW = 0;       // 0: 写操作
    LCD_DATA = cmd;   // 送出指令数据
    delay_us(5);
    LCD_E = 1;        // 产生使能高脉冲
    delay_us(10);
    LCD_E = 0;
    delay_ms(2);      // 指令执行需要时间
}

// 写数据函数
void LCD_WriteData(unsigned char dat) {
    LCD_RS = 1;       // 1: 数据寄存器
    LCD_RW = 0;       // 0: 写操作
    LCD_DATA = dat;   // 送出显示数据
    delay_us(5);
    LCD_E = 1;        // 产生使能高脉冲
    delay_us(10);
    LCD_E = 0;
    delay_ms(1);
}

// LCD1602 初始化
void LCD_Init(void) {
    delay_ms(15);
    LCD_WriteCmd(0x38); // 8位数据接口，两行显示，5x7点阵
    delay_ms(5);
    LCD_WriteCmd(0x38);
    LCD_WriteCmd(0x0C); // 开启显示，光标不显示，不闪烁
    LCD_WriteCmd(0x06); // 写入数据后光标自增，屏幕不移动
    LCD_WriteCmd(0x01); // 清屏
    delay_ms(5);
}

// 在指定行列显示字符串
// line: 行号 (1 或 2)
// col:  列号 (0 ~ 15)
void LCD_ShowString(unsigned char line, unsigned char col, char *str) {
    unsigned char addr;
    if (line == 1) {
        addr = 0x80 + col; // 第一行起始地址 0x80
    } else {
        addr = 0xC0 + col; // 第二行起始地址 0xC0
    }
    LCD_WriteCmd(addr);
    
    while (*str != '\0') {
        LCD_WriteData(*str);
        str++;
    }
}

void main(void) {
    LCD_Init(); // 初始化液晶屏
    
    // 第一行显示 "   Welcome to   "
    LCD_ShowString(1, 3, "Welcome to");
    
    // 第二行显示 "Harbin Institute"（占满16格）
    LCD_ShowString(2, 0, "Harbin Institute");
    
    while (1) {
        // 主循环保持显示
    }
}
