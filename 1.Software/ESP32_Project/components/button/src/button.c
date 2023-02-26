#include "button.h"
/*******************************************************************
 *                          变量声明                               
 *******************************************************************/
#define KEY1_IO GPIO_NUM_21
#define KEY2_IO GPIO_NUM_46
#define KEY3_IO GPIO_NUM_45

Button_t Button_UP;
Button_t Button_DOWN;
Button_t Button_SEL;

uint8_t Button_Value = BT_NONE;
//按键一单击
void Btn1_Down_CallBack(void *btn)
{
	Button_Value = BT1_DOWN;
	//printf("Button 1 down!\n");
}
//按键一双击
void Btn1_Double_CallBack(void *btn)
{
	Button_Value = BT1_DOUBLE;
	//printf("Button 1 Double!\n");
}
//按键一双击
void Btn1_Long_CallBack(void *btn)
{
	Button_Value = BT1_LONG;
	//printf("Button 1 Long!\n");
}
void Btn1_Long_Free_CallBack(void *btn)
{
	Button_Value = BT1_LONGFREE;
	//printf("Button 1 Long_Free!\n");
}
///////////////////////////////////////////////////////
//按键二单击
void Btn2_Down_CallBack(void *btn)
{
	Button_Value = BT2_DOWN;
	//printf("Button 2 down!\n");
}
//按键一双击
void Btn2_Double_CallBack(void *btn)
{
	Button_Value = BT2_DOUBLE;
	//printf("Button 2 Double!\n");
}
//按键一双击
void Btn2_Long_CallBack(void *btn)
{
	Button_Value = BT2_LONG;
	//printf("Button 2 Long!\n");
}
void Btn2_Long_Free_CallBack(void *btn)
{
	Button_Value = BT2_LONGFREE;
	//printf("Button 2 Long_Free!\n");
}
///////////////////////////////////////////////////////
//按键二单击
void Btn3_Down_CallBack(void *btn)
{
	Button_Value = BT3_DOWN;
}
//按键一双击
void Btn3_Double_CallBack(void *btn)
{
	Button_Value = BT3_DOUBLE;
}
//按键一双击
void Btn3_Long_CallBack(void *btn)
{
	Button_Value = BT3_LONG;
}
void Btn3_Long_Free_CallBack(void *btn)
{
	Button_Value = BT3_LONGFREE;
}

uint8_t Read_KEY1_Level(void)
{
	//printf("IO1:%d\n",gpio_get_level(KEY1_IO));
	return gpio_get_level(KEY1_IO);
}

uint8_t Read_KEY2_Level(void)
{
	//printf("IO2:%d\n",gpio_get_level(KEY2_IO));
	return gpio_get_level(KEY2_IO);
}

uint8_t Read_KEY3_Level(void)
{
	//printf("IO2:%d\n",gpio_get_level(KEY3_IO));
	return gpio_get_level(KEY3_IO);
}

void gpio_init(void)
{
    gpio_config_t io_conf;  // 定义一个gpio_config类型的结构体，下面的都算对其进行的配置

    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;  // 禁止中断  
    io_conf.mode = GPIO_MODE_INPUT;             // 选择输入模式
    io_conf.pin_bit_mask = 1ULL << KEY1_IO;  // 配置GPIO_IN寄存器
    io_conf.pull_down_en = 0;                   
    io_conf.pull_up_en = 1;                    
    gpio_config(&io_conf);                      // 最后配置使能
	io_conf.pin_bit_mask = 1ULL << KEY2_IO;  // 配置GPIO_IN寄存器
	gpio_config(&io_conf);                      // 最后配置使能
	io_conf.pin_bit_mask = 1ULL << KEY3_IO;  // 配置GPIO_IN寄存器
	gpio_config(&io_conf);                      // 最后配置使能
}

void Button_Init(void)
{
	gpio_init();

	Button_Create("Button_UP",//按键名字
				  &Button_UP,//按键句柄
				  Read_KEY1_Level,//按键电平检测函数接口
				  KEY_OFF);//触发电平
	Button_Attach(&Button_UP, BUTTON_DOWN, Btn1_Down_CallBack);			//单击
	//Button_Attach(&Button_UP, BUTTON_DOUBLE, Btn1_Double_CallBack);		//双击
	Button_Attach(&Button_UP, BUTTON_LONG, Btn1_Long_CallBack);			//长按按下
	//Button_Attach(&Button_UP, BUTTON_LONG_FREE, Btn1_Long_Free_CallBack); //长按松开

	Button_Create("Button_DOWN",
				  &Button_DOWN,
				  Read_KEY2_Level,
				  KEY_OFF);
	Button_Attach(&Button_DOWN, BUTTON_DOWN, Btn2_Down_CallBack);			//单击
	//Button_Attach(&Button_DOWN, BUTTON_DOUBLE, Btn2_Double_CallBack);		//双击
	Button_Attach(&Button_DOWN, BUTTON_LONG, Btn2_Long_CallBack);			//长按按下
	//Button_Attach(&Button_DOWN, BUTTON_LONG_FREE, Btn2_Long_Free_CallBack); //长按松开

	Button_Create("Button_SEL",
				  &Button_SEL,
				  Read_KEY3_Level,
				  KEY_OFF);
	Button_Attach(&Button_SEL, BUTTON_DOWN, Btn3_Down_CallBack);			//单击
	//Button_Attach(&Button_SEL, BUTTON_DOUBLE, Btn3_Double_CallBack);		//双击
	Button_Attach(&Button_SEL, BUTTON_LONG, Btn3_Long_CallBack);			//长按按下

	Get_Button_Event(&Button_UP);
	Get_Button_Event(&Button_DOWN);
	Get_Button_Event(&Button_SEL);
}
