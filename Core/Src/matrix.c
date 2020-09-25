#include "gpio.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = 5;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

//static matrix_row_t read_cols(void);
//static void init_cols(void);
static void unselect_rows(uint8_t row);
static void select_row(uint8_t row);



uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}


uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

/* generic STM32F103C8T6 board */
#ifdef BOARD_GENERIC_STM32_F103
#define LED_ON()    do { palClearPad(GPIOC, GPIOC_LED) ;} while (0)
#define LED_OFF()   do { palSetPad(GPIOC, GPIOC_LED); } while (0)
#define LED_TGL()   do { palTogglePad(GPIOC, GPIOC_LED); } while (0)
#endif

/* Maple Mini */
#ifdef BOARD_MAPLEMINI_STM32_F103
#define LED_ON()    do { palSetPad(GPIOB, 1) ;} while (0)
#define LED_OFF()   do { palClearPad(GPIOB, 1); } while (0)
#define LED_TGL()   do { palTogglePad(GPIOB, 1); } while (0)
#endif

void matrix_init(void)
{
    // initialize row and col
//    init_rows();
//    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }


}

uint8_t matrix_scan(void)
{

    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    	select_row(i);
    	HAL_Delay(1); // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            matrix[i] = matrix_debouncing[i];

            debouncing = DEBOUNCE;
        }
        unselect_rows(i);
    }

    if (debouncing) {
        if (--debouncing) {
        	HAL_Delay(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

void delay_us(void){
	uint8_t n=30;
	while(n--);
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}


inline matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}
//
//void matrix_print(void)
//{
//    print("\nr/c 0123456789ABCDEF\n");
//    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
//        phex(row); print(": ");
//        pbin_reverse16(matrix_get_row(row));
//        print("\n");
//    }
//}
//
///*
//    设置键盘矩阵，每行的引脚，设为输出，默认输出低电平，一共5行
// */
//static void  init_rows(void)
//{
//    // row1
//    palSetPadMode(ROW_PORT_1, ROW_PIN_1, PAL_MODE_OUTPUT_PUSHPULL);
//    palSetPad(ROW_PORT_1, ROW_PIN_1, PAL_LOW);
//    // row2
//    palSetPadMode(ROW_PORT_2, ROW_PIN_2, PAL_MODE_OUTPUT_PUSHPULL);
//    palSetPad((ROW_PORT_2, ROW_PIN_2, PAL_LOW);
//    // row3
//    palSetPadMode(ROW_PORT_3, ROW_PIN_3, PAL_MODE_OUTPUT_PUSHPULL);
//    palSetPad(ROW_PORT_3, ROW_PIN_3, PAL_LOW);
//    // row4
//    palSetPadMode(ROW_PORT_4, ROW_PIN_4, PAL_MODE_OUTPUT_PUSHPULL);
//    palSetPad(ROW_PORT_4, ROW_PIN_4, PAL_LOW);
//    // row5
//    palSetPadMode(ROW_PORT_5, ROW_PIN_5, PAL_MODE_OUTPUT_PUSHPULL);
//    palSetPad(ROW_PORT_5, ROW_PIN_5, PAL_LOW);
//}
//
///* Column pin configuration
//    设置键盘矩阵，每列的引脚，设为输入，用于检测输入电平，一共14列
// */
//static void  init_cols(void)
//{
//    palSetPadMode(COL_PORT_1, COL_PIN_1, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_2, COL_PIN_2, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_3, COL_PIN_3, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_4, COL_PIN_4, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_5, COL_PIN_5, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_6, COL_PIN_6, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_7, COL_PIN_7, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_8, COL_PIN_8, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_9, COL_PIN_9, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_10, COL_PIN_10, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_11, COL_PIN_11, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_12, COL_PIN_12, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_13, COL_PIN_13, PAL_MODE_INPUT_PULLDOWN);
//    palSetPadMode(COL_PORT_14, COL_PIN_14, PAL_MODE_INPUT_PULLDOWN);
//}
//
/* Returns status of switches(1:on, 0:off)
    读取每列的电平，如果检测到高电平，则视为该列的键被按下
*/
matrix_row_t read_cols(void)
{
    return ((HAL_GPIO_ReadPin(COL0_GPIO_Port, COL0_Pin)==1) ? (1<<0):0) |
           ((HAL_GPIO_ReadPin(COL1_GPIO_Port, COL1_Pin)==1) ? (1<<1):0) |
           ((HAL_GPIO_ReadPin(COL2_GPIO_Port, COL2_Pin)==1) ? (1<<2):0) |
           ((HAL_GPIO_ReadPin(COL3_GPIO_Port, COL3_Pin)==1) ? (1<<3):0) |
           ((HAL_GPIO_ReadPin(COL4_GPIO_Port, COL4_Pin)==1) ? (1<<4):0) |
           ((HAL_GPIO_ReadPin(COL5_GPIO_Port, COL5_Pin)==1) ? (1<<5):0) |
           ((HAL_GPIO_ReadPin(COL6_GPIO_Port, COL6_Pin)==1) ? (1<<6):0) |
           ((HAL_GPIO_ReadPin(COL7_GPIO_Port, COL7_Pin)==1) ? (1<<7):0) |
           ((HAL_GPIO_ReadPin(COL8_GPIO_Port, COL8_Pin)==1) ? (1<<8):0) |
           ((HAL_GPIO_ReadPin(COL9_GPIO_Port, COL9_Pin)==1) ? (1<<9):0) |
           ((HAL_GPIO_ReadPin(COL10_GPIO_Port, COL10_Pin)==1) ? (1<<10):0) |
           ((HAL_GPIO_ReadPin(COL11_GPIO_Port, COL11_Pin)==1) ? (1<<11):0) |
           ((HAL_GPIO_ReadPin(COL12_GPIO_Port, COL12_Pin)==1) ? (1<<12):0) |
           ((HAL_GPIO_ReadPin(COL13_GPIO_Port, COL13_Pin)==1) ? (1<<13):0);
}

/* Row pin configuration
    复位每行的电平为低电平
 */
static void unselect_rows(uint8_t row)
{
//    HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_RESET);
	switch (row) {
		case 0:
			HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_RESET);
			break;
		case 4:
			HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_RESET);
			break;
	}
}

/* 设置每行的输出电平为高电平*/
static void select_row(uint8_t row)
{
    // Output low to select
    switch (row) {
        case 0:
        	HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, GPIO_PIN_SET);
            break;
        case 1:
        	HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);
            break;
        case 2:
        	HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);
            break;
        case 3:
        	HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_SET);
            break;
        case 4:
        	HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_SET);
            break;
    }
}
