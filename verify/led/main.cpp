extern "C" void delay_1s();
extern "C" void _exit_exceution();

//时间
volatile int time = 0;
// led io 地址
volatile int *led_base = (int *)0x3000;
volatile int *led;

int main()
{
    //初始化 LED 为0
    led = led_base;
    for (int i = 0; i < 8; i++)
        *(led + i) = 0;
    // led 偏移
    int i = 0;
    while (1)
    {
        //点亮LED 1秒
        *led = 1;
        delay_1s();
        //熄灭LED
        *led = 0;
        //下一个LED 的地址 点亮往后第三个LED
        i = (i + 3) % 8;
        led = led_base + i;
        // time++;
        // if(time > 30 )
        //  _exit_exceution();
    }
}