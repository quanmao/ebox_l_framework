#include "ebox.h"
#include "math.h"
#include "bsp_ebox.h"
/**
	*	1	此例程演示了PWM操作
	*	2	PWM输出频率1Khz,占空比可调的矩形波
	*	3	实际运行时LED渐变
	*   4	可用资源请参考ebox_pwm.h说明
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"PWM example"
#define EXAMPLE_DATE	"2018-08-08"

float x;
uint16_t y;
E_PWM pwm1(PWM);
void setup()
{
    eboxInit();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    pwm1.begin(47000000, 500);
//    pwm1.setPolarity(1);//set output polarity after compare
    UART.printf("max frq = %dKhz\r\n", pwm1.getMaxFrequency() / 1000);
    UART.printf("max frq = %.2fMhz \r\n", pwm1.getSourceClock()/1000000.0);
}
void test(void)
{
    while(1)
    {
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2000 - (sin(x) + 1) * 1000;
        pwm1.setDutyCycle(y);
        delayMs(10);
    }
}
