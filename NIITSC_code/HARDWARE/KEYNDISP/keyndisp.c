#include "stm32f4xx.h"                  // Device header
#include "oled.h"
#include "keyndisp.h"
#include "ops.h"
#include "flags.h"
#include "motor.h"
#include "delay.h"
#include "openmv.h"
#include "control.h"

static int min_page = -1, max_page = 3, min_var_page = 0, max_var_page = 3;
int flag_varmod = 0;
int flag_page = 0;
int flag_var_page = 0;
int row = 1;

//Private:
/*
	key_read_col() for scanning the current state of line 4 5 6
*/
static int key_read_col(void)
{
	if(COL_SCAN1 == 1) return 1;
	if(COL_SCAN2 == 1) return 2;
	if(COL_SCAN3 == 1) return 3;
	return 0;
}

/* 
	key_scan_position() pulls all input pins in one row to high 
	and detect which column's ouput is high, in order to detect the button pressed,
  even POTENCIALLY partially support multiple keys.
*/
static int key_scan_position(void)
{
	int column_pressed = 0;
	
	ROW_RESET2;
	ROW_RESET3;
  
	ROW_SET1;
	column_pressed = key_read_col();
	if (column_pressed != 0)
		return column_pressed;
	ROW_RESET1;
	
	ROW_SET2;
	column_pressed = key_read_col();
	if (column_pressed != 0)
		return column_pressed + 3;
	ROW_RESET2;
	
	ROW_SET3;
	column_pressed = key_read_col();
	if (column_pressed != 0)
		return column_pressed + 6;
	ROW_RESET3;
	
	return 0;
}

/*
	This function act as a excutor of true logics of different variable changing functions, which is excuted in func page_logic() as its sub logic.
	All sub logic functions have to take variable d as the key press direction. Left:0, Right:1, Stub Error: -1.
*/
static void row_logic(int row, int pressed_key, void (*logic1)(int d), void (*logic2)(int d), void (*logic3)(int d), void (*logic4)(int d))
{
	int direction = 0;
	if (pressed_key == 4)
		direction = -1;
	if (pressed_key == 6)
		direction = 1;
	
	switch(row){
		case 1:
			(*logic1)(direction);
			break;
		case 2:
			(*logic2)(direction);
			break;
		case 3:
			(*logic3)(direction);
			break;
		case 4:
			(*logic4)(direction);
			break;
	}
}

static void do_nothing(int d){}

static void changeIntVar(int direction, int *var, int min_lim, int max_lim, int change_by)
{
	if (direction == -1 && *var - change_by >= min_lim )
	{
		*var -= change_by;
	}
	if (direction == 1 && *var + change_by <= max_lim)
	{
		*var += change_by;
	}
}

/*
	These are functions for each page&row.
	d = -1 for L, d = -2 for R.
*/
static void p0_r1(int d)
{
	if (flag_start == 0)
	{
		 flag_start = 1;
		 flag_page = 1;
	} else {
		 flag_start = 0;
	}
}
	
	
/*
	Switching between pages, excute functinons changing the variables using func row_logic().
	Take this as the main commad center for variable changeing based on what page is currently showing (Handling left/right button press).
*/
static void page_logic(int page, int pressed_key)
{
	
	if (!flag_varmod){
		switch (flag_page)
		{
			case 0://??0?
				row_logic(row, pressed_key, p0_r1, do_nothing, System_Test, do_nothing);
				break;
			case -1://-1?
				do_nothing(0);
				break;
			case 1://1?
				do_nothing(0);
				break;
			case 2://1?
				do_nothing(0);
				break;
		}
	} else {
		switch (flag_var_page)
		{
			case 0://??0?
				do_nothing(0);
				break;
			case 1://1?
				do_nothing(0);
				break;
			case 2://2?
				do_nothing(0);
				break;
			case 3://3?
				do_nothing(0);
				break;
		}
	}
	
}

static int key_logic_process(int pressed_key)
{
	switch(pressed_key)
	{
		case 0:
			return 0;
		case 1:
			if(!flag_varmod)
				flag_varmod = 1;
			else
				changeIntVar(-1, &flag_var_page, min_var_page, max_var_page, 1); //varmod????
			break;
		case 2:
			//??
			if (row > 1) 
				row--;
			break;
		case 3:
			if(!flag_varmod)
				flag_varmod = 1;
			else
				changeIntVar(1, &flag_var_page, min_var_page, max_var_page, 1); //varmod ????
			break;
		case 7:
			if (flag_varmod)
				flag_varmod = 0;
			else
				changeIntVar(-1, &flag_page, min_page, max_page, 1); //????
			break;
		case 8:
			//??
			if (row < 4) 
				row++;
			break;
		case 9:
			if (flag_varmod)
				flag_varmod = 0;
			else
				changeIntVar(1, &flag_page, min_page, max_page, 1);
			break;
		default:
			page_logic(flag_page, pressed_key);
	}
	return 1;
}
//Public:

/*
	m_p_flag_start for starting flag of the whole system
*/
void KEY_Init()
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10;        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);						 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);		
}

/*
	Supposed to be called in main function for refreshing the displayed content. Also calls key pressing functions.
*/
void Input_Disp_Loop(void)
{
	//Refreshing display content while reducing blinking.
	static int oled_rfsh_itr = 0;
	if (oled_rfsh_itr >= 100) //OLED_Refreash_freq
	{
		OLED_Clear();
		oled_rfsh_itr = 0;
	}
	oled_rfsh_itr++;
	
	//Cursor display
	OLED_Clear();
	OLED_ShowChar(row, 1, '-');
	
	//Key press detection and press lock
	static int press_down_lock = 0; // No more press is going to be readed when long pressed.
	int pressed_key = key_scan_position();
	if(!press_down_lock && pressed_key)
	{
		OLED_Clear();
		press_down_lock = 1;
		key_logic_process(pressed_key);
	}
	else press_down_lock = pressed_key; // Reset the lock after no press input.
	
	//Content display
	if (!flag_varmod){
		switch(flag_page) {
			case -1:
				OpenMV_Display_Specs();
				OLED_ShowString(4, 14, "p-1");
				break;
			case 0:
				OLED_ShowString(1, 2, "Start:");
				OLED_ShowSignedNum(1, 8, flag_start, 2);

				OLED_ShowString(3, 2, "Run test");
				OLED_ShowString(4, 14, "p0");
				break;
			case 1:
				OPS_Display_Specs();
				OLED_ShowString(4, 14, "p1");
				break;
			case 2:
				Control_Display_Specs();
				OLED_ShowString(4, 14, "p2");
				break;
			case 3:
				OLED_ShowString(4, 14, "p3");
				break;
		}
	} else {
		switch(flag_var_page) {
			case 0:
				OLED_ShowString(4, 14, "vp0");
				break;
			case 1:
				OLED_ShowString(4, 14, "vp1");
				break;
			case 2:
				OLED_ShowString(4, 14, "vp2");
				break;
			case 3:
				OLED_ShowString(4, 14, "vp3");
				break;
		}
	}
	
}

void System_Test(int d)
{
	Set_Speed_All(50, 0, 0, 0);
	delay_ms(500);
	Set_Speed_All(0, 50, 0, 0);
	delay_ms(500);
	Set_Speed_All(0, 0, 50, 0);
	delay_ms(500);
	Set_Speed_All(0, 0, 0, 50);
	delay_ms(500);
	Set_Speed_All(0, 0, 0, 0);
	delay_ms(200);
}
