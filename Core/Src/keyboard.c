#include <stdint.h>
#include "keyboard.h"
#include "matrix.h"
#include "keymap.h"
#include "keycode.h"
#include "action.h"
#include "usbd_hid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
uint8_t report[8] = {0,0,0,0,0,0,0,0};

void keyboard_setup(void)
{
    matrix_setup();
}

void keyboard_init(void)
{
    matrix_init();
}

/*
 * Do keyboard routine jobs: scan matrix, light LEDs, ...
 * This is repeatedly called as fast as possible.
 */
void keyboard_task(void)
{
	static matrix_row_t matrix_prev[MATRIX_ROWS];
    matrix_row_t matrix_row = 0;
    matrix_row_t matrix_change = 0;
    uint8_t is_null = 0;

    matrix_scan();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        matrix_row = matrix_get_row(r);
        matrix_change = matrix_row ^ matrix_prev[r];
        if (matrix_change) {
        	matrix_prev[r] = matrix_row;
            matrix_row_t col_mask = 1;
            for (uint8_t c = 0; c < MATRIX_COLS; c++, col_mask <<= 1 ) {
                if (matrix_change & col_mask) {
                    keyevent_t e = (keyevent_t){
                        .key = (keypos_t){ .row = r, .col = c },
                        .pressed = (matrix_row & col_mask),
                        .time = 1/* time should not be 0 */
                    };
                    action_exec(e);

                    // record a processed key
                    // 同一个按键按多次会不会有冲突
                    //matrix_prev[r] ^= col_mask;

                    // This can miss stroke when scan matrix takes long like Topre
                    // process a key per task call
                    //goto MATRIX_LOOP_END;
                }
            }
        }
        if(matrix_row == 0){
        	is_null++;
        }
    }
    if(is_null == 5){
    	clear();
    }
    // call with pseudo tick event when no real key event.
    //action_exec(TICK);

}
