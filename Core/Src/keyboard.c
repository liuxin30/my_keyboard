#include <stdint.h>
#include "keyboard.h"
#include "matrix.h"
#include "keymap.h"
#include "keycode.h"
#include "action.h"
#include "usbd_hid.h"

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
	static uint8_t is_change = 0;
    matrix_row_t matrix_row = 0;
    matrix_row_t matrix_change = 0;
//    uint8_t is_null = 0;

    matrix_scan();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {

        matrix_row = matrix_get_row(r);

        matrix_change = matrix_row ^ matrix_prev[r];
        if (matrix_change) {
        	is_change = 1;
//        	matrix_prev[r] = matrix_row;
            matrix_row_t col_mask = 1;
            for (uint8_t c = 0; c < MATRIX_COLS; c++, col_mask <<= 1 ) {
                if (matrix_change & col_mask) {

                    keyevent_t e = (keyevent_t){
                        .key = (keypos_t){ .row = r, .col = c },
                        .pressed = (matrix_row & col_mask),
                        .time = 1/* time should not be 0 */
                    };
                    action_exec(e);
                    if(r==4 && c==10){
						is_change = 0;
						break;
					}

                    // record a processed key
                    matrix_prev[r] ^= col_mask;
                }
            }
        }
//        if(matrix_row == 0){
//        	is_null++;
//        }
    }
    if (is_change == 1){
    	send_keyboard_report();
    	is_change = 0;
    }

//    if(is_null == 5){
//    	clear();
//    }
}
