#include <reg52.h>

// 延时函数声明
void Delay_ms(unsigned int ms);

// -------------------------------------------------------------
// "Hello World" 字符字模表（每列对应一个字节，高电平点亮，自顶向下 D7~D0）
// 字符点阵尺寸：大部分字符宽 5~6 列，末尾加 0x00 作为字符间隔
// -------------------------------------------------------------
unsigned char code Scroll_Text[] = {
    // 前置 8 列空白，方便首个字符从屏幕右侧完全滚入
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // 'H'
    0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00,
    // 'e'
    0x38, 0x54, 0x54, 0x54, 0x18, 0x00,
    // 'l'
    0x00, 0x41, 0x7F, 0x01, 0x00, 0x00,
    // 'l'
    0x00, 0x41, 0x7F, 0x01, 0x00, 0x00,
    // 'o'
    0x38, 0x44, 0x44, 0x44, 0x38, 0x00,
    // ' ' (空格)
    0x00, 0x00, 0x00,
    // 'W'
    0x7F, 0x20, 0x18, 0x20, 0x7F, 0x00,
    // 'o'
    0x38, 0x44, 0x44, 0x44, 0x38, 0x00,
    // 'r'
    0x7C, 0x08, 0x04, 0x04, 0x08, 0x00,
    // 'l'
    0x00, 0x41, 0x7F, 0x01, 0x00, 0x00,
    // 'd'
    0x18, 0x24, 0x24, 0x28, 0x7F, 0x00,

    // 后置 8 列空白，让最后一个字符完全滚出屏幕
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 计算字模总列数
#define TOTAL_COLS (sizeof(Scroll_Text) / sizeof(Scroll_Text[0]))

void main(void)
{
    unsigned int offset;   // 当前滚动窗口起始列索引
    unsigned char scan_i;  // 8列动态扫描索引
    unsigned char frame;   // 每步滚动的停留帧数（控制流动速度）

    while (1)
    {
        // offset 从 0 滑动到最后一个窗口起始位置
        for (offset = 0; offset <= TOTAL_COLS - 8; offset++)
        {
            // 每个位移步长刷新 20 帧（大约持续 160ms，数字越小滚动越快）
            for (frame = 0; frame < 20; frame++)
            {
                // 动态刷新当前 8x8 视窗里的 8 列
                for (scan_i = 0; scan_i < 8; scan_i++)
                {
                    // 1. 消隐（关闭当前数据输出，防拖影）
                    P0 = 0x00;

                    // 2. 74HC154 选通通道切换 (输入 A~D 对应 P1.0~P1.3)
                    P1 = (P1 & 0xF0) | (scan_i & 0x0F);

                    // 3. 送出当前列的字模数据 (视窗偏移 + 当前列)
                    // 如果点阵极性反了（亮暗颠倒），改为: P0 = ~Scroll_Text[offset + scan_i];
                    P0 = Scroll_Text[offset + scan_i];

                    // 4. 扫描保持延时 1ms
                    Delay_ms(1);
                }
            }
        }
    }
}

/**
 * @brief 毫秒延时函数（基于 11.0592MHz 或 12MHz 晶振）
 */
void Delay_ms(unsigned int ms)
{
    unsigned int x, y;
    for (x = ms; x > 0; x--)
        for (y = 110; y > 0; y--);
}