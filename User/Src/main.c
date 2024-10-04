/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * Description of project
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include "ts_calibration.h"


/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static int GetUserButtonPressed(void);
static int GetTouchState (int *xCoord, int *yCoord);





/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
	HAL_IncTick();
}

volatile int timer_select = 0;
volatile int colour_select = 0;

void EXTI0_IRQHandler(void){
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

	timer_select = !timer_select;
}

void EXTI3_IRQn_IRQHandler(void){
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);

	colour_select = !colour_select;
}


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize LCD and touch screen */
	LCD_Init();
	TS_Init(LCD_GetXSize(), LCD_GetYSize());
	/* touch screen calibration */
	//	TS_Calibration();

	/* Clear the LCD and display basic starter text */
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_SetBackColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font20);
	// There are 2 ways to print text to screen: using printf or LCD_* functions
	LCD_DisplayStringAtLine(0, "    HTL Wels");
	// printf Alternative
	LCD_SetPrintPosition(1, 0);
	printf(" Fischergasse 30");
	LCD_SetPrintPosition(2, 0);
	printf("   A-4600 Wels");

	LCD_SetFont(&Font8);
	LCD_SetColors(LCD_COLOR_MAGENTA, LCD_COLOR_BLACK); // TextColor, BackColor
	LCD_DisplayStringAtLineMode(39, "copyright xyz", CENTER_MODE);

	GPIO_InitTypeDef led;

	led.Alternate = 0;
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pull = GPIO_NOPULL;
	led.Speed = GPIO_SPEED_FAST;

	led.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOG, &led);

	int cnt = 0;
	int cnt2 = 0;

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	GPIO_InitTypeDef User;
	User.Alternate = 0;
	User.Mode = GPIO_MODE_IT_RISING;
	User.Pull = GPIO_NOPULL;
	User.Speed = GPIO_SPEED_FAST;

	User.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &User);

	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	GPIO_InitTypeDef Colour;
	Colour.Alternate = 0;
	Colour.Mode = GPIO_MODE_IT_RISING;
	Colour.Pull = GPIO_PULLDOWN;
	Colour.Speed = GPIO_SPEED_FAST;

	Colour.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOC, &Colour);





	/* Infinite loop */
	while (1)
	{
		//execute main loop every 100ms
		HAL_Delay(100);

		// display timer

		LCD_SetFont(&Font20);

		if(colour_select == 1){
			LCD_SetTextColor(LCD_COLOR_BLUE);
			HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);,
		}else{
			LCD_SetTextColor(LCD_COLOR_RED);
		}

		LCD_SetPrintPosition(5, 0);
		printf("   Timer: %.1f", cnt/10.0);
		LCD_SetPrintPosition(7, 0);
		printf("   Timer2: %.1f", cnt2/10.0);

		if(timer_select == 0){
			cnt++;
		}else if(timer_select == 1){
			cnt2++;
		}

	}
}

/**
 * Check if User Button has been pressed
 * @param none
 * @return 1 if user button input (PA0) is high
 */


/**
 * Check if touch interface has been used
 * @param xCoord x coordinate of touch event in pixels
 * @param yCoord y coordinate of touch event in pixels
 * @return 1 if touch event has been detected
 */



